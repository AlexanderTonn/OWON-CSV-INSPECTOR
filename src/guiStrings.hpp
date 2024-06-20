#ifndef GUISTRINGS_HPP
#define GUISTRINGS_HPP

#include <array>
#include <string>

class guiTexts
{

public:

enum languages
{
    ENG,
    GER
};

struct buttons
{
    std::array<std::string, 5> menu ;
    std::array<std::string, 3> fileBrowser ;
    std::array<std::string, 2> choiceWindow ;
    std::array<std::string, 1> footer ;
};
struct labels
{
    std::array<std::string, 4> menu ;
    std::array<std::string, 2> plot ;
    std::array<std::string, 2> dialogNames;
    std::array<std::string, 2> fileBrowser ;
    std::array<std::string, 9> header ;
    std::array<std::string, 2> footer ;
};
struct comboBoxes
{
    std::array<std::string, 1> names ; // names of comboboxes
    std::array<std::string, 2> unitY ; // entries for the y-axis unit combobox
};
struct checkBoxes
{
    std::array<std::string, 3> names ;
} ;

std::array<buttons,sizeof(languages)> btn;
std::array<labels,sizeof(languages)> lbl;
std::array<comboBoxes,sizeof(languages)> cb;
std::array<checkBoxes,sizeof(languages)> chkbx;

auto init() -> void;

};

extern guiTexts _guiTexts;
extern size_t languageSelection ; // 0 = English, 1 = German

// TODO!: later handle here the internationalization in a separate class


#endif // GUISTRINGS_HPP
