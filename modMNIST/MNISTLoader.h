#pragma once

#include <string>
#include <fstream>
#include <vector>

class MNISTLoader
{
public:

	static const size_t
		imageSide = 28,
		imageSize = imageSide * imageSide,
		offset = 16;

private:

	std::ifstream file;

	const size_t fileSize;

	std::vector<unsigned char> fileData;

	static size_t defineFileSize(std::ifstream& file);

public:

	MNISTLoader(const std::string fileName);
	
	~MNISTLoader();

	std::vector<unsigned char> getImage(const size_t number);

};