#include "filesystem.hpp"
#include <iostream>

int main() {
    // Инициализируем файловую систему с корневой директорией "root" и путем "/"
    FileSystem fs("root", "/");

    // Демонстрация создания директорий и файлов
    fs.mkdir("Documents");    // создается "/Documents"
    fs.mkdir("Downloads");    // создается "/Downloads"
    fs.touch("readme.txt");   // создается "/readme.txt"
    fs.ls();
    fs.tree();

    // Переходим в директорию Documents
    fs.cd("Documents");       // теперь текущая директория: "/Documents"
    fs.pwd();
    fs.touch("report.docx");  // создается "/Documents/report.docx"
    fs.mkdir("Photos");       // создается "/Documents/Photos"
    fs.ls();
    fs.tree();

    // Возвращаемся на уровень выше (в корень)
    fs.cd("..");
    fs.pwd();

    // Удаляем директорию Downloads
    fs.rm("Downloads");
    fs.ls();
    fs.tree();

    // Попытка перейти в несуществующую директорию
    fs.cd("NotExist");

    return 0;
}