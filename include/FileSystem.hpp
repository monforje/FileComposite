#pragma once
#ifndef FILESYSTEM_HPP
#define FILESYSTEM_HPP

#include <string>
#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>

// Scoped enum для типа компонента файловой системы
enum class ComponentType {
    File,
    Directory
};

// Scoped enum для типа файла
enum class FileType {
    txt,
    docx,
    pptx,
    py,
    cpp
};

// Функция для получения строкового представления расширения файла.
// Объявлена inline, чтобы избежать проблем множественного определения.
inline std::string FileTypeToString(FileType type) {
    switch (type) {
    case FileType::txt:   return ".txt";
    case FileType::docx:  return ".docx";
    case FileType::pptx:  return ".pptx";
    case FileType::py:    return ".py";
    case FileType::cpp:   return ".cpp";
    default:              return "unknown";
    }
}

// Предварительное объявление класса Directory для использования в FileSystemComponent
class Directory;

// Абстрактный базовый класс файловой системы (общий интерфейс для всех компонентов)
class FileSystemComponent {
protected:
    std::string name;
    std::string path;
    // Родительская директория (если есть)
    std::weak_ptr<Directory> parent;
public:
    FileSystemComponent(const std::string& name, const std::string& path)
        : name(name), path(path) {
    }

    virtual ~FileSystemComponent() {}

    // Чисто виртуальная функция, возвращающая тип компонента
    virtual ComponentType getComponentType() const = 0;

    // Чисто виртуальный метод для отображения структуры с заданным отступом (для debug)
    virtual void display(int indent = 0) const = 0;

    const std::string& getName() const { return name; }
    const std::string& getPath() const { return path; }

    // Устанавливает родительскую директорию
    void setParent(const std::shared_ptr<Directory>& p) { parent = p; }

    // Возвращает родительскую директорию, если она существует
    std::shared_ptr<Directory> getParent() const { return parent.lock(); }
};

// Класс File – лист композиции (конкретный файл)
class File : public FileSystemComponent {
private:
    FileType fileType;
public:
    // Статический метод для определения типа файла по расширению.
    // Если расширение не найдено или неизвестно, возвращается FileType::txt.
    static FileType parseFileExtension(const std::string& fileName) {
        size_t pos = fileName.rfind('.');
        if (pos == std::string::npos)
            return FileType::txt;
        std::string ext = fileName.substr(pos);
        if (ext == ".txt")   return FileType::txt;
        if (ext == ".docx")  return FileType::docx;
        if (ext == ".pptx")  return FileType::pptx;
        if (ext == ".py")    return FileType::py;
        if (ext == ".cpp")   return FileType::cpp;
        return FileType::txt;
    }

    // Конструктор, автоматически определяющий FileType по имени.
    File(const std::string& name, const std::string& path)
        : FileSystemComponent(name, path), fileType(parseFileExtension(name)) {
        std::cerr << "[DEBUG] File created: " << name
            << " | Path: " << path
            << " | Extension: " << FileTypeToString(fileType) << std::endl;
    }

    // Конструктор с явным указанием FileType.
    File(const std::string& name, const std::string& path, FileType type)
        : FileSystemComponent(name, path), fileType(type) {
        std::cerr << "[DEBUG] File created: " << name
            << " | Path: " << path
            << " | Extension: " << FileTypeToString(fileType) << std::endl;
    }

    ComponentType getComponentType() const override { return ComponentType::File; }
    FileType getFileType() const { return fileType; }

    // Отображение информации о файле с заданным отступом.
    void display(int indent = 0) const override {
        for (int i = 0; i < indent; i++) std::cout << "  ";
        std::cout << "[File] " << name << " (" << path << ") "
            << FileTypeToString(fileType) << std::endl;
    }
};

// Класс Directory – композит, представляющий директорию, которая может содержать дочерние компоненты
class Directory : public FileSystemComponent, public std::enable_shared_from_this<Directory> {
private:
    std::vector<std::shared_ptr<FileSystemComponent>> children;
public:
    Directory(const std::string& name, const std::string& path)
        : FileSystemComponent(name, path) {
        std::cerr << "[DEBUG] Directory created: " << name
            << " | Path: " << path << std::endl;
    }

    ComponentType getComponentType() const override { return ComponentType::Directory; }

    // Добавление дочернего компонента; при добавлении устанавливается родительский указатель.
    void add(const std::shared_ptr<FileSystemComponent>& comp) {
        children.push_back(comp);
        comp->setParent(shared_from_this());
        std::cerr << "[DEBUG] Added component '" << comp->getName()
            << "' to directory '" << name << "'" << std::endl;
    }

    // Удаление дочернего компонента.
    void remove(const std::shared_ptr<FileSystemComponent>& comp) {
        auto it = std::remove(children.begin(), children.end(), comp);
        if (it != children.end()) {
            std::cerr << "[DEBUG] Removed component '" << comp->getName()
                << "' from directory '" << name << "'" << std::endl;
        }
        children.erase(it, children.end());
    }

