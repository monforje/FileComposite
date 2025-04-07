#include <FileSystem.hpp>
#include <iostream>

int main()
{
    directory* root = new directory("root", "/");
    file* f1 = new file("file1.txt", "/root");
    directory* d1 = new directory("/subdir", "/root");
    file* f2 = new file("file2.txt", "/root");
    directory* d2 = new directory("/docs", "/root/subdir");
    directory* d3 = new directory("/downloads", "/root/subdir/docs");
    file* f3 = new file("file2.txt", "/root/subdir/docs");

    std::cout << "\nContents of '/':\n";
    for (auto* comp : fileSystemRegistry["/"]) {
        std::cout << " - " << comp->getType() << ": " << comp->getName() << std::endl;
    }

    std::cout << "\nContents of '/root/subdir/docs':\n";
    for (auto* comp : fileSystemRegistry["/root/subdir/docs"]) {
        std::cout << " - " << comp->getType() << ": " << comp->getName() << std::endl;
    }

    return 0;
}