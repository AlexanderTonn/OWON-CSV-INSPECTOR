#include "usbHid.hpp"

/**
 * @brief Start the usb connection and set the debug level
 *
 * @param lvl - enum class debugLevel
 * @return true - if the usb connection was successful otherwise false
 */
auto usbHID::Begin(debugLevel lvl) -> bool
{
    // Init usb connection, if no success return error
    if (libusb_init(&context) < 0)
        return false;

    else
        libusb_set_debug(context, static_cast<int>(lvl)); // set debuglvl
    return true;
}
/**
 * @brief Free the memory and close the usb device
 *
 */
auto usbHID::End() -> void
{
    libusb_free_device_list(deviceList, 1); // free memory and close usb device (1)
    libusb_exit(context);                   // exit libusb
}
/**
 * @brief Get the device amount
 *
 * @return
 */
auto usbHID::getDeviceList() -> int
{
    // get the device list
    deviceCount = libusb_get_device_list(context, &deviceList);

    try
    {
        if (deviceCount <= 0)
            std::cerr << "Error: " << libusb_error_name(static_cast<int>(deviceCount)) << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }

    return static_cast<int>(deviceCount);
}

auto usbHID::connectionOpen() -> bool
{
    if (!getDeviceList())
        return false;
    else
    {
        libusb_device_descriptor descriptor;

        // go through all devices and check if the owon device is found
        for (ssize_t i = 0; i < deviceCount; i++)
        {
            libusb_device *device = deviceList[i];

            // failed to get device descriptor
            if (libusb_get_device_descriptor(deviceList[i], &descriptor) < 0)
                return false;
            else
            {
                if (descriptor.idProduct == OWON_PRODUCT_ID && descriptor.idVendor == OWON_VENDOR_ID)
                {
                    // -1 == Memory could not be allocated
                    // -3 == No permission to access the device
                    // -4 == Device was removed
                    if (libusb_open(device, &deviceHandle) < 0)
                        return false;
                        // TODO! Don't forget closing the device
                    else
                        return true;
                }
            }
        }
    }
}
