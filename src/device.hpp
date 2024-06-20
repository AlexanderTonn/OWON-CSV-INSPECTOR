/**
 * @file device.hpp
 * @author Alexander Tonn
 * @brief This class handling device settings
 * @version 0.1
 * @date 2024-03-03
 *
 * @copyright Copyright (c) 2024
 *
 */
#ifndef DEVICE_HPP
#define DEVICE_HPP

#include <cstdint>

class device
{
private:

public:

    enum class deviceType
    {
        HDS242,
        HDS272,
        HDS2102,
        HDS2202
    };
    uint16_t memoryDepth = 0; // contains the memory depth of the device (CH1 operation)

private:
    auto getMemoryDepth(deviceType device) -> uint16_t;


};


#endif // DEVICE_HPP
