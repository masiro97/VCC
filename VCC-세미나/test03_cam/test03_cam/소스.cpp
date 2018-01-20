#include <opencv\highgui.h>

int main() {
	IplImage *frame; // ������ ����

	CvCapture* capture = cvCaptureFromCAM(0); //ī�޶� ��ȣ�� ��Ÿ���� index(0)���κ��� ���� ȹ���� �ʱ�ȭ�Ͽ� �����͸� ��ȯ

	cvNamedWindow("Test", 1); // ���ڿ� Test�� ������ ĸ������ ����ϴ� �����츦 ����

	while (capture) {
		frame = cvQueryFrame(capture);	//ī�޶� �Ǵ� ���� ������ capture�κ��� ������������ ���, ������ Ǯ� ���������͸� ��ȯ
		cvShowImage("Test", frame); // Test �̸��� ���� �����쿡 ���� frame�� ǥ���Ѵ�.

		if (cvWaitKey(33) >= 27) // ESC ��ư�� ������ while���� �������´�.
			break;
	}
	cvReleaseCapture(&capture); // ���� ȹ���� ���� ������ capture�� �����Ѵ�.
	cvDestroyWindow("Test");// ������ �ı� Allwindows -> ������ ��� �����츦 �Ѳ����� �ı�.

	return 0;
}