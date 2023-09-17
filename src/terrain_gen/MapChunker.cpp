
#include "MapChunker.h"

void terrainMapLoader(GLuint &indices, GLfloat &vertices) {


     const char* filename = "../src/ressources/16-bit-terrain-map.png";
    int width, height, channels;

    // Load the PNG image
    unsigned char* image = stbi_load(filename, &width, &height, &channels, STBI_rgb_alpha);

    if (!image) {
        std::cerr << "Error loading image: " << stbi_failure_reason() << std::endl;
    }

    //GLuint indices[height * width * 3]; //Per Vertex, 3 indices
    //GLfloat vertices[height * width];
    std::vector<GLuint> indices_vec = {};
    std::vector<GLfloat> vertices_vec = {};

    std::cout << "Image SIZE: H- " << height << "px, W- " << width << "px " << std::endl;

    // Iterate through the pixels
    for (int y = 0; y < height; ++y) {
    
        for (int x = 0; x < width; ++x) {

            indices_vec.push_back(y*width+x);
            indices_vec.push_back((y+1)*width+x);
            indices_vec.push_back(y*width+(x+1));

            int raw_img_index = (y * width + x) * 4; // Each pixel has 4 channels (RGBA)

            // Access the RGBA components of the pixel
            unsigned char color_c = image[raw_img_index];
            float normalized_color = int(color_c) / 255;

            //X: 
            vertices_vec.push_back(static_cast<GLfloat>(x)/width);
            //Y:
            vertices_vec.push_back(normalized_color);
            //Z:
            vertices_vec.push_back(static_cast<GLfloat>(y)/width);
            //
            for (int i = 0; i < 5; ++i) {
                vertices_vec.push_back(static_cast<GLuint>(0.0f));
            }
        }
    }
    //HERE 
    // Copy vector elements to the arrays
    std::copy(indices_vec.begin(), indices_vec.end(), indices);
    std::copy(vertices_vec.begin(), vertices_vec.end(), vertices);
    
    std::cout << "Parsed the terrain?" << std::endl;

    stbi_image_free(image); // Free the image data when done
}