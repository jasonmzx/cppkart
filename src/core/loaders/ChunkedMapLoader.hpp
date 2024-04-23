#ifndef CHUNKEDMAPLOADER_CLASS_HPP
#define CHUNKEDMAPLOADER_CLASS_HPP

#include <vector>
#include <glm/glm.hpp>

// Imports for file reading
#include <iostream>
#include <fstream>
#include <string>


struct LoadedChunk {

    glm::vec3 centre_point;
    std::vector<glm::vec3> faces;
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
                chunks.push_back(chunk);

            } else { //* Face line (V0,V1,V2) : part of latest chunk
                    
                    // Push corresponding face to latest chunk
                    chunks.back().faces.push_back(parseVec3(line));
            }

        }

        chunkfile.close();

        //! Debug print chunks with centre points
        // for (const LoadedChunk& chunk : chunks) {
        //     std::cout << "Chunk at " << chunk.centre_point.x << ", " << chunk.centre_point.y << ", " << chunk.centre_point.z << std::endl;
        
        //     for (const glm::vec3& face : chunk.faces) {
        //         std::cout << "Face at " << face.x << ", " << face.y << ", " << face.z << std::endl;
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