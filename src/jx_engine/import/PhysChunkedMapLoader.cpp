#include "PhysChunkedMapLoader.hpp"

std::vector<LoadedChunk> PhysChunkedMapLoader::loadChunks(const std::string &filename)
{
    Logger* logger = Logger::getInstance();
    std::vector<LoadedChunk> chunks;

    // Open file with Input File (IF) stream
    std::ifstream chunkfile;
    chunkfile.open(filename);

    // Assertion to check if file is open
    if (!chunkfile.is_open())
    {
        logger->log(Logger::ERROR, "Failed to open chunk map file: " + filename);
        return chunks;
    }

    std::string line; 

    while (std::getline(chunkfile, line)) // Line by Line Reading
    {

        if (line[0] == '#')
        { // Centre-Point line, denoting a new chunk

            // Split off first charof line
            line = line.substr(1);

            // Trim left & right white spaces
            line.erase(0, line.find_first_not_of(" \t\n\r\f\v"));

            // Init chunk with centre point
            LoadedChunk chunk;
            chunk.centre_point = parseVec3(line);

            chunk.X_origin = chunk.centre_point.x;
            chunk.Z_origin = chunk.centre_point.z;

            chunks.push_back(chunk);
        }
        else
        { //* Triangle Line (V0,V1,V2), where all verts are vec3 

            std::vector<glm::vec3> vec3List = PhysChunkedMapLoader::parseVec3List(line);

            // Push Triangle of verts to latest chunk

            chunks.back().triangle_ordered_verts.push_back(vec3List[0]);
            chunks.back().triangle_ordered_verts.push_back(vec3List[1]);
            chunks.back().triangle_ordered_verts.push_back(vec3List[2]);

        }
    }

    chunkfile.close();

    return chunks;
}

glm::vec3 PhysChunkedMapLoader::parseVec3(const std::string& line) {
        std::istringstream iss(line);
        std::vector<float> values;
        std::string token;

        while (std::getline(iss, token, ',')) {
            values.push_back(std::stof(token));
        }

        if (values.size() < 3) return glm::vec3(0.0f, 0.0f, 0.0f); // Return zero vector if not enough values
        return glm::vec3(values[0], values[1], values[2]);
}

std::vector<glm::vec3> PhysChunkedMapLoader::parseVec3List(const std::string& line) {
    std::vector<glm::vec3> vec3List;
    std::string token;
    std::istringstream iss(line);

    while (std::getline(iss, token, ')')) {
        size_t start = token.find('(');
        if (start != std::string::npos) {
            token = token.substr(start + 1);
            vec3List.push_back(parseVec3(token));
        }
    }

    return vec3List;
}