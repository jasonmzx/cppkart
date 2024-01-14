
#include "MapChunker.h"

#include <cstdio> // for printf

bool chunkHeightDataFromIMG(const char *filename, const int *chunk_size,
                            std::vector<std::vector<float>> &chunkVecs,
                            int &N_chunks_x, int &N_chunks_y, btScalar &globalChunkMin, btScalar &globalChunkMax)
{

    // Image Data from STB Load fn.
    int img_width;
    int img_length;
    int img_channels;

    // Load the PNG image
    unsigned char *image = stbi_load(filename, &img_width, &img_length, &img_channels, STBI_rgb_alpha);
    if (!image)
    {
        std::cerr << "Error loading image: " << stbi_failure_reason() << std::endl;
        return false;
    }

    //* Chunking Algorithm, to store to in chunkVecs

    int N_chunks_X = img_width / (*chunk_size);
    int N_chunks_Y = img_length / (*chunk_size);

    //* Setting reference chunk sizes

    N_chunks_x = N_chunks_X;
    N_chunks_y = N_chunks_Y;

    chunkVecs.resize(N_chunks_X * N_chunks_Y); // Hold the chunks

    float maxPixelValue = 0.0f;
    float minPixelValue = FLT_MAX;

    for (int y = 0; y < img_length; ++y)
    {
        for (int x = 0; x < img_width; ++x)
        {

            int raw_img_index = (y * img_width + x) * 4;  // Each pixel has 4 channels (RGBA)
            unsigned char color_c = image[raw_img_index]; // Assuming height is represented by the red channel

            float heightValue = static_cast<float>(color_c) / (10.0f) ;

            // Determine which chunk this pixel belongs to
            int chunk_x = x / (*chunk_size);
            int chunk_y = y / (*chunk_size);
            int chunk_index = chunk_y * N_chunks_X + chunk_x;

            //* ===== COLUMN SEAM : Adding that extra Column to the Left of every chunk
            int rXflood = x % (*chunk_size);
            int rYflood = y % (*chunk_size);


            if (x != 0 && rXflood == 0)
            {
                chunkVecs[chunk_index - 1].push_back(heightValue);
            }
            //If it's the Last chunk in the Row, 
            //and we're at the Last Column of that row, just add another HeightValue for X padding

            else if (chunk_x == N_chunks_x - 1 && (x + 1) % (*chunk_size) == 0)
            {
                chunkVecs[chunk_index].push_back(heightValue);
            }

            //! ROW SEAM : Adding Horizontal Seams (X Axis)

            if (y != 0 && rYflood == 0)
            {
                int above_chunk_index = chunk_index - N_chunks_X;
                chunkVecs[above_chunk_index].push_back(heightValue);
            }

            chunkVecs[chunk_index].push_back(heightValue);

            maxPixelValue = std::max(maxPixelValue, heightValue);
            minPixelValue = std::min(minPixelValue, heightValue);
        }
    }

    //* ====== Bottom-Right Corner Seaming =====
    //TODO: Bottom-Right Corner's for the N_Chunk_Yth row are still broken, but idrc

    for(int cI = 0; cI < chunkVecs.size(); cI++){
    
        int chunk_below_index = cI + N_chunks_X;

        if(chunk_below_index < chunkVecs.size()) {    
            float element = chunkVecs[chunk_below_index][*chunk_size];
            chunkVecs[cI].push_back(element);
        }

    }

    // Duplicate the last *chunk_size elements in each chunk of the last row
    int start_chunk_index_last_row = (N_chunks_Y - 1) * N_chunks_X;

    for (int chunk_index = start_chunk_index_last_row; chunk_index < start_chunk_index_last_row + N_chunks_X; ++chunk_index)
    {
        if (chunkVecs[chunk_index].size() >= *chunk_size)
        {
            // Get the starting position for the last *chunk_size elements
            auto start_it = chunkVecs[chunk_index].end() - *chunk_size;
            // Copy the last *chunk_size elements to the end of the same vector
            chunkVecs[chunk_index].insert(chunkVecs[chunk_index].end(), start_it, chunkVecs[chunk_index].end());
        }
    }

    // Setting Global Min/Max for all chunks to be cohesive
    globalChunkMin = static_cast<btScalar>(minPixelValue);
    globalChunkMax = static_cast<btScalar>(maxPixelValue);

    stbi_image_free(image);

    return true;
}
