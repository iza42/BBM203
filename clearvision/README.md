# **Grayscale Image Processing & Secret Image Embedding System**

## ⚙️ **Project Overview**

This project simulates image processing and secret message embedding in grayscale images. It consists of various classes that provide functionalities such as image manipulation, filter application, and secret message encryption using Least Significant Bit (LSB) techniques. The system is organized into four primary classes:

1. **GrayscaleImage**: Represents a grayscale image and handles loading, saving, and basic image manipulation (e.g., addition, subtraction).
2. **SecretImage**: Manages the secure storage and reconstruction of a grayscale image by splitting it into upper and lower triangular matrices.
3. **Filter**: Applies different filters (mean filter, Gaussian smoothing, unsharp mask) to the grayscale images.
4. **Crypto**: Handles embedding and extracting secret messages within the image using LSB techniques.

## 🗂 **Project Structure**

ImageProcessingSystem/
├── GrayscaleImage.java # Class to handle grayscale image operations
├── SecretImage.java # Class to handle secret image operations
├── Filter.java # Class to apply filters to images
├── Crypto.java # Class to embed and extract messages in images
├── Main.java # Entry point of the system
├── inputs/ # Folder containing image and text files for testing
│ └── sample_image.png # Example grayscale image file
├── outputs/ # Folder containing the output files after processing
│ └── output_image.png # Output image after processing
└── README.md # Project description and usage guide

## 🔧 **How It Works**

The project simulates the process of working with grayscale images and embedding secret messages within them. It consists of the following key components:

### **GrayscaleImage Class**

This class represents a grayscale image stored in a 2D matrix. It supports the following functionalities:

- **Constructors**: Load image from file, initialize from 2D data matrix, or create a blank image.
- **Operator Overloading**: Compare images for equality (`==`), add (`+`), and subtract (`-`) pixel values between images.
- **Methods**: Save the image to a file and clean up dynamically allocated memory when destroyed.

### **SecretImage Class**

The `SecretImage` class stores a grayscale image securely by splitting it into upper and lower triangular matrices. This class provides methods to:

- **Reconstruct** the full image from the triangular matrices.
- **Save** and **Load** the image to/from a file.
- **Update** the matrices with modified pixel data.

### **Filter Class**

The `Filter` class applies various filters to grayscale images:

- **Mean Filter**: Applies a box filter to the image.
- **Gaussian Smoothing**: Applies Gaussian smoothing with a specified kernel size and sigma.
- **Unsharp Mask**: Sharpens the image by emphasizing edges.

### **Crypto Class**

The `Crypto` class is responsible for embedding and extracting secret messages in the least significant bits (LSB) of the image pixels:

- **Encrypt**: Convert an ASCII message into its binary form for embedding.
- **Embed**: Embed the encrypted message into the image pixels.
- **Extract & Decrypt**: Extract the LSBs from the image and convert them back into readable ASCII characters.

## ▶️ **How to Use**

### 1. **Compiling the Code**

- Compile the C++ files provided in the project using the following command:

```bash
g++ -o clearvision main.cpp GrayscaleImage.cpp SecretImage.cpp Filter.cpp Crypto.cpp
### 2. **Running the Program**
 - Once the code is compiled, run the program using the following command:
 ./clearvision input_image.png output_image.png
### 3. **Input File**
input_image.png: The grayscale image file you want to process (e.g., PNG format).

### 4. **Output File**
output_image.png: The processed image will be saved to this file. It could include filtered pixel data or embedded secret messages.

### 5. **Interacting with the Program**
The system allows you to:
Apply filters (mean filter, Gaussian smoothing, unsharp mask).
Embed a secret message in the least significant bits (LSB) of the image.
Extract hidden messages from the image.
You can interact with these features through the respective classes (Filter, Crypto, GrayscaleImage, SecretImage).
```
