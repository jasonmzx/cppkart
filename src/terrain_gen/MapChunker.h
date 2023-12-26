#ifndef MAPCHUNKER_H
#define MAPCHUNKER_H

#include<stb/stb_image.h>
#include <iostream>
#include <vector>
#include <glad/glad.h>
#include <btBulletDynamicsCommon.h>

void terrainMapLoader(std::vector<GLuint>& indices_vec, std::vector<GLfloat>& vertices_vec);

bool loadHeightfieldData(const char* filename, std::vector<float>& heightData, 
                        int& width, int& length, btScalar& minHeight, btScalar& maxHeight);               


bool chunkHeightDataFromIMG(const char* filename, const int* chunk_size,
                            std::vector<std::vector<float>>& chunkVecs, int& N_chunks_x, int& N_chunks_y, btScalar& globalChunkMin, btScalar& globalChunkMax );


#endif