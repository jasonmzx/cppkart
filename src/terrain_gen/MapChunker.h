#ifndef MAPCHUNKER_H
#define MAPCHUNKER_H

#include<stb/stb_image.h>
#include <iostream>
#include <vector>
#include <glad/glad.h>
#include <btBulletDynamicsCommon.h>

void terrainMapLoader(std::vector<GLuint>& indices_vec, std::vector<GLfloat>& vertices_vec);

bool loadHeightfieldData(const char* filename, std::vector<unsigned short>& heightData, 
                         int& width, int& length, btScalar& minHeight, btScalar& maxHeight);

#endif