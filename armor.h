/*
	���ļ�������
			�ж�װ�װ�������
			ѡ��װ�װ�
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
