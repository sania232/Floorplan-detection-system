#ifndef OBJ_HEADER
#define OBJ_HEADER
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

class Obj_Detection {
	// Initialize the parameters
	string image_name;
	float confThreshold = 0.5; // Confidence threshold
	float nmsThreshold = 0.4;  // Non-maximum suppression threshold
	int inpWidth = 416;  // Width of network's input image
	int inpHeight = 416; // Height of network's input imagebl
	vector<string>classes;
	//vector<Pointd>points_found;


public:
	Obj_Detection(string image_name,ifstream& file);
	vector<String> getOutputsNames(const Net& net);
	int getInpWidth();
	int getInpHeight();
	vector<Pointd> getCoordinates(Mat& frame, const vector<Mat>& outs, vector<Pointd>& points_found);
	void drawPred(int classId, float conf, int left, int top, int right, int bottom, Mat& frame, vector<Pointd>& points_found);
};
//ostream& operator<<(ostream& o, Pointd s);

#endif
