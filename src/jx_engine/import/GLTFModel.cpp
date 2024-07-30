#include "GLTFModel.hpp"

GLTFModel::GLTFModel(const std::string &filepath)
{
    Logger* logger = Logger::getInstance();
    logger->log(Logger::INFO, "GLTFModel.cpp | Loading the model: " + filepath);
}
