#include "MNISTLoader.h"

#include <fstream>

MNISTLoader::MNISTLoader(const std::string path):
    trainImagesFileName("train-images.idx3-ubyte"),
    testImagesFileName("t10k-images.idx3-ubyte"),
    trainLabelsFileName("train-labels.idx1-ubyte"),
    testLabelsFileName("t10k-labels.idx1-ubyte"),
    trainImagesFile(path + trainImagesFileName, std::ios::binary),
    testImagesFile(path + testImagesFileName, std::ios::binary),
    trainLabelsFile(path + trainLabelsFileName, std::ios::binary),
    testLabelsFile(path + testLabelsFileName, std::ios::binary),
    trainImagesFileSize(defineFileSize(trainImagesFile)),
    testImagesFileSize(defineFileSize(testImagesFile)),
    trainLabelsFileSize(defineFileSize(trainLabelsFile)),
    testLabelsFileSize(defineFileSize(testLabelsFile)),
    trainImagesCount(trainImagesFileSize / imageSize),
    testImagesCount(testImagesFileSize / imageSize),
    trainImagesData(trainImagesFileSize),
    testImagesData(testImagesFileSize),
    trainLabelsData(trainLabelsFileSize),
    testLabelsData(testLabelsFileSize)
{
    trainImagesFile.seekg(0, std::ios::beg);
    trainImagesFile.read((char*)&trainImagesData[0], trainImagesFileSize);
    trainImagesFile.close();

    testImagesFile.seekg(0, std::ios::beg);
    testImagesFile.read((char*)&testImagesData[0], testImagesFileSize);
    testImagesFile.close();

    trainLabelsFile.seekg(0, std::ios::beg);
    trainLabelsFile.read((char*)&trainLabelsData[0], trainLabelsFileSize);
    trainLabelsFile.close();

    testLabelsFile.seekg(0, std::ios::beg);
    testLabelsFile.read((char*)&testLabelsData[0], testLabelsFileSize);
    testLabelsFile.close();
}

MNISTLoader::~MNISTLoader()
{
}

size_t MNISTLoader::defineFileSize(std::ifstream& _file)
{
    _file.seekg(0, std::ios::end);

    return _file.tellg();
}

size_t MNISTLoader::getTrainImagesCount() const
{
    return trainImagesCount;
}

size_t MNISTLoader::getTestImagesCount() const
{
    return testImagesCount;
}

std::vector<unsigned char> MNISTLoader::getTrainImage(const size_t imageNumber)
{
    std::vector<unsigned char> image(imageSize);

    memcpy(&image[0], &trainImagesData[imageNumber * imageSize + imagesOffset], imageSize);

    return image;
}

std::vector<unsigned char> MNISTLoader::getTestImage(const size_t imageNumber)
{
    std::vector<unsigned char> image(imageSize);

    memcpy(&image[0], &testImagesData[imageNumber * imageSize + imagesOffset], imageSize);

    return image;
}

size_t MNISTLoader::getTrainLabel(const size_t number)
{
    return trainLabelsData[number + labelsOffset];
}

size_t MNISTLoader::getTestLabel(const size_t number)
{
    return testLabelsData[number + labelsOffset];
}