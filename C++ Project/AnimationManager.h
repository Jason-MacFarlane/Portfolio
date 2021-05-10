// AnimationManager.h
#pragma once

class AnimationManager
{
	string managerName;
	vector<Animation> animations;
public:
	AnimationManager(string name) :managerName(name) {}
	~AnimationManager() {}
	void EditAnimation();
	void DeleteAnimation();
	friend istream& operator>>(istream&, AnimationManager&);// add an Animation
	friend ostream& operator<<(ostream&, AnimationManager&);// output the Animations
};

