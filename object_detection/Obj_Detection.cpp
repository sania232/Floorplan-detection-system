#include "Obj_Detection.h"
#include <vector>
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

Obj_Detection::Obj_Detection(string image_name,ifstream& file) {
	this->image_name = image_name;
	float confThreshold = 0.5; // Confidence threshold
	float nmsThreshold = 0.4;  // Non-maximum suppression threshold
	int inpWidth = 416;  // Width of network's input image
	int inpHeight = 416; // Height of network's input imagebl
	string line;
	while (getline(file, line)) classes.push_back(line);
}

// Get the names of the output layers
vector<String> Obj_Detection::getOutputsNames(const Net& net)
{
    static vector<String> names;
    if (names.empty())
    {
        //Get the indices of the output layers, i.e. the layers with unconnected outputs
        vector<int> outLayers = net.getUnconnectedOutLayers();

        //get the names of all the layers in the network
        vector<String> layersNames = net.getLayerNames();

        // Get the names of the output layers in names
        names.resize(outLayers.size());
        for (size_t i = 0; i < outLayers.size(); ++i)
            names[i] = layersNames[outLayers[i] - 1];
    }
    return names;
}
// Remove the bounding boxes with low confidence using non-maxima suppression
vector<Pointd> Obj_Detection::getCoordinates(Mat& frame, const vector<Mat>& outs,vector<Pointd>& points_found)
{
    vector<int> classIds;
    vector<float> confidences;
    vector<Rect> boxes;

    for (size_t i = 0; i < outs.size(); ++i)
    {
        // Scan through all the bounding boxes output from the network and keep only the
        // ones with high confidence scores. Assign the box's class label as the class
        // with the highest score for the box.
        float* data = (float*)outs[i].data;
        for (int j = 0; j < outs[i].rows; ++j, data += outs[i].cols)
        {
            Mat scores = outs[i].row(j).colRange(5, outs[i].cols);
            Point classIdPoint;
            double confidence;
            // Get the value and location of the maximum score
            minMaxLoc(scores, 0, &confidence, 0, &classIdPoint);
            if (confidence > this->confThreshold)
            {
                int centerX = (int)(data[0] * frame.cols);
                int centerY = (int)(data[1] * frame.rows);
                int width = (int)(data[2] * frame.cols);
                int height = (int)(data[3] * frame.rows);
                int left = centerX - width / 2;
                int top = centerY - height / 2;

                classIds.push_back(classIdPoint.x);
                confidences.push_back((float)confidence);
                boxes.push_back(Rect(left, top, width, height));
            }
        }
    }

    // Perform non maximum suppression to eliminate redundant overlapping boxes with
    // lower confidences
    vector<int> indices;
    
    dnn::NMSBoxes(boxes, confidences, confThreshold, nmsThreshold, indices);
    std::cout << indices.size() << endl;
    for (size_t i = 0; i < indices.size(); ++i)
    {
        int idx = indices[i];
        Rect box = boxes[idx];
        drawPred(classIds[idx], confidences[idx], box.x, box.y,
            box.x + box.width, box.y + box.height, frame,points_found);


    }
    cout << points_found.size();
    return points_found;


}
// Draw the predicted bounding box
void Obj_Detection::drawPred(int classId, float conf, int left, int top, int right, int bottom, Mat& frame,vector<Pointd>& points_found)
{

    //Draw a rectangle displaying the bounding box
    rectangle(frame, Point(left, top), Point(right, bottom), Scalar(255, 178, 50), 3);

    //Get the label for the class name and its confidence
    string name;
    string label;
    if (!classes.empty())
    {
        CV_Assert(classId < (int)classes.size());
        label = classes[classId];
        name = classes[classId];
    }
    points_found.push_back(Pointd(name, left, right, top, bottom));
    //Display the label at the top of the bounding box
    int baseLine;
    Size labelSize = getTextSize(label, FONT_HERSHEY_SIMPLEX, 0.25, 1, &baseLine);
    top = max(top, labelSize.height);
    rectangle(frame, Point(left, top - round(1.5 * labelSize.height)), Point(left + round(1.5 * labelSize.width), top + baseLine), Scalar(255, 255, 255), FILLED);
    putText(frame, label, Point(left, top), FONT_HERSHEY_SIMPLEX, 0.25, Scalar(0, 0, 0), 1);
   
}
int Obj_Detection::getInpWidth() {
    return this->inpWidth;
}
int Obj_Detection::getInpHeight() {
    return this->inpHeight;
}