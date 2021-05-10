//Animation.h
#pragma once

class Animation
{
	string animationName;
	forward_list<Frame*> frames;
public:
	Animation(string name) :animationName(name) {}
	~Animation();
	void EditFrame();
	void DeleteFrame();
	friend istream& operator>>(istream&, Animation&);// Add a Frame as in cin >> A;
	friend ostream& operator<<(ostream&, Animation&);// output the Frames as in cout << A;
};

