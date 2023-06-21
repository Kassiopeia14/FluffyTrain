#include "MNISTLoader.h"

#include <fstream>

MNISTLoader::MNISTLoader(const std::string fileName):
    file(fileName, std::ios::binary),
    fileSize(defineFileSize(file)),
    fileData(fileSize)
{
    file.seekg(0, std::ios::beg);

    file.read((char*)&fileData[0], fileSize);
}

MNISTLoader::~MNISTLoader()
{
}

size_t MNISTLoader::defineFileSize(std::ifstream& _file)
{
    _file.seekg(0, std::ios::end);

    return _file.tellg();
}

std::vector<char> MNISTLoader::getImage(const size_t imageNumber)
{
    std::vector<char> image(imageSize);

    memcpy(&image[0], &fileData[imageNumber * imageSize + offset], imageSize);

    return image;
}