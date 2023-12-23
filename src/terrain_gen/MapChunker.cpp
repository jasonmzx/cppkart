
#include "MapChunker.h"

#include <cstdio> // for printf

void printHeightData(const std::vector<float>& heightData, int width, int length) {
    printf("Printing Height Data (Width x Height): %d x %d\n", width, length);
    for (int y = 0; y < length; ++y) {
        for (int x = 0; x < width; ++x) {
            float value = heightData[y * width + x];
            printf("[%2d,%2d]: %5.2f  ", x, y, value); // %5.2f for formatting float with 2 decimal places
        }
        printf("\n"); // New line after each row
    }
}

void terrainMapLoader(std::vector<GLuint>& indices_vec, std::vector<GLfloat>& vertices_vec) {

     const char* filename = "../src/ressources/Map_A.png";
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

        if (x < (width - 1) && y < (height - 1)) { //Prevent Wrapping of Indices
            //Triangle 1 / 2 (per unit)
            indices_vec.push_back(y * width + x);
            indices_vec.push_back((y + 1) * width + x);
            indices_vec.push_back(y * width + (x + 1));

            //Triangle 2 / 2 (per unit)
            indices_vec.push_back((y + 1) * width + x);
            indices_vec.push_back(y * width + (x + 1));
            indices_vec.push_back((y + 1) * width + (x + 1));
        }

            int raw_img_index = (y * width + x) * 4; // Each pixel has 4 channels (RGBA)

            // Access the RGBA components of the pixel
    unsigned char color_c = image[raw_img_index];
    float normalized_color = static_cast<float>(color_c) / ( 255.0f*0.75f);

            //X: 
            vertices_vec.push_back(static_cast<GLfloat>(x)/width);
            //Y:
            vertices_vec.push_back(static_cast<GLfloat>(normalized_color/1.5)); //height vertical
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

bool loadHeightfieldData(const char* filename, std::vector<float>& heightData, int& width, int& length, btScalar& minHeight, btScalar& maxHeight) {
    int channels;

    // Load the PNG image
    unsigned char* image = stbi_load(filename, &width, &length, &channels, STBI_rgb_alpha);
    if (!image) {
        std::cerr << "Error loading image: " << stbi_failure_reason() << std::endl;
        return false;
    }

    std::cout << "Image SIZE: H- " << length << "px, W- " << width << "px " << std::endl;

    // Initially set LOW & HIGH
    float maxPixelValue = 0.0f;
    float minPixelValue = FLT_MAX;

    // Iterate through the pixels to populate heightData and find minHeight/maxHeight
    for (int y = 0; y < length; ++y) {
        for (int x = 0; x < width; ++x) {
            int raw_img_index = (y * width + x) * 4; // Each pixel has 4 channels (RGBA)
            unsigned char color_c = image[raw_img_index]; // Assuming height is represented by the red channel

            float heightValue = static_cast<float>(color_c) / 20.0f; // Convert to float
            heightData.push_back(heightValue);

            maxPixelValue = std::max(maxPixelValue, heightValue);
            minPixelValue = std::min(minPixelValue, heightValue);
        }
    }



    minHeight = static_cast<btScalar>(minPixelValue);
    maxHeight = static_cast<btScalar>(maxPixelValue);

    printHeightData(heightData, width, length);

    printf("MAX %f", maxPixelValue);
    printf("MIN %f", minPixelValue);

    stbi_image_free(image);

    return true;
}
