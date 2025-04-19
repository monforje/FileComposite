#include "filesystem.hpp"
#include <iostream>
#include <sstream>

int main() {
    FileSystem fs("root", "/");

    std::string input;
    std::cout << "> ";
    while (std::getline(std::cin, input)) 
    {
        std::istringstream iss(input);
        std::string command, arg;
        iss >> command;
        std::getline(iss, arg);
        if (!arg.empty() && arg[0] == ' ') arg = arg.substr(1);

        if (command == "mkdir") 
        {
            fs.mkdir(arg);
        } 
        else if (command == "touch") 
        {
            fs.touch(arg);
        } 
        else if (command == "ls") 
        {
            fs.ls();
        } 
        else if (command == "cd") 
        {
            fs.cd(arg);
        } 
        else if (command == "pwd") 
        {
            fs.pwd();
        } 
        else if (command == "rm") 
        {
            fs.rm(arg);
        } 
        else if (command == "exit") 
        {
            break;
        } 
        else 
        {
            std::cerr << "command not found: " << command << std::endl;
        }

        std::cout << "> ";
    }

    return 0;
}