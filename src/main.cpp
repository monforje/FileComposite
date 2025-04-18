#include <filesystem.hpp>
#include <iostream>

int main() {
    FileSystem fs("root", "/");

    fs.mkdir("Documents");
    fs.mkdir("Downloads");
    fs.touch("readme.txt");
    fs.ls();
    fs.tree();

    fs.cd("Documents");
    fs.pwd();
    fs.touch("report.docx");
    fs.mkdir("Photos");
    fs.ls();
    fs.tree();

    fs.cd("..");
    fs.pwd();

    fs.rm("Downloads");
    fs.ls();
    fs.tree();

    fs.cd("NotExist");

    return 0;
}