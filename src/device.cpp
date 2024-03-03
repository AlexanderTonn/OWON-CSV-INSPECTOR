#include "device.hpp"

auto device::getMemoryDepth(deviceType device) -> uint16_t
{
    switch (device)
    {
    case deviceType::HDS242:
        return 24000;
        break;
    case deviceType::HDS272:
        return 72000;
        break;
    case deviceType::HDS2102:
        return 1000000;
        break;
    case deviceType::HDS2202:
        return 2000000;
        break;
    default:
        return 0;
        break;
    }
}
