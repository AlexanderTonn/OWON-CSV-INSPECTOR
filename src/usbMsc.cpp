#include "usbMsc.hpp"

/**
 * @brief Create the save directory for the mass storage data if it does not exist
 *
 */
auto usbMSC::checkMainSaveDir() -> void
{
    if (std::filesystem::exists(sSaveDir))
        return;
    else
        std::filesystem::create_directory(sSaveDir);
}

/**
 * @brief Find the owon volume in the file system
 * @note call this cyclically to check if the owon device is connected
 * @param xActive Activate the function
 * @return
 */
auto usbMSC::findOwonVolume(bool xActive) -> bool
{
    if (xActive)
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
                                                          , "r"),
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
auto usbMSC::moveFiles() -> bool
{
    checkMainSaveDir();

    sCurrentDate = getDate();

    // Create subfolder for saving the files
    if(createSubDir() && sCurrentDate != "")
    {
        getBMPFiles();
        getCSVFiles();
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

// TODO! Test this on a linux machine
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
 * @brief
 *
 */
auto usbMSC::getBMPFiles() -> void
{
    std::string sBmpPath = sVolumePath + ".BMP";
    if (std::filesystem::exists(sBmpPath))
    {
        std::filesystem::copy(sBmpPath, sSaveDir, std::filesystem::copy_options::recursive);
    }
}
auto usbMSC::getCSVFiles() -> void
{
    std::string sCsvPath = sVolumePath + ".CSV";
    if (std::filesystem::exists(sCsvPath))
    {
        std::filesystem::copy(sCsvPath, sSaveDir, std::filesystem::copy_options::recursive);
    }
}

/**
 * @brief
 *
 * @return
 */
auto usbMSC::createSubDir() -> bool
{
    std::string sSavePath = std::string(sSaveDir) + "/" + sSubDir;

    if (std::filesystem::exists(sSavePath))
        return true;
    else
        return std::filesystem::create_directory(sSubDir);
}
/**
 * @brief
 *
 * @return
 */
auto usbMSC::getDate() -> std::string
{
    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);

    std::stringstream ss;
    ss << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d");
    return ss.str();
}
