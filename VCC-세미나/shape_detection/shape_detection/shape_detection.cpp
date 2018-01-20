#include <opencv\cv.h>
#include <opencv\highgui.h>
#include <opencv2\objdetect\objdetect.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include <iostream>
#include <stdio.h>
#include <opencv2\opencv.hpp>


IplImage* GetThresholdedImageHSV(IplImage* img);
void trackObject(IplImage* img1, IplImage* img2);


int main(){

	CvCapture* capture = cvCaptureFromCAM(1);
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
		frame = cvCloneImage(frame); // 영상을 복사
		IplImage * img = cvCreateImage(cvGetSize(frame), 8, 1); // IplImage 구조체 메모리를 생성하여 그 포인터를 반환한다.
		img = GetThresholdedImageHSV(frame);    //이미지에서 원하는 색깔만을 추출
		cvCanny(img, img, 100, 200, 3);    //입력 이미지에서 경계선을 검출하는 함수, 경계선을 검출한 후에는 그 경계선을 출력이미지에 표시한다.
		trackObject(img, frame);   //도형을 인식하는 함수
		cvShowImage("opencv", img);  // 이미지를 영상에 띄운다.
		cvShowImage("video", frame);  //원본영상을 띄운다.
		cvReleaseImage(&img);  //생성했던 IplImage 형태의 이미지 포인터를 해체한다.
		cvReleaseImage(&frame);

		int c = cvWaitKey(10);//키보드 키(key)의 입력 신호를 기다힌다.
		if ((char)c == 27) break; // ESC키를 누르면 종료
	}

	cvDestroyAllWindows(); //열려있는 모든 HighGUI 창을 닫는 함수
	cvReleaseCapture(&capture);//CvCapture 구조체를 해제한다.

	return 0;
}


IplImage* GetThresholdedImageHSV(IplImage* img)
{
	
	IplImage* imgHSV = cvCreateImage(cvGetSize(img), 8, 3);

	cvCvtColor(img, imgHSV, CV_BGR2HSV);

	
	IplImage * img2 = cvCreateImage(cvGetSize(img), 8, 1);

	cvSmooth(imgHSV, imgHSV, 2, 7, 7);

	cvInRangeS(imgHSV, cvScalar(0, 129, 60), cvScalar(40, 255, 255), img2); //cvscalar : 색을 저장하는 구조체
	// 원본 배열의 원소들이 두 개의 다른 스칼라 값들 사이에 포함되는지 확인하는 함수
	cvReleaseImage(&imgHSV);
	return img2;
}


void trackObject(IplImage* img1, IplImage* img2){


	//이미지에 넣을 폰트 초기화
	CvFont font;
	cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX, 1.0, 1.0, 0, 1, CV_AA);

	CvSeq* contour;  //contour값을 저장할 포인터
	CvSeq* result;     //contour 값을 저장할 sequence 
	CvMemStorage *storage = cvCreateMemStorage(0); //contour를 저장할 메모리
	
	cvFindContours(img1, storage, &contour, sizeof(CvContour), CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE, cvPoint(0, 0));//이미지에서 모든 contour를 찾는다.
	//이진화 된 이미지에서 윤곽을 찾아 그 갯수를 반환한다.

	while (contour)
	{

		CvBox2D box;
		CvBox2D c_box;
		box = cvMinAreaRect2(contour, 0); // 2D의 점들의 집합을 둘러 싸는 최소의 직사각형을 찾는다.

		int width = img1->width; // width 는 img1의 너비
		int height = img1->height; // height는 img1의 높이

		CvPoint center = cvPointFrom32f(box.center);
		CvPoint2D32f p2 = cvPoint2D32f(center.x, center.y);
		CvRect r = cvBoundingRect(contour, 0);
		float s_area = r.width * r.height;

		CvSize size;
		CvSize c_size;

		size.width = box.size.width;
		size.height = box.size.height;
		float box_area = size.width * size.height;
		float contour_area = cvContourArea(contour, CV_WHOLE_SEQ);
		float ratio = box_area / contour_area;
		float radius = size.width / 2;
		float c_area = radius * radius * CV_PI;
		float c_ratio = box_area / c_area;
		float s_ratio = s_area / box_area;
		float area = contour_area / c_area;

		printf("%f\n", box_area);

		result = cvApproxPoly(contour, sizeof(CvContour), storage, CV_POLY_APPROX_DP, cvContourPerimeter(contour)*0.02, 0);
		// 윤곽선을 근사화하는 함수. cvFindContours()로 찾아낸 CvSeq 형태의 Contour을 받아 그 값을 근사화 시킨다


		if (result->total == 3 && ratio > 1.85 && ratio < 2.0)
		{
			cvPutText(img2, "Triangle", cvPoint(10, 130), &font, cvScalar(255, 255, 255, 0));
		}

		else if (result->total == 8 && (area >= 0.95 && area <= 1.05) && box_area > 35000 && box_area < 39000){
			if (box_area > 30000 && box_area < 50000)
				cvPutText(img2, "Circle", cvPoint(40, 130), &font, cvScalar(255, 255, 255, 0));
		}

		else if (result->total == 12 && ratio >1.6 && ratio < 1.8)
		{
			
			if (box_area > 30000 && box_area < 40000){
				cvPutText(img2, "Cross", cvPoint(10, 300), &font, cvScalar(255, 255, 255, 0));
			}
		}

		
		else if (result->total == 4 && ratio > 1.00 && ratio < 1.10)
		{
			
			if (box_area > 20000 && box_area < 35000){
				cvPutText(img2, "Square", cvPoint(70, 130), &font, cvScalar(255, 255, 255, 0));
			}
		}

		else if (result->total == 5 && ratio > 1.35 && ratio < 1.45)
		{
			
			if (box_area > 30000 && box_area < 50000){
				cvPutText(img2, "Pentagon", cvPoint(100, 130), &font, cvScalar(255, 255, 255, 0));
			}
		}

		else if ((area > 0.8 && area < 1.0) && box_area > 45000 && box_area < 50000)
		{
			
			cvPutText(img2, "Heart", cvPoint(10, 160), &font, cvScalar(255, 255, 255, 0));

		}

	
		else if (result->total == 7 && ratio > 1.75 && ratio < 1.85)
		{
			
			if (box_area > 30000 && box_area < 50000)
				cvPutText(img2, "Arrow", cvPoint(10, 190), &font, cvScalar(255, 255, 255, 0));
		}

		else if (c_ratio > 1.0 && c_ratio < 1.3 && ratio >2.65 && ratio < 2.75)
		{
			
			cvPutText(img2, "Star", cvPoint(10, 210), &font, cvScalar(255, 255, 255, 0));

		}
		else if (result->total == 6 && ratio > 1.25 && ratio < 1.35){
			if (box_area > 30000 && box_area < 50000)
				cvPutText(img2, "Hexagon", cvPoint(10, 240), &font, cvScalar(255, 255, 255, 0));

		}

		else if (result->total < 8 && ratio + c_ratio > 2.5 && ratio + c_ratio < 5.0 && box_area > 38000 && box_area < 41000){
			cvPutText(img2, "Crescent", cvPoint(10, 270), &font, cvScalar(255, 255, 255, 0));
		}

		contour = contour->h_next;
	}
	cvReleaseMemStorage(&storage);
}