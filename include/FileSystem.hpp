#pragma once
#ifndef FILESYSTEM_HPP
#define FILESYSTEM_HPP

#include <string>
#include <iostream>
#include <vector>
#include <algorithm>

enum class ComponentType { File, Directory };

enum class FileType
{
    txt, docx, pptx, py, cpp
};

std::string FileTypeToString(FileType type)
{
    switch (type) {
    case FileType::txt: return ".txt";
    case FileType::docx: return ".docx";
    case FileType::pptx: return ".pptx";
    case FileType::py: return ".py";
    case FileType::cpp: return ".cpp";
    default: return "unknown";
    }
}

class Directory;

class FileSystemComponent
{
protected:
    std::string name;
    std::string path;
    Directory* parent = nullptr;

public:
    FileSystemComponent(const std::string& name, const std::string& path) : name(name), path(path) {}
    virtual ~FileSystemComponent() = default;

    virtual ComponentType getComponentType() const = 0;

    const std::string& getName() const { return name; }
    const std::string& getPath() const { return path; }

    void setParent(Directory* p) { parent = p; }
    Directory* getParent() const { return parent; }
};

class File : public FileSystemComponent
{
private:
    FileType fileType;

    FileType parseFileExtension(const std::string& fileName)
    {
        size_t pos = fileName.rfind('.');
        if (pos == std::string::npos) return FileType::txt;

        std::string ext = fileName.substr(pos);
        if (ext == ".txt") return FileType::txt;
        if (ext == ".docx") return FileType::docx;
        if (ext == ".pptx") return FileType::pptx;
        if (ext == ".py") return FileType::py;
        if (ext == ".cpp") return FileType::cpp;

        return FileType::txt;
    }

public:
    File(const std::string& name, const std::string& path) : FileSystemComponent(name, path), fileType(parseFileExtension(name)) {}

    ComponentType getComponentType() const override { return ComponentType::File; }
    FileType getFileType() const { return fileType; }
};

class Directory : public FileSystemComponent
{
private:
    std::vector<FileSystemComponent*> kids;

public:
    Directory(const std::string& name, const std::string& path) : FileSystemComponent(name, path) {}

    ~Directory() override
    {
        for (auto* child : kids)
            delete child;
    }

    ComponentType getComponentType() const override { return ComponentType::Directory; }

    void add(FileSystemComponent* comp)
    {
        kids.push_back(comp);
        comp->setParent(this);
    }

    void remove(FileSystemComponent* comp) {
        auto it = std::find(kids.begin(), kids.end(), comp);
        if (it != kids.end()) 
        {
            kids.erase(it);
            delete comp;
        }
    }

    const std::vector<FileSystemComponent*>& getkids() const { return kids; }
};

class FileSystem
{
private:
    Directory* root = nullptr;
    Directory* current = nullptr;

    FileSystemComponent* findChild(Directory* dir, const std::string& name)
    {
        for (auto* child : dir->getkids())
        {
            if (child->getName() == name)
                return child;
        }
        return nullptr;
    }

public:
    FileSystem(const std::string& rootName, const std::string& rootPath)
    {
        root = new Directory(rootName, rootPath);
        current = root;
    }

    ~FileSystem()
    {
        delete root;
    }

    void cd(const std::string& path)
    {
        if (path == "/" || path == root->getName())
        {
            current = root;
            return;
        }

        if (path == "..")
        {
            if (auto* parent = current->getParent())
            {
                current = parent;
            }
            return;
        }

        auto* child = findChild(current, path);
        if (child && child->getComponentType() == ComponentType::Directory)
        {
            current = static_cast<Directory*>(child);
        }
    }

    void ls() const
    {
        for (auto* child : current->getkids())
        {
            std::cout << child->getName() << "\n";
        }
    }

    void mkdir(const std::string& name)
    {
        std::string newPath;

        if (current->getPath() == "/") {
            newPath = current->getPath() + name;
        } else {
            newPath = current->getPath() + "/" + name;
        }

        Directory* newDir = new Directory(name, newPath);
        current->add(newDir);
    }

    void touch(const std::string& name)
    {
        std::string newPath;
        
        if (current->getPath() == "/") {
            newPath = "/" + name;
        } else {
            newPath = current->getPath() + "/" + name;
        }
        File* newFile = new File(name, newPath);
        current->add(newFile);
    }

    void rm(const std::string& name)
    {
        auto* child = findChild(current, name);
        if (child)
        {
            current->remove(child);
        }
    }

    void pwd() const
    {
        std::cout << current->getPath() << std::endl;
    }
};

#endif // FILESYSTEM_HPP