    const std::vector<std::shared_ptr<FileSystemComponent>>& getChildren() const {
        return children;
    }

    // Отображение структуры директории и её дочерних компонентов с отступами.
    void display(int indent = 0) const override {
        for (int i = 0; i < indent; i++) std::cout << "  ";
        std::cout << "[Directory] " << name << " (" << path << ")" << std::endl;
        for (const auto& child : children)
            child->display(indent + 1);
    }
};

// Класс FileSystem – оболочка для работы с файловой системой.
// Здесь реализованы команды cd, ls, mkdir, touch, rm.
// Все действия сопровождаются отладочным выводом для контроля формирования путей и операций.
class FileSystem {
private:
    std::shared_ptr<Directory> root;
    std::shared_ptr<Directory> current;

    // Вспомогательный метод для поиска дочернего компонента по имени в данной директории.
    std::shared_ptr<FileSystemComponent> findChild(const std::shared_ptr<Directory>& dir, const std::string& name) {
        for (auto& child : dir->getChildren()) {
            if (child->getName() == name)
                return child;
        }
        return nullptr;
    }
public:
    // Конструктор, создающий корневую директорию.
    // Корневой путь передается явно (например, "/") и имя может быть, например, "root".
    FileSystem(const std::string& rootName, const std::string& rootPath) {
        root = std::make_shared<Directory>(rootName, rootPath);
        current = root;
        std::cerr << "[DEBUG] FileSystem initialized with root: " << rootName
            << " | Path: " << rootPath << std::endl;
    }

    // Команда cd: изменение текущей директории.
    // Поддерживаются: абсолютный путь ("/"), относительное имя каталога и переход на уровень выше ("..").
    void cd(const std::string& path) {
        if (path == "/" || path == root->getName()) {
            current = root;
            std::cerr << "[DEBUG] Changed directory to root: " << current->getPath() << std::endl;
        }
        else if (path == "..") {
            if (auto parent = current->getParent()) {
                current = parent;
                std::cerr << "[DEBUG] Moved up to directory: " << current->getPath() << std::endl;
            }
            else {
                std::cerr << "[DEBUG] Already at root directory, cannot move up." << std::endl;
            }
        }
        else {
            auto child = findChild(current, path);
            if (child && child->getComponentType() == ComponentType::Directory) {
                current = std::dynamic_pointer_cast<Directory>(child);
                std::cerr << "[DEBUG] Changed directory to: " << current->getPath() << std::endl;
            }
            else {
                std::cerr << "[DEBUG] Directory not found: " << path << std::endl;
            }
        }
    }

    // Команда ls: вывод содержимого текущей директории.
    void ls() const {
        std::cout << "Listing directory: " << current->getName() << std::endl;
        for (const auto& child : current->getChildren())
            std::cout << child->getName() << " ";
        std::cout << std::endl;
        std::cerr << "[DEBUG] ls executed in directory: " << current->getPath() << std::endl;
    }

    // Команда mkdir: создание новой директории в текущей директории.
    void mkdir(const std::string& name) {
        std::string newPath = (current->getPath() == "/" ? current->getPath() : current->getPath() + "/") + name;
        auto newDir = std::make_shared<Directory>(name, newPath);
        current->add(newDir);
        std::cerr << "[DEBUG] mkdir: Created directory '" << name
            << "' | Path: " << newPath << std::endl;
    }

    // Команда touch: создание нового файла в текущей директории.
    void touch(const std::string& name) {
        std::string newPath = (current->getPath() == "/" ? current->getPath() : current->getPath() + "/") + name;
        auto newFile = std::make_shared<File>(name, newPath);
        current->add(newFile);
        std::cerr << "[DEBUG] touch: Created file '" << name
            << "' | Path: " << newPath << std::endl;
    }

    // Команда rm: удаление компонента (файла или директории) по имени из текущей директории.
    void rm(const std::string& name) {
        auto child = findChild(current, name);
        if (child) {
            current->remove(child);
            std::cerr << "[DEBUG] rm: Removed component '" << name
                << "' from directory: " << current->getPath() << std::endl;
        }
        else {
            std::cerr << "[DEBUG] rm: Component not found: " << name << std::endl;
        }
    }

    // Вывод абсолютного пути текущей директории.
    void pwd() const {
        std::cout << "Current directory: " << current->getPath() << std::endl;
        std::cerr << "[DEBUG] pwd: " << current->getPath() << std::endl;
    }

    // Вывод всей структуры файловой системы.
    void tree() const {
        std::cout << "File system structure:" << std::endl;
        root->display();
        std::cerr << "[DEBUG] tree: Displayed file system structure." << std::endl;
    }
};

#endif // FILESYSTEM_HPP