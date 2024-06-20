#include "usbMsc.hpp"

/**
 * @brief Create the save directory for the mass storage data if it does not exist
 *
 * @return true if the dir exists or was created, otherwise false
 */
auto usbMSC::createSaveDir(std::string stringInputPath) -> bool
{
    if (std::filesystem::exists(stringInputPath))
        return true;
    else
        return std::filesystem::create_directory(stringInputPath);
}

/**
 * @brief Find the owon volume in the file system
 * @note the function is blocking the execution if volume was already found
 * @param xActive Activate the function
 * @return true if the volume was found
 */
auto usbMSC::findOwonVolume(bool active) -> bool
{
    if (active && !volumeFound)
    {
        // Buffer for the command output
        // if buffer is too small the fget() have to be called multiple times
        std::array<char, 2'048> arrayBuffer;
        std::string stringParseResult;

        // Open and execute command in shell


#ifdef _WIN32
        std::unique_ptr<FILE,decltype(&_pclose)>pipe(_popen("wmic logicaldisk get volumeName", "r"), _pclose);
#elif __linux__
        std::unique_ptr<FILE, decltype(&pclose)> pipe(popen("lsblk -o NAME,SIZE,FSTYPE,MOUNTPOINT", "r"), pclose);
#elif __APPLE__
        std::unique_ptr<FILE, decltype(&pclose)> pipe(popen("diskutil list", "r"), pclose);
#endif

        // Throw exception if volume listing failed
        try
        {
            if(!pipe)
                throw std::runtime_error("popen() failed!");
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
        }

        while (fgets(arrayBuffer.data(), arrayBuffer.size(), pipe.get()) != nullptr)
        {
            stringParseResult += arrayBuffer.data();
            std::cout << stringParseResult << std::endl; // Show Volumes in Terminal
        }

#if defined(_WIN32) || defined(__APPLE__)
        if (stringParseResult.find(stringOwonVolume) != std::string::npos)
        {
            getVolumePath(arrayBuffer);
            volumeFound = true;
            return true;
        }
        else
            return false;
#elif __linux__
        if (isSize_and_FAT12(aBuffer))
        {
            xVolumeFound = true;
            return true;
        }
        else
            return false;
#endif
    }
    else
        return false;
}
/**
 * @brief move files from mass storage to the save directory
 *
 * @return true if files are found
 */
auto usbMSC::copy(std::string stringTargetSavePath) -> bool
{
    // Create subfolder for saving the files
    if (volumeFound && setSavePath(stringTargetSavePath))
    {
        // move the files
        for (uint8_t i = 1; i <= MAX_FILE_COUNT; i++)
        {
            getFiles(getFile::CSV, i);
            getFiles(getFile::BMP, i);
        }

        // unmount the volume
        std::string stringUnmountCmd = "diskutil unmount \"" + std::string(stringOwonVolume) + "\"";
        system(stringUnmountCmd.c_str());
        volumeFound = false;
        return true;
    }
    else
        return false;
}
/**
 * @brief
 *
 * @tparam size
 * @param arrayBuffer
 */
template <std::size_t size>
auto usbMSC::getVolumePath(std::array<char, size> arrayBuffer) -> void
{
#ifdef _WIN32 | __linux__
    std::string stringBuffer(arrayBuffer.data(), arrayBuffer.size());
#endif

#ifdef _WIN32
    std::size_t volPos = stringBuffer.find(sOwonVolume);
    if (volPos != std::string::npos)
        stringVolumePath = stringBuffer.substr(volPos - 1, 2) + "\\";

#elif __linux__
    // on Linux you have to use std::string instead of std::array
    std::string sBuffer(arrayBuffer.data(), arrayBuffer.size());

    std::size_t volPos = stringBuffer.find(stringOwonVolume);
    if (volPos != std::string::npos)
    {
        std::size_t startPos = stringBuffer.rfind('\n', volPos) + 1;
        std::size_t endPos = stringBuffer.find(' ', startPos);
        stringVolumePath = stringBuffer.substr(startPos, endPos - startPos);
    }
#elif __APPLE__
    stringVolumePath = "/Volumes/" + std::string(stringOwonVolume);
#endif
}


/**
 * @brief Before start copy the files, create the subfolder for saving the files
 * @param stringInputpath Inputpath is the target path passed by the user
 * @return true if the subfolder exists or was created
 */
auto usbMSC::setSavePath(std::string stringInputpath) -> bool
{
    stringCurrentDate = getDate();
    stringSavePath = stringInputpath + std::string(stringSaveDir) + "/" + stringCurrentDate + "/";

    //auto mainDirPresent = createSaveDir(sInputpath);

    if (std::filesystem::exists(stringSavePath))
        return true;
    else
    {
        if (stringCurrentDate != "")
            return std::filesystem::create_directories(stringSavePath);
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
 * @param fileNo used for the file number
 */
auto usbMSC::getFiles(getFile type, uint8_t fileNo) -> bool
{
    std::string stringVolFilePath; // Path on OWON Volume
    switch (type)
    {
    case getFile::BMP:
        stringVolFilePath = stringVolumePath + "/" + "IMAGE" + std::to_string(fileNo) + ".BMP";
        break;
    case getFile::CSV:
        stringVolFilePath = stringVolumePath + "/" + "WAVE" + std::to_string(fileNo) + ".CSV";
        break;
    default:
        return false;
        break;
    }
    // Move
    if (std::filesystem::exists(stringVolFilePath) && std::filesystem::exists(stringSavePath))
    {
        std::filesystem::copy(stringVolFilePath, stringSavePath, std::filesystem::copy_options::overwrite_existing);
        return true;
    }
    else
        return false;
}
