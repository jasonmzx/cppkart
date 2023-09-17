#ifndef MAPCHUNKER_H
#define MAPCHUNKER_H

#include<stb/stb_image.h>
#include <iostream>
#include <vector>
#include <glad/glad.h>

void terrainMapLoader(std::vector<GLuint>& indices_vec, std::vector<GLfloat>& vertices_vec);

#endif