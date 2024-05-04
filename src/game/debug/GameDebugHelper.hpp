#ifndef GAMEDEBUGHELPER_HPP
#define GAMEDEBUGHELPER_HPP

#include <vector>
#include <glad/glad.h>
#include <fstream>
#include <iostream>

class GameDebugHelper {
public:
    static void ExportVertexMappingToFile(std::vector<GLfloat>& terrainVerts, const std::string& filename);
};

#endif // GAMEDEBUGHELPER_HPP
