#include "PhysicsChunkManager.h"
#include "terrain_gen/MapChunker.h"

//Constructor
PhysicsChunkManager::PhysicsChunkManager(const std::string& filename){

    std::vector<std::vector<float>> tempChunkVec;

    bool chunkHeightMap = chunkHeightDataFromIMG(filename.c_str(), &chunk_size, tempChunkVec, N_chunks_x, N_chunks_y, globalChunkMin, globalChunkMax);

    //* Building Chunk Vector:
    for (const auto& heightDataVec : tempChunkVec) {

        float* heightData = new float[heightDataVec.size()];
        std::copy(heightDataVec.begin(), heightDataVec.end(), heightData);

        PhysicsChunk chunk;
        chunk.active = false;
        chunk.rawHeightData = heightData;
        
        chunkVector.push_back(chunk);
    }

}