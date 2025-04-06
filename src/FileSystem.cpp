#include <FileSystem.h>
#include <iostream>

fileSystemComponent::fileSystemComponent(const std::string& componentName, const std::string& componentPath)
{
	this->componentName = componentName;
	this->componentPath = componentPath;
}

void fileSystem::addComponent(fileSystemComponent* component)
{
	fileComponents.push_back(component);
	std::cerr << "The " << component->getName() << " is added to " << component->getPath() << std::endl;
}

fileSystemComponent::~fileSystemComponent() {}

file::file(const std::string& fileName, const std::string& filePath) 
	: fileSystemComponent(fileName, filePath)
{
	if (!this->componentName.empty() && !this->componentPath.empty())
	{
		std::cerr << "File name is: " << componentName << std::endl;
		std::cerr << "File path is: " << componentPath << std::endl;
	}
	else
	{
		std::cerr << "Can't create file!" << std::endl;
	}
}

file::~file()
{
	std::cerr << "File " << this->componentName << " is deleted" << std::endl;
}

directory::directory(const std::string& directoryName, const std::string& directoryPath) 
	: fileSystemComponent(directoryName, directoryPath)
{
	if (!this->componentName.empty() && !this->componentPath.empty())
	{
		std::cerr << "Directory name is: " << componentName << std::endl;
		std::cerr << "Directory path is: " << componentPath << std::endl;
	}
	else
	{
		std::cerr << "Can't create directory!" << std::endl;
	}
}

directory::~directory()
{
	std::cerr << "Directory " << this->componentName << " is deleted" << std::endl;
}