#include <iostream>

#include <fmt/format.h>
#include <imgui.h>
#include <imgui_stdlib.h>
#include <implot.h>
#include <string>

#include "render.hpp"

/**
 * @brief Main Drawfunction for ImGui
 *
 * @param label
 */
void WindowClass::Draw(std::string_view label)
{
    // ##########
    // Backend stuff
    // ##########

    handleFileData();

    // ##########
    // Frontend stuff
    // ##########
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::Begin(label.data(), nullptr, window_flags);
    ImGui::SetCursorPos(ImVec2(0.0F, 0.0F));

    switch (pageId)
    {
    case Dialogs::currentPage::MAIN:
        drawMenu();
        ImGui::SetCursorPos(ImVec2(0.0F, 18.0F));

        drawHeader();
        drawCursorData();
        drawPlot(voltUnitId);
        trigMscDetection();

        drawFooter();
        break;

    case Dialogs::currentPage::OPEN_CSV_FILE:
        _dialogs.drawFilebrowser(_fileCSV.path,
                                 _fileCSV.sCurrentFile,
                                 _fileCSV.sNewFile,
                                 _fileCSV.xFileLoaded,
                                 _fileCSV,
                                 fileHandler::contentPathOption::WAVE_FILE,
                                 pageId);
        break;

    case Dialogs::currentPage::CHOOSE_MSC_PATH:
        _dialogs.drawFilebrowser(_fileMsc.path,
                                 _fileMsc.sCurrentFile,
                                 _fileMsc.sNewFile,
                                 _fileMsc.xFileLoaded,
                                 _fileMsc,
                                 fileHandler::contentPathOption::DIRECTORY,
                                 pageId);
        break;

    case Dialogs::currentPage::CHOICE_WINDOW:
        _dialogs.choiceWindow(guiText::lbl.dialogNames.at(0).c_str(), guiText::lbl.fileBrowser.at(0).c_str());
        break;

    default:
        break;
    }

    ImGui::End();
}

/**
 * @brief Render the ImGui Window
 *
 * @param window_obj
 */
auto render(WindowClass &window_obj) -> void
{
    window_obj.Draw("OWON HDS Series CSV Wave Viewer");
}
/**
 * @brief  Draw the csv file as ImPlot
 *
 * @param unit - The unit of the y-axis of the type enum class voltUnit
 */
auto WindowClass::drawPlot(voltUnit unit) -> void
{

    static constexpr auto numOfPoints = 8'000; // 8k points

    static auto PlotX = std::array<double, numOfPoints>{};
    static auto PlotY = std::array<double, numOfPoints>{};

    ImVec2 dynPlotSize;
    dynPlotSize.y = footerStartPos() - ImGui::GetCursorPos().y;
    dynPlotSize.x = ImGui::GetWindowWidth() - ImGui::GetCursorPos().x;

    if (xResetView)
    {
        ImPlot::SetNextAxesToFit();
        xResetView = false;
    }

    if (ImPlot::BeginPlot("###Plot", dynPlotSize, (ImPlotFlags_NoTitle | ImPlotFlags_Crosshairs)))
    {
        ImPlot::SetupAxes(guiText::lbl.plot.at(1).c_str(),
                          guiText::lbl.plot.at(0).c_str(),
                          ImPlotAxisFlags_None,
                          ImPlotAxisFlags_AutoFit);
        ImPlot::SetupAxisLimitsConstraints(ImAxis_X1, 0, numOfPoints);

        for (std::size_t i = 0; i < numOfPoints; ++i)
        {
            PlotX.at(i) = static_cast<double>(i);

            // Raw data comes in as mv
            switch (unit)
            {
            case voltUnit::mV:
                PlotY.at(i) = _csvHandler.extractData(_csvHandler.csvData, i);
                break;
            case voltUnit::V:
                PlotY.at(i) = _csvHandler.extractData(_csvHandler.csvData, i) * 0.001;
                break;
            default:
                break;
            }
        }

        ImPlot::PlotLine(_csvHandler.extractHeaderData(_csvHandler.csvData, 0).c_str(),
                         PlotX.data(),
                         PlotY.data(),
                         numOfPoints);

        drawCursors();

        ImPlot::EndPlot();
    }
}

