/************************************************
Filename: VideoFrame.cpp
Version: 1.0
Author: Jason MacFarlane
Student Number: 040890266
Course Name: C++ Programming
Course Code: CST8219
Lab Section Number: 302
Assignment Number: 3
Assignment Name: Animation in C++ with Inheritance
Due Date: 2019/11/30
Submission Date: 2019/11/27
Professor's Name: Lab: Mariusz Makos, Lecture: Surbhi Bahri
Purpose: Defining the VideoFrame derived class
**********************************/

#include <iostream>
using namespace std;
#include "Frame.h"
#include "VideoFrame.h"

/***********************************************
Function name: CalculateFrameResource
Purpose: Calculates the file size for the frame and outputs the details

In parameters: N/A
Out Parameters: N/A

Version: 1.0
Author: Jason MacFarlane
************************************************/
void VideoFrame::CalculateFrameResource() {

	double fileSize; // calculated file size

	cout << *this << endl; //Print name and type of frame
	cout << "Lempel-Ziv-Welch Lossless Compression" << endl; //create chart
	cout << "---------------------------------------------------------------------------------" << endl;
	cout << "colours: \t| 2 \t| 4 \t| 8 \t| 16 \t| 32 \t| 64 \t| 128 \t| 256" << endl;
	cout << "---------------------------------------------------------------------------------" << endl;
	cout << "file size (MB): |";

	cout.precision(4); //set precision for output

	for (int i = 0; i < 8; i++) {

		fileSize = size / (COMPRESSION_RATIO * BITDEPTH_FACTOR[i]); //calculate file size

		cout << " " << fileSize; //output numbers

		if (i < 7) {

			cout << "\t|"; // add line

		}

	}

	cout << endl << "---------------------------------------------------------------------------------" << endl;

}

/***********************************************
Function name: operator<< (Operator overload)
Purpose: Prints the frame name and the frame type

In parameters: [ostream& output] output stream object
			   [VideoFrame& frame] video frame object
Out Parameters: [ostream&] output stream object

Version: 1.0
Author: Jason MacFarlane
************************************************/
ostream& operator<<(ostream& output, VideoFrame& videoFrame) {

	output << "VideoFrame: frameName = " << *((Frame*)&videoFrame); //Output frame type and frame name

	return output; //return output stream

}