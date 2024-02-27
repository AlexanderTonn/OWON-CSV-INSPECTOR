/**
 * @file csvHandler.cpp
 * @author Alexander Tonn
 * @brief Handling the CSV File
 * @version 0.1
 * @date 2024-02-28
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "csvHandler.hpp"

/**
 * @brief Parsing the CSV File which was opened by file dialog
 * The CSV file is parsed into a struct
 * the first 11 lines are going to be parsed as std::string
 * the rest of the file is going to be parsed as float for the waveform data
 * @param sFilename
 * @param data
 */
auto csvHandler::parseCSV(std::string sFilename, csvDataStruct *data) -> void
{
    std::ifstream file(sFilename);

    if (!file.is_open())
    {
        throw std::runtime_error("Could not open file");
    }

    // Reading whole line
    std::string line;
    uint16_t lineCount = 0;
    while(std::getline(file,line))
    {

        // Reading single line
        std::stringstream sStream(line);
        std::string token;
        // Parse the first lines as std::string for header Data
        if(lineCount < 11)
        {
            std::getline(sStream,token,',');
            data[lineCount].headerName = (token);
            std::getline(sStream,token,',');
            data[lineCount].headerData = token;
            lineCount++;
        }else
        {
            // Parse the first column as float
            std::getline(sStream,token,',');
            data[lineCount].x = std::stod(token);
            // Parse the second column
            std::getline(sStream,token,',');
            std::string s = token;
            data[lineCount].y = std::stod(token);
            lineCount++;

        }
    }
}


/**
 * @brief extracting the waveform data
 *
 * @param data
 * @param lineNo have to be double because of the ImPlot API ( the passed const double is used for the x-axis)
 * @return
 */
auto csvHandler::extractData(csvDataStruct *data, const double lineNo) -> double
{
    return data[static_cast<uint16_t>(lineNo)].y;
}

/**
 * @brief extracting the waveform informations (dividor settings, voltages and so on);
 *
 * @param data
 * @param lineNo lineNo have to be double, because of the ImPlot API ( the passed const double is used for the x-axis)
 * @return
 */
auto csvHandler::extractHeaderData(csvDataStruct *data, const double lineNo) -> std::string
{
    return data[static_cast<uint16_t>(lineNo)].headerData;
}

/**
 * @brief Debug print function for the 2D vector
 *
 * @param vector
 */
auto csvHandler::debugPrint(std::vector<std::vector<std::string>> vector) -> void
{
    for (auto &row : vector)
    {
        for (auto &field : row)
        {
            std::cout << field << " ";
        }
        std::cout << std::endl;
    }
}
