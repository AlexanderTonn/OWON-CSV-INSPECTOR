#pragma once

#include "csvHandler.hpp"
#include "fileHandler.hpp"
#include <cstdint>
#include <string_view>

#ifdef _WIN32
#include <Windows.h>
#endif

class WindowClass
{
private:
    constexpr static auto window_flags =
        ImGuiWindowFlags_AlwaysAutoResize |
        ImGuiWindowFlags_NoDecoration |
        ImGuiWindowFlags_AlwaysVerticalScrollbar |
        //ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_MenuBar;
    constexpr static auto window_size = ImVec2(1280.0F, 720.0F);
    constexpr static auto window_pos = ImVec2(0.0F, 0.0F);

    bool xFirstCycle = true; // Flag for the first cycle

    enum class currentPage
    {
        MAIN,
        OPEN_FILE
    };
    currentPage pageId = currentPage::MAIN;

    enum class voltUnit
    {
        mV,
        V
    };
    voltUnit voltUnitId = voltUnit::mV;
    std::string_view sCursorUnit; // Used for the cursor data
    struct labels
    {
        std::string_view fileDialog = "WAVE.csv Browser"; // Label for the file dialog
        std::string_view plotY = "Voltage";               // Label for the y-axis
        std::string_view plotX = "Time";                  // Label for the x-axis
        std::string_view menu[4] = {"Menu",
                                    "View",
                                    "Measure",
                                    "?"};
        std::string_view buttons[5] = {"Open .csv file",
                                       "Reset Cursors",
                                       "Reset View" ,
                                       "Help" ,
                                       "Bug Report" };
        std::string_view comboBoxes[1] = {"Unit"};
        std::string_view checkBoxes[2] = {"Add Cursors: Voltage",
                                          "Add Cursors: Time"};
        // Items for the y-axis unit
        std::string_view cbUnitY[2] = {"mV",
                                       "V"};
        std::string_view header[9] = {"Channel",
                                      "Probe attenuation",
                                      "Peak to Peak",
                                      "Average",
                                      "Vertical pos.",
                                      "Frequency",
                                      "Period",
                                      "V per ADC Value",
                                      "Time interval"};
    };
    labels labels;

    csvHandler _csvHandler;

    // All about file and demo function
private:
    fileHandler _fileHandler;
    auto drawFilebrowser(std::filesystem::path &fPath) -> void;
    auto drawPlot(voltUnit unit) -> void;
    auto drawHeader() -> void;

    // Menubar
private:
    bool xResetView = false; // Reset the view of the plot
    auto drawMenu() -> void;
    auto drawComboboxYUnit(voltUnit &unit) -> void;
    auto openBugReport() -> void;
    bool xBugReportOpen = false;

    // Handling Plot Cursors
private:
    bool xCursorY = false;                        // Cursor for the voltage
    bool xCursorX = false;                        // Cursor for the time
    std::array<double, 4> aPlottCursors = {0.0f}; // Array for the cursor positions
    auto drawCursors() -> void;
    auto drawCursorData() -> void;
    auto resetCursors() -> void;

public:
    void Draw(std::string_view label);
};

auto render(WindowClass &window_obj) -> void;
