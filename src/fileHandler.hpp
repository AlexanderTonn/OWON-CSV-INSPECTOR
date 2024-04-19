#ifndef FILE_HANDLER_HPP
#define FILE_HANDLER_HPP

#include <filesystem>
#include <string_view>
#include <imgui.h>
#include <imgui_stdlib.h>
#include <iostream>
#include <string>

class fileHandler
{

public:
    std::filesystem::path path; // Path to the file

    std::string sCurrentFile = ""; // Current filename
    std::string sNewFile = ""; // New filename, which was selected by file dialog
    std::string sDirSeparator = "/"; // Directory separator, defined for Unix systems
    bool xFileLoaded = false; // Flag to check if the CSV file was loaded

    auto checkFileString(std::string sFileName) -> bool; // Check if the file exists
    //auto getDesktopPath() -> std::filesystem::path;

    enum class contentPathOption
    {
        WAVE_FILE,
        DIRECTORY
    };

    auto getContentOfPath(std::filesystem::path &fPath, contentPathOption option ) ->  std::string ; // Get content of current path and print it as ImGui::Text
    auto getNumberOfFiles(std::filesystem::path fPath) -> int; // Get number of files in a directory
    auto getFileAtPos(std::filesystem::path fPath, uint16_t uiPos) -> std::string; // Get file at position uiPos

    enum class standardPath
    {
        DESKTOP,
        DOCUMENTS,
        DOWNLOADS,
        PICTURES,
        MUSIC,
        VIDEOS
    };
    auto initFilePath(standardPath option) -> void; // Initialize the file path
    auto check() -> bool;

};



#endif // FILE_HANDLER_HPP
