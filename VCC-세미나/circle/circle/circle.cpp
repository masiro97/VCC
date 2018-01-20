
#include "stdafx.h"
#include <opencv\cv.h>
#include <opencv\highgui.h>
#include <opencv2\objdetect\objdetect.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include <iostream>
#include <stdio.h>
#include <opencv2\opencv.hpp>

void circle(IplImage* gray, IplImage* img){
CvFont font;
cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX, 1.0, 1.0, 0, 1, CV_AA);

int i;
float *p;
CvSeq *circles = 0;
CvMemStorage *store = cvCreateMemStorage(0);

CvSize sizeOfImage = cvGetSize(img);
cvCvtColor(img, gray, CV_BGR2GRAY);
cvSmooth(gray, gray, CV_GAUSSIAN, 11, 11, 0, 0);

store = cvCreateMemStorage(0);

circles = cvHoughCircles(gray, store, CV_HOUGH_GRADIENT, 1, 100, 50, 80, 10, MAX(gray->width, gray->height));

for (i = 0; i < circles->total; i++){
p = (float*)cvGetSeqElem(circles, i);
cvCircle(img, cvPoint(cvRound(p[0]), cvRound(p[1])), 3, CV_RGB(0, 255, 0), -1.8, 0);
cvCircle(img, cvPoint(cvRound(p[0]), cvRound(p[1])), cvRound(p[2]), CV_RGB(255, 0, 0), 3, 8, 0);
//cvPutText(img, "Circle", cvPoint(10, 130), &font, cvScalar(255, 255, 255, 0));
}
}

int main(){

	CvCapture* capture = cvCaptureFromCAM(0);
	if (!capture){
		printf("Capture is failure\n");
		return -1;
	}

	IplImage * frame = 0;
	frame = cvQueryFrame(capture);
	if (!frame) return -1;

	cvNamedWindow("video");

	while (1){
		frame = cvQueryFrame(capture);
		if (!frame) break;
		frame = cvCloneImage(frame);
		IplImage * img = cvCreateImage(cvGetSize(frame), 8, 1);
		//cvCanny(img, img, 100, 200, 3);
		circle(img, frame);
		cvShowImage("opencv", img);
		cvShowImage("video", frame);
		cvReleaseImage(&img);
		cvReleaseImage(&frame);

		int c = cvWaitKey(10);
		if ((char)c == 27) break;
	}

	cvDestroyAllWindows();
	cvReleaseCapture(&capture);

	return 0;
}