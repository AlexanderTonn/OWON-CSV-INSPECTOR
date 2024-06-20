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
auto Dialogs::drawFilebrowser(std::filesystem::path &path,
                                  std::string &stringCurrent,
                                  std::string &stringNew,
                                  bool &notLoaded,
                                  fileHandler &_fileHandler,
                                  fileHandler::contentPathOption option,
                                  currentPage &page) -> void
{
    ImGui::OpenPopup(guiText::lbl.dialogNames.at(0).c_str());

    if (ImGui::BeginPopupModal(guiText::lbl.dialogNames.at(0).c_str(), nullptr, ImGuiWindowFlags_AlwaysAutoResize))
    {
        // go back in file path
        if (ImGui::Button(guiText::btn.fileBrowser.at(0).c_str()))
        {
            // Check whether parent is available
            if (path.has_parent_path() && path.parent_path() != path.root_path())
                path = path.parent_path();
        }
        ImGui::Text("Current path: %s", path.string().c_str());
        ImGui::Separator();

        stringNew = _fileHandler.getContentOfPath(path, option);

        ImGui::Separator();
        // OK Btn pressed
        if (ImGui::Button(guiText::btn.fileBrowser.at(1).c_str()))
        {
            stringCurrent = stringNew;
            notLoaded = false; // Allow to load new file
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
auto Dialogs::drawChoiceWindow(std::string stringName, std::string stringQuestion) -> bool
{
    auto ret = false;
    ImGui::OpenPopup(stringName.c_str());

    if (ImGui::BeginPopupModal(stringName.c_str(), nullptr, ImGuiWindowFlags_AlwaysAutoResize))
    {
        ImGui::Text("%s", stringQuestion.c_str());
        ImGui::Spacing();

        // OK Button
        if (ImGui::Button(guiText::btn.choiceWindow.at(0).c_str()))
        {
            ImGui::CloseCurrentPopup();
            ret = true;
        }
        // Cancel Button
        else if (ImGui::Button(guiText::btn.choiceWindow.at(1).c_str()))
        {
            ImGui::CloseCurrentPopup();
            ret = false;
        }
    }
    ImGui::EndPopup();

    return ret;
}
