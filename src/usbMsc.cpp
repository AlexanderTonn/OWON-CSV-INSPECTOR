#include "usbMsc.hpp"


/**
 * @brief Create the save directory for the mass storage data
 *
 */
auto usbMSC::createMainSaveDir() -> void
{
    if (std::filesystem::exists(sSaveDir))
        return;
    else
    {
        try
        {
            std::filesystem::create_directory(sSaveDir);
        }
        catch (const std::exception &e)
        {
            std::cerr << e.what() << '\n';
        }
    }
}
/**
 * @brief Find the owon volume in the file system
 * @note call this cyclically to check if the owon device is connected
 * @return
 */
auto usbMSC::findOwonVolume() -> bool
{
    std::array<char, 256> aBuffer;
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
        sParseResult += aBuffer.data();

    return sParseResult.find(sOwonVolume) != std::string::npos;
}
