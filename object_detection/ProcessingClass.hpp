#ifndef PREPROCESS
#define PREPROCESS

#include <filesystem>
#include <random>
#include <iostream>
#include <opencv2/imgproc.hpp>  //Usage of cv::Mat structure to store/read images
#include <opencv2/highgui.hpp>  //Usage of cv::imwrite() and cv::imread()
namespace fs = std::filesystem;


class ProcessingClass
{
private:
    size_t totalPixelNumber;
    const int threshold_value = 228;
    const int max_value = 255;

public:
    ProcessingClass() {}
    //extracts the filename from the original image and th
    std::string extractAndAppend(std::string filename)
    {
        fs::path name = filename;
        std::string temp = name.filename().string().c_str();
        size_t pos = temp.find(".png");
        temp = temp.substr(0, pos);
        temp += "-";

        std::string Lookup =
            "0123456789"
            "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
            "abcdefghijklmnopqrstuvwxyz";

        std::random_device rd;
        std::mt19937 g(rd());
        std::uniform_int_distribution<> distribution(0, Lookup.size() - 1);

        for (size_t i = 0; i < 10; i++)
            temp += Lookup[distribution(g)];

        return temp;
    }

    // takes in an image as input and will return the resized image back
    cv::Mat resizeImage(cv::Mat image)
    {
        int width = image.cols;
        int height = image.rows;
        int resolution = 416;
        int interpolation;

        if (height > resolution || width > resolution)
        {
            interpolation = CV_INTER_AREA; //downscale the image
        }
        else
        {
            interpolation = CV_INTER_CUBIC; //upscale the image
        }

        float aspect_ratio = (float)width / (float)height;

        int new_width, new_height;
        int left_padding, right_padding, top_padding, bottom_padding;
        float horizontal_padding, vertical_padding;

        if (aspect_ratio > 1) //horizontal image
        {
            new_width = resolution;
            new_height = floor(new_width / aspect_ratio);
            std::cout << new_height << std::endl;
            vertical_padding = (float)(resolution - new_height) / 2;
            std::cout << vertical_padding << std::endl;
            top_padding = floor(vertical_padding);
            bottom_padding = ceil(vertical_padding);
            left_padding = 0;
            right_padding = 0;
        }
        else if (aspect_ratio < 1) //vertical image
        {
            new_height = resolution;
            new_width = floor((float)new_height * aspect_ratio);
            std::cout << new_width << std::endl;
            horizontal_padding = (float)(resolution - new_width) / 2;
            std::cout << horizontal_padding << std::endl;
            left_padding = floor(horizontal_padding);
            right_padding = ceil(horizontal_padding);
            top_padding = 0;
            bottom_padding = 0;
        }
        cv::Size dim(new_width, new_height);
        //resive the image with the aspect ratio
        cv::resize(image, image, dim, interpolation);
        //pad the necessary sides
        cv::copyMakeBorder(image, image, top_padding, bottom_padding, left_padding, right_padding, cv::BORDER_CONSTANT, cv::Scalar(255, 255, 255));

        return image;
    }

    cv::Mat grayscale(cv::Mat image)
    {
        //convert image from BGR to grayscale (one tuple value per cell)
        cv::Mat gray;
        cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);

        return gray;
    }

    cv::Mat generateBinaryMask(cv::Mat gray)
    {
        //Apply Gaussian Blur to the image
        cv::Mat blurred;
        cv::blur(gray, blurred, cv::Size(3, 3));

        //Apply a binary threshold to the image
        // const int threshold_value = 228;
        // const int max_value = 255;
        cv::Mat threshold;
        cv::threshold(blurred, threshold, threshold_value, max_value, 0);

        //Apply edge detection to get fine edges
        cv::Canny(threshold, threshold, threshold_value, threshold_value * 2);

        //Reduces the filling
        cv::dilate(threshold, threshold, cv::Mat(), cv::Point(-1, -1));

        std::vector<std::vector<cv::Point>> contours;
        std::vector<cv::Vec4i> hierarchy;
        cv::findContours(threshold, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

        cv::Scalar color = cv::Scalar(255, 255, 255);
        cv::Mat drawing = cv::Mat::zeros(threshold.size(), CV_8UC1);
        cv::drawContours(drawing, contours, -1, color, cv::FILLED);

        return drawing;
    }

    size_t fillAndCountPixels(cv::Mat drawing)
    {
        totalPixelNumber = cv::countNonZero(drawing);

        return totalPixelNumber;
    }


};




#endif