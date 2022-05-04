#pragma once

#include <chrono>
#include <thread>
#include <vector>
#include <string>

//Sleep for ms
#define psl_sleep(ms) std::this_thread::sleep_for(std::chrono::milliseconds(ms))
//Get the length of a normal array
#define pls_getArrayLength(array) sizeof(array)/sizeof(array[0])
//Print the name and contents of a variable, very useful for debugging
#define psl_printVarContentsName(command) std::cout << #command << ": " << command << std::endl;
//This allows for helper functions
//Any function that would normally be in main can be wrapped in this and a 'return -1;' at the bottom, and it will function the same as if it were in main.
#define psl_helperFunctionRunner(INT) {int output = INT; if(output != -1) return output;}
//Longer long abbreviation
#define llong long long
//Unsigned longer long abbreviation
#define ullong unsigned long long
#define uchar unsigned char


namespace psl {
    //Convert an argc and argv to a vector of strings.
    std::vector<std::string> argcvToStringVector(int &argc, char** argv);

    //My image system
    class Image;
}