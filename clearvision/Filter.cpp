#include "Filter.h"
//#include <algorithm>
#include <cmath>
#include <vector>
//#include <numeric>
#include <iostream>


 int kernel_summing_for_mean(int** array, int width, int height ,int kernel_size, int xcord, int ycord) {
    int sum=0;
    int movement= (kernel_size-1)/2; // this number gives  how much step i move to right, left, south or north
    for (int dx = -movement; dx <= movement; dx++) {
        for (int dy = -movement; dy <= movement; dy++) {
            int newX = xcord + dx;  // new x coordinate
            int newY = ycord + dy;  // new y coordinate
            if (newY<0 ||newX<0||newX>width-1 ||newY>height-1) {
                continue; //instead of summing with 0, I pretend that zero has no effect on the total, so if it goes beyond acceptable limits, it doesn't exist
            }
            else {
                sum+=array[newX][newY];
            }
        }
    }
    return sum/(kernel_size*kernel_size);
}

// Mean Filter
void Filter::apply_mean_filter(GrayscaleImage& image, int kernelSize) {
    // 1. Copy the original image for reference.
     GrayscaleImage image2=image; // copied with the help of copy constructor

     // 2. For each pixel, calculate the mean value of its neighbors using a kernel. // did that in another func cuz thought that it will look like a mess

     // 3. Update each pixel with the computed mean.
    for(int i=0;i<image.get_height();i++) {
        for (int j=0; j<image.get_width(); j++) { // basically traversing all pixels of image
            image.get_data()[i][j]=kernel_summing_for_mean(image2.get_data(), image2.get_width(), image2.get_height(),kernelSize,i,j);

        }
    }

}


std::vector<std::vector<double>> kernel_and_normalization_for_gaussian(int kernel_size, double sigma) {
    // this is a 2-dimensional vector structure, the first one given in the kernel refers to the number of rows and the other one refers to the number of columns
     std::vector<std::vector<double>> kernel(kernel_size, std::vector<double>(kernel_size));

     double sum=0.0; // for normalization, used double going to floor later
     int movement= (kernel_size-1)/2; // this number gives  how much step i move to right, left, south or north

     for (int dx = -movement; dx <= movement; dx++) {
         for (int dy = -movement; dy <= movement; dy++) {

             //gaussian kernel calculation
             double value = (1 / (2 * M_PI * sigma * sigma)) * exp(-(dx*dx + dy*dy) / (2 * sigma * sigma));
             kernel[dx + movement][dy + movement] = value;
             sum += value;
         }
     }

             for (int i = 0; i < kernel_size; i++) {
                 for (int j = 0; j < kernel_size; j++) {
                     kernel[i][j] /= sum;  // normalization here

                 }
             }
            /* double control_sum=0.0; // after the kernel is normalized, I'm wondering if the sum of the elements will be 1
             // because it should be 1
             for (const auto& row : kernel) {
                 for (double value : row) {
                     control_sum += value;
                 }
             }
             if (std::abs(control_sum - 1.0) > 1e-6) {
                 std::cerr << "Warning: Kernel sum is not close to 1.0 (" << control_sum << ")" << std::endl;
            }*/

     return kernel;
 }



int kernel_sum_for_one_pixel(int** array,int width, int height,int kernel_size, int xcord, int ycord, double sigma) {
     auto kernel = kernel_and_normalization_for_gaussian(kernel_size, sigma);
     double sum=0.0;
     int movement= (kernel_size-1)/2; //  this number gives  how much step i move to right, left, south or north

     for (int dx = -movement; dx <= movement; dx++) {
         for (int dy = -movement; dy <= movement; dy++) {
             int newX = xcord + dx;  // new x coordinate
             int newY = ycord + dy;  // new y coordinate
             if (newY >= 0 && newX >= 0 && newX < width && newY < height) {
                 sum += array[newY][newX] * kernel[dx + movement][dy + movement];
             }

             }
         }
    sum=std::floor(sum);
     sum=std::max(0.0,std::min(sum,255.0));

     return static_cast<int>(sum);
     }




// Gaussian Smoothing Filter
void Filter::apply_gaussian_smoothing(GrayscaleImage& image, int kernelSize, double sigma) {
    // 1. Create a Gaussian kernel based on the given sigma value. //did first to in the kernel_and_normalization_for_gaussian(..) func
     // 2. Normalize the kernel to ensure it sums to 1.   // to be more clean cuz my code already is a mess

    // 3. For each pixel, compute the weighted sum using the kernel. //  did it in the second func which is kernel_sum_for_one_pixel(..)

     // 4. Update the pixel values with the smoothed results.
     GrayscaleImage image2 = image; // copy constructor have been called
     for(int i =0; i<image.get_height(); i++) {
         for (int j=0; j<image.get_width();j++) {
            image.get_data()[i][j]=kernel_sum_for_one_pixel(image2.get_data(),image2.get_width(), image2.get_height(),kernelSize,j,i,sigma);
         }
     }



}

// Unsharp Masking Filter
void Filter::apply_unsharp_mask(GrayscaleImage& image, int kernelSize, double amount) {
     GrayscaleImage blurred_image = image; // copy constructor have been called
     // 1. Blur the image using Gaussian smoothing, use the default sigma given in the header.
     apply_gaussian_smoothing(blurred_image,kernelSize,1.0);
     int height = image.get_height();
     int width=image.get_width();
     for (int i=0;i<height; i+=1) {
         for(int j=0; j<width; j+=1) {
             int original_pixel=image.get_data()[i][j];
             int blurred_pixel=blurred_image.get_data()[i][j];
             // 2. For each pixel, apply the unsharp mask formula: original + amount * (original - blurred).
             int unsharp_pixel=original_pixel+amount*(original_pixel-blurred_pixel);

             // 3. Clip values to ensure they are within a valid range [0-255].
             unsharp_pixel=std::max(0,std::min(255,unsharp_pixel));
             image.get_data()[i][j]=unsharp_pixel;
         }
     }


}
