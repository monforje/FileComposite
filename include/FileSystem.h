#pragma once
#include <string>
#include <vector>

class fileSystemComponent;

class fileSystem {
private:
    std::vector<fileSystemComponent*> fileComponents;
public:
    void addComponent(fileSystemComponent* component);
};

class fileSystemComponent {
protected:
    std::string componentName = "";
    std::string componentPath = "";
public:
    std::string getName() { return componentName; }
    std::string getPath() { return componentPath; }

    fileSystemComponent(const std::string& componentName, const std::string& componentPath);
    virtual ~fileSystemComponent();
};

class file : public fileSystemComponent {
public:
    file(const std::string& fileName, const std::string& filePath);
    ~file() override;
};

class directory : public fileSystemComponent {
public:
    directory(const std::string& directoryName, const std::string& directoryPath);
    ~directory() override;
};