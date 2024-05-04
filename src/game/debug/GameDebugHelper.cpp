#include "GameDebugHelper.hpp"

void GameDebugHelper::ExportVertexMappingToFile(std::vector<GLfloat>& terrainVerts, const std::string& filename) {
 
    std::ofstream outFile(filename);

    if (!outFile) {
        std::cerr << "Failed to open the file for writing: " << filename << std::endl;
        return;
    }

    for (int i = 0; i < terrainVerts.size(); i += 11) {
        outFile << "VID: " << i / 11 << ", ("
                << terrainVerts[i] << ", "
                << terrainVerts[i + 1] << ", "
                << terrainVerts[i + 2] << ")\n";
    }

    outFile.close();
    std::cout << "Data exported successfully to " << filename << std::endl;

}

