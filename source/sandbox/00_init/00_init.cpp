#include <iostream>

#include "utils/log.hpp"

int main() {
    Airwave::Log::Init();

    std::cout << "Hello, AirwaveEngine!" << std::endl;

    return 0;
}