#include "IostreamIoFacility.h"

#include <iostream>

namespace task_manager
{
void IostreamIoFacility::Print(const std::string& string)
{
    std::cout << string;
}

std::string IostreamIoFacility::GetLine()
{
    std::string input;
    std::getline(std::cin, input);
    return input;
}
}  // namespace task_manager