/**
 * @brief Draw the header informations of the csv file
 *
 */
auto WindowClass::drawHeader() -> void
{
    ImGui::Columns(9, "###mycolumns");
    ImGui::Separator();
    ImGui::Text("%s", guiText::lbl.header.at(0).c_str());
    ImGui::Text("%s", _csvHandler.extractHeaderData(_csvHandler.csvData, 0).c_str());
    ImGui::NextColumn();

    ImGui::Text("%s", guiText::lbl.header.at(1).c_str());
    ImGui::Text("%s", _csvHandler.extractHeaderData(_csvHandler.csvData, 6).c_str());
    ImGui::NextColumn();

    ImGui::Text("%s", guiText::lbl.header.at(2).c_str());
    ImGui::Text("%s", _csvHandler.extractHeaderData(_csvHandler.csvData, 3).c_str());
    ImGui::NextColumn();

    ImGui::Text("%s", guiText::lbl.header.at(3).c_str());
    ImGui::Text("%s", _csvHandler.extractHeaderData(_csvHandler.csvData, 4).c_str());
    ImGui::NextColumn();

    ImGui::Text("%s", guiText::lbl.header.at(4).c_str());
    ImGui::Text("%s", _csvHandler.extractHeaderData(_csvHandler.csvData, 5).c_str());
    ImGui::NextColumn();

    ImGui::Text("%s", guiText::lbl.header.at(5).c_str());
    ImGui::Text("%s", _csvHandler.extractHeaderData(_csvHandler.csvData, 1).c_str());
    ImGui::NextColumn();

    ImGui::Text("%s", guiText::lbl.header.at(6).c_str());
    ImGui::Text("%s", _csvHandler.extractHeaderData(_csvHandler.csvData, 2).c_str());
    ImGui::NextColumn();

    ImGui::Text("%s", guiText::lbl.header.at(7).c_str());
    ImGui::Text("%s", _csvHandler.extractHeaderData(_csvHandler.csvData, 7).c_str());
    ImGui::NextColumn();

    ImGui::Text("%s", guiText::lbl.header.at(8).c_str());
    ImGui::Text("%s", _csvHandler.extractHeaderData(_csvHandler.csvData, 8).c_str());
    ImGui::NextColumn();
    ImGui::Separator();

    ImGui::Columns(1);
}

/**
 * @brief Draw the combobox for setting up the y-axis unit
 *
 * @return voltUnit
 */
auto WindowClass::drawComboboxYUnit(voltUnit &unit) -> void
{

    static size_t item_current = 0;
    if (ImGui::BeginCombo(guiText::cb.names.at(0).c_str(), guiText::cb.unitY.at(item_current).c_str()))
    {
        size_t aSize = guiText::cb.unitY.size();
        for (size_t n = 0; n < aSize; n++)
        {
            const bool is_selected = (item_current == n);
            if (ImGui::Selectable(guiText::cb.unitY.at(n).c_str(), is_selected))
            {
                item_current = n;
                unit = static_cast<voltUnit>(item_current);
            }
            if (is_selected)
            {
                for (size_t i = 0; i < 4; i++)
                {
                    aPlottCursors.at(i) = 0.0f;
                }
                ImGui::SetItemDefaultFocus();
            }
        }
        ImGui::EndCombo();
    }
}
/**
 * @brief Draw the menu bar
 *
 */
