#ifndef GUISTRINGS_HPP
#define GUISTRINGS_HPP

#include <array>
#include <string>

namespace guiText
{

struct buttons
{
    std::array<std::string, 5> menu = {"Open file", "Reset Cursors", "Reset View","Bug Report"};
    std::array<std::string, 3> fileBrowser = {"<", "OK", "Cancel"};
    std::array<std::string, 2> choiceWindow = {"OK", "Cancel"};
    std::array<std::string, 1> footer = {"Yes"};
} btn;

struct labels
{
    std::array<std::string, 4> menu = {"Options", "View", "CSV", "?"};
    std::array<std::string, 2> plot = {"Voltage", "Time"};
    std::array<std::string, 1> dialogNames = {"Filebrowser"};
    std::array<std::string, 1> fileBrowser = {"OWON Volume was found, do you want to copy files?"};
    std::array<std::string, 9> header = {"Channel",
                                      "Probe attenuation",
                                      "Peak to Peak",
                                      "Average",
                                      "Vertical pos.",
                                      "Frequency",
                                      "Period",
                                      "V per ADC Value",
                                      "Time interval"};
    std::array<std::string, 2> footer = {"No file found", "Current file: "};
} lbl;



struct comboBoxes
{
    std::array<std::string, 1> names = {"Unit"}; // names of comboboxes
    std::array<std::string, 2> unitY = {"mV", "V"}; // entries for the y-axis unit combobox
} cb;

struct checkBoxes
{
    std::array<std::string, 3> names = {"Add Cursors: Voltage", "Add Cursors: Time", "Search for OWON MSC"};
} chkbx;

} // namespace guiText


// TODO!: later handle here the internationalization in a separate class


#endif // GUISTRINGS_HPP
