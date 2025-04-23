#include "GrayscaleImage.h"
#include <iostream>
#include <cstring>  // For memcpy
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include <stdexcept>


// Constructor: load from a file
GrayscaleImage::GrayscaleImage(const char* filename) {
    int channels;
    unsigned char* image = stbi_load(filename, &width, &height, &channels, STBI_grey);
    if (image == nullptr) {
        std::cerr << "Error: Could not load image " << filename << std::endl;
        exit(1);}
    data=new int *[height]; // Dynamically allocate memory for a 2D matrix based on the given dimensions.
    for(int i=0;i<height ;i++) {
        data[i]= new int[width];}

    // Fill the matrix with pixel values from the image
    for (int i=0; i<height; i++) {
        for (int j=0; j<width; j++) {
            data[i][j] = image[i * width + j]; // the transformation of info from 1d to 2d array
            }
    }
    stbi_image_free(image);
}

// Constructor: initialize from a pre-existing data matrix
GrayscaleImage::GrayscaleImage(int** inputData, int h, int w) {
    height=h;
    width=w;

    //  dynamically allocate memory for the matrix.
    data=new int *[height];
    for(int i=0;i<height ;i++) {
        data[i]= new int[width];
    }

    // Initialize the image with a pre-existing data matrix by copying the values.
    for (int i=0; i<height; i++) {
        for (int j=0; j<width; j++) {
            data[i][j] = inputData[i][j] ;
        }
    }
}

// Constructor to create a blank image of given width and height
GrayscaleImage::GrayscaleImage(int w, int h) : width(w), height(h) {
    // Just dynamically allocate the memory for the 2D new matrix based on the given dimensions.
    data =new int *[h];
    for(int i=0;i<h ;i++) {
        data[i]= new int[w];
    }

// initializing with zero to not having garbage values.
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            data[i][j] = 0;
        }
    }

}

// Copy constructor
GrayscaleImage::GrayscaleImage(const GrayscaleImage& other) {
    height=other.height;
    width=other.width;

    data= new int* [height];  // dynamically allocating memory and
    for (int i =0; i<height; i++) {
        data[i] = new int[width];
    }
    for (int i=0; i<height;i++) { // copying pixel values from another image.
        for (int j=0; j<width; j++) {
            data[i][j] = other.data[i][j]; // copying every pixel
        }
    }
}

// Destructor
GrayscaleImage::~GrayscaleImage() {
    // Destructor: deallocate memory for the matrix.

    if (data != nullptr) { // if the data pointer is not null
        for (int i = 0; i < height; i++) {
            delete[] data[i];  // deallocate every row
        }
        delete[] data;  // deallocate the matrix itself
        data = nullptr; // making the pointer null feels right to me

    }

}

// Equality operator
bool GrayscaleImage::operator==(const GrayscaleImage& other) const {
    // Checking if two images have the same dimensions.
    if(!(this->height == other.height && this->width == other.width)) { // if they are having different dimensions return false
        return false;
    }

    // checking if two images have the same pixel value.
    for (int i =0; i<other.height; i++) { // if they're having same dimension but...
        for (int j =0; j<other.width; j++) {
            if(this->data[i][j]!= other.data[i][j]) { // ...but having different pixel values there return false
                return false;
            }
        }
    }
    return true;

}

// Addition operator
GrayscaleImage GrayscaleImage::operator+(const GrayscaleImage& other) const {
    // assuming the 2 image that we are summing up are having the same dimensions

    GrayscaleImage result(width, height); //initializes a blank image

    // Adding two images' pixel values and returning a new image, clamping the results.
    for (int i=0; i<other.height; i++) {
        for (int j=0; j<other.width; j++) {
            if ((this->data[i][j] + other.data[i][j])>255) {
                result.data[i][j] =255;
            }
            else {
                result.data[i][j] =this->data[i][j] + other.data[i][j];
            }
        }
    }
    return result;
}

// Subtraction operator
GrayscaleImage GrayscaleImage::operator-(const GrayscaleImage& other) const {
    // assuming the 2 image that we are subtracting are having the same dimensions

    GrayscaleImage result(width, height); //initializes a blank image

    // Subtracting pixel values of two images and returning a new image, clamping the results.
    for (int i=0; i<other.height; i++) {
        for (int j=0; j<other.width; j++) {
            if ((this->data[i][j] - other.data[i][j])<0) {
                result.data[i][j] =0;
            }
            else {
                result.data[i][j] =this->data[i][j] - other.data[i][j];
            }
        }
    }
    return result;
}

// Get a specific pixel value
int GrayscaleImage::get_pixel(int row, int col) const {
    return data[row][col];
}

// Set a specific pixel value
void GrayscaleImage::set_pixel(int row, int col, int value) {
    data[row][col] = value;
}

// Function to save the image to a PNG file
void GrayscaleImage::save_to_file(const char* filename) const {
    // Create a buffer to hold the image data in the format stb_image_write expects
    unsigned char* imageBuffer = new unsigned char[width * height];

    // Fill the buffer with pixel data (convert int to unsigned char)
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            imageBuffer[i * width + j] = static_cast<unsigned char>(data[i][j]);
        }
    }

    // Write the buffer to a PNG file
    if (!stbi_write_png(filename, width, height, 1, imageBuffer, width)) {
       std::cerr << "Error: Could not save image to file " << filename << std::endl;
    }

    // Clean up the allocated buffer
    delete[] imageBuffer;
}
