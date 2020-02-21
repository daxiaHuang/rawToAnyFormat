#ifndef RAWTOANYFORM_H
#define RAWTOANYFORM_H

#include <opencv2/core.hpp>

enum ColorFormat
{
	RGB8888,
	YUV422_YUYV_Packed
};


class RawToAnyForm
{
public:
	RawToAnyForm(void);
	~RawToAnyForm(void);
public:
	void convertForm(char* filename, cv::Mat& dst, int with, int height, int format);





};




#endif
