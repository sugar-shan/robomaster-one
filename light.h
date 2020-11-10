/*
	该文件用来：
			选择单个灯条以及拟合灯条成为装甲板区域的候选
*/
#pragma once
#include "base.h"

class armor;

class Light
{
public:
	Light(Mat& src, Base& base)
	{
		 flag = findsinglelight(src, base);
		 if(flag)	flag = finddoublelight(base);
	}
	bool findsinglelight(Mat& src , Base& base);
	bool finddoublelight(Base& base);

public:
	int lengthradioS;	//单个灯条长宽比最低数值
	int lengthradioL;	//单个灯条长宽比最高数值
	vector<RotatedRect> LED_SINGLE_CHOOSE;		//正确的单个灯条组
	vector<Rect> rect_choose;		//正确的两灯条组
	bool flag = 0;
	int doublelight_x = 100;		//两灯条x间距值
}; 

