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
 * @brief Get the desktop path
 *
 * @return std::filesystem::path
 */
auto fileHandler::getDesktopPath() -> std::filesystem::path
{
    std::filesystem::path desktopPath;
    #ifdef _WIN32
        desktopPath = std::filesystem::path(std::getenv("USERPROFILE")) / "Desktop";

    #elif __APPLE__
        desktopPath = std::filesystem::path(std::getenv("HOME")) / "Desktop";

    #elif __linux__
        desktopPath = std::filesystem::path(std::getenv("XDG_DESKTOP_DIR"));
    #endif

    return desktopPath;
}

/**
 * @brief Get the Content Of Path
 * List it as ImGui::Text
 * @param fPath
 * @return std::string
 */
auto fileHandler::getContentOfPath(std::filesystem::path &fPath ) -> std::string
{
   static std::filesystem::path selectedPath; // after selecting in next frame, this will be the selected path

    for( const auto &entry : std::filesystem::directory_iterator(fPath))
    {

        const auto isSelected = entry.path() == selectedPath;
        const auto isDir = entry.is_directory();
        auto entryName = entry.path().filename().string();

        if(isDir)
        {
            entryName = entryName + "/";
        }

        if(ImGui::Selectable(entryName.c_str(), isSelected))
        {
            if(isDir)
            {
                fPath /= entry.path().filename();
                std::cout << fPath << std::endl;
            }
            selectedPath = entry.path();
        }
    }

    ImGui::Separator();
    // This is the path of the selected item
    if(!selectedPath.empty())
    {
        auto selectedEntry = selectedPath.filename().string();
        if(std::filesystem::is_regular_file(selectedPath))
        {
            // only allow the WAVE.CSV files
            if(selectedEntry.find("WAVE") != std::string::npos)
            {
                ImGui::Text("file is valid");
                return selectedPath.string();
            }else
                ImGui::Text("Selected file is not WAVE CSV file!");
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
