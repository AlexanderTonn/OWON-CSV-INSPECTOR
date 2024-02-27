#ifndef FILE_HANDLER_HPP
#define FILE_HANDLER_HPP

#include <filesystem>
#include <string_view>
#include <imgui.h>
#include <imgui_stdlib.h>
#include <iostream>


class fileHandler
{
private:


public:
    std::filesystem::path path; // Path to the file

    auto checkFileString(std::string sFileName) -> bool; // Check if the file exists
    auto getDesktopPath() -> std::filesystem::path;
    auto getContentOfPath(std::filesystem::path &fPath ) ->  std::string ; // Get content of current path and print it as ImGui::Text
    auto getNumberOfFiles(std::filesystem::path fPath) -> int; // Get number of files in a directory
    auto getFileAtPos(std::filesystem::path fPath, uint16_t uiPos) -> std::string; // Get file at position uiPos

};



#endif // FILE_HANDLER_HPP
