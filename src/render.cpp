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
    case currentPage::MAIN:
        drawMenu();
        ImGui::SetCursorPos(ImVec2(0.0F, 18.0F));

        drawHeader();
        drawCursorData();
        drawPlot(voltUnitId);
        trigMscDetection();

        drawFooter();
        break;

    case currentPage::OPEN_CSV_FILE:
        drawFilebrowser(_fileCSV.path,
                        _fileCSV.sCurrentFile,
                        _fileCSV.sNewFile,
                        _fileCSV.xFileLoaded,
                        _fileCSV,
                        fileHandler::contentPathOption::WAVE_FILE);
        break;

    case currentPage::CHOOSE_MSC_PATH:
        drawFilebrowser(_fileMsc.path,
                        _fileMsc.sCurrentFile,
                        _fileMsc.sNewFile,
                        _fileMsc.xFileLoaded,
                        _fileMsc,
                        fileHandler::contentPathOption::DIRECTORY);
        break;

    case currentPage::CHOICE_WINDOW:
        choiceWindow(labels.dialogNames[1].data(), labels.description[0].data());
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

        ImPlot::SetupAxes(labels.plotX.data(), labels.plotY.data(), ImPlotAxisFlags_None, ImPlotAxisFlags_AutoFit);
        ImPlot::SetupAxisLimitsConstraints(ImAxis_X1, 0, numOfPoints);

        for (std::size_t i = 0; i < numOfPoints; ++i)
        {
            PlotX[i] = i;

            // Raw data comes in as mv
            switch (unit)
            {
            case voltUnit::mV:
                PlotY[i] = _csvHandler.extractData(_csvHandler.csvData, i);
                break;
            case voltUnit::V:
                PlotY[i] = _csvHandler.extractData(_csvHandler.csvData, i) * 0.001;
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
    ImGui::Text("%s", labels.header[0].data());
    ImGui::Text("%s", _csvHandler.extractHeaderData(_csvHandler.csvData, 0).c_str());
    ImGui::NextColumn();

    ImGui::Text("%s", labels.header[1].data());
    ImGui::Text("%s", _csvHandler.extractHeaderData(_csvHandler.csvData, 6).c_str());
    ImGui::NextColumn();

    ImGui::Text("%s", labels.header[2].data());
    ImGui::Text("%s", _csvHandler.extractHeaderData(_csvHandler.csvData, 3).c_str());
    ImGui::NextColumn();

    ImGui::Text("%s", labels.header[3].data());
    ImGui::Text("%s", _csvHandler.extractHeaderData(_csvHandler.csvData, 4).c_str());
    ImGui::NextColumn();

    ImGui::Text("%s", labels.header[4].data());
    ImGui::Text("%s", _csvHandler.extractHeaderData(_csvHandler.csvData, 5).c_str());
    ImGui::NextColumn();

    ImGui::Text("%s", labels.header[5].data());
    ImGui::Text("%s", _csvHandler.extractHeaderData(_csvHandler.csvData, 1).c_str());
    ImGui::NextColumn();

    ImGui::Text("%s", labels.header[6].data());
    ImGui::Text("%s", _csvHandler.extractHeaderData(_csvHandler.csvData, 2).c_str());
    ImGui::NextColumn();

    ImGui::Text("%s", labels.header[7].data());
    ImGui::Text("%s", _csvHandler.extractHeaderData(_csvHandler.csvData, 7).c_str());
    ImGui::NextColumn();

    ImGui::Text("%s", labels.header[8].data());
    ImGui::Text("%s", _csvHandler.extractHeaderData(_csvHandler.csvData, 8).c_str());
    ImGui::NextColumn();
    ImGui::Separator();

    ImGui::Columns(1);
}
/**
 * @brief draw the filebrowser
 *
 * @param fPath contains the complete path of selected item
 * @param sCurrent string path of the current file
 * @param sNew string path of the new file
 * @param xNotLoaded - true, if the file is not loaded
 * @note use the xNoatLoaded for proceeding the file once
 */
auto WindowClass::drawFilebrowser(std::filesystem::path &fPath,
                                  std::string &sCurrent,
                                  std::string &sNew,
                                  bool &xNotLoaded,
                                  fileHandler &_fileHandler,
                                  fileHandler::contentPathOption option) -> void
{
    ImGui::OpenPopup(labels.dialogNames[0].data());

    if (ImGui::BeginPopupModal(labels.dialogNames[0].data(), nullptr, ImGuiWindowFlags_AlwaysAutoResize))
    {
        static std::string sFileName;
        // go back in file path
        if (ImGui::Button(labels.buttons[5].data()))
        {
            // Check whether parent is available
            if (fPath.has_parent_path() && fPath.parent_path() != fPath.root_path())
                fPath = fPath.parent_path();
        }
        ImGui::Text("Current path: %s", fPath.string().c_str());
        ImGui::Separator();

        sNew = _fileHandler.getContentOfPath(fPath, option);

        ImGui::Separator();
        // OK Btn pressed
        if (ImGui::Button(labels.buttons[6].data()))
        {
            sCurrent = sNew;
            xNotLoaded = false; // Allow to load new file
            pageId = currentPage::MAIN;
            ImGui::CloseCurrentPopup();
        }
        ImGui::SameLine();

        // Cancel Btn pressed
        if (ImGui::Button(labels.buttons[7].data()))
        {
            pageId = currentPage::MAIN;
            ImGui::CloseCurrentPopup();
        }
    }
    ImGui::EndPopup();
}

/**
 * @brief Draw the combobox for setting up the y-axis unit
 *
 * @return voltUnit
 */
auto WindowClass::drawComboboxYUnit(voltUnit &unit) -> void
{

    static int item_current = 0;
    if (ImGui::BeginCombo(labels.comboBoxes[0].data(), labels.cbUnitY[item_current].data()))
    {
        for (int n = 0; n < IM_ARRAYSIZE(labels.cbUnitY); n++)
        {
            const bool is_selected = (item_current == n);
            if (ImGui::Selectable(labels.cbUnitY[n].data(), is_selected))
            {
                item_current = n;
                unit = static_cast<voltUnit>(item_current);
            }
            if (is_selected)
            {
                for (size_t i = 0; i < 4; i++)
                {
                    aPlottCursors[i] = 0.0f;
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
        if (ImGui::BeginMenu(labels.menu[0].data()))
        {
            ImGui::MenuItem(labels.description[1].data(), nullptr, false, false);
            if (ImGui::MenuItem(labels.buttons[0].data()))
                pageId = currentPage::OPEN_CSV_FILE;
            ImGui::MenuItem(labels.checkBoxes[2].data(), nullptr, &xFindOwonVolumeActive);
            ImGui::EndMenu();
        }
        // "View"
        if (ImGui::BeginMenu(labels.menu[1].data()))
        {
            if (ImGui::MenuItem(labels.buttons[2].data(), nullptr, nullptr))
                xResetView = true;
            drawComboboxYUnit(voltUnitId);

            ImGui::EndMenu();
        }
        // "Measure"
        if (ImGui::BeginMenu(labels.menu[2].data()))
        {
            if (ImGui::MenuItem(labels.buttons[1].data(), nullptr, nullptr))
                resetCursors();
            ImGui::MenuItem(labels.checkBoxes[0].data(), nullptr, &xCursorY);
            ImGui::MenuItem(labels.checkBoxes[1].data(), nullptr, &xCursorX);

            ImGui::EndMenu();
        }
        // "?" (Help) and other stuff
        if (ImGui::BeginMenu(labels.menu[3].data()))
        {
            ImGui::MenuItem(labels.buttons[4].data(), nullptr, &xBugReportOpen);
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

        sCursorUnit = (voltUnitId == voltUnit::V) ? labels.cbUnitY[1] : labels.cbUnitY[0];

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
    if (_trig.at(0).fire(10'000))
        if (!_usbMSC.xVolumeFound && _usbMSC.findOwonVolume(xFindOwonVolumeActive))
            aFooterData.at(1) = "Owon Volume found. Want open? ";

}
/**
 * @brief Draw the footer data
 *
 */
auto WindowClass::drawFooter() -> void
{
    aFooterData.at(0) = _fileCSV.sCurrentFile;
    if (aFooterData.at(0).empty())
        aFooterData.at(0) = labels.footer[0].data();

    ImGui::SetCursorPosY(footerStartPos());

    ImGui::BeginTable("###Footer", 2, ImGuiTableFlags_SizingFixedSame);
    ImGui::TableNextRow();
    // COL 1
    ImGui::TableNextColumn();
    ImGui::Text("%s %s", labels.footer[1].data(), aFooterData.at(0).c_str());

    // COL 2
    ImGui::TableNextColumn();
    ImGui::Text("%s", aFooterData.at(1).c_str());
    // cheack each 10s if owon msc is connected
    ImGui::SameLine();
    if (_usbMSC.xVolumeFound && ImGui::Button(labels.buttons[8].data()))
        pageId = currentPage::CHOOSE_MSC_PATH;


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

    if(_fileMsc.check())
        if(_usbMSC.copy(_fileMsc.sCurrentFile))
        {
            aFooterData.at(1) = "Files copied to " + _fileMsc.sCurrentFile;
            _usbMSC.xVolumeFound = false;
        }


}
/**
 * @brief pening a dialog which allow to decide window
 *
 */
auto WindowClass::choiceWindow(std::string_view sName, std::string_view sQuestion) -> bool
{
    auto xReturn = false;
    ImGui::OpenPopup(sName.data());

    if (ImGui::BeginPopupModal(sName.data(), nullptr, ImGuiWindowFlags_AlwaysAutoResize))
    {
        ImGui::Text("%s", sQuestion.data());
        ImGui::Spacing();

        // OK Button
        if (ImGui::Button(labels.buttons[6].data()))
        {
            ImGui::CloseCurrentPopup();
            xReturn = true;
        }
        // Cancel Button
        else if (ImGui::Button(labels.buttons[7].data()))
        {
            ImGui::CloseCurrentPopup();
            xReturn = false;
        }
    }
    ImGui::EndPopup();

    return xReturn;
}
