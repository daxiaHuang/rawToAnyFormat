#include "rawToAnyForm.h"
#include <algorithm>
#include <opencv2/highgui.hpp>

#define INT_TO_UCHAR(V) (unsigned char)((unsigned)V <= 255 ? V : V > 0 ? 255 : 0)

RawToAnyForm::RawToAnyForm(void)
{
}


RawToAnyForm::~RawToAnyForm(void)
{
}

void RawToAnyForm::convertForm(std::string filename, std::string savePath, cv::Mat& dst, int width, int height, int format)
{
	dst = cv::Mat(height, width, CV_8UC3);
	int framesize;
	//Open raw Bayer image.
	FILE *fp = NULL;
	fp = fopen(filename.c_str(), "rb");
	if (format == RGB8888)
	{
		framesize = width * height * 4;
		//Memory allocation for bayer image data buffer.
		uchar *imagedata = NULL;
		imagedata = (uchar*)malloc(sizeof(uchar) * framesize);
		//Read image data and store in buffer.
		fread(imagedata, sizeof(uchar), framesize, fp);
		unsigned char* pImageDataBuf = dst.data;
		for (int row = 0; row < height; ++row, imagedata += (width * 4))
		{
			uchar *ptr = dst.ptr<uchar>(row);
			for (int col = 0; col < width; ++col)
			{
				ptr[3 * col] = imagedata[4 * col];
				ptr[3 * col + 1] = imagedata[4 * col + 1];
				ptr[3 * col + 2] = imagedata[4 * col + 2];
			}
		}
	}
	else if (format == YUV422_YUYV_Packed)
	{
		framesize = width * height * 2;
		uchar* pYuvDataBuf = new uchar[framesize];
		fread(pYuvDataBuf, sizeof(uchar), framesize, fp);
		uchar* pImageDataBuf = dst.data;
		uchar* pImagebuf = (uchar *)pYuvDataBuf;
		int i, j;

		for (j = 0; j < height; j++, pYuvDataBuf += (width * 2))
		{
			for (i = 0; i < 2 * width; i += 4, pImageDataBuf += 6)
			{
				int u = (int)(pYuvDataBuf[i + 3]) - 128;
				int v = (int)(pYuvDataBuf[i + 1]) - 128;

				int ruv = (1 << 19) + 2130771 * v;
				int guv = (1 << 19) - 413812 * v - 608826 * u;
				int buv = (1 << 19) + 1195253 * u;
				if (0)
				{
					int temp = ruv;
					ruv = buv;
					buv = temp;
				}
				int y00 = std::max(0, (int)(pYuvDataBuf[i + 0])) << 20;
				pImageDataBuf[0] = INT_TO_UCHAR((y00 + ruv) >> 20);
				pImageDataBuf[1] = INT_TO_UCHAR((y00 + guv) >> 20);
				pImageDataBuf[2] = INT_TO_UCHAR((y00 + buv) >> 20);

				int y01 = std::max(0, (int)(pYuvDataBuf[i + 2])) << 20;
				pImageDataBuf[3] = INT_TO_UCHAR((y01 + ruv) >> 20);
				pImageDataBuf[4] = INT_TO_UCHAR((y01 + guv) >> 20);
				pImageDataBuf[5] = INT_TO_UCHAR((y01 + buv) >> 20);
			}
		}

	}
	else
	{

	}
	fclose(fp);

	char filepath[256];
	//save image
	std::vector<cv::Mat> channels;
	split(dst, channels);//利用vector对象分离
	sprintf_s(filepath, "%s\\origin.bmp", savePath.c_str());
	cv::imwrite(filepath, dst);
	sprintf_s(filepath, "%s\\red.bmp", savePath.c_str());
	cv::imwrite(filepath, channels[0]);
	sprintf_s(filepath, "%s\\green.bmp", savePath.c_str());
	cv::imwrite(filepath, channels[1]);
	sprintf_s(filepath, "%s\\blue.bmp", savePath.c_str());
	cv::imwrite(filepath, channels[2]);

	return;
}