#pragma once

#include <cstdint>
#include <string_view>
#include "csvHandler.hpp"
#include "fileHandler.hpp"

class WindowClass
{
private:
    constexpr static auto window_flags =
        ImGuiWindowFlags_AlwaysUseWindowPadding
        | ImGuiWindowFlags_NoMove
        | ImGuiWindowFlags_NoCollapse;
        //| ImGuiWindowFlags_NoDecoration;
    constexpr static auto window_size = ImVec2(1280.0F, 720.0F);
    constexpr static auto window_pos = ImVec2(0.0F, 0.0F);

    bool xFirstCycle = true; // First cycle of the draw function

    enum class currentPage{
        MAIN,
        OPEN_FILE
    };
    currentPage pageId = currentPage::MAIN;

    enum class voltUnit{
        mV,
        V
    };
    voltUnit voltUnitId = voltUnit::mV;


    std::string_view labelBtnOpenFile = "Open File";    // Label for the open file button
    std::string_view labelFileDialog = "WAVE.csv Browser";  // Label for the file dialog

    csvHandler _csvHandler;   // instance of csvHandler class
    fileHandler _fileHandler; // instance of fileHandler class

    auto drawPlot(voltUnit unit)  -> void;  // Draw the csv file as ImPlot
    auto drawHeader() -> void; // Draw the header informations of the csv file
    auto drawFilebrowser(std::filesystem::path &fPath) -> void; // Draw the filebrowser
    auto drawComboboxYUnit() -> voltUnit; // Draw the combobox for setting up the y-axis unit

public:
    void Draw(std::string_view label);
};

void render(WindowClass &window_obj);
