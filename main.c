#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define N 5 // Number of bins for the histogram

// Structure representing an RGB pixel
typedef struct {
    unsigned char r, g, b;
} RGB;

// Function prototypes
void readPPM(const char *filename, RGB **pixels, int *width, int *height);
void writePPM(const char *filename, RGB *pixels, int width, int height);
void convertToGrayscale(RGB *pixels, int width, int height);
void applySharpening(RGB *pixels, int width, int height);
void calculateAndWriteHistogram(RGB *pixels, int width, int height, const char* filename);

// Reads PPM image file
void readPPM(const char *filename, RGB **pixels, int *width, int *height) { 
    FILE *file = fopen(filename, "rb"); // Open file for reading in binary mode
    if (!file) {
        exit(EXIT_FAILURE);
    }
    char format[3];
    // Check for correct PPM format
    if (fscanf(file, "%s\n%d %d\n255\n", format, width, height) != 3 || format[0] != 'P' || format[1] != '6') {
        fclose(file);
        exit(EXIT_FAILURE);
    }
    // Allocate memory for pixels
    *pixels = malloc((*width) * (*height) * sizeof(RGB));
    if (!*pixels) {
        fclose(file);
        exit(EXIT_FAILURE);
    }
    // Read pixel data from file
    if (fread(*pixels, sizeof(RGB), (*width) * (*height), file) < (*width) * (*height)) {
        fclose(file);
        free(*pixels);
        exit(EXIT_FAILURE);
    }
    fclose(file);
}

// Writes PPM image file
void writePPM(const char *filename, RGB *pixels, int width, int height) {
    FILE *file = fopen(filename, "wb"); // Open file for writing in binary mode
    if (!file) {
        exit(EXIT_FAILURE);
    }
    // Write PPM header
    fprintf(file, "P6\n%d %d\n255\n", width, height);
    // Write pixel data to file
    fwrite(pixels, sizeof(RGB), width * height, file);
    fclose(file);
}

// Converts an image to grayscale
void convertToGrayscale(RGB *pixels, int width, int height) {
    for (int i = 0; i < width * height; i++) {
        // Calculate grayscale value
        int gray = (int)(0.2126f * pixels[i].r + 0.7152f * pixels[i].g + 0.0722f * pixels[i].b + 0.5f);
        // Apply grayscale value to all channels
        pixels[i].r = pixels[i].g = pixels[i].b = (unsigned char)gray;
    }
}

// Applies a sharpening filter to the image
void applySharpening(RGB *pixels, int width, int height) {
    // Sharpening kernel
    int kernel[3][3] = {{0, -1, 0}, {-1, 5, -1}, {0, -1, 0}};
    // Temporary buffer to hold the result
    RGB *temp = (RGB *)malloc(width * height * sizeof(RGB));
    if (!temp) {
        exit(EXIT_FAILURE);
    }
    
    memcpy(temp, pixels, width * height * sizeof(RGB));
    
    // Apply sharpening filter
    for (int y = 1; y < height - 1; y++) {
        for (int x = 1; x < width - 1; x++) {
            int sumR = 0, sumG = 0, sumB = 0;
            for (int ky = -1; ky <= 1; ky++) {
                for (int kx = -1; kx <= 1; kx++) {
                    int p = (y + ky) * width + (x + kx);
                    sumR += temp[p].r * kernel[ky + 1][kx + 1];
                    sumG += temp[p].g * kernel[ky + 1][kx + 1];
                    sumB += temp[p].b * kernel[ky + 1][kx + 1];
                }
            }
            // Calculate final pixel value
            int idx = y * width + x;
            pixels[idx].r = (unsigned char)fmax(0, fmin(255, sumR));
            pixels[idx].g = (unsigned char)fmax(0, fmin(255, sumG));
            pixels[idx].b = (unsigned char)fmax(0, fmin(255, sumB));
        }
    }
    
    free(temp);
}

// Calculates and writes histogram to a file
void calculateAndWriteHistogram(RGB *pixels, int width, int height, const char* filename) {
    int histogram[N] = {0}; // Histogram array
    // Calculate histogram
    for (int i = 0; i < width * height; i++) {
        unsigned char gray = pixels[i].r; // Assuming the image is already in grayscale
        int binIndex = gray / (256 / N);
        if (binIndex >= N) { 
            binIndex = N - 1;
        }
        histogram[binIndex]++;
    }

    FILE *file = fopen(filename, "w"); // Open file for writing
    if (!file) {
        exit(EXIT_FAILURE);
    }
    // Write histogram data to file
    for (int i = 0; i < N; i++) {
        fprintf(file, "%d ", histogram[i]);
    }
    fclose(file);
}

int main(int argc, char **argv) {
    if (argc != 2) {
        printf("Usage: %s <input.ppm>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    int width, height;
    RGB *pixels = NULL;
    
    // Sequence of operations on the image
    readPPM(argv[1], &pixels, &width, &height); // Read pixels from file
    applySharpening(pixels, width, height);  // Apply sharpening mask
    writePPM("output.ppm", pixels, width, height); // Write the processed image to a file
    convertToGrayscale(pixels, width, height); // Convert the image to grayscale
    calculateAndWriteHistogram(pixels, width, height, "output.txt"); // Calculate and write the histogram
    free(pixels); // Free allocated memory

    return EXIT_SUCCESS;
}
