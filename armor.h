/*
	该文件用来：
			判断装甲板的真与假
			选择装甲板
*/

#pragma once
#include "base.h"

class Light;

class Armor {
public:
	Armor(Light& light, Base base)
	{
		flag = istruetemple(light, base);
	}
	bool isnumber(Rect rect, Base base);
	bool istruetemple(Light& light , Base base);

public:
	vector<Rect> armor_rect;
	bool flag = 0;
};
