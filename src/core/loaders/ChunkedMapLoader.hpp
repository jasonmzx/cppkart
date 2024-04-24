#ifndef CHUNKEDMAPLOADER_CLASS_HPP
#define CHUNKEDMAPLOADER_CLASS_HPP

#include <vector>
#include <glm/glm.hpp>
#include <sstream>  // Include for std::istringstream
#include <iostream>
#include <fstream>
#include <string>


struct LoadedChunk {

    glm::vec3 centre_point;
    std::vector<GLuint> faces;

    float X_origin;
    float Z_origin;
};

class ChunkedMapLoader {
    public:
    static std::vector<LoadedChunk> loadChunks(const std::string& filename) {
        
        std::vector<LoadedChunk> chunks;

        // Open file with Input File (IF) stream
        std::ifstream chunkfile; chunkfile.open(filename);
        //std::ifstream chunkfile; chunkfile.open("../../src/ressources/first_car.obj");

        // Assertion to check if file is open
        if (!chunkfile.is_open()) {
            std::cerr << "Error: Could not open file " << filename << std::endl;
            return chunks;
        }

        std::string line; // Read file line by line

        while (std::getline(chunkfile, line)) {

            
            if(line[0] == '#') { // Centre-Point line, denoting a new chunk

                // Split off first charof line
                line = line.substr(1);

                // Trim left & right white spaces
                line.erase(0, line.find_first_not_of(" \t\n\r\f\v"));

                //  Parse here!


                LoadedChunk chunk;
                chunk.centre_point = parseVec3(line);
                
                chunk.X_origin = chunk.centre_point.x;
                chunk.Z_origin = chunk.centre_point.z;
                
                chunks.push_back(chunk);

            } else { //* Face line (V0,V1,V2) : part of latest chunk
                    

                    //printf("Chunk Current Length: %d\n", chunks.size());

                    glm::vec3 triangle_idx = parseVec3(line);

                    // Convert and push each face to vector faces (flat GLuint array)
                    chunks.back().faces.push_back(triangle_idx.x);
                    chunks.back().faces.push_back(triangle_idx.y);
                    chunks.back().faces.push_back(triangle_idx.z);
            }

        }

        chunkfile.close();

        //! Debug print chunks with centre points
        // for (int i = 0; i < chunks.size(); i++) {
        //     std::cout << "Chunk " << i << " Centre Point: " << chunks[i].centre_point.x << ", " << chunks[i].centre_point.y << ", " << chunks[i].centre_point.z << std::endl;
        
        //     for (int j = 0; j < chunks[i].faces.size(); j+=3) {
        //         std::cout << "Face " << j/3 << ": " << chunks[i].faces[j] << ", " << chunks[i].faces[j+1] << ", " << chunks[i].faces[j+2] << std::endl;
        //     }
        // }

        return chunks;
    }

    private:
    static glm::vec3 parseVec3(const std::string& line) {
        std::istringstream iss(line);
        std::vector<float> values;
        std::string token;

        while (std::getline(iss, token, ',')) {
            values.push_back(std::stof(token));
        }

        if (values.size() < 3) return glm::vec3(0.0f, 0.0f, 0.0f); // Return zero vector if not enough values
        return glm::vec3(values[0], values[1], values[2]);
    }
};

#endif