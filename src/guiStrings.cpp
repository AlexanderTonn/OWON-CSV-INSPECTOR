#include "guiStrings.hpp"

namespace guiText
{


guiText::buttons btn = {{"Open file", "Reset Cursors", "Reset View", "Bug Report"},
                        {"<", "OK", "Cancel"},
                        {"OK", "Cancel"},
                        {"Yes"}};

guiText::labels lbl = {{"Options", "View", "CSV", "?"},
                       {"Voltage", "Time"},
                       {"Filebrowser"},
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
                       {"No file found", "Current file: "}};

guiText::comboBoxes cb = {{"Unit"}, {"mV", "V"}};

guiText::checkBoxes chkbx = {{"Add Cursors: Voltage", "Add Cursors: Time", "Search for OWON MSC"}};

} // namespace guiText
