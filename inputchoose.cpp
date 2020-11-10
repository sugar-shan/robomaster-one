#include "inputchoose.h"

void Inputchoose::inputchoose(Base& base, String string)
{
#ifdef PICTURE
	base.src =imread(string);
#endif

#ifdef VIDEO
	VideoCapture video;
	video.open(string);
	if (!video.isOpened())
	{
		cout << "cant open video" << endl;
		return;
	}
	while (1)
	{
		video >> base.src;
		if (base.src.empty())
			break;
	}
	video.release();
	return;
#endif

#ifdef CAMERA

#endif
	return;
}

