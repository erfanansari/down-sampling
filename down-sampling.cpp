#include <opencv2/opencv.hpp>
#include <vector>
#include <complex>
#include <iostream>

using namespace std;
using namespace cv;
using cd = complex<double>;
const double PI = acos(-1);

void fft(vector<cd> & a, bool invert) {
    int n = a.size();
    if (n == 1)
        return;

    vector<cd> a0(n / 2), a1(n / 2);
    for (int i = 0; 2 * i < n; i++) {
        a0[i] = a[2*i];
        a1[i] = a[2*i+1];
    }
    fft(a0, invert);
    fft(a1, invert);

    double ang = 2 * PI / n * (invert ? -1 : 1);
    cd w(1), wn(cos(ang), sin(ang));
    for (int i = 0; 2 * i < n; i++) {
        a[i] = a0[i] + w * a1[i];
        a[i + n/2] = a0[i] - w * a1[i];
        if (invert) {
            a[i] /= 2;
            a[i + n/2] /= 2;
        }
        w *= wn;
    }
}

void fft2(vector<vector<cd>>& a, bool invert) {
    int n = a.size();
    int m = a[0].size();

    for (int i = 0; i < n; i++) {
        fft(a[i], invert);
    }

    vector<vector<cd>> a_transpose(m, vector<cd>(n));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            a_transpose[j][i] = a[i][j];
        }
    }

    for (int i = 0; i < m; i++) {
        fft(a_transpose[i], invert);
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            a[i][j] = a_transpose[j][i];
        }
    }
}


void lowPassFilter(vector<vector<cd>>& a, double cutoff) {
    int n = a.size();
    int m = a[0].size();

    int filterSizeN = n * cutoff;
    int filterSizeM = m * cutoff;

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            if (i > filterSizeN / 2 && i < n - filterSizeN / 2 &&
                j > filterSizeM / 2 && j < m - filterSizeM / 2) {
                a[i][j] = 0;
            }
        }
    }
}

vector<vector<cd>> loadImage(const string& path) {
    Mat image = imread(path, IMREAD_GRAYSCALE);
    if (image.empty()) {
        cerr << "Could not open or find the image at " << path << endl;
        exit(EXIT_FAILURE);
    }

    vector<vector<cd>> complexImage(image.rows, vector<cd>(image.cols));
    for (int i = 0; i < image.rows; i++) {
        for (int j = 0; j < image.cols; j++) {
            complexImage[i][j] = cd(static_cast<double>(image.at<uchar>(i, j)), 0.0);
        }
    }
    return complexImage;
}

void saveImage(const string& path, const vector<vector<cd>>& complexImage) {
    if (complexImage.empty() || complexImage[0].empty()) {
        cerr << "Complex image is empty, cannot save." << endl;
        return;
    }

    Mat image(complexImage.size(), complexImage[0].size(), CV_8UC1);
    double maxVal = 0.0;
    for (const auto& row : complexImage) {
        for (const auto& val : row) {
            double mag = abs(val);
            maxVal = max(maxVal, mag);
        }
    }

    for (int i = 0; i < image.rows; i++) {
        for (int j = 0; j < image.cols; j++) {
            image.at<uchar>(i, j) = static_cast<uchar>(255.0 * abs(complexImage[i][j]) / maxVal);
        }
    }

    if (!imwrite(path, image)) {
        cerr << "Failed to save the image to " << path << endl;
    }
}

void downSample(vector<vector<cd>>& image, int factor) {
    int rows = image.size();
    int cols = image[0].size();
    vector<vector<cd>> downSampled;

    for (int i = 0; i < rows; i += factor) {
        vector<cd> row;
        for (int j = 0; j < cols; j += factor) {
            row.push_back(image[i][j]);
        }
        downSampled.push_back(row);
    }

    image = downSampled;
}

int main() {
    string inputPath = "input.jpg";
    string outputPath = "output.jpg";

    auto image = loadImage(inputPath);
    int downSampleFactor = 2;
    downSample(image, downSampleFactor);


    int rows = pow(2, ceil(log2(image.size())));
    int cols = pow(2, ceil(log2(image[0].size())));
    for (auto& row : image) {
        row.resize(cols, 0);
    }
    image.resize(rows, vector<cd>(cols, 0));

    fft2(image, false);
    double cutoff = .1;
    lowPassFilter(image, cutoff);
    fft2(image, true);

    saveImage(outputPath, image);


    return 0;
}