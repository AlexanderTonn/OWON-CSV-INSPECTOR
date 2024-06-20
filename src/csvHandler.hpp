/*
Parsing the CSV File which was opened by file dialog
*/

#ifndef CSV_HANDLER_HPP
#define CSV_HANDLER_HPP

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

class csvHandler
{
private:
    /* data */
public:
    constexpr static uint16_t DATA_ROWS = 8'100;

    struct csvDataStruct {
        std::string headerName = "";
        std::string headerData = "";
        double x = 0.0;
        double y = 0.0;
        };
    csvDataStruct csvData[DATA_ROWS];

    auto parseCSV(std::string stringFilename, csvDataStruct *csvData) -> void;
    auto extractData(csvDataStruct *csvData, const double lineNo) -> double;
    auto extractHeaderData(csvDataStruct *csvData, const double lineNo) -> std::string;
    auto debugPrint(std::vector<std::vector<std::string>> vector) -> void;

    // TODO: scale the vertical and horizontal data
    // TODO: for this you need to know the resolution of the scope
    // TODO: -> HDS242 = 8bit vertical resolution
    // TODO: -> HDS272 = 8bit vertical resolution
    // TODO: -> HDS2102 = 8bit vertical resolution
    // TODO: -> HDS2202 = 14bit vertical resolution




};



#endif // CSV_HANDLER_HPP
