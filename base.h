/*
	该文件用来:
			存储基本的内容方便操作
			储存各种必要数据便于在赛场上维护
			储存各种宏变量便于测试

*/
#pragma once
#include "opencv2/opencv.hpp"
#include <string.h>

using namespace std;
using namespace cv;

	#define PICTURE
//	#define VIDEO
//	#define CAMERA

	#define SHOW_IMAGE 
	#define SHOW_DATA 
//	#define LARGE_SRC

typedef struct base
{
	bool _enemy_color = 0;		//0 for blue ,1 for red
	int single_light_lowest_threshold = 70;		//单个灯条阈值最低值
	int angle_single_threshold = 20;	//判断单个灯条角度的阈值

	int angle_double_threshould = 10;	//判断两个灯条角度差的阈值

	int white_threshould = 120;		//中间装甲板白色阈值
	int white_pixel = 5;		//中间装甲板 白色比总像素 选择值
    float gamma = 0.2;        //gamma值

	String string = "C:/Users/lenovo/Desktop/12.mp4";		//图片地址
	Mat src ;		//图片
	Mat gray;		//src的灰度图片
}Base;
