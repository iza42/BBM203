#include "Crypto.h"
#include "GrayscaleImage.h"


// Extract the least significant bits (LSBs) from SecretImage, calculating x, y based on message length
std::vector<int> Crypto::extract_LSBits(SecretImage& secret_image, int message_length) {
    std::vector<int> LSB_array;
    // 1. Reconstruct the SecretImage to a GrayscaleImage.
    GrayscaleImage image= secret_image.reconstruct();
    // 2. Calculate the image dimensions.
    int height= image.get_height();
    int width= image.get_width();
    int image_size= height*width;
    // 3. Determine the total bits required based on message length.
    int total_bits=message_length*7;
    // 4. Ensure the image has enough pixels; if not, throw an error.
    if (total_bits>image_size)
        throw std::runtime_error("not enough pixels for the message in extraction");
    // 5. Calculate the starting pixel from the message_length knowing that  
    //    the last LSB to extract is in the last pixel of the image.
   int starting_pixel_idx= (image_size-total_bits ) ; // +1 cuz indexing starts from 0
    int start_j= (starting_pixel_idx) % image.get_width();
    int start_i= (starting_pixel_idx) / image.get_width();

    // 6. Extract LSBs from the image pixels and return the result.

    for (int i=start_i ;i<image.get_height() ; i++) {
        for (int j=(i == start_i ? start_j : 0) ; j<image.get_width(); j++) {
            std::bitset<8> b(image.get_data()[i][j]); //converted to binary from given pixel value
           LSB_array.push_back(b.test(0)); //  pushed the LSB values of binary bitstream  to LSB_array

        }

    }

    return LSB_array;
}


// Decrypt message by converting LSB array into ASCII characters
std::string Crypto::decrypt_message(const std::vector<int>& LSB_array) {
    std::string message;
    int array_size= static_cast<int>(LSB_array.size());
    // 1. Verify that the LSB array size is a multiple of 7, else throw an error.
    if (array_size%7!=0)
        throw std::runtime_error("the array size is not valid, should be the multiple of 7.");
    for(int i=0; i<array_size; i+=7) {
        std::bitset<7> bitGroup;
        for (int j = 0; j < 7 && i + j < array_size; ++j) {
            bitGroup[6 - j] = LSB_array[i + j]; // adding bits in order
        }
        char character = static_cast<char>(bitGroup.to_ulong());     // 2. Convert each group of 7 bits into an ASCII character.
        message += character; // 3. Collect the characters to form the decrypted message.
    }

    return message; // 4. Return the resulting message.
}

// Encrypt message by converting ASCII characters into LSBs
std::vector<int> Crypto::encrypt_message(const std::string& message) {
    std::vector<int> LSB_array(7*message.size());
    for (int j = 0; j < message.size(); j++) {
        unsigned char c = message[j];
        int ascii = c;
        std::bitset<7> bitstream(ascii);   // 1. Convert each character of the message into a 7-bit binary representation. by using std::bitset

        for (int i=0;i<7;i++) {
            LSB_array[(j*7)+(6-i)] = bitstream.test(i);     // 2. Collect the bits into the LSB array.
        }
    }
    return LSB_array;   // 3. Return the array of bits.
}

// Embed LSB array into GrayscaleImage starting from the last bit of the image
SecretImage Crypto::embed_LSBits(GrayscaleImage& image, const std::vector<int>& LSB_array) {
    // 1. Ensure the image has enough pixels to store the LSB array, else throw an error.
    int size_of_LSB_array=static_cast<int>(LSB_array.size());
    int image_size=image.get_height()*image.get_width();
    if(size_of_LSB_array>image_size)
        throw std::runtime_error("not enough pixel for embedding.");
  int count=0;
    // 2. Find the starting pixel based on the message length knowing that
    //    the last LSB to embed should end up in the last pixel of the image.
    int starting_pixel_idx= (image_size-size_of_LSB_array) ;
    int start_j= (starting_pixel_idx) % image.get_width();
    int start_i= (starting_pixel_idx / image.get_width());

    // 3. Iterate over the image pixels, embedding LSBs from the array.
    for (int i=start_i ;i<image.get_height() ; i++) {
        for (int j=(i == start_i ? start_j : 0) ; j<image.get_width(); j++) {
            std::bitset<8> b(image.get_data()[i][j]); //verilen pixel değerini binary çevirdim
             b.set(0, LSB_array[count++]); // positionu 0 yapmayı da dene
            image.get_data()[i][j] = static_cast<int>(b.to_ulong());

        }
    }
    // 4. Return a SecretImage object constructed from the given GrayscaleImage
    //    with the embedded message.
    SecretImage secret_image = SecretImage(image);

    return secret_image;
}
