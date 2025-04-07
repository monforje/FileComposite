#include <FileSystem.h>
#include <iostream>

fileSystem Sys;

fileSystemComponent::fileSystemComponent(const std::string& componentName, const std::string& componentPath)
{
	this->componentName = componentName;
	this->componentPath = componentPath;

	auto result = Sys.rootDir.insert(*this);
	if (!result.second) {
		std::cerr << getType() << " already exists in the system!" << std::endl;
	}
}

bool fileSystemComponent::operator<(const fileSystemComponent& other) const
{
	return std::tie(componentName, componentPath) < std::tie(other.componentName, other.componentPath);
}

bool fileSystemComponent::isCreated() const 
{
	if (componentName.empty() || componentPath.empty()) 
	{
		std::cerr << "Can't create " << getType() << ": name or path is empty!" << std::endl;
		return false;
	}
	if (Sys.rootDir.find(*this) == Sys.rootDir.end()) 
	{
		std::cerr << getType() << " not found in the system!" << std::endl;
		return false;
	}
	std::cout << getType() << " '" << componentName << "' at path '" << componentPath
		<< "' is successfully created in the system." << std::endl;
	return true;
}

fileSystemComponent::~fileSystemComponent() {}

file::file(const std::string& fileName, const std::string& filePath)
	: fileSystemComponent(fileName, filePath)
{
	isCreated();
}

file::~file()
{
	std::cerr << "File " << this->componentName << " is deleted" << std::endl;
}

directory::directory(const std::string& directoryName, const std::string& directoryPath)
	: fileSystemComponent(directoryName, directoryPath)
{
	isCreated();
}


directory::~directory()
{
	std::cerr << "Directory " << componentName << " is deleted" << std::endl;
}