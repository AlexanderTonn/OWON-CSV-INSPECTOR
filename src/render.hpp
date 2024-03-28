#pragma once

#include "csvHandler.hpp"
#include "fileHandler.hpp"
#include "funcTrigger.hpp"
#include "usbHid.hpp"
#include "usbMsc.hpp"
#include <array>
#include <cstdint>
#include <string_view>

#ifdef _WIN32
#include <Windows.h>
#endif

class WindowClass
{
    // Member initialization list
public:
    WindowClass() : sCursorUnit(labels.cbUnitY[0]),
                    labels(),
                    _usbHID(),
                    _usbMSC(),
                    _csvHandler(),
                    _fileCSV() {}

private:
    std::string sCursorUnit;
    csvHandler _csvHandler;
    usbHID _usbHID;

    // Window settings

private:
    constexpr static auto window_flags = ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoDecoration |
                                         ImGuiWindowFlags_AlwaysVerticalScrollbar | ImGuiWindowFlags_NoMove |
                                         ImGuiWindowFlags_MenuBar;
    constexpr static auto window_size = ImVec2(1280.0F, 720.0F);
    constexpr static auto window_pos = ImVec2(0.0F, 0.0F);

    bool xFirstCycle = true; // Flag for the first cycle

    enum class currentPage
    {
        MAIN,
        OPEN_CSV_FILE,
        CHOOSE_MSC_PATH,
        CHOICE_WINDOW
    };
    currentPage pageId = currentPage::MAIN;

    enum class voltUnit
    {
        mV,
        V
    };
    voltUnit voltUnitId = voltUnit::mV;

    struct labels
    {
        std::string_view dialogNames[1] = {"Filebrowser"}; // Label for the file dialog
        std::string_view plotY = "Voltage";                // Label for the y-axis
        std::string_view plotX = "Time";                   // Label for the x-axis
        std::string_view menu[4] = {
            "Options",
            "View",
            "CSV",
            "?",
        };
        std::string_view buttons[9] =
            {"Open file", "Reset Cursors", "Reset View", "Help", "Bug Report", " < ", " OK ", " Cancel ", " Yes "};
        std::string_view comboBoxes[1] = {"Unit"};
        std::string_view checkBoxes[3] = {"Add Cursors: Voltage", "Add Cursors: Time", "Search for OWON MSC"};
        // Items for the y-axis unit
        std::string_view cbUnitY[2] = {"mV", "V"};
        std::string_view header[9] = {"Channel",
                                      "Probe attenuation",
                                      "Peak to Peak",
                                      "Average",
                                      "Vertical pos.",
                                      "Frequency",
                                      "Period",
                                      "V per ADC Value",
                                      "Time interval"};
        std::string_view description[2] = {"The OWON Volume was found, do you want to copy files?", "CSV File options"};
        std::string_view footer[2] = {"No file found", "Current file: "};
    } labels;





    // All about file and demo function
private:
    fileHandler _fileCSV; // Instance for CSV file handling
    auto drawFilebrowser(std::filesystem::path &fPath,
                         std::string &sCurrent,
                         std::string &sNew,
                         bool &xNotLoaded ,
                         fileHandler &_fileHandler,
                         fileHandler::contentPathOption option) -> void;
    auto choiceWindow(std::string_view sName, std::string_view sQuestion) -> bool;
    auto handleFileData() -> void;

    fileHandler _fileMsc; // Instance for mass storage file handling


private:
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

    // Handling the OWON mass storage
private:
    std::array<funcTrigger, 1> _trig; // increase the Array size if you need more triggers
    usbMSC _usbMSC;
    auto trigMscDetection() -> void;
    bool xFindOwonVolumeActive = false;  // if true, the program will search for the owon volume

    // Footer
private:
    std::array<std::string, 2> aFooterData;
    uint8_t aFooterSize = 20;
    auto drawFooter() -> void;
    auto footerStartPos() -> float;


public:
    void Draw(std::string_view label);
};

auto render(WindowClass &window_obj) -> void;
