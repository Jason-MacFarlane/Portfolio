/************************************************
Filename: Frame.cpp
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
Purpose: Defining the Frame base class
**********************************/

#include <iostream>
using namespace std;
#include "Frame.h"

/***********************************************
Function name: operator<< (Operator overload)
Purpose: Prints the Frame name

In parameters: [ostream& output] output stream object
			   [Frame& frame] frame object
Out Parameters: [ostream&] output stream object

Version: 1.0
Author: Jason MacFarlane
************************************************/
ostream& operator<<(ostream& output, Frame& frame) {

	output << frame.frameName; //output frame name

	return output;

}
