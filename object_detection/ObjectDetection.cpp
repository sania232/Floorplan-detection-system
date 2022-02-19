
#include <fstream>
#include <sstream>
#include <iostream>
#include <opencv2/dnn.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include "opencv2/imgproc/imgproc_c.h"
#include "windows.h" 
#include <filesystem>
#include <string>
#include "Point.h"
#include "Post_Processing.h"
#include "Obj_Detection.h"
#include <algorithm>
#include <random>
#include "ProcessingClass.hpp"
using namespace cv;
using namespace dnn;
using namespace std;
namespace fs = std::filesystem;





vector<Pointd>points_found;

int main() {
    //get the absolute path where this program is executed
    fs::path absoluteCurrentPath = fs::current_path();
   
    std::cout << "Current Path Determined: " << absoluteCurrentPath << std::endl;

    //append the "SharedFolder" to the found path
    fs::path input_directory = absoluteCurrentPath;
    input_directory /= "SharedFolder";

    if (!(std::filesystem::exists(input_directory)))
    {
        //create the directory if it does not exist and wait until an image is supplied.
        fs::create_directories(input_directory);
        std::cout << "Created the Shared Folder" << input_directory << std::endl;
    }
    std::cout << std::endl;
    std::cout << "Located Shared Folder: " << input_directory << std::endl;


    fs::space_info devi = fs::space(input_directory);

    //create the outputting directory for the preprocessed images
    fs::path output_directory = absoluteCurrentPath;
    output_directory /= "Processed Directory";
    fs::create_directories(output_directory);

    //original image location
    input_directory /= "Floorplan.png";
    std::string image_path = input_directory.string().c_str();
    std::cout << "Expecting Floorplan image: " << image_path << std::endl;

    //stores the original image
    cv::Mat image;

    //after removal go back to looping until the person re-imports the image
    loop: while (!std::filesystem::exists(image_path))
    {
        Sleep(1);
    }

    try
    {
        image = cv::imread(image_path, cv::IMREAD_COLOR);
        if (image.empty())
        {
            std::cout << "Could not read the image: " << image_path << std::endl;
            throw - 99;
        }
    }
    catch (int errorcode)
    {
        std::cout << "Re-supply a floorplan image that's not empty, invalid or corrupt" << std::endl;
        //otherwise its an infinte loop and we'd like to go back to waiting for the correct image
        fs::remove(image_path);
        goto loop;
    }
    //instance of Preprocessing to use the functions
    ProcessingClass p;

    //call to generate imwrite filename
    std::string writeName;
    writeName = p.extractAndAppend(image_path);

    //create a deep copy of the original in the processed directory
    cv::Mat copy = image.clone();
    cv::imwrite(std::string(output_directory.string().c_str()) + "\\" + writeName + ".png", copy);
    string image_name = string(output_directory.string().c_str()) + "\\" + writeName + ".png";
    std::cout << "Wrote copy to disk for image = " << writeName << std::endl;

    //convert image from BGR to grayscale (one tuple value per cell)
    cv::Mat gray;
    gray = p.grayscale(image);

    //resize the image to 416,416
    writeName = p.extractAndAppend(image_path);
   // cv::Mat resized = p.resizeImage(gray);
    cv::imwrite(std::string(output_directory.string().c_str()) + "\\" + writeName + ".png", gray);
    std::cout << "Resized and wrote to disk for image = " << writeName << std::endl;

    cv::Mat binaryMask;
    binaryMask = p.generateBinaryMask(gray);
    writeName = p.extractAndAppend(image_path);
    cv::imwrite(std::string(output_directory.string().c_str()) + "\\" + writeName + ".png", binaryMask);
    std::cout << "Binary mask wrote to disk for image = " << writeName << std::endl;

    size_t finalvalue = p.fillAndCountPixels(binaryMask);
    std::cout << "Total Number of White Pixels Counted = " << finalvalue << " For Image = " << writeName << std::endl;



    // Load names of classes
   
    string classesFile = "obj.names";
    ifstream ifs(classesFile.c_str());
    string line;
    Obj_Detection obj(image_name, ifs);
   

    // Give the configuration and weight files for the model
    String modelConfiguration = "yolov3_custom2.cfg";
    String modelWeights = "yolov3_custom2_last.weights";

    // Load the network
    Net net = readNetFromDarknet(modelConfiguration, modelWeights);
    std::cout << "Loaded the network" << std::endl;


    net.setPreferableBackend(DNN_BACKEND_OPENCV);
    net.setPreferableTarget(DNN_TARGET_CPU);

    // Open a video file or an image file or a camera stream.
    string  outputFile;


    Mat frame, blob;

    //read image
    frame = imread(image_name, IMREAD_COLOR);
    //output labelled image
    outputFile = "yolo_out_cpp.avi";
    image_name.replace(image_name.end() - 4, image_name.end(), "_yolo_out_cpp.png");
    outputFile = image_name;


   //output json file
    ofstream outputFileJ;
    fs::path shared_directory = absoluteCurrentPath;
    shared_directory /= "SharedFolder";
    outputFileJ.open(string(shared_directory.string().c_str()) + "\\" + "Dataset.json");
  


    // Create a 4D blob from a frame.
    blobFromImage(frame, blob, 1 / 255.0, cvSize(obj.getInpWidth(), obj.getInpHeight()), Scalar(0, 0, 0), true, false);

    //Sets the input to the network
    net.setInput(blob);

    // Runs the forward pass to get output of the output layers
    vector<Mat> outs;
    net.forward(outs, obj.getOutputsNames(net));


    Post_Processing p2(outputFileJ);
    
    // Remove the bounding boxes with low confidence
    obj.getCoordinates(frame, outs,points_found);
    p2.postprocess(points_found);
    
    if (points_found.size()<= 4) {
        outputFileJ << "{\"Assets\": [" << endl;
        outputFileJ << "{\"Name\":\"TotalImageHeight\",\"Pos\":[0,0],\"Size\":[0,0]}," << endl;
    }
    else {
        outputFileJ << "{\"Assets\": [" << endl;
        outputFileJ << "{\"Name\":\"TotalImageHeight\",\"Pos\":[0,0],\"Size\":[" << frame.rows << ",0]}," << endl;
    }
    float x = p2.getPixels();
    p2.writeToFile(outputFileJ);
   
    // Put efficiency information. The function getPerfProfile returns the overall time for inference(t) and the timings for each of the layers(in layersTimes)
    vector<double> layersTimes;
    double freq = getTickFrequency() / 1000;
    double t = net.getPerfProfile(layersTimes) / freq;
    string label = format("Inference time for a frame : %.2f ms", t);
    putText(frame, label, Point(0, 15), FONT_HERSHEY_SIMPLEX, 0.25, Scalar(0, 0, 255));

    // Write the frame with the detection boxes
    Mat detectedFrame;
    frame.convertTo(detectedFrame, CV_8U);
    imwrite(outputFile, detectedFrame);

      //NOTE: COORDINATES ARE FROM NON RESIZED IMAGE

    float floor_area = finalvalue - x;
    outputFileJ << endl;
    outputFileJ << ",{\"Name\":\"TotalPixelArea\",\"Pos\":[0,0],\"Size\":[" << floor_area << ",0]}]}";
    outputFileJ.close();
    std::cout << "Generated the JSON dataset" << std::endl;
    

    //remove the file which is Flooplan.png (only in the Share Directory) - and get back to waiting
    //fs::remove(image_path);
    fs::remove_all(output_directory);
    std::cout << "Floorplan.png has been removed, supply a new one." << std::endl;
    

    return 0;
    
}



