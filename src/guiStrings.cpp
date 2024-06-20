#include "guiStrings.hpp"
guiTexts _guiTexts;
size_t languageSelection ;

auto guiTexts::init() -> void
{
    languageSelection = 0;

    btn[ENG] = {{"Open file", "Reset Cursors", "Reset View", "Bug Report"},
                {"<", "OK", "Cancel"},
                {"OK", "Cancel"},
                {"Yes"}};

    lbl[ENG] = {{"Options", "View", "CSV", "?"},
                {"Voltage", "Time"},
                {"Filebrowser", "Mass Storage Device Selector"},
                {"OWON Volume was found, do you want to copy files?"},
                {"Channel",
                 "Probe attenuation",
                 "Peak to Peak",
                 "Average",
                 "Vertical pos.",
                 "Frequency",
                 "Period",
                 "V per ADC Value",
                 "Time interval"},
                {"No file", "Current file: "}};

    cb[ENG] = {{"Unit"}, {"mV", "V"}};

    chkbx[ENG] = {{"Add Cursors: Voltage", "Add Cursors: Time", "Search for OWON MSC"}};

}
