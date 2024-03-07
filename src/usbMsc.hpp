/**
 * @file usbMsc.hpp
 * @author Alexander Tonn
 * @brief Get file data if owon device is as mass storage device connected
 * @version 0.1
 * @date 2024-03-05
 *
 * @copyright Copyright (c) 2024
 *
 */
#ifndef USB_MSC_HPP
#define USB_MSC_HPP

#include <iostream>
#include <string>
#include <fstream>
#include <array>
#include <memory>
#include <chrono>
#include <sstream>

class usbMSC
{

public:
    auto findOwonVolume(bool xActive) -> bool;
    auto copy() -> bool;

private:
    constexpr static std::string_view sOwonVolume = "NO NAME";
    constexpr static std::string_view sSaveDir = "massStorageData";

    // create volume path
private:
    template <std::size_t size>
    auto getVolumePath(std::array<char, size> aBuffer) -> void;
    std::string sVolumePath;

private:
    auto checkMainSaveDir() -> bool;
    enum class getFile
    {
        BMP,
        CSV,
        MOVE
    };
    auto getFiles(getFile type, uint8_t uiFileNo) -> bool;
    constexpr static uint8_t MAX_FILE_COUNT = 4;

// Create subfolder for saving the files
private:
    std::string sCurrentDate;
    auto setSavePath() -> bool;
    auto getDate() -> std::string;
public:
    std::string sSavePath;


};

#endif // USB_MSC_HPP
