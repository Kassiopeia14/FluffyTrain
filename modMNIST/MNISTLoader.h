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

	const std::string
		trainImagesFileName,
		testImagesFileName;

	std::ifstream 
		trainImagesFile,
		testImagesFile;

	const size_t 
		trainImagesFileSize,
		testImagesFileSize,
		trainImagesCount,
		testImagesCount;

	std::vector<unsigned char> 
		trainImagesData,
		testImagesData;

	static size_t defineFileSize(std::ifstream& file);

public:

	MNISTLoader(const std::string path);
	
	~MNISTLoader();

	size_t getTrainImagesCount() const;

	size_t getTestImagesCount() const;

	std::vector<unsigned char> getTrainImage(const size_t number);

	std::vector<unsigned char> getTestImage(const size_t number);
};