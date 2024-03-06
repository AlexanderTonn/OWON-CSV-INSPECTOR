/**
 * @file funcTrigger.hpp
 * @author Alexander Tonn
 * @brief This is a cpp implementation of my function trigger which i also use for Arduino
 * @brief https://github.com/AlexanderTonn/MyArduinoSnippets/blob/main/src/myFunctionTrigger.cpp
 * @AlexanderTonn Probably this is a better solution, if yes change it also in Arduino Snippets repo
 * @note For using this class you have to create instances of the class each time you want to use the trigger
 * @version 0.1
 * @date 2024-03-06
 *
 * @copyright Copyright (c) 2024
 *
 */
#ifndef FUNC_TRIGGER_HPP
#define FUNC_TRIGGER_HPP

#include <chrono>



class funcTrigger
{
public:
    auto fire(uint32_t uiTriggerTime) -> bool;

private:
    std::chrono::time_point<std::chrono::high_resolution_clock> tStart;
    auto getTimeSinceStart() -> int64_t;
    int64_t uiOldTime = 0; // Save the olt time for cyclic triggering

};




#endif // FUNC_TRIGGER_HPP
