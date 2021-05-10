/************************************************
Filename: AnimationManager.cpp
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
Purpose: Defining the AnimationManager class
**********************************/

#include <iostream> 
#include <vector>
#include <forward_list>
using namespace std;
#include "Frame.h"
#include "Animation.h"
#include "AnimationManager.h"

/***********************************************
Function name: EditAnimation
Purpose: Edit a specific Animation object

In parameters: N/A
Out Parameters: N/A

Version: 1.0
Author: Jason MacFarlane
************************************************/
void AnimationManager::EditAnimation() {

	int indexSelected; //To store user inputed index
	char menuSelection = '0'; //To store user inputed menu selection

	if (animations.empty()) {

		cout << "There are currently no Animations in this AnimationManager" << endl;

	}
	else {

		cout << "Which Animation do you wish to edit? Please give the index (from 0 to " << animations.size() - 1 << "): ";
		cin >> indexSelected; //Take user input for index

		while (cin.fail() || indexSelected < 0 || indexSelected >= (int)animations.size()) { //if user input is invalid

			if (cin.fail()) { //if cin failed clear and ignore

				cin.clear();
				cin.ignore(numeric_limits<streamsize>::max(), '\n');

			}

			//try again for input
			cout << "Please enter a valid selection: ";
			cin >> indexSelected;

		}

		cout << "Editing Animation #" << indexSelected << endl;

		while (menuSelection != '4') {

			//Creating Edit Animation menu
			cout << "MENU" << endl;
			cout << "1. Insert a Frame at front" << endl;
			cout << "2. Delete first Frame" << endl;
			cout << "3. Edit a Frame" << endl;
			cout << "4. Quit" << endl;
			cin >> menuSelection;

			switch (menuSelection) {

			case '1':

				cin >> animations[indexSelected];
				break;

			case '2':

				animations[indexSelected].DeleteFrame();
				break;

			case '3':

				animations[indexSelected].EditFrame();
				break;

			case '4':

				cout << "Animation #" << indexSelected << " edit complete" << endl;
				break;

			default:

				cout << "Please enter a valid input..." << endl;
				break;

			}

		}

	}

}

/***********************************************
Function name: DeleteAnimation
Purpose: Delete a specific Animation object

In parameters: N/A
Out Parameters: N/A

Version: 1.0
Author: Jason MacFarlane
************************************************/
void AnimationManager::DeleteAnimation() {

	int indexSelected; //To store user inputed index

	cout << "Delete an Animation from the Animation Manager" << endl;

	if (!animations.empty()) {

		cout << "Which Animation do you wish to delete? Please give the index in the range 0 to " << animations.size() - 1 << ": ";
		cin >> indexSelected; //Take user input for index

		while (cin.fail() || indexSelected < 0 || indexSelected >= (int)animations.size()) { //if user input is invalid

			if (cin.fail()) { //if cin failed clear and ignore

				cin.clear();
				cin.ignore(numeric_limits<streamsize>::max(), '\n');

			}

			//try again for input
			cout << "Please enter a valid selection: ";
			cin >> indexSelected;

		}

		animations.erase(animations.begin() + indexSelected); //erase

		cout << "Animation #" << indexSelected << " deleted" << endl;

	}
	else {

		cout << "Animation Manager is empty" << endl;

	}

}

/***********************************************
Function name: operator>> (Operator overload)
Purpose: Add an Animation object ot the AnimationManager

In parameters: [istream& input] input stream object
			   [Animation& manager] AnimationManager object
Out Parameters: [istream&] input stream object

Version: 1.0
Author: Jason MacFarlane
************************************************/
istream& operator>>(istream& input, AnimationManager& manager) {
	
	string animationName; //Name for new Animation

	if (manager.animations.empty()) {

		manager.animations.reserve(32); //Reserve memory for frames so vector doesn't need to reallocate

	}

	cout << "Add an Animation to the Animation Manager" << endl;
	cout << "Please enter the Animation Name: ";
	input >> animationName; //User input

	manager.animations.emplace_back(animationName); //Add to the back of animations

	cout << "Animation " << animationName << " added at the back of animations" << endl;

	return input; //return input stream

}

/***********************************************
Function name: operator<< (Operator overload)
Purpose: Print the contents of the AnimationManager

In parameters: [ostream& output] output stream object
			   [Animation& manager] AnimationManager object
Out Parameters: [ostream&] output stream object

Version: 1.0
Author: Jason MacFarlane
************************************************/
ostream& operator<<(ostream& output, AnimationManager& manager) {

	output << "AnimationManager: " << manager.managerName << endl;

	if (manager.animations.empty()) { //If AnimationManager is empty

		output << "There are currently no Animations in this AnimationManager" << endl;

	}
	else {

		//Print contents of AnimationManager
		for (int i = 0; i < (int)manager.animations.size(); i++) {

			output << "Animation: " << i << endl;
			output << manager.animations[i];

		}

	}

	return output; //return output stream

}