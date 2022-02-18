#include "Post_Processing.h"
#include <iostream>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <opencv2/dnn.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include "opencv2/imgproc/imgproc_c.h"
#include "windows.h" 
#include <filesystem>
#include <string>
#include "Point.h"
#include <algorithm>
#include <random>
using namespace cv;
using namespace dnn;
using namespace std;
namespace fs = std::filesystem;

using namespace std;


struct less_than_key
{
    inline bool operator() (const Pointd& struct1, const Pointd& struct2)
    {
        return (struct1.getRight() < struct2.getRight());
    }
};


Post_Processing::Post_Processing(ofstream& o) {
    total_wall_win_area = 0;
}


vector<Pointd> Post_Processing::postprocess(vector<Pointd>points) {
    sort(points.begin(), points.end());

    for (int i = 0; i < points.size(); i++) {
        for (int j = i + 1; j < points.size(); j++) {
            int diff = points[i].getBottom() - points[j].getTop();
            if (diff > -10 && diff < 0) {
                points[j].setTop(points[j].getTop() -
                    (-1 * diff));
            }
        }
    }


    std::sort(points.begin(), points.end(), less_than_key());
    for (int i = 0; i < points.size(); i++) {
        for (int j = i + 1; j < points.size(); j++) {
            int diff = points[i].getRight() - points[j].getLeft();
            if (diff > -10 && diff < 0) {
                points[j].setLeft(points[j].getLeft() -
                    (-1 * diff));
            }
        }
    }


    this->final_points = points;
    return this->final_points;
}

float Post_Processing::getPixels() {
    float total_area_of_window_wall = 0;
    for (int i = 0; i < final_points.size(); i++) {
        int area = 0;
        if (final_points[i].getClass() == "Wall" || final_points[i].getClass()=="Window") {
            int width = final_points[i].getRight() - final_points[i].getLeft();
            int height = final_points[i].getBottom() - final_points[i].getTop();
            area = width * height;
            total_area_of_window_wall = total_area_of_window_wall + area;
        }
      
    }
    total_wall_win_area = total_area_of_window_wall;
    return total_wall_win_area;
}
ofstream& Post_Processing::writeToFile(ofstream& output_file) {
    
    for (int i = 0; i < final_points.size(); i++) {

        int width = final_points[i].getRight() - final_points[i].getLeft();
        int height = final_points[i].getBottom() - final_points[i].getTop();

        output_file << "{\"Name\":";
        output_file << "\"" + final_points[i].getClass() + "\"";
        output_file << ",";
        output_file << "\"Pos\":";
        output_file << string("[") + to_string(final_points[i].getLeft()) + string(",") + to_string(final_points[i].getBottom()) + string("],\"Size\":");
        output_file << string("[") + to_string(width) + string(",") + to_string(height) + string("]}");
        if (i < final_points.size() - 1) {
            output_file << "," << endl;
        }
    }


}