auto WindowClass::drawMenu() -> void
{
    if (ImGui::BeginMenuBar())
    {
        // "Menu"
        if (ImGui::BeginMenu(guiText::lbl.menu.at(0).c_str()))
        {
            //btn open file
            if (ImGui::MenuItem(guiText::btn.menu.at(0).c_str()))
                pageId = Dialogs::currentPage::OPEN_CSV_FILE;
            ImGui::MenuItem(guiText::chkbx.names.at(2).c_str(), nullptr, &xFindOwonVolumeActive);
            ImGui::EndMenu();
        }
        // "View"
        if (ImGui::BeginMenu(guiText::lbl.menu.at(1).c_str()))
        {
            // Reset View
            if (ImGui::MenuItem(guiText::btn.menu.at(2).c_str(), nullptr, false))
                xResetView = true;
            drawComboboxYUnit(voltUnitId);

            ImGui::EndMenu();
        }
        // "CSV"
        if (ImGui::BeginMenu(guiText::lbl.menu.at(2).c_str()))
        {
            if (ImGui::MenuItem(guiText::btn.menu.at(1).c_str(), nullptr, nullptr))
                resetCursors();
            ImGui::MenuItem(guiText::chkbx.names.at(0).c_str(), nullptr, &xCursorY);
            ImGui::MenuItem(guiText::chkbx.names.at(1).c_str(), nullptr, &xCursorX);

            ImGui::EndMenu();
        }
        // "?" (Help) and other stuff
        if (ImGui::BeginMenu(guiText::lbl.menu.at(3).c_str()))
        {
            ImGui::MenuItem(guiText::btn.menu.at(3).c_str(), nullptr, &xBugReportOpen);
            openBugReport();
            ImGui::EndMenu();
        }

        ImGui::EndMenuBar();
    }
}
/**
 * @brief Draw the cursors
 * @attention Call this only within ImPlot::Begin and ImPlot::End !
 *
 */
auto WindowClass::drawCursors() -> void
{
    if (xCursorX)
    {
        ImPlot::DragLineX(0, &aPlottCursors[0], ImVec4(1, 1, 0, 1));
        ImPlot::DragLineX(1, &aPlottCursors[1], ImVec4(1, 1, 0, 1));

        ImPlot::PlotText("A", aPlottCursors[0], 0, ImVec2(10, 0));
        ImPlot::PlotText("B", aPlottCursors[1], 0, ImVec2(10, 0));
    }
    if (xCursorY)
    {
        ImPlot::DragLineY(2, &aPlottCursors[2], ImVec4(1, 1, 0, 1));
        ImPlot::DragLineY(3, &aPlottCursors[3], ImVec4(1, 1, 0, 1));
        ImPlot::PlotText("A", 0, aPlottCursors[2], ImVec2(10, 10));
        ImPlot::PlotText("B", 0, aPlottCursors[3], ImVec2(10, 10));
    }
}
/**
 * @brief Draw the cursor data in a table
 *
 */
auto WindowClass::drawCursorData() -> void
{
    constexpr static auto tableFlags = ImGuiTableFlags_Borders | ImGuiTableFlags_SizingStretchSame;

    if (xCursorX)
    {
        ImGui::BeginTable("###XCursor", 4, tableFlags);
        ImGui::TableNextColumn();
        ImGui::Text("Time: ");
        ImGui::TableNextColumn();
        ImGui::Text("A: %.2f micro seconds", aPlottCursors[0]);
        ImGui::TableNextColumn();
        ImGui::Text("B: %.2f micro seconds", aPlottCursors[1]);
        ImGui::TableNextColumn();
        ImGui::Text("AB: %.2f micro seconds", aPlottCursors[1] - aPlottCursors[0]);
        ImGui::EndTable();
    }
    if (xCursorY)
    {
        ImGui::BeginTable("###YCursor", 4, tableFlags);
        ImGui::TableNextColumn();
        ImGui::Text("Voltage: ");
        ImGui::TableNextColumn();

        sCursorUnit = (voltUnitId == voltUnit::V) ? guiText::cb.unitY.at(1) : guiText::cb.unitY.at(0);

        ImGui::Text("A: %.2f %s", aPlottCursors[2], sCursorUnit.data());
        ImGui::TableNextColumn();
        ImGui::Text("B: %.2f %s", aPlottCursors[3], sCursorUnit.data());
        ImGui::TableNextColumn();
        ImGui::Text("AB: %.2f %s", aPlottCursors[3] - aPlottCursors[2], sCursorUnit.data());
        ImGui::EndTable();
    }
}
/**
 * @brief Reset the cursor positions to zero
 *
 */
