#include "funcTrigger.hpp"
/**
 * @brief Fire time dependent events by passing the time in milliseconds and use the return value to trigger the event
 *
 * @param uiTriggerTime target time in milliseconds
 * @return
 */
auto funcTrigger::fire(uint32_t uiTriggerTime) -> bool
{
    if(getTimeSinceStart() - uiOldTime >= uiTriggerTime)
    {
        uiOldTime = getTimeSinceStart();
        return true;
    }
    else
        return false;
}
/**
 * @brief Get the Time Since Start object
 *
 * @return current Time stamp
 */
auto funcTrigger::getTimeSinceStart() -> int64_t
{
    return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - tStart).count();
}
