#include "light.h"
#include "armor.h"

int total = 0;

void Gamma(Mat& src, Mat& dst, float fGamma)        //Gamma提升亮度
{
	unsigned char lut[256];
	for (int i = 0; i < 256; i++)
	{
		lut[i] = saturate_cast<uchar>(pow((float)(i / 255.0), fGamma) * 255.0f);
	}
	dst = src.clone();
	const int channels = dst.channels();
	MatIterator_<uchar> it, end;
	for (it = dst.begin<uchar>(), end = dst.end<uchar>(); it != end; it++)
		*it = lut[(*it)];
}

bool Armor::isnumber(Rect rect , Base base)		//扫描数字区域来判断
{
	Point2i lu, rd;
	size_t sum = 0;
	size_t bai = 0;
	lu.x = rect.x + rect.width / 3 ;		//左上角x
	lu.y = rect.y ;							//左上角y（y上)
	rd.x = rect.x + rect.width / 3 * 2 ;	//右上角x
	rd.y = rect.y + rect.height;			//左下角y（y下）
	int width = rd.x - lu.x;
	int height = rd.y - lu.y;
	if (width <= 0 || height <= 0 || lu.x < 0 || lu.y < 0 || rd.x < 0 || rd.y < 0 || lu.x + width > base.gray.cols || lu.y + height > base.gray.rows)
		return 0;
	Mat gray = base.gray(rect);
	//Mat gray = base.gray(Rect(lu.x , lu.y , width, height));
	Gamma(gray, gray, base.gamma);
	for (size_t i = 0; i < width; i++)		//计算白色比值确定数字
	{
		for (size_t j = 0; j < height; j++)
		{
			sum++;
			if ((int)gray.at<uchar>(j, i) > base.white_threshould) bai++;
			//cout << (int)gray.at<uchar>(j, i) << endl;
		}
	}
#ifdef	SHOW_DATA
	cv::imshow("gray", gray);
	Mat img;
	base.src.copyTo(img);
	Rect rect1(lu, rd);
	rectangle(img, rect1, Scalar(255, 0, 0), 3, LINE_8);
#ifdef LARGE_SRC
	resize(img, img, 
	Point(img.rows * 0.3, img.cols * 0.15));
#endif
	cv::imshow("isnumber", img);
	if(!bai)
		cout << "bai = 0" << endl;
	else
	{
		cout << "bai " << bai << endl;
		cout << "sum: " << sum << endl;
		cout << "bai/sum: " << (sum / bai) << endl;
	}
#endif
	if (0 == bai)
		return 0;
	else if ((sum / bai)  < base.white_pixel)
		return 1;
	else
		return 0;
}

bool Armor::istruetemple(Light& light, Base base)		//检测是否有数字来判断是否是装甲板
{
	armor_rect.clear();
	for (size_t i = 0; i < light.rect_choose.size(); i++)
	{
		Rect rect = light.rect_choose[i];
		if (isnumber(rect, base))
		{
			armor_rect.push_back(rect);
		}
	}
#ifdef SHOW_IMAGE
	Mat img;
	RNG rng(time(0));
	base.src.copyTo(img);
	if (armor_rect.size())
		for (size_t i = 0; i < armor_rect.size(); i++)
		{
			rectangle(img, armor_rect[i], Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255)), 3, LINE_8);
			cout << "x: " << armor_rect[i].x << " y: " << armor_rect[i].x << " width: " << armor_rect[i].width << " height: " << armor_rect[i].height << endl;
		}
	else
	{
		cout << "no true temple" << endl;
		cout << "rect_choose: " << light.rect_choose.size() << endl;
	}
#ifdef LARGE_SRC
	resize(img, img, Point(img.rows * 0.7, img.cols * 0.5));
#endif
	cv::imshow("istruetemple", img);
#endif
	if (armor_rect.size())
		return 1;
	else
		return 0;
}