/**
 * @file usbHid.hpp
 * @author Alexander Tonn
 * @brief Handling the USB HID connection (data stream)
 * @version 0.1
 * @date 2024-03-05
 *
 * @copyright Copyright (c) 2024
 *
 */
#ifndef USB_HID_HPP
#define USB_HID_HPP

#include "libusb-1.0/libusb.h"
#include <iostream>

class usbHID
{
private:
    libusb_context *context = nullptr; // libusb context
    libusb_device **deviceList = nullptr; // list of devices found
    libusb_device_handle *deviceHandle = nullptr; // device handle if owon device is found
    ssize_t deviceCount = 0; // amount of devices found
    constexpr static auto OWON_VENDOR_ID = 0x5345; // Vendor ID
    constexpr static auto OWON_PRODUCT_ID = 0x1234; // Product ID

public:
    enum class debugLevel
    {
        NONE, // no debug messages
        ERROR_MSG, // only error messages
        WARN_ERROR_MSG, // error and warning messages
        EXTENDED_MSG, // error, warning and informative messages
        ALL_MSG, // all messages
    };

    auto Begin(debugLevel lvl) -> bool;
    auto End() -> void;
    auto connectionOpen() -> bool;

private:
    auto getDeviceList() -> int;


};




#endif // USB_HID_HPP
