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
    // if file not loaded, load it
    if (xFirstCycle)
    {
        _fileHandler.path = _fileHandler.getDesktopPath();
        xFirstCycle = false;
    }


    if (!_csvHandler.xCsvLoaded)
    {
        _csvHandler.parseCSV(_csvHandler.sCurrentFile, _csvHandler.csvData);
        _csvHandler.xCsvLoaded = true;
    }

    // ##########
    // Frontend stuff
    // ##########
    ImGui::Begin(label.data(), nullptr, window_flags);


    switch (pageId)
    {
    case currentPage::MAIN:
        ImGui::SetCursorPos(ImVec2(20, 25));
        if (ImGui::Button(labelBtnOpenFile.data()))
        {
            pageId = currentPage::OPEN_FILE;
        }
        ImGui::SameLine();
        voltUnitId = drawComboboxYUnit();

        drawHeader();
        drawPlot(voltUnitId); // TODO! Crashes here


        break;
    case currentPage::OPEN_FILE:
        drawFilebrowser(_fileHandler.path);
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
void render(WindowClass &window_obj)
{
    window_obj.Draw("OWON HDS-N Series CSV Wave Inspector");
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


    if (ImPlot::BeginPlot("###Plot", ImVec2(-1,-1), ImPlotFlags_NoTitle))
    {
        ImPlot::SetupAxes(nullptr, nullptr, false, ImPlotAxisFlags_AutoFit);
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

        ImPlot::EndPlot();
    }

}

/**
 * @brief Draw the header informations of the csv file
 *
 */
auto WindowClass::drawHeader() -> void
{
    ImGui::Columns(9, "mycolumns");
    ImGui::Separator();
    ImGui::Text("Channel");
    ImGui::Text("%s", _csvHandler.extractHeaderData(_csvHandler.csvData, 0).c_str());
    ImGui::NextColumn();

    ImGui::Text("Probe attenuation");
    ImGui::Text("%s", _csvHandler.extractHeaderData(_csvHandler.csvData, 6).c_str());
    ImGui::NextColumn();

    ImGui::Text("Peak to Peak");
    ImGui::Text("%s", _csvHandler.extractHeaderData(_csvHandler.csvData, 3).c_str());
    ImGui::NextColumn();

    ImGui::Text("Average");
    ImGui::Text("%s", _csvHandler.extractHeaderData(_csvHandler.csvData, 4).c_str());
    ImGui::NextColumn();

    ImGui::Text("vertical pos.");
    ImGui::Text("%s", _csvHandler.extractHeaderData(_csvHandler.csvData, 5).c_str());
    ImGui::NextColumn();

    ImGui::Text("Frequency");
    ImGui::Text("%s", _csvHandler.extractHeaderData(_csvHandler.csvData, 1).c_str());
    ImGui::NextColumn();

    ImGui::Text("Period");
    ImGui::Text("%s", _csvHandler.extractHeaderData(_csvHandler.csvData, 2).c_str());
    ImGui::NextColumn();

    ImGui::Text("V per ADC Value");
    ImGui::Text("%s", _csvHandler.extractHeaderData(_csvHandler.csvData, 7).c_str());
    ImGui::NextColumn();

    ImGui::Text("Time interval");
    ImGui::Text("%s", _csvHandler.extractHeaderData(_csvHandler.csvData, 8).c_str());
    ImGui::NextColumn();
    ImGui::Separator();

    ImGui::Columns(1);
}
/**
 * @brief Draw the filebrowser
 *
 * @param fPath contains the path of selected item
 */
auto WindowClass::drawFilebrowser(std::filesystem::path &fPath) -> void
{
    ImGui::OpenPopup(labelFileDialog.data());

    if (ImGui::BeginPopupModal(labelFileDialog.data(), nullptr, ImGuiWindowFlags_AlwaysAutoResize))
    {
        std::string_view labelBtnBack = "<";
        std::string_view labelBtnOk = "OK";
        std::string_view labelBtnCancel = "Cancel";

        static std::string sFileName;
        // go back in file path
        if (ImGui::Button(labelBtnBack.data()))
        {
            // Check whether parent is available
            if (fPath.has_parent_path())
            {
                fPath = fPath.parent_path();
            }
        }
        ImGui::Text("Currentpath: %s", fPath.string().c_str());
        ImGui::Separator();

        //SIGSEGV here
        _csvHandler.sNewFile = _fileHandler.getContentOfPath(fPath);

        ImGui::Separator();
        if (ImGui::Button(labelBtnOk.data()))
        {
            _csvHandler.sCurrentFile = _csvHandler.sNewFile;
            _csvHandler.xCsvLoaded = false; // Allow to load new file
            pageId = currentPage::MAIN;
            ImGui::CloseCurrentPopup();
        }
        ImGui::SameLine();
        if (ImGui::Button(labelBtnCancel.data()))
        {
            pageId = currentPage::MAIN;
            ImGui::CloseCurrentPopup();
        }
    }
    ImGui::EndPopup();
}

/**
 * @brief Draw the combobox for setting up the y-axis unit
 * TODO! Crashes here if the combobox is selected
 *
 * @return voltUnit
 */
auto WindowClass::drawComboboxYUnit() -> voltUnit
{
    std::string_view label = "Y-Axis Unit";
    std::string_view items[] = {"select", "mV", "V"};
    static int item_current = 0;
    if (ImGui::BeginCombo(label.data(), items[item_current].data()))
    {
        for (int n = 0; n < IM_ARRAYSIZE(items); n++)
        {
            const bool is_selected = (item_current == n);
            if (ImGui::Selectable(items[n].data(), is_selected))
            {
                item_current = n;
                return static_cast<voltUnit>(item_current);
            }
            if (is_selected)
            {
                ImGui::SetItemDefaultFocus();
            }
        }
        ImGui::EndCombo();
    }
}
