#include "opencv2/opencv.hpp"
#include "light.h"
#include "armor.h"
#include <time.h>
#include "allprocess.h"
#include "inputchoose.h"

using namespace std;
using namespace cv;

void Gammas(Mat& src, Mat& dst, float fGamma)        //Gamma提升亮度
{
	unsigned char lut[256];
	for (int i = 0; i < 256; i++)
	{
		lut[i] = saturate_cast<uchar>(pow((float)(i / 255.0), fGamma) * 255.0f);
	}
	dst = src.clone();
	const int channels = dst.channels();
	MatIterator_<Vec3b> it, end;
	for (it = dst.begin<Vec3b>(), end = dst.end<Vec3b>(); it != end; it++)
	{
		//(*it)[0] = pow((float)(((*it)[0])/255.0), fGamma) * 255.0;
		//(*it)[1] = pow((float)(((*it)[1])/255.0), fGamma) * 255.0;
		//(*it)[2] = pow((float)(((*it)[2])/255.0), fGamma) * 255.0;
		(*it)[0] = lut[((*it)[0])];
		(*it)[1] = lut[((*it)[1])];
		(*it)[2] = lut[((*it)[2])];
	}
}

int main()
{
	Base base;
//	Inputchoose input(base, base.string);
	VideoCapture cap(1);
	if (!cap.isOpened())
		return -1;
	int 循环调节 = -12;
	while (waitKey(30) != 27)
	{
		//cout << "设置之后曝光为：" << cap.get(CAP_PROP_EXPOSURE);
		cap.set(CV_CAP_PROP_EXPOSURE, 循环调节 );
		//cout << "设置之后曝光为：" << cap.get(CAP_PROP_EXPOSURE)<<endl;
		cap >> base.src;
		imshow("usb",base.src);
		Gammas(base.src, base.src, 1.5);
		Light light(base.src, base);
		if (light.flag)		Armor armor(light, base);
	}
	cap.release();
/*	VideoCapture video;
	video.open(base.string);

	if (!video.isOpened())
	{
		cout << "cant open video" << endl;
		return 0;
	}
	while (1)
	{
		video >> base.src;
		if (base.src.empty())
			break;

	Light light(base.src, base);
	if (light.flag)		Armor armor(light ,base);
	cvWaitKey(33);
	}
	video.release();*/
//	Light light(base.src , base);
//	Armor armor(light ,base);
//	waitKey(0);
	return 0;
}