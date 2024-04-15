#ifndef GUISTRINGS_HPP
#define GUISTRINGS_HPP

#include <array>
#include <string>

namespace guiText
{

struct buttons
{
    std::array<std::string, 5> menu ;
    std::array<std::string, 3> fileBrowser ;
    std::array<std::string, 2> choiceWindow ;
    std::array<std::string, 1> footer ;
};

extern buttons btn;

struct labels
{
    std::array<std::string, 4> menu ;
    std::array<std::string, 2> plot ;
    std::array<std::string, 1> dialogNames;
    std::array<std::string, 1> fileBrowser ;
    std::array<std::string, 9> header ;
    std::array<std::string, 2> footer ;
};

extern labels lbl;



struct comboBoxes
{
    std::array<std::string, 1> names ; // names of comboboxes
    std::array<std::string, 2> unitY ; // entries for the y-axis unit combobox
};

extern comboBoxes cb;

struct checkBoxes
{
    std::array<std::string, 3> names ;
} ;

extern checkBoxes chkbx;

}


// TODO!: later handle here the internationalization in a separate class


#endif // GUISTRINGS_HPP
