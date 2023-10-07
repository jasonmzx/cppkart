
#include "MapChunker.h"

void terrainMapLoader(std::vector<GLuint>& indices_vec, std::vector<GLfloat>& vertices_vec) {

     const char* filename = "../src/ressources/16-bit-terrain-map.png";
    int width, height, channels;

    // Load the PNG image
    unsigned char* image = stbi_load(filename, &width, &height, &channels, STBI_rgb_alpha);

    if (!image) {
        std::cerr << "Error loading image: " << stbi_failure_reason() << std::endl;
    }

    std::cout << "Image SIZE: H- " << height << "px, W- " << width << "px " << std::endl;

    // Iterate through the pixels
    for (int y = 0; y < height; ++y) {
    
        for (int x = 0; x < width; ++x) {

            //Triangle 1 / 2 (per unit)
            indices_vec.push_back(y*width+x);
            indices_vec.push_back((y+1)*width+x);
            indices_vec.push_back(y*width+(x+1));

            //Triangle 2 / 2 (per unit)
            indices_vec.push_back((y+1)*width+x);
            indices_vec.push_back(y*width+(x+1));
            indices_vec.push_back((y+1)*width+(x+1));

            int raw_img_index = (y * width + x) * 4; // Each pixel has 4 channels (RGBA)

            // Access the RGBA components of the pixel
    unsigned char color_c = image[raw_img_index];
    float normalized_color = static_cast<float>(color_c) / ( 255.0f*0.75f);

            //X: 
            vertices_vec.push_back(static_cast<GLfloat>(x)/width);
            //Y:
            vertices_vec.push_back(static_cast<GLfloat>(normalized_color));
            //Z:
            vertices_vec.push_back(static_cast<GLfloat>(y)/width); //Not multiplying by height, as I dont want to stretch the proportions
            //
            for (int i = 0; i < 5; ++i) {
                vertices_vec.push_back(static_cast<GLuint>(0.0f));
            }
        }
    }
    
    std::cout << "Parsed the terrain?" << std::endl;

    stbi_image_free(image); // Free the image data when done
}