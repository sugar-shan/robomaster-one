/*
	���ļ�����:
			�洢���������ݷ������
			������ֱ�Ҫ���ݱ�����������ά��
			������ֺ�������ڲ���

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
	int single_light_lowest_threshold = 70;		//����������ֵ���ֵ
	int angle_single_threshold = 20;	//�жϵ��������Ƕȵ���ֵ

	int angle_double_threshould = 10;	//�ж����������ǶȲ����ֵ

	int white_threshould = 120;		//�м�װ�װ��ɫ��ֵ
	int white_pixel = 5;		//�м�װ�װ� ��ɫ�������� ѡ��ֵ
    float gamma = 0.2;        //gammaֵ

	String string = "C:/Users/lenovo/Desktop/12.mp4";		//ͼƬ��ַ
	Mat src ;		//ͼƬ
	Mat gray;		//src�ĻҶ�ͼƬ
}Base;
