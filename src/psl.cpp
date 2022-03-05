#include <string>
#include <vector>
//#include <iostream>

#include "psl.h"

std::vector<std::string> psl::argcvToStringVector(int argc, char** argv) {
    std::vector<std::string> out;
    int i = 1;
    while(i <= argc) {
        out.emplace_back(argv[i]);
        i++;
    }
    return out;
}

