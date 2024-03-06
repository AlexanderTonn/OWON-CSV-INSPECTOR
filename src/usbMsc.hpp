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
    auto moveFiles() -> bool;

private:
    constexpr static std::string_view sOwonVolume = "NO NAME";
    constexpr static std::string_view sSaveDir = "massStorageData";

    // Save volume path
private:
    template <std::size_t size>
    auto getVolumePath(std::array<char, size> aBuffer) -> void;
    std::string sVolumePath;



private:
    auto checkMainSaveDir() -> void;
    auto getBMPFiles() -> void;
    auto getCSVFiles() -> void;

// Create subfolder for saving the files
private:
    std::string sSubDir;
    std::string sCurrentDate;
    auto createSubDir() -> bool;
    auto getDate() -> std::string;


};

#endif // USB_MSC_HPP
