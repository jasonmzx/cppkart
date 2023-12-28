#ifndef MAPCHUNKER_H
#define MAPCHUNKER_H

#include<stb/stb_image.h>
#include <iostream>
#include <vector>
#include <glad/glad.h>
#include <btBulletDynamicsCommon.h>

bool chunkHeightDataFromIMG(const char* filename, const int* chunk_size,
                            std::vector<std::vector<float>>& chunkVecs, int& N_chunks_x, int& N_chunks_y, btScalar& globalChunkMin, btScalar& globalChunkMax );

#endif