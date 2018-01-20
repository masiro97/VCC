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
		frame = cvCloneImage(frame); // ������ ����
		IplImage * img = cvCreateImage(cvGetSize(frame), 8, 1); // IplImage ����ü �޸𸮸� �����Ͽ� �� �����͸� ��ȯ�Ѵ�.
		img = GetThresholdedImageHSV(frame);    //�̹������� ���ϴ� ������ ����
		cvCanny(img, img, 100, 200, 3);    //�Է� �̹������� ��輱�� �����ϴ� �Լ�, ��輱�� ������ �Ŀ��� �� ��輱�� ����̹����� ǥ���Ѵ�.
		trackObject(img, frame);   //������ �ν��ϴ� �Լ�
		cvShowImage("opencv", img);  // �̹����� ���� ����.
		cvShowImage("video", frame);  //���������� ����.
		cvReleaseImage(&img);  //�����ߴ� IplImage ������ �̹��� �����͸� ��ü�Ѵ�.
		cvReleaseImage(&frame);

		int c = cvWaitKey(10);//Ű���� Ű(key)�� �Է� ��ȣ�� �������.
		if ((char)c == 27) break; // ESCŰ�� ������ ����
	}

	cvDestroyAllWindows(); //�����ִ� ��� HighGUI â�� �ݴ� �Լ�
	cvReleaseCapture(&capture);//CvCapture ����ü�� �����Ѵ�.

	return 0;
}


IplImage* GetThresholdedImageHSV(IplImage* img)
{
	
	IplImage* imgHSV = cvCreateImage(cvGetSize(img), 8, 3);

	cvCvtColor(img, imgHSV, CV_BGR2HSV);

	
	IplImage * img2 = cvCreateImage(cvGetSize(img), 8, 1);

	cvSmooth(imgHSV, imgHSV, 2, 7, 7);

	cvInRangeS(imgHSV, cvScalar(0, 129, 60), cvScalar(40, 255, 255), img2); //cvscalar : ���� �����ϴ� ����ü
	// ���� �迭�� ���ҵ��� �� ���� �ٸ� ��Į�� ���� ���̿� ���ԵǴ��� Ȯ���ϴ� �Լ�
	cvReleaseImage(&imgHSV);
	return img2;
}


void trackObject(IplImage* img1, IplImage* img2){


	//�̹����� ���� ��Ʈ �ʱ�ȭ
	CvFont font;
	cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX, 1.0, 1.0, 0, 1, CV_AA);

	CvSeq* contour;  //contour���� ������ ������
	CvSeq* result;     //contour ���� ������ sequence 
	CvMemStorage *storage = cvCreateMemStorage(0); //contour�� ������ �޸�
	
	cvFindContours(img1, storage, &contour, sizeof(CvContour), CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE, cvPoint(0, 0));//�̹������� ��� contour�� ã�´�.
	//����ȭ �� �̹������� ������ ã�� �� ������ ��ȯ�Ѵ�.

	while (contour)
	{

		CvBox2D box;
		CvBox2D c_box;
		box = cvMinAreaRect2(contour, 0); // 2D�� ������ ������ �ѷ� �δ� �ּ��� ���簢���� ã�´�.

		int width = img1->width; // width �� img1�� �ʺ�
		int height = img1->height; // height�� img1�� ����

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
		// �������� �ٻ�ȭ�ϴ� �Լ�. cvFindContours()�� ã�Ƴ� CvSeq ������ Contour�� �޾� �� ���� �ٻ�ȭ ��Ų��


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