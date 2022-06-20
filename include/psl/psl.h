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
typedef long long llong;
typedef unsigned long long ullong;
typedef unsigned char uchar;

//Prints stuff if debug is defined
#ifdef DEBUG
#define debugPrint(in) std::cout << in;
#else
#define debugPrint(in)
#endif

//Runs stuff if debug is defined
#ifdef DEBUG
#define debugRun(command) command
#else
#define debugRun(command)
#endif

#if (defined(_WIN32) || defined(__WIN32)) && !defined(WIN32)
#define WIN32
#endif

#include "image.h"

namespace psl {
    //Convert an argc and argv to a vector of strings.
    std::vector<std::string> argcvToStringVector(int &argc, char** argv);

    //My image system
    //FIX: Make this use an array not a vector by overriding the [] operator
    class Image : public std::vector<std::vector<psl::image::Color>> {
    public:
        //This can be really slow, just wait.
        void writeFile(std::string filename);
        int loadFile(std::string filename);

        Image(std::string fileName);
        Image();

        int getHeight();
        int getWidth();
    };

    //Multithreading Dispatcher
    class ThreadDispatcher;
}