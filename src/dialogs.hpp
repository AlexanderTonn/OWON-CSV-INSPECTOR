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

    auto drawFilebrowser(std::filesystem::path &fPath,
                         std::string &sCurrent,
                         std::string &sNew,
                         bool &xNotLoaded,
                         fileHandler &_fileHandler,
                         fileHandler::contentPathOption option,
                         currentPage &page) -> void;

    auto choiceWindow(std::string sName, std::string sQuestion) -> bool;
};

#endif // DIALOGS_HPP
