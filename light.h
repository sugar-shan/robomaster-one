/*
	���ļ�������
			ѡ�񵥸������Լ���ϵ�����Ϊװ�װ�����ĺ�ѡ
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
	int lengthradioS;	//������������������ֵ
	int lengthradioL;	//������������������ֵ
	vector<RotatedRect> LED_SINGLE_CHOOSE;		//��ȷ�ĵ���������
	vector<Rect> rect_choose;		//��ȷ����������
	bool flag = 0;
	int doublelight_x = 100;		//������x���ֵ
}; 

