//
// Hough Lines
//
// Dong-Chul Kim
// Media System Lab., Yonsei University
// http://msl.yonsei.ac.kr
// http://opencv.co.kr
//

#include "stdafx.h"

int main()
{
	IplImage* src0;
	CvCapture* captue = cvCaptureFromCAM(0);

	while(1){
		cvGrabFrame(captue);
		if(  (src0=cvRetrieveFrame(captue)) != 0)
		{
			IplImage* dst = cvCreateImage( cvGetSize(src0), 8, 1 );
			IplImage* color_dst = cvCreateImage( cvGetSize(src0), 8, 3 );
			
			IplImage* src = cvCreateImage( cvGetSize(src0), IPL_DEPTH_8U, 1);
	            
			cvCvtColor(src0, src, CV_RGB2GRAY);

			CvMemStorage* storage = cvCreateMemStorage(0);

			CvSeq* lines = 0;

			cvCanny( src, dst, 50, 200, 3 );
			cvCvtColor( dst, color_dst, CV_GRAY2BGR );
			lines = cvHoughLines2( dst, storage, CV_HOUGH_STANDARD, 1, CV_PI/180, 100, 0, 0 );

			for( int i = 0; i < MIN(lines->total,100); i++ )
			{
				float* line = (float*)cvGetSeqElem(lines,i);
				float rho = line[0];
				float theta = line[1];
				CvPoint pt1, pt2;
				double a = cos(theta), b = sin(theta);
				double x0 = a*rho, y0 = b*rho;
				pt1.x = cvRound(x0 + 1000*(-b));
				pt1.y = cvRound(y0 + 1000*(a));
				pt2.x = cvRound(x0 - 1000*(-b));
				pt2.y = cvRound(y0 - 1000*(a));
				cvLine( color_dst, pt1, pt2, CV_RGB(255,0,0), 3, 8 );
			}
			lines = cvHoughLines2( dst, storage, CV_HOUGH_PROBABILISTIC, 1, CV_PI/180, 80, 30, 10 );
			for( i = 0; i < lines->total; i++ )
			{
				CvPoint* line = (CvPoint*)cvGetSeqElem(lines,i);
				cvLine( color_dst, line[0], line[1], CV_RGB(255,0,0), 3, 8 );
			}
			cvNamedWindow( "Source", 0 );
			cvShowImage( "Source", src );

			cvNamedWindow( "Hough", 0 );
			cvShowImage( "Hough", color_dst );
			
			cvReleaseMemStorage(&storage);
		}
		if(cvWaitKey(10) >= 0) break;

	}
	cvDestroyAllWindows();
	cvReleaseCapture(&captue);
    return 0;
}
