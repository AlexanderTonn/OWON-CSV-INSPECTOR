#include "fileHandler.hpp"
/**
 * @brief Check if the file exists
 *
 * @param sFileName
 * @return true
 * @return false
 */
auto fileHandler::checkFileString(std::string sFileName) -> bool
{
    std::cout << sFileName << std::endl;
    if (std::filesystem::exists(sFileName))
    {
        return true;
    }
    else
    {
        return false;
    }
}


/**
 * @brief Get the Content Of Path
 * List it as ImGui::Text
 * @param fPath
 * @return std::string
 */
auto fileHandler::getContentOfPath(std::filesystem::path &fPath, contentPathOption option) -> std::string
{
    static std::filesystem::path selectedPath; // after selecting in next frame, this will be the selected path

    for (const auto &entry : std::filesystem::directory_iterator(fPath))
    {

        const auto isSelected = entry.path() == selectedPath;
        const auto isDir = entry.is_directory();
        auto entryName = entry.path().filename().string();

        if (isDir)
        {
            entryName = entryName + "/";
        }

        if (ImGui::Selectable(entryName.c_str(), isSelected))
        {
            if (isDir)
            {
                fPath /= entry.path().filename();
                std::cout << fPath << std::endl;
            }
            selectedPath = entry.path();
        }
    }

    ImGui::Separator();
    // This is the path of the selected item
    if (!selectedPath.empty())
    {
        auto selectedEntry = selectedPath.filename().string();

        switch (option)
        {
        case contentPathOption::WAVE_FILE:
            if (std::filesystem::is_regular_file(selectedPath))
            {
                // only allow the WAVE.CSV files
                if (selectedEntry.find("WAVE") != std::string::npos)
                {
                    ImGui::Text("file is valid");
                    return selectedPath.string();
                }
                else
                    ImGui::Text("Selected file is not WAVE CSV file!");
            }
            break;
        case contentPathOption::DIRECTORY:
            // help finding a directory
            if (std::filesystem::is_directory(selectedPath))
            {
                ImGui::Text("This is a directory");

                return selectedPath.string() + '/';
            }
            else
                ImGui::Text("This is not a direktory");
            break;
        }
    }
    return "";
}
/**
 * @brief Get the Number Of Files in the current path
 *
 * @param fPath
 * @return int
 */
auto fileHandler::getNumberOfFiles(std::filesystem::path fPath) -> int
{
    auto count = std::distance(std::filesystem::directory_iterator(fPath), std::filesystem::directory_iterator{});
    return static_cast<int>(count);
}
/**
 * @brief Get the Filename at the selected position
 *
 * @param fPath
 * @param uiPos
 * @return std::string
 */
auto fileHandler::getFileAtPos(std::filesystem::path fPath, uint16_t uiPos) -> std::string
{
    uint16_t uiCount = 0;
    for (const auto &entry : std::filesystem::directory_iterator(fPath))
    {
        if (uiCount == uiPos)
        {
            return entry.path().filename().string();
        }
        uiCount++;
    }
    return "";
}
/**
 * @brief Initialize the file path
 * @note use this for preventing that the path is empty (otherwise runtime error)
 * @note e.g. you can call this in first cycle of the main loop
 * @param option
 * @return
 */
auto fileHandler::initFilePath(standardPath option) -> void
{
    char * ptr_xdgPath = nullptr;
    switch (option)
    {
    case standardPath::DESKTOP:
#ifdef _WIN32
        path = std::filesystem::path(std::getenv("USERPROFILE")) / "Desktop";

#elif __APPLE__ || __linux__
        // check if XDG_DESKTOP_DIR is set
        if (std::getenv("XDG_DESKTOP_DIR"))
            path = std::filesystem::path(std::getenv("XDG_DESKTOP_DIR"));
        // if XDG_DESKTOP_DIR is not set ...
        else if(std::getenv("HOME/Desktop"))
            path = std::filesystem::path(std::getenv("HOME/Desktop"));
        // if HOME/Desktop is not set ...
        else
            path = std::filesystem::path(std::getenv("HOME"));
#endif

        break;

    case standardPath::DOCUMENTS:
#ifdef _WIN32
        path = std::filesystem::path(std::getenv("USERPROFILE")) / "Documents";
#elif __APPLE__ || __linux__
        ptr_xdgPath = std::getenv("XDG_DOCUMENTS_DIR");
        if (ptr_xdgPath)
            path = std::filesystem::path(std::getenv("XDG_DOCUMENTS_DIR"));
        else if (std::getenv("HOME/Documents"))
            path = std::filesystem::path(std::getenv("HOME")) / "Documents";
        else
            path = std::filesystem::path(std::getenv("HOME"));
#endif
        break;

    case standardPath::DOWNLOADS:
#ifdef _WIN32
        path = std::filesystem::path(std::getenv("USERPROFILE")) / "Downloads";
#elif __APPLE__ || __linux__
        if (std::getenv("XDG_DOWNLOAD_DIR"))
            path = std::filesystem::path(std::getenv("XDG_DOWNLOAD_DIR"));
        else if(std::getenv("HOME/Downloads"))
            path = std::filesystem::path(std::getenv("HOME/Downloads"));
        else
            path = std::filesystem::path(std::getenv("HOME"));
#endif
        break;

    case standardPath::PICTURES:
#ifdef _WIN32
        path = std::filesystem::path(std::getenv("USERPROFILE")) / "Pictures";
#elif __APPLE__ || __linux__
        if (std::getenv("XDG_PICTURES_DIR"))
            path = std::filesystem::path(std::getenv("XDG_PICTURES_DIR"));
        else if (std::getenv("HOME/Pictures"))
            path = std::filesystem::path(std::getenv("HOME/Pictures"));
        else
            path = std::filesystem::path(std::getenv("HOME"));
#endif
        break;
    case standardPath::MUSIC:
#ifdef _WIN32
        path = std::filesystem::path(std::getenv("USERPROFILE")) / "Music";
#elif __APPLE__ || __linux__
        if (std::getenv("XDG_MUSIC_DIR"))
            path = std::filesystem::path(std::getenv("XDG_MUSIC_DIR"));
        else if (std::getenv("HOME/Music"))
            path = std::filesystem::path(std::getenv("HOME/Music"));
        else
            path = std::filesystem::path(std::getenv("HOME"));
#endif
        break;
    case standardPath::VIDEOS:
#ifdef _WIN32
        path = std::filesystem::path(std::getenv("USERPROFILE")) / "Videos";
#elif __APPLE__ || __linux__
        if (std::getenv("XDG_VIDEOS_DIR"))
            path = std::filesystem::path(std::getenv("XDG_VIDEOS_DIR"));
        else if(std::getenv("HOME/Videos"))
            path = std::filesystem::path(std::getenv("HOME/Videos"));
        else
            path = std::filesystem::path(std::getenv("HOME"));
#endif
        break;
    default:
        std::cout << "No valid path selected, use desktop path as default" << std::endl;
        break;
    }
}
/**
 * @brief Check filename whether it's emtpy
 * @note checking this for starting file handling is essential
 * @note sets "xFileLoad" to true if file is not empty
 * @return true if file is not empty,
 */
auto fileHandler::check() -> bool
{
    if (!xFileLoaded && !sCurrentFile.empty())
    {
        xFileLoaded = true;
        return true;
    }
    else
        return false;
}
