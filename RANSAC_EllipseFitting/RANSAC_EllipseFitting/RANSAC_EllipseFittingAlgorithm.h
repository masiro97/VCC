#pragma once
#include <math.h>

#define M_PI	3.14159

struct sPoint {
	double x, y;
};

struct sEllipse {
	double a, b, c, d, e, f;	// Ÿ��������: ax^2 + bxy + cy^2 + dx + ey + f = 0
	double cx, cy, w, h;		// ǥ�� ����: (x - cx)^2/w^2 + (y - cy)^2/h^2 = 1
	double theta;				// ǥ�� ���� Ÿ���� ������ ����

	inline bool convert_std_form ()
	{
		// Ÿ�� �����Ŀ��� ǥ�� ������ Ÿ���� �Ű������� ����
		// ����: http://blog.naver.com/helloktk/80035366367

		// orientation of ellipse;    
		theta = atan2(b, a-c)/2.;
		// scaled major/minor axes of ellipse;
		double ct = cos(theta);
		double st = sin(theta);
		double ap = a*ct*ct + b*ct*st + c*st*st;
		double cp = a*st*st - b*ct*st + c*ct*ct;
	    
		// translations 
		cx = (2*c*d - b*e) / (b*b - 4*a*c);
		cy = (2*a*e - b*d) / (b*b - 4*a*c);
	    
		// scale factor
		double val = a*cx*cx + b*cx*cy + c*cy*cy;
		double scale_inv = val - f;
	    
		if (scale_inv/ap <= 0 || scale_inv/cp <= 0) { 
			//not ellipse;
			return false;
		}
		
		w = sqrt(scale_inv / ap);
		h = sqrt(scale_inv / cp);
		return true;
	}
};

extern double compute_distance (sEllipse &ellipse, sPoint &p);
extern double ransac_ellipse_fitting (sPoint *data, int no_data, sEllipse &model, double distance_threshold);
