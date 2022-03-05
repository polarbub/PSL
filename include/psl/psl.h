#pragma once

#include <chrono>
#include <thread>
#include <vector>
#include <string>

#define psl_sleep(ms) std::this_thread::sleep_for(std::chrono::milliseconds(ms))
#define pls_getArrayLength(array) sizeof(array)/sizeof(array[0])
#define psl_printVarContentsName(command) std::cout << #command << ": " << command << std::endl;

namespace psl {
    std::vector<std::string> argcvToStringVector(int argc, char** argv);
}