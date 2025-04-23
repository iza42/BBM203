#include "SecretImage.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <complex>


// Constructor: Split image into upper and lower triangular arrays
SecretImage::SecretImage(const GrayscaleImage& image) {
    width = image.get_width();
    height = image.get_height();

    int upper_size = (height * (height + 1)) / 2;
    int lower_size = (height * (height - 1)) / 2;

    upper_triangular = new int[upper_size];
    lower_triangular = new int[lower_size];


    // Filling the upper and lower triangular matrices
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < height; j++) {
            if (i <= j) {  // upper triangular
                upper_triangular[(i * (2 * height - i - 1)) / 2 + j] = image.get_data()[i][j];
            } else {  // lower triangular
                lower_triangular[(i * (i - 1)) / 2 + j] = image.get_data()[i][j];
            }
        }
    }
}

// Constructor: Initialize from given arrays
SecretImage::SecretImage(int w, int h, const int* upper, const int* lower) {
    width = w;
    height = h;

    int upper_size = (w * (w + 1)) / 2;
    int lower_size = (w * (w - 1)) / 2;

    upper_triangular = new int[upper_size];
    lower_triangular = new int[lower_size];

    // copying the contents of the given arrays
    for (int i = 0; i < upper_size; i++) {
        upper_triangular[i] = upper[i];
    }
    for (int i = 0; i < lower_size; i++) {
        lower_triangular[i] = lower[i];
    }
}

// Destructor: Free the dynamically allocated memory
SecretImage::~SecretImage() {
    delete[] upper_triangular;
    delete[] lower_triangular;
}

// Reconstruct and return the full image from the triangular arrays
GrayscaleImage SecretImage::reconstruct() const {
    GrayscaleImage image(width, height);
    int n = image.get_height();

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (i <= j) {  // Upper triangular
                int upper_index = (i * (2 * n - i - 1)) / 2 + j;
                image.get_data()[i][j] = upper_triangular[upper_index];
            } else {  // Lower triangular
                int lower_index = (i * (i - 1)) / 2 + j;
                image.get_data()[i][j] = lower_triangular[lower_index];
            }
        }
    }
    return image;
}

// Save the filtered image back to the triangular arrays
void SecretImage::save_back(const GrayscaleImage& image) {
    int n = image.get_height();

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (i <= j) {  // Upper triangular
                int index = (i * (2 * n - i - 1)) / 2 + j;
                upper_triangular[index] = image.get_data()[i][j];
            } else {  // Lower triangular
                int index = (i * (i - 1)) / 2 + j;
                lower_triangular[index] = image.get_data()[i][j];
            }
        }
    }

}

// Save the upper and lower triangular arrays to a file
void SecretImage::save_to_file(const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Cannot open file for writing." << std::endl;
        return;
    }
    file << width << " " << height << std::endl;

    // writing the upper triangular array
    int upper_size = (height * (height + 1)) / 2;
    for (int i = 0; i < upper_size; ++i) {
        file << upper_triangular[i];
        if (i < upper_size - 1) file << " ";
    }
    file << "\n";

    //writing the lower triangular array
    int lower_size = (height * (height - 1)) / 2;
    for (int i = 0; i < lower_size; ++i) {
        file << lower_triangular[i];
        if (i < lower_size - 1) file << " ";
    }
    file << "\n";
    file.close();

}


SecretImage SecretImage::load_from_file(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Cannot open the file." << std::endl;
        exit(-1);
    }

    // reading width and height
    int width, height;
    file >> width >> height;


    int upper_size = (height * (height + 1)) / 2;
    int lower_size = (height * (height - 1)) / 2;

    // allocating memory for the arrays
    int* upper_triangular = new int[upper_size];
    int* lower_triangular = new int[lower_size];

    // reading the upper triangular array
    for (int i = 0; i < upper_size; ++i) {
        file >> upper_triangular[i];
    }
    // reading the lower triangular array
    for (int i = 0; i < lower_size; ++i) {
        file >> lower_triangular[i];
    }
    file.close();

    SecretImage secret_image(width, height, upper_triangular, lower_triangular);

    delete[] upper_triangular;
    delete[] lower_triangular;

    return secret_image;
}


int* SecretImage::get_upper_triangular() const {
    return upper_triangular;
}
int* SecretImage::get_lower_triangular() const {
    return lower_triangular;
}

int SecretImage::get_width() const {
    return width;
}
int SecretImage::get_height() const {
    return height;
}