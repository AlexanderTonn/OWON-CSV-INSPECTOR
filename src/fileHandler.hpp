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

    std::string stringCurrentFile = ""; // Current filename
    std::string stringNewFile = ""; // New filename, which was selected by file dialog
    std::string stringDirSeparator = "/"; // Directory separator, defined for Unix systems
    bool fileLoaded = false; // Flag to check if the CSV file was loaded

    auto checkFileString(std::string stringFileName) -> bool; // Check if the file exists
    //auto getDesktopPath() -> std::filesystem::path;

    enum class contentPathOption
    {
        WAVE_FILE,
        DIRECTORY
    };

    auto getContentOfPath(std::filesystem::path &path, contentPathOption option ) ->  std::string ; // Get content of current path and print it as ImGui::Text
    auto getNumberOfFiles(std::filesystem::path path) -> int; // Get number of files in a directory
    auto getFileAtPos(std::filesystem::path path, uint16_t pos) -> std::string; // Get file at position uiPos

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
