#include <opencv\cv.h>
#include <opencv\highgui.h>
#include <opencv2\objdetect\objdetect.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include <iostream>
#include <stdio.h>
#include <opencv2\opencv.hpp>


IplImage* GetThresholdedImageHSV(IplImage* img)
{
	IplImage* imgHSV = cvCreateImage(cvGetSize(img), 8, 3);

	cvCvtColor(img, imgHSV, CV_BGR2HSV);

	IplImage * img2 = cvCreateImage(cvGetSize(img), 8, 1);

	cvSmooth(imgHSV, imgHSV, 2, 7, 7);

	cvInRangeS(imgHSV, cvScalar(0, 82, 113), cvScalar(35, 255, 255), img2);

	cvReleaseImage(&imgHSV);
	return img2;
}

void trackObject(IplImage* img1, IplImage* img2){
	CvFont font;
	cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX, 1.0, 1.0, 0, 1, CV_AA);

	CvSeq* contour;  //hold the pointer to a contour
	CvSeq* result;     //hold sequence of points of a contour
	CvMemStorage *storage = cvCreateMemStorage(0); //storage area for all contours
	//finding all contours in the image
	cvFindContours(img1, storage, &contour, sizeof(CvContour), CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE, cvPoint(0, 0));
	//iterating through each contour

	
	while (contour)
	{
		double perimeter = cvArcLength(contour, CV_WHOLE_SEQ, -1);
		double diameter = perimeter / CV_PI;
		double circle_area = diameter * diameter * CV_PI / 4;

		CvBox2D box;
		CvBox2D c_box;
		box = cvMinAreaRect2(contour, 0);
		
		int width = img1->width;
		int height = img1->height;

		CvSize size;
		CvSize c_size;

		size.width = box.size.width;
		size.height = box.size.height;
		float box_area = size.width * size.height;
		float contour_area = cvContourArea(contour, CV_WHOLE_SEQ);
		float ratio = box_area / contour_area;
		float radius = size.width / 2;
		float c_area = radius * radius * CV_PI;
		float c_ratio = width *height / c_area;
		printf("%f\n", ratio);

		//obtain a sequence of points of contour, pointed by the variable 'contour'
		//result = cvApproxPoly(contour, sizeof(CvContour), storage, CV_POLY_APPROX_DP, cvContourPerimeter(contour)*0.02, 0);
		result = cvApproxPoly(contour, sizeof(CvContour), storage, CV_POLY_APPROX_DP, 40, 2);


		//if (cvContourArea(contour, CV_WHOLE_SEQ) < 2000) continue;
		//if there are 3  vertices  in the contour(It should be a triangle)
		if (result->total == 3)
		{
			cvPutText(img2, "Triangle", cvPoint(10, 130), &font, cvScalar(255, 255, 255, 0));
			//cvCvtColor(frame, imgGrayScale, CV_BGR2GRAY);
		}

		else if ((result->total == 4 || result->total == 5) && (c_ratio >6.0&& c_ratio < 7.0) && (ratio >= 1.2 && ratio <= 1.35)){
			cvPutText(img2, "Circle", cvPoint(10, 130), &font, cvScalar(255, 255, 255, 0));
		}
		//if there are 4 vertices in the contour(It should be a quadrilateral)
		else if (result->total == 4 && (0.9 < ratio && ratio < 1.10))
		{
			//iterating through each point
			//drawing lines around the quadrilateral
			cvPutText(img2, "Square", cvPoint(10, 130), &font, cvScalar(255, 255, 255, 0));

		}

		else if (result->total == 5 && 1.35 <= ratio && ratio <= 1.45)
		{
			//drawing lines around the quadrilateral
			cvPutText(img2, "Pentagon", cvPoint(10, 130), &font, cvScalar(255, 255, 255, 0));
		}

		else if ((result->total == 6 || result->total == 7) && (ratio > 1.4 && ratio < 1.6))
		{
			//drawing lines around the quadrilateral
			cvPutText(img2, "Heart!", cvPoint(10, 130), &font, cvScalar(255, 255, 255, 0));
		}

		//if there are 7  vertices  in the contour(It should be a heptagon)
		else if (result->total == 7)
		{
			//drawing lines around the heptagon
			cvPutText(img2, "Arrow", cvPoint(10, 130), &font, cvScalar(255, 255, 255, 0));
		}

		
		else if (result->total == 10 && ratio >= 2.5 && ratio <= 3.0)
		{
			//drawing lines around the heptagon
			cvPutText(img2, "Star", cvPoint(10, 130), &font, cvScalar(255, 255, 255, 0));

		}
		else if (result->total == 6 && (ratio > 1.2 && ratio < 1.4)){
			cvPutText(img2, "Hexagon", cvPoint(10, 130), &font, cvScalar(255, 255, 255, 0));

		}
		else if (result->total > 5 && ratio > 2.0 && ratio < 2.4){

			cvPutText(img2, "Crescent", cvPoint(10, 130), &font, cvScalar(255, 255, 255, 0));

		}
		else if (result->total == 10 || (ratio>1.6 && ratio < 1.85))
		{
			//drawing lines around the heptagon
			cvPutText(img2, "Cross", cvPoint(10, 130), &font, cvScalar(255, 255, 255, 0));
		}
		//obtain the next contour
		contour = contour->h_next;
	}
	cvReleaseMemStorage(&storage);
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
		IplImage* img2 = cvCreateImage(cvGetSize(frame), 8, 1);
		img = GetThresholdedImageHSV(frame);
		img2 = GetThresholdedImageHSV(frame);
		//cvCanny(img, img, 100, 200, 3);
		trackObject(img, frame);
		//cvShowImage("opencv", img2);
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