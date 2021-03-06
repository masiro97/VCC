#include <opencv\cv.h>
#include <opencv\highgui.h>
void main(int argc, char* argv[])
{
	IplImage* image = 0; //openCV에서 사용되는 자료형이다.
	IplImage* out = 0;

	out = cvCreateImage(cvSize(640, 480), 8, 1);

	CvPoint offset = cvPoint(3, 3);

	CvCapture* capture = cvCaptureFromCAM(0); //현재 인식된 웹캠을 찾고,
	cvNamedWindow("OpenCvCamtest", 0); // 화면을 그려줄 윈도우를 생성한다.
	cvNamedWindow("OpenCvCamtest out", 0);

	int aperature_size = 7;
	double low_thresh = 400;
	double high_thresh = 100;

	cvResizeWindow("OpenCvCamtest", 640, 480); // 사이즈를 조절한다.(lpIImage를 할당하면서도 조절가능)

	while (1)
	{
		cvGrabFrame(capture);
		image = cvRetrieveFrame(capture); // 현재 인식된 장면을 받아오고image에 넣는다.

		cvCanny(image, out, low_thresh * 10 * 4, high_thresh * 10 * 4, aperature_size);

		cvShowImage("OpenCvCamtest", image); // image에 있는 장면을 윈도우에 그린다.
		cvShowImage("OpenCvCamtest out", out);

		if (cvWaitKey(10) >= 0) // 이게 가장 중요한데 이 WaitKey 함수가 없으면 아무 것도 안그린다.
			break;
	}
	cvReleaseCapture(&capture); // 할당받았던 웹캠을 해제하고,
	cvDestroyWindow("OpenCvCamtest"); // 윈도우를 종료한다. 
	cvDestroyWindow("OpenCvCamtest out");
}
