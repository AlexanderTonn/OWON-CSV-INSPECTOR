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

class usbMSC
{

public:
    auto findOwonVolume() -> bool;

private:
constexpr static std::string_view sOwonVolume = "NO NAME";
constexpr static std::string_view sSaveDir = "massStorageData";



private:
    auto createMainSaveDir() -> void;

};

#endif // USB_MSC_HPP
