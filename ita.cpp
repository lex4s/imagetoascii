#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

const string ASCII_CHARS = "@%#*+=-:. ";

string image_to_ascii(const Mat& img, int output_width) {
    // Resize image maintaining aspect ratio
    int width = img.cols;
    int height = img.rows;
    float aspect_ratio = static_cast<float>(height) / width;
    int new_height = static_cast<int>(output_width * aspect_ratio);

    Mat resized_img;
    resize(img, resized_img, Size(output_width, new_height));

    // Convert image to grayscale
    Mat gray_img;
    cvtColor(resized_img, gray_img, COLOR_BGR2GRAY);

    // Map pixels to ASCII characters
    string ascii_art;
    for (int i = 0; i < gray_img.rows; ++i) {
        for (int j = 0; j < gray_img.cols; ++j) {
            int pixel_value = gray_img.at<uchar>(i, j);
            ascii_art += ASCII_CHARS[pixel_value / 25];
        }
        ascii_art += '\n';
    }
    return ascii_art;
}

void save_to_file(const string& ascii_art, const string& output_path) {
    ofstream file(output_path);
    if (!file) {
        cerr << "Error saving ASCII art to file." << endl;
        exit(1);
    }
    file << ascii_art;
    file.close();
}

int main(int argc, char* argv[]) {
    if (argc != 4) {
        cerr << "Usage: ./image_to_ascii <image_path> <output_path> <width>" << endl;
        return 1;
    }

    string image_path = argv[1];
    string output_path = argv[2];
    int output_width;
    try {
        output_width = stoi(argv[3]);
    }
    catch (...) {
        cerr << "Width must be an integer." << endl;
        return 1;
    }

    // Read image
    Mat img = imread(image_path);
    if (img.empty()) {
        cerr << "Error opening image." << endl;
        return 1;
    }

    // Generate ASCII Art
    string ascii_art = image_to_ascii(img, output_width);

    // Save ASCII Art to file
    save_to_file(ascii_art, output_path);

    cout << "ASCII art saved to " << output_path << endl;
    return 0;
}
