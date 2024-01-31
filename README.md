## FFT Image Processing
### Overview
This project implements a two-dimensional Fast Fourier Transform (FFT) for image processing. It includes downsampling an image, applying a 2D FFT, a low-pass filter, and then saving the processed image.

### Features
- **FFT Implementation:** Recursive FFT and inverse FFT.
- **Image Downsampling:** Reduces image resolution for efficient processing.
- **Low-Pass Filtering:** Applies a frequency domain filter to smooth the image and reduce high-frequency noise.
- **Image IO:** Loads and saves images using OpenCV.
### Requirements
- C++ Compiler
- OpenCV Library
###Usage
1. **Compile the Code:** Use a C++ compiler to compile the code. For example: `g++ -o fft_processor main.cpp -lopencv_core -lopencv_imgcodecs -lopencv_highgui`
2. **Run the Program:** Execute the compiled program. Ensure an input image `input.jpg` is available in the program directory.
### Input/Output
- **Input:** An image file named `input.jpg`.
- **Output:** Processed image saved as `output.jpg`.
### Author
[Erfan Ansari](https://github.com/erfanansari) - 991406504

