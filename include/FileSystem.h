#pragma once
#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <string>
#include <set>

class fileSystemComponent;

class fileSystem {
public:
    std::set<fileSystemComponent> rootDir;
};

class fileSystemComponent {
protected:
    std::string componentName = "";
    std::string componentPath = "";
public:
    fileSystemComponent(const std::string& componentName, const std::string& componentPath);
    virtual ~fileSystemComponent();
    bool operator<(const fileSystemComponent& other) const;

    bool isCreated() const;

    virtual std::string getType() const { return "Component"; }
};

class file : public fileSystemComponent {
public:
    file(const std::string& fileName, const std::string& filePath);
    ~file() override;

    std::string getType() const override { return "File"; }
};

class directory : public fileSystemComponent {
public:
    std::set<directory> directories;
    directory(const std::string& directoryName, const std::string& directoryPath);
    ~directory() override;

    std::string getType() const override { return "Directory"; }
};

#endif // FILESYSTEM_H