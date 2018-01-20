#include <opencv\highgui.h>

int main() {
	IplImage *frame; // 포인터 선언

	CvCapture* capture = cvCaptureFromCAM(0); //카메라 번호를 나타내는 index(0)으로부터 비디오 획득을 초기화하여 포인터를 반환

	cvNamedWindow("Test", 1); // 문자열 Test를 윈도우 캡션으로 사용하는 윈도우를 생성

	while (capture) {
		frame = cvQueryFrame(capture);	//카메라 또는 비디오 파일인 capture로부터 영상프레임을 잡고, 압축을 풀어서 영상포인터를 반환
		cvShowImage("Test", frame); // Test 이름을 갖는 윈도우에 영상 frame을 표시한다.

		if (cvWaitKey(33) >= 27) // ESC 버튼을 누르면 while문을 빠져나온다.
			break;
	}
	cvReleaseCapture(&capture); // 비디오 획득을 위한 포인터 capture를 해제한다.
	cvDestroyWindow("Test");// 윈도우 파괴 Allwindows -> 생성된 모든 윈도우를 한꺼번에 파괴.

	return 0;
}