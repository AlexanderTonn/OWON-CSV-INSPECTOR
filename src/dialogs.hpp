#ifndef DIALOGS_HPP
#define DIALOGS_HPP

#include <filesystem>
#include <string>

#include "fileHandler.hpp"
#include "guiStrings.hpp"


class Dialogs
{


public:
    enum class currentPage
    {
        MAIN,
        OPEN_CSV_FILE,
        CHOOSE_MSC_PATH,
        CHOICE_WINDOW
    };

    auto drawFilebrowser(std::filesystem::path &path,
                         std::string &stringCurrent,
                         std::string &stringNew,
                         bool &notLoaded,
                         fileHandler &_fileHandler,
                         fileHandler::contentPathOption option,
                         currentPage &page) -> void;

    auto drawChoiceWindow(std::string stringName, std::string stringQuestion) -> bool;
    auto drawMsDeviceSelector() -> void;
};

#endif // DIALOGS_HPP
