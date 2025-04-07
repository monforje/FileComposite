#pragma once
#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <map>
#include <set>
#include <iostream>
#include <string>

class fileSystemComponent {
protected:
    std::string componentName;
    std::string componentPath;
public:
    fileSystemComponent(const std::string& componentName, const std::string& componentPath)
        : componentName(componentName), componentPath(componentPath) {}

    virtual ~fileSystemComponent() {}

    bool operator<(const fileSystemComponent& other) const {
        return std::tie(componentName, componentPath) < std::tie(other.componentName, other.componentPath);
    }

    bool isCreated() const 
    {
        if (componentName.empty() || componentPath.empty()) 
        {
            std::cerr << "Can't create " << getType() << ": name or path is empty!" << std::endl;
            return false;
        }
        std::cout << getType() << " '" << componentName << "' is successfully created in '" << componentPath << "'\n";
        return true;
    }

    virtual std::string getType() const { return "Component"; }

    const std::string& getName() const { return componentName; }
    const std::string& getPath() const { return componentPath; }

    std::string getFullPath() const {
        return componentPath == "/" ? "/" + componentName : componentPath + "/" + componentName;
    }
};

inline std::map<std::string, std::set<fileSystemComponent*>> fileSystemRegistry;

bool isValidParentPath(const std::string& fullPath)
{
    size_t lastSlash = fullPath.find_last_of('/');
    std::string parentPath = "/";
    std::string currentName = fullPath;

    if (lastSlash != std::string::npos) {
        parentPath = (lastSlash == 0) ? "/" : fullPath.substr(0, lastSlash);
        currentName = fullPath.substr(lastSlash + 1);
    }

    auto it = fileSystemRegistry.find(parentPath);
    if (it != fileSystemRegistry.end()) 
    {
        for (auto* comp : it->second) 
        {
            if (comp->getName() == currentName && comp->getType() == "File") {
                std::cerr << "Error: '" << fullPath << "' is a file, can't add anything to it.\n";
                return false;
            }
        }

        for (auto* comp : it->second) 
        {
            if (comp->getName() == parentPath && comp->getType() == "File") {
                std::cerr << "Error: can't add to '" << parentPath << "' — it's a file!\n";
                return false;
            }
        }
    }

    return true;
}

class file : public fileSystemComponent {
public:
    file(const std::string& name, const std::string& path)
        : fileSystemComponent(name, path)
    {
        if (!isValidParentPath(getFullPath())) return;
        if (isCreated()) {
            fileSystemRegistry[path].insert(this);
        }
    }

    ~file() override {
        std::cout << getType() << " '" << componentName << "' has been deleted from '" << componentPath << "'\n";
    }

    std::string getType() const override { return "File"; }
};

class directory : public fileSystemComponent {
public:
    directory(const std::string& name, const std::string& path)
        : fileSystemComponent(name, path)
    {
        if (!isValidParentPath(getFullPath())) return;
        if (isCreated()) {
            fileSystemRegistry[path].insert(this);
        }
    }

    ~directory() override {
        std::cout << getType() << " '" << componentName << "' has been deleted from '" << componentPath << "'\n";
    }

    std::string getType() const override { return "Directory"; }
};

#endif // FILESYSTEM_H