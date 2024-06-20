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
#include <filesystem>
#include <cstdio>

class usbMSC
{

public:
    auto findOwonVolume(bool active) -> bool;
    auto copy(std::string stringTargetSavePath) -> bool;
    bool volumeFound = false; // if true, the owon volume is found

private:
    constexpr static std::string_view stringOwonVolume = "NO NAME";
    constexpr static std::string_view stringSaveDir = "massStorageData";

    // create volume path
private:
    template <std::size_t size>
    auto getVolumePath(std::array<char, size> arrayBuffer) -> void;
    std::string stringVolumePath;


private:
    auto createSaveDir(std::string stringInputPath) -> bool;
    enum class getFile
    {
        BMP,
        CSV,
        MOVE
    };
    auto getFiles(getFile type, uint8_t fileNo) -> bool;
    constexpr static uint8_t MAX_FILE_COUNT = 4;

// Create subfolder for saving the files
private:
    std::string stringCurrentDate;
    std::string stringSavePath;
    auto setSavePath(std::string stringInputpath) -> bool;
    auto getDate() -> std::string;

};

#endif // USB_MSC_HPP
