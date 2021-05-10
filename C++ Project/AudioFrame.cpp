/************************************************
Filename: AudioFrame.cpp
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
Purpose: Defining the AudioFrame derived class
**********************************/

#include <iostream>
using namespace std;
#include "Frame.h"
#include "AudioFrame.h"


/***********************************************
Function name: CalculateFrameResource
Purpose: Calculates the file size for the frame and outputs the details

In parameters: N/A
Out Parameters: N/A

Version: 1.0
Author: Jason MacFarlane
************************************************/
void AudioFrame::CalculateFrameResource(){

	double fileSize; //calculated file size

	cout << *this << endl; //print frame name and type
	cout << "MP3 Lossy Compression" << endl; //create chart
	cout << "-----------------------------------------------------" << endl;
	cout << "bitrate (kbits/s): \t| 128 \t| 160 \t| 192" << endl;
	cout << "-----------------------------------------------------" << endl;
	cout << "file size (MB): \t|";

	cout.precision(3); //set precision for output

	for (int i = 0; i < 3; i++) {

		fileSize = size / (COMPRESSION_RATIO[i]); //calculate file size

		cout << " " << fileSize; //output

		if (i < 2) {

			cout << "\t|"; //add lines

		}

	}

	cout << endl << "-----------------------------------------------------" << endl;


}

/***********************************************
Function name: operator<< (Operator overload)
Purpose: Prints the frame name and the frame type

In parameters: [ostream& output] output stream object
			   [AudioFrame& frame] audio frame object
Out Parameters: [ostream&] output stream object

Version: 1.0
Author: Jason MacFarlane
************************************************/
ostream& operator<<(ostream& output, AudioFrame& audioFrame) {

	output << "AudioFrame: frameName = " << *((Frame*)&audioFrame); //output frame name and frame type

	return output;//return output stream

}