auto WindowClass::resetCursors() -> void
{
    for (size_t i = 0; i < 4; i++)
    {
        aPlottCursors[i] = 0.0f;
    }
}
/**
 * @brief Allow to open the bug report page of Github
 * @details To avoid opening the page multiple times, the variable xBugReportOpen is used
 *
 */
auto WindowClass::openBugReport() -> void
{
    if (xBugReportOpen)
    {
        xBugReportOpen = false;
        static std::string_view sBugReportUrl = "https://github.com/AlexanderTonn/OWON-CSV-INSPECTOR/issues";

#ifdef _WIN32
        ShellExecute(NULL, "open ", sBugReportUrl.data(), NULL, NULL, SW_SHOWNORMAL);
#elif __APPLE__
        std::string sCmd = std::string("open ") + sBugReportUrl.data();
        system(sCmd.c_str());
#elif __linux__
        std::string sCmd = std::string("xdg-open ") + sBugReportUrl.data();
        system(sCmd.c_str());
#endif
    }
}
/**
 * @brief Trigger the Mass storage volume detection
 *
 */
auto WindowClass::trigMscDetection() -> void
{
    // owon msc was found
    if (_trig.at(0).fire(1'000))
        _usbMSC.findOwonVolume(xFindOwonVolumeActive);
        if (_usbMSC.xVolumeFound)
            aFooterData.at(1) = guiText::lbl.fileBrowser.at(0);
}
/**
 * @brief Draw the footer data
 *
 */
auto WindowClass::drawFooter() -> void
{
    aFooterData.at(0) = _fileCSV.sCurrentFile;
    if (aFooterData.at(0).empty())
        aFooterData.at(0) = guiText::lbl.footer.at(0);

    ImGui::SetCursorPosY(footerStartPos());

    ImGui::BeginTable("###Footer", 2, ImGuiTableFlags_SizingFixedSame);
    ImGui::TableNextRow();
    // COL 1
    ImGui::TableNextColumn();
    ImGui::Text("%s %s", guiText::lbl.footer.at(1).c_str(), aFooterData.at(0).c_str());

    // COL 2
    ImGui::TableNextColumn();
    ImGui::Text("%s", aFooterData.at(1).c_str());

    ImGui::SameLine();
    if (_usbMSC.xVolumeFound && ImGui::Button(guiText::btn.footer.at(0).c_str()))
        pageId = Dialogs::currentPage::CHOOSE_MSC_PATH;


    ImGui::EndTable();
}
/**
 * @brief Set the start position of the footer
 *
 * @return start position of the footer
 */
auto WindowClass::footerStartPos() -> float
{
    return ImGui::GetWindowHeight() - aFooterSize;
}

auto WindowClass::handleFileData() -> void
{
    // Get path of the desktop
    if (xFirstCycle)
    {
        _fileCSV.initFilePath(fileHandler::standardPath::DOCUMENTS);
        _fileMsc.initFilePath(fileHandler::standardPath::DOCUMENTS);
        xFirstCycle = false;
    }

    // Draw the plot, if file is present
    if (_fileCSV.check())
        _csvHandler.parseCSV(_fileCSV.sCurrentFile, _csvHandler.csvData);

    if (_fileMsc.check())
        if (_usbMSC.copy(_fileMsc.sCurrentFile))
        {
            aFooterData.at(1) = "Files copied to " + _fileMsc.sCurrentFile;
            _usbMSC.xVolumeFound = false;
        }
}
