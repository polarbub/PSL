#pragma once

#include <functional>
#include "string"
#include "vector"
#include "any"

//See this project for basis: https://github.com/CLIUtils/CLI11
//Do I want subcommands?
namespace psl {
    class Config {
    public:
        enum SettableFrom {
            commandLine,
            JSON,
            JSONThenCommandLine,
            CommandLineThenJson
        };

    private:
        struct RegisterOptionArguments {
            //The description shown in the help text
            std::string description = "";
            //The one-character abbreviation that can be invoked from the command line by using a hyphen. Ex. '-I'
            char abbreviation = 0;
            //The place(s) and priority where this option can be set
            SettableFrom settableFrom = commandLine;
            //Sets if this option is required to be set for the program to start
            bool required = false;
            //Sets if this option takes an argument
            //bool takesArgument = false;
            //First string is argument name, the second one is for the description. This is only used for help text generation
            std::vector<std::pair<std::string, std::string>> arguments;
            //Sets if this is the argument that specifies the location of the JSON config file.
            bool configFilePath = false;
        };
        struct Option : public RegisterOptionArguments {
            //ToBeFilledType* toBeFilledType;
            void* varToFill;
            std::function<void(std::string, void*)> fromStringConverterFunction;
            std::string name;
        };

        std::vector<Option> options;
        //std::vector<std::any> varsToFill;
    public:
        Config();

        //ADD: Function to take argv and argc instead of a vector
        void parse(std::vector<std::string> commandLineArguments, std::string JSON);

        template<typename ToBeFilledType>
        //This had to be called registerOption because register is taken
        void registerOption(ToBeFilledType* varToFill /*A pointer to the variable to put the parsed data*/,
                            std::string name /*The long name of the option. This argument can be invoked from the command line by prefacing its name with two hyphens. Ex. '--name' */,
                            RegisterOptionArguments selectedOptions,
                            std::function<void(std::string, void*)> fromStringConverterFunction /*The function that is used to turn the string argument into the data type. It can be set to a custom lambda or function for unsupported types, or it can be set to a
                            default function for some basic types*/) {
            void* varToFillAddress = reinterpret_cast<void*>(varToFill);
            Option option = {{selectedOptions.description, selectedOptions.abbreviation, selectedOptions.settableFrom, selectedOptions.required, selectedOptions.arguments, selectedOptions.configFilePath}, varToFillAddress, fromStringConverterFunction, name};
            options.push_back(option);
        }

        void clearOptions();

        static void stringArgumentHandler(std::string input, void* pointer);
        static inline void configFilePathHandler(std::string input, void* pointer);
    };
}