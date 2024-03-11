#include "usbMsc.hpp"

/**
 * @brief Create the save directory for the mass storage data if it does not exist
 *
 * @return true if the dir exists or was created, otherwise false
 */
auto usbMSC::createSaveDir(std::string sInputPath) -> bool
{
    if (std::filesystem::exists(sInputPath))
        return true;
    else
        return std::filesystem::create_directory(sInputPath);
}

/**
 * @brief Find the owon volume in the file system
 * @note the function is blocking the execution if volume was already found
 * @param xActive Activate the function
 * @return true if the volume was found
 */
auto usbMSC::findOwonVolume(bool xActive) -> bool
{
    if (xActive && !xVolumeFound)
    {
        // Buffer for the command output
        // if buffer is too small the fget() have to be called multiple times
        std::array<char, 2'048> aBuffer;
        std::string sParseResult;

        // Open and execute command in shell
        std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(
#ifdef _WIN32
                                                          "wmic logicaldisk get name",
#elif __linux__
                                                          "lsblk"
#elif __APPLE__
                                                          "diskutil list"
#endif
                                                          ,
                                                          "r"),
                                                      pclose);

        if (!pipe)
            throw std::runtime_error("popen() failed!");

        while (fgets(aBuffer.data(), aBuffer.size(), pipe.get()) != nullptr)
        {
            sParseResult += aBuffer.data();
            std::cout << sParseResult << std::endl;
        }


        if (sParseResult.find(sOwonVolume) != std::string::npos)
        {
            getVolumePath(aBuffer);
            xVolumeFound = true;
            return true;
        }
    }
    else
        return false;
}
/**
 * @brief move files from mass storage to the save directory
 *
 * @return true if files are found
 */
auto usbMSC::copy(std::string sTargetSavePath) -> bool
{
    // Create subfolder for saving the files
    // TODO: Create dir is working properly
    if (xVolumeFound && setSavePath(sTargetSavePath) )
    {
        // move the files
        for (uint8_t i = 1; i <= MAX_FILE_COUNT; i++)
        {
            // TODO: Moving did not work
            getFiles(getFile::CSV, i);
            getFiles(getFile::BMP, i);
        }

        // unmount the volume
        std::string sUnmountCmd = "diskutil unmount \"" + std::string(sOwonVolume) + "\"";
        system(sUnmountCmd.c_str());
        xVolumeFound = false;
        return true;
    }
    else
        return false;
}
/**
 * @brief
 *
 * @tparam size
 * @param aBuffer
 */
template <std::size_t size>
auto usbMSC::getVolumePath(std::array<char, size> aBuffer) -> void
{
#ifdef _WIN32
    std::size_t volPos = aBuffer.find(sOwonVolume);
    if (volPos != std::string::npos)
        sVolumePath = aBuffer.substr(volPos - 1, 2) + "\\";

#elif __linux__
    std::size_t volPos = aBuffer.find(sOwonVolume);
    if (volPos != std::string::npos)
    {
        std::size_t startPos = aBuffer.rfind('\n', volPos) + 1;
        std::size_t endPos = aBuffer.find(' ', startPos);
        sVolumePath = aBuffer.substr(startPos, endPos - startPos);
    }
#elif __APPLE__
    sVolumePath = "/Volumes/" + std::string(sOwonVolume);
#endif
}
/**
 * @brief Before start copy the files, create the subfolder for saving the files
 * @param sInputpath Inputpath is the target path passed by the user
 * @return true if the subfolder exists or was created
 */
auto usbMSC::setSavePath(std::string sInputpath) -> bool
{
    sCurrentDate = getDate();
    sSavePath = sInputpath + std::string(sSaveDir) + "/" + sCurrentDate + "/";

    //auto mainDirPresent = createSaveDir(sInputpath);

    if (std::filesystem::exists(sSavePath) )
        return true;
    else
    {
        if (sCurrentDate != "")
            return std::filesystem::create_directories(sSavePath);
        else
            return false;
    }
}
/**
 * @brief get the Date of today
 *
 * @return day as string
 */
auto usbMSC::getDate() -> std::string
{
    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);

    std::stringstream ss;
    ss << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d");
    return ss.str();
}
/**
 * @brief Move the files from volume to the save directory
 *
 * @param type enum class fileTypes
 * @param uiFileNo used for the file number
 */
auto usbMSC::getFiles(getFile type, uint8_t uiFileNo) -> bool
{
    std::string sVolFilePath; // Path on OWON Volume
    switch (type)
    {
    case getFile::BMP:
        sVolFilePath = sVolumePath + "/" + "IMAGE" + std::to_string(uiFileNo) + ".BMP";
        break;
    case getFile::CSV:
        sVolFilePath = sVolumePath + "/" + "WAVE" + std::to_string(uiFileNo) + ".CSV";
        break;
    default:
        return false;
    break;
    }
    std::cout << "Image Path on Deive: " <<  sVolFilePath << std::endl;
    std::cout << "TargetPath: " <<  sSaveDir << std::endl;
        // Move
        if (std::filesystem::exists(sVolFilePath) && std::filesystem::exists(sSavePath) )
        {
            std::filesystem::copy(sVolFilePath, sSavePath, std::filesystem::copy_options::overwrite_existing);
            return true;
        }
        else
            return false;

}
