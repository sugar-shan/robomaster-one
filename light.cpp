#include "light.h"
#include "armor.h"

bool truelengthRatio(RotatedRect& rect)		//对单个灯条长宽和比值进行判断
{
	if (rect.size.height / rect.size.width < 9.5 && rect.size.height / rect.size.width > 3 && rect.size.width > 1 && rect.size.height > 3)
		return 1;
	else
		return 0;
}

void truerect(RotatedRect& rect)		//调整灯条的正确长宽
{
	float ch;
	ch = rect.size.width;
	rect.size.width = rect.size.height;
	rect.size.height = ch;
}

bool trueangle(RotatedRect& rect , Base base)		//判断灯条角度
{
	if (rect.angle < 0)
	{
		rect.angle += 90;
		truerect(rect);
	}
	if (rect.angle < base.angle_single_threshold)
		return 1;
	else
		return 0;
}

bool Light::findsinglelight(Mat &src , Base& base)		//通过提取颜色和二值化提取轮廓，进而确定单个灯条
{	
	LED_SINGLE_CHOOSE.clear();
	vector<Mat> channels;
	split(src, channels);
	Mat enemy_img;
	RotatedRect rect;
	cvtColor(src, base.gray, COLOR_BGR2GRAY);
	if (base._enemy_color == 1)		//提取不同颜色下的灯条图像
	{
		subtract(channels[2] , channels[1] , enemy_img);
	}
	else
	{
		subtract(channels[0] , channels[2] , enemy_img);
	}
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	threshold(enemy_img, enemy_img, base.single_light_lowest_threshold, 255, CV_THRESH_BINARY);
	findContours(enemy_img, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
	#ifdef SHOW_DATA
	Mat img;
	enemy_img.copyTo(img);
	resize(img, img, Point(img.rows * 0.3, img.cols * 0.15));
	imshow("enemy_img",img);
#endif
	if (!contours.size())
		return 0;
	for (size_t i = 0; i < contours.size(); i++)
	{
		rect = minAreaRect(contours[i]);
		if (!trueangle(rect, base))
		{
			continue;
		}
		if (!truelengthRatio(rect))
		{
			continue;
		}
		LED_SINGLE_CHOOSE.push_back(rect);
	}
	if (LED_SINGLE_CHOOSE.size() < 2)
		return 0;
#ifdef SHOW_DATA
		base.src.copyTo(img);
		Point2f vertex[4];
		for (size_t j = 0; j < LED_SINGLE_CHOOSE.size(); j++)
		{
			LED_SINGLE_CHOOSE[j].points(vertex);
			for (size_t i = 0; i < 4; i++)
			{
				line(img, vertex[i], vertex[(i + 1) % 4], Scalar(0, 120, 255), 3, LINE_AA);
			}
			/*	if (j == 0) putText(img, "1", Point(LED_SINGLE_CHOOSE[j].center.x - 10, LED_SINGLE_CHOOSE[j].center.y-50), cv::FONT_HERSHEY_PLAIN,2,Scalar(0,0,255),2,1);
				if (j == 1) putText(img, "2", Point(LED_SINGLE_CHOOSE[j].center.x - 10, LED_SINGLE_CHOOSE[j].center.y - 50), cv::FONT_HERSHEY_PLAIN, 2, Scalar(0, 0, 255), 2, 1);
				if (j == 2) putText(img, "3", Point(LED_SINGLE_CHOOSE[j].center.x - 10, LED_SINGLE_CHOOSE[j].center.y - 50), cv::FONT_HERSHEY_PLAIN, 2, Scalar(0, 0, 255), 2, 1);
				if (j == 3) putText(img, "4", Point(LED_SINGLE_CHOOSE[j].center.x - 10, LED_SINGLE_CHOOSE[j].center.y - 50), cv::FONT_HERSHEY_PLAIN, 2, Scalar(0, 0, 255), 2, 1);
				if (j == 4) putText(img, "5", Point(LED_SINGLE_CHOOSE[j].center.x - 10, LED_SINGLE_CHOOSE[j].center.y - 50), cv::FONT_HERSHEY_PLAIN, 2, Scalar(0, 0, 255), 2, 1);
				*/
			printf("x,y:%f,%f , width,height:%f,%f , angle:%f\n", LED_SINGLE_CHOOSE[j].center.x, LED_SINGLE_CHOOSE[j].center.y, LED_SINGLE_CHOOSE[j].size.width, LED_SINGLE_CHOOSE[j].size.height, LED_SINGLE_CHOOSE[j].angle);
		}
#ifdef LARGE_SRC
		resize(img, img, Point(img.rows * 0.7, img.cols * 0.5));
#endif
		imshow("findsinglelight", img);
		cout << "LED_SINGLE_CHOOS: "  << LED_SINGLE_CHOOSE.size() << endl;
#endif
	return 1;
}

bool truedoubleangle(RotatedRect lightL, RotatedRect lightR , Base base)		//两灯条的角度差
{
	if (abs(lightL.angle - lightR.angle) < base.angle_double_threshould)
		return 1;
	else
		return 0;
}

bool truedoubley(RotatedRect lightL, RotatedRect lightR)		//两灯条y高度超过一定不要
{
	if (abs(lightR.center.y - lightL.center.y) < 0.7 * min(lightR.size.height, lightL.size.height))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

bool truedoublex(RotatedRect lightL, RotatedRect lightR)		//两灯条x距离长度短或长过一定不要
{
	float width = min(lightR.size.width, lightL.size.width);
	size_t threshold = (size_t)(2 * width);
	if ((size_t)(lightR.center.x) - (size_t)(lightL.center.x) > 0.8 * threshold && (size_t)(lightR.center.x) - (size_t)(lightL.center.x) < 10 * threshold)
		return 1;
	else
		return 0;
}

bool Light::finddoublelight(Base& base)		//通过两两匹配灯条确定是否可以拟合
{
	rect_choose.clear();
	RotatedRect lightL, lightR;
	Point2f lu, rd;
	Point2f point[4];
	for (size_t i = 0; i < LED_SINGLE_CHOOSE.size(); i++)
	{
		for (size_t j = i + 1; j < LED_SINGLE_CHOOSE.size(); j++)
		{
			if (LED_SINGLE_CHOOSE[i].center.x < LED_SINGLE_CHOOSE[j].center.x)
			{
				lightL = LED_SINGLE_CHOOSE[i];
				lightR = LED_SINGLE_CHOOSE[j];
			}
			else
			{
				lightL = LED_SINGLE_CHOOSE[j];
				lightR = LED_SINGLE_CHOOSE[i];
			}
			if (!truedoubleangle(lightL,lightR,base))
				continue;
			if (!truedoubley(lightL, lightR))
				continue;
			if (!truedoublex(lightL, lightR))
				continue;
			lightL.points(point);
			lu = point[1];
			lightR.points(point);
			rd = point[3];
			if (lu.x == rd.x || lu.y == rd.y)
				return 1;
			Rect rect(lu, rd);
			if (rect.area() <= 1)
				continue;
			rect_choose.push_back(rect);
		}
	}
	if (!rect_choose.size())
		return 0;
#ifdef SHOW_DATA
			Mat img1;
			RNG rng(time(0));
			base.src.copyTo(img1);
			for (size_t i = 0; i < rect_choose.size(); i++)
			{
				rectangle(img1, rect_choose[i], Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255)), 3, LINE_8, 0);
			}
			cout << "rect_choose: " << rect_choose.size() << endl;
#ifdef LARGE_SRC
			resize(img1, img1, Point(img1.rows * 0.7, img1.cols * 0.5));
#endif
			imshow("doublelight", img1);
#endif
	return 1;
}