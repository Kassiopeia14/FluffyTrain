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
		imagesOffset = 16,
		labelsOffset = 8,
		colorScale = 256,
		classCount = 10;

private:

	const std::string
		trainImagesFileName,
		testImagesFileName,
		trainLabelsFileName,
		testLabelsFileName;

	std::ifstream 
		trainImagesFile,
		testImagesFile,
		trainLabelsFile,
		testLabelsFile;

	const size_t 
		trainImagesFileSize,
		testImagesFileSize,
		trainLabelsFileSize,
		testLabelsFileSize,
		trainImagesCount,
		testImagesCount;

	std::vector<unsigned char> 
		trainImagesData,
		testImagesData,
		trainLabelsData,
		testLabelsData;

	static size_t defineFileSize(std::ifstream& file);

public:

	MNISTLoader(const std::string path);
	
	~MNISTLoader();

	size_t getTrainImagesCount() const;

	size_t getTestImagesCount() const;

	std::vector<unsigned char> getTrainImage(const size_t number);

	std::vector<unsigned char> getTestImage(const size_t number);

	size_t getTrainLabel(const size_t number);

	size_t getTestLabel(const size_t number);
};