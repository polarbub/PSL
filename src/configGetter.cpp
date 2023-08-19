#include "configGetter.h"

psl::Config::Config() {
}

void psl::Config::clearOptions() {
    options.clear();
}

void psl::Config::parse(std::vector<std::string> commandLineArguments, std::string JSON) {

}

inline void psl::Config::configFilePathHandler(std::string input, void *pointer) {
    stringArgumentHandler(input, pointer);
}

void psl::Config::stringArgumentHandler(std::string input, void *pointer) {
    auto *output = reinterpret_cast<std::string *>(pointer);
    *output = input;
}


