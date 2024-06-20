#pragma once

#include "csvHandler.hpp"
#include "fileHandler.hpp"
#include "funcTrigger.hpp"
#include "usbHid.hpp"
#include "usbMsc.hpp"
#include "guiStrings.hpp"
#include "dialogs.hpp"

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
    WindowClass() : stringCursorUnit(_guiTexts.cb.at(languageSelection).unitY.at(0).c_str()),
                    _dialogs(),
                    _usbHID(),
                    _usbMSC(),
                    _csvHandler(),
                    _fileCSV() {}

private:
    std::string stringCursorUnit;
    Dialogs _dialogs;
    usbHID _usbHID;
    usbMSC _usbMSC;
    csvHandler _csvHandler;
    fileHandler _fileMsc; // Instance for mass storage file handling
    fileHandler _fileCSV; // Instance for CSV file handling

    // Window settings

private:
    constexpr static auto window_flags = ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoDecoration |
                                         ImGuiWindowFlags_AlwaysVerticalScrollbar | ImGuiWindowFlags_NoMove |
                                         ImGuiWindowFlags_MenuBar;
    constexpr static auto window_size = ImVec2(1280.0F, 720.0F);
    constexpr static auto window_pos = ImVec2(0.0F, 0.0F);

    bool firstCycle = true; // Flag for the first cycle
    Dialogs::currentPage pageId = Dialogs::currentPage::MAIN;

    enum class voltUnit
    {
        mV,
        V
    };
    voltUnit voltUnitId = voltUnit::mV;

    // All about file and demo function
private:
    auto handleFileData() -> void;

private:
    auto drawPlot(voltUnit unit) -> void;
    auto drawHeader() -> void;

    // Menubar
private:
    bool resetView = false; // Reset the view of the plot
    auto drawMenu() -> void;
    auto drawComboboxYUnit(voltUnit &unit) -> void;
    auto openBugReport() -> void;
    bool bugReportOpen = false;

    // Handling Plot Cursors
private:
    bool cursorY = false;                        // Cursor for the voltage
    bool cursorX = false;                        // Cursor for the time
    std::array<double, 4> arrayPlottCursors = {0.0f}; // Array for the cursor positions
    auto drawCursors() -> void;
    auto drawCursorData() -> void;
    auto resetCursors() -> void;

    // Handling the OWON mass storage
private:
    std::array<funcTrigger, 1> _trig; // increase the Array size if you need more triggers

    auto trigMscDetection() -> void;
    bool findOwonVolumeActive = false;  // if true, the program will search for the owon volume

    // Footer
private:
    std::array<std::string, 2> arrayFooterData;
    uint8_t footerSize = 20;
    auto drawFooter() -> void;
    auto footerStartPos() -> float;


public:
    void Draw(std::string_view label);
};

auto render(WindowClass &window_obj) -> void;
