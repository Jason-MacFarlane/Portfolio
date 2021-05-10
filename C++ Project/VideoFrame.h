// VideoFrame.h
#pragma once

class VideoFrame :public Frame {
	static const int BITS = 8;
	static constexpr double COMPRESSION_RATIO = 6.0;
	static constexpr double BITDEPTH_FACTOR[]{ 11.1,4.6,3.5,2.4,1.9,1.5,1.2,1.0 };
	void CalculateFrameResource();
public:
	VideoFrame(string fileName, double fileSize) :Frame(fileName, fileSize) {}
	VideoFrame(const VideoFrame& RV) :Frame(RV) {}
	~VideoFrame() {}
	friend ostream& operator<<(ostream&, VideoFrame&);
};

