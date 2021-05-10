////AudioFrame.h
#pragma once

class AudioFrame :public Frame {
	static const int RATES = 3;
	static constexpr double BITRATE[]{ 128,160,192 };
	static constexpr double COMPRESSION_RATIO[]{ 11.1,9.1,7.1 };
	void CalculateFrameResource();
public:
	AudioFrame(string frameName, double fileSize) :Frame(frameName, fileSize) {}
	AudioFrame(const AudioFrame& RA) : Frame(RA) {}
	~AudioFrame() {}
	friend ostream& operator<<(ostream&, AudioFrame&);
};
