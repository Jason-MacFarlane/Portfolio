//Frame.h
#pragma once

class Frame {
	string frameName;
protected:
	double size;
public:
	Frame(string name, double sz) :frameName(name), size(sz) {};
	virtual ~Frame() {};
	virtual void CalculateFrameResource() = 0;
	friend ostream& operator<<(ostream&, Frame&);
};

