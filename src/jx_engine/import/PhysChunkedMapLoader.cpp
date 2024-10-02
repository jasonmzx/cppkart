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

// Load AI Splines

std::vector<SplineVertex> PhysChunkedMapLoader::loadAISpline(const std::string &filename, float scale) 
{
    Logger* logger = Logger::getInstance();

    std::vector<SplineVertex> spline_vertices;

    std::vector<glm::vec3> spline_vert_positions;

    // Open file with Input File (IF) stream
    std::ifstream spline_file;
    spline_file.open(filename);

    // Assertion to check if file is open
    if (!spline_file.is_open())
    {
        logger->log(Logger::ERROR, "Failed to open chunk map file: " + filename);
        return spline_vertices;
    }

    std::string line; 

    while (std::getline(spline_file, line)) // Line by Line Reading
    {
        line.erase(0, line.find_first_not_of(" \t\n\r\f\v"));
        glm::vec3 spline_vert = parseVec3(line);

        spline_vert.y = spline_vert.y + 0.215f; // Offset Y to avoid clipping with terrain

        glm::vec3 scaled_vert = spline_vert * scale;

        spline_vert_positions.push_back(scaled_vert);
    }

    // Debug
    logger->log(Logger::INFO, "Loaded " + std::to_string(spline_vertices.size()) + " verts from AI Spline file: " + filename);

    // Pass #2, Get Spline Direction and Perpendicular Direction

    glm::vec3 Y_UP = glm::vec3(0.0f, 1.0f, 0.0f);

    for (size_t i = 0; i < spline_vert_positions.size(); ++i) {
        
        glm::vec3 direction;
        glm::vec3 perpendicular_direction;

        if (i < spline_vertices.size() - 1) {
            direction = glm::normalize(spline_vert_positions[i + 1] - spline_vert_positions[i]);
        } else {
            // Go backwards to the previous vertex
            direction = glm::normalize(spline_vert_positions[i] - spline_vert_positions[i - 1]);
        }

        direction = -direction; // Flip for side of road (bi-dir traffic?)

        // Calculate XZ orthogonal direction


        if(direction.x == direction.x && direction.y == direction.y && direction.z == direction.z) { // If Direction is not NaN, build spline vertex
            
            SplineVertex spline_vert;

            spline_vert.position = spline_vert_positions[i];
            spline_vert.direction = direction;
            spline_vert.xz_orthogonal_dir = glm::normalize(glm::cross(direction, Y_UP)); // If direction is NaN, this will be NaN too

            spline_vertices.push_back(spline_vert);
        }
        // Debug Print

    }


    for(auto vert : spline_vertices) {

        std::cout << "Spline Vert: " << vert.position.x << ", " << vert.position.y << ", " << vert.position.z << std::endl;
        std::cout << "Direction: " << vert.direction.x << ", " << vert.direction.y << ", " << vert.direction.z << std::endl;
        std::cout << "XZ Orthogonal: " << vert.xz_orthogonal_dir.x << ", " << vert.xz_orthogonal_dir.y << ", " << vert.xz_orthogonal_dir.z << std::endl;
        printf("\n");
    }


        std::cout << "SIZEOF SPLINE VERTS: " << spline_vertices.size() << std::endl;    


    return spline_vertices;
}