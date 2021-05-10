/************************************************
Filename: Animation.cpp
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
Purpose: Defining the Animation class
**********************************/

#include <iostream> 
#include <forward_list>
using namespace std;
#include "Frame.h"
#include "AudioFrame.h"
#include "VideoFrame.h"
#include "Animation.h"

/***********************************************
Function name: ~Animation
Purpose: Animation destructor

In parameters: N/A
Out Parameters: N/A

Version: 1.0
Author: Jason MacFarlane
************************************************/
Animation::~Animation() {

	if (!frames.empty()) {

		for (Frame*& frame : frames) { //Cycle through frames

			delete frame; //Delete all frames in frames

		}

	}

}

/***********************************************
Function name: EditFrame
Purpose: Edit a specific Frame object

In parameters: N/A
Out Parameters: N/A

Version: 1.0
Author: Jason MacFarlane
************************************************/
void Animation::EditFrame() {

	int listSize = 0; //Counter
	int indexSelected; //User inputed index
	forward_list <Frame*> ::iterator it; //interator for forward_list
	string frameName; //new Frame name
	double frameMB; //new frame size
	char frameType; //new frame type

	for (Frame*& frame : frames) { //get size of frames list

		listSize++;

	}

	cout << "Edit a Frame in the Animation" << endl;

	if (listSize > 0) { //if there are contents in the list

		cout << "There are " << listSize << " Frame(s) in the list. Please specify the index (<=" << listSize - 1 << ") to edit at : ";
		cin >> indexSelected;

		while (cin.fail() || indexSelected < 0 || indexSelected >= listSize) { //if user input is invalid

			if (cin.fail()) { //if cin failed clear and ignore

				cin.clear();
				cin.ignore(numeric_limits<streamsize>::max(), '\n');

			}

			//try again for input
			cout << "Please enter a valid selection: ";
			cin >> indexSelected;

		}

		it = frames.begin(); //assign iterator to beginning of list

		for (int i = 0; i < indexSelected; i++) { //iterate to index selected

			it++;

		}

		cout << "The name of this Frame is " << *(*it) << "." << endl;
		cout << "Please enter new Frame frameName: ";
		cin >> frameName; //User input for new name

		cout << "Please enter new Frame size(MB): ";
		cin >> frameMB; //User input for new size

		while (cin.fail() || frameMB < 0) { //if user input is invalid

			if (cin.fail()) { //if cin failed clear and ignore

				cin.clear();
				cin.ignore(numeric_limits<streamsize>::max(), '\n');

			}

			//try again for input
			cout << "Please enter a valid selection: ";
			cin >> frameMB;

		}

		cout << "Please enter new Frame type (AudioFrame = A, VideoFrame = V): ";
		cin >> frameType; //user input for new type

		while (frameType != 'A' && frameType != 'V') { //Loop until correct input

			cout << "Please enter a valid selection: ";
			cin >> frameType;

		}

		delete *it; //delete the frame in the current location

		if (frameType == 'A') { //add new frame to the current location

			*it = new AudioFrame(frameName, frameMB);

		}
		else {

			*it = new VideoFrame(frameName, frameMB);

		}

		cout << "Frame #" << indexSelected << " edit completed" << endl;

	}
	else {

		cout << "This are no Frames in the Animation" << endl;

	}

}

/***********************************************
Function name: DeleteFrame
Purpose: Delete Frame object from the front of the forward list

In parameters: N/A
Out Parameters: N/A

Version: 1.0
Author: Jason MacFarlane
************************************************/
void Animation::DeleteFrame() {

	if (frames.empty()) {

		cout << "There are no frames to delete." << endl;

	}
	else {

		delete *frames.begin(); //call delete on frame* before pop to prevent memory leaks
		frames.pop_front(); //delete first frame in the list
		cout << "First Frame deleted" << endl;

	}

}

/***********************************************
Function name: operator>> (Operator overload)
Purpose: Add an Frame object ot the Animation

In parameters: [istream& input] input stream object
			   [Animation& animation] animation object
Out Parameters: [istream&] input stream object

Version: 1.0
Author: Jason MacFarlane
************************************************/
istream& operator>>(istream& input, Animation& animation) {

	string frameName; //New frame name
	double frameMB; //New frame size
	char frameType; //New frame type
	AudioFrame* newAudioFrame; //Possible AudioFrame*
	VideoFrame* newVideoFrame; //Possible VideoFrame*

	cout << "Insert a Frame in the Animation" << endl;
	cout << "Please enter the Frame frameName: ";
	input >> frameName; //User input for frame name

	cout << "Please enter the Frame size(MB): ";
	input >> frameMB; //User input for frame size

	while (input.fail() || frameMB < 0) { //if user input is invalid

		if (input.fail()) { //if cin failed clear and ignore

			input.clear();
			input.ignore(numeric_limits<streamsize>::max(), '\n');

		}

		//try again for input
		cout << "Please enter a valid selection: ";
		input >> frameMB;

	}

	cout << "Please enter the Frame type (AudioFrame = A, VideoFrame = V): ";
	input >> frameType; //uder input for frame type

	while (frameType != 'A' && frameType != 'V') { //loop until correct input given

		cout << "Please enter a valid selection: ";
		input >> frameType;

	}

	if (frameType == 'A') { // case to frame* and add to frames

		newAudioFrame = new AudioFrame(frameName, frameMB);
		animation.frames.push_front((Frame*)newAudioFrame);
	
	}
	else {

		newVideoFrame = new VideoFrame(frameName, frameMB);
		animation.frames.push_front((Frame*)newVideoFrame);

	}

	cout << "Frame " << frameName << " Frame* added at the front of frames" << endl;

	return input;

}

/***********************************************
Function name: operator<< (Operator overload)
Purpose: Print the contents of the Animation

In parameters: [ostream& output] output stream object
			   [Animation& animation] animation object
Out Parameters: [ostream&] output stream object

Version: 1.0
Author: Jason MacFarlane
************************************************/
ostream& operator<<(ostream& output, Animation& animation) {

	forward_list <Frame*> ::iterator it; //Iterator
	int i = 0; //Counter

	output << "\tAnimation name is " << animation.animationName << endl;
	output << "\tReport the Animation" << endl;

	if (animation.frames.empty()) {

		output << "\tNo frames in the Animation" << endl;

	}
	else {
		//Display contents of each frame
		for (it = animation.frames.begin(); it != animation.frames.end(); it++) {

			output << "Frame #" << i << endl;
			(*it)->CalculateFrameResource();//call function on each frame
			i++;

		}

	}

	return output; //return output stream

}