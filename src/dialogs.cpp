#include "dialogs.hpp"

/**
 * @brief draw the filebrowser
 *
 * @param fPath contains the complete path of selected item
 * @param sCurrent string path of the current file
 * @param sNew string path of the new file
 * @param xNotLoaded - true, if the file is not loaded
 * @param _fileHandler instance of fileHandler
 * @param option contentPathOption
 * @param page reference to currentPage
 * @note use the xNoatLoaded for proceeding the file once
 */
auto Dialogs::drawFilebrowser(std::filesystem::path &fPath,
                                  std::string &sCurrent,
                                  std::string &sNew,
                                  bool &xNotLoaded,
                                  fileHandler &_fileHandler,
                                  fileHandler::contentPathOption option,
                                  currentPage &page) -> void
{
    ImGui::OpenPopup(guiText::lbl.dialogNames.at(0).c_str());

    if (ImGui::BeginPopupModal(guiText::lbl.dialogNames.at(0).c_str(), nullptr, ImGuiWindowFlags_AlwaysAutoResize))
    {

        static std::string sFileName;
        // go back in file path
        if (ImGui::Button(guiText::btn.fileBrowser.at(0).c_str()))
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
        if (ImGui::Button(guiText::btn.fileBrowser.at(1).c_str()))
        {
            sCurrent = sNew;
            xNotLoaded = false; // Allow to load new file
            page = currentPage::MAIN;
            ImGui::CloseCurrentPopup();
        }
        ImGui::SameLine();

        // Cancel Btn pressed
        if (ImGui::Button(guiText::btn.fileBrowser.at(2).c_str()))
        {
            page = currentPage::MAIN;
            ImGui::CloseCurrentPopup();
        }
    }
    ImGui::EndPopup();
}

/**
 * @brief pening a dialog which allow to decide window
 *
 */
auto Dialogs::choiceWindow(std::string sName, std::string sQuestion) -> bool
{
    auto xReturn = false;
    ImGui::OpenPopup(sName.c_str());

    if (ImGui::BeginPopupModal(sName.c_str(), nullptr, ImGuiWindowFlags_AlwaysAutoResize))
    {
        ImGui::Text("%s", sQuestion.c_str());
        ImGui::Spacing();

        // OK Button
        if (ImGui::Button(guiText::btn.choiceWindow.at(0).c_str()))
        {
            ImGui::CloseCurrentPopup();
            xReturn = true;
        }
        // Cancel Button
        else if (ImGui::Button(guiText::btn.choiceWindow.at(1).c_str()))
        {
            ImGui::CloseCurrentPopup();
            xReturn = false;
        }
    }
    ImGui::EndPopup();

    return xReturn;
}
