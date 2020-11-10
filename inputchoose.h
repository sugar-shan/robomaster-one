/*
	该文件用来:
			判断输入原图像的类型：
								1）图片
								2）视频
								3）摄像头
							

*/
#pragma once
#include "base.h"

class Inputchoose
{
public:
	Inputchoose(Base& base, String string)
	{
		inputchoose(base, string);
	}
	void inputchoose(Base& base, String string);
};

