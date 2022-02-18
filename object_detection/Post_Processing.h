#ifndef Post_HEADER
#define Post_HEADER
#include <iostream>
#include <vector>
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

class Post_Processing {
	
	vector<Pointd> final_points;
	float total_wall_win_area;


public: 
	Post_Processing(ofstream& o);
	vector<Pointd>postprocess(vector<Pointd>points);
	float getPixels();
	ofstream& writeToFile(ofstream& output_file);
};
#endif
