#include <opencv\cv.h>

#include <opencv\highgui.h>


int main()
{

	IplImage *image = cvLoadImage("c:\\test.jpg");

	cvShowImage("Image", image);

	cvWaitKey(0);

	cvReleaseImage(&image);

	return 0;


}

