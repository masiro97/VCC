
// RANSAC_EllipseFittingDlg.cpp : implementation file
//

#include "stdafx.h"
#include "RANSAC_EllipseFitting.h"
#include "RANSAC_EllipseFittingDlg.h"

#include "RANSAC_EllipseFittingAlgorithm.h"
#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CRANSAC_EllipseFittingDlg dialog
CRANSAC_EllipseFittingDlg::CRANSAC_EllipseFittingDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CRANSAC_EllipseFittingDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CRANSAC_EllipseFittingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CRANSAC_EllipseFittingDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_ELLIPSE_FITTING, &CRANSAC_EllipseFittingDlg::OnBnClickedButtonEllipseFitting)
END_MESSAGE_MAP()


// CRANSAC_EllipseFittingDlg message handlers

BOOL CRANSAC_EllipseFittingDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CRANSAC_EllipseFittingDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CRANSAC_EllipseFittingDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CRANSAC_EllipseFittingDlg::EllipseRot (CDC *pDC, sEllipse &ellipse)
{
	double ct = cos(ellipse.theta);
	double st = sin(ellipse.theta);

	for (int i=0; i<=360; i+=1) {
		double a = i*M_PI/180.;
		double x = ellipse.w*cos(a);
		double y = ellipse.h*sin(a);

		double rx = x*ct - y*st;
		double ry = x*st + y*ct;

		if (!i) pDC->MoveTo ((int)(ellipse.cx + rx), (int)(ellipse.cy + ry));
		else    pDC->LineTo ((int)(ellipse.cx + rx), (int)(ellipse.cy + ry));
	}
}


void CRANSAC_EllipseFittingDlg::OnBnClickedButtonEllipseFitting()
{
	// TODO: Add your control notification handler code here

	const int no_data = 500;
	sPoint *data = new sPoint[no_data];

	static double theta = 0;
	theta += 5*M_PI/180;

	double ct = cos(theta);
	double st = sin(theta);

	for (int i=0; i<no_data; ++i) {
		if (i%2) {
			// 중심 (250, 250), 단축 100, 장축 200인 타원을 만든다
			double x = 100*cos(i*7.77);
			double y = 200*sin(i*7.77);
			// 타원을 theta 만큼 회전한다
			double rx = x*ct - y*st;
			double ry = x*st + y*ct;

			data[i].x = 250 + rx + 20.*rand()/RAND_MAX - 10;
			data[i].y = 250 + ry + 20.*rand()/RAND_MAX - 10;
		}
		else {
			data[i].x = 500.*rand()/RAND_MAX;
			data[i].y = 500.*rand()/RAND_MAX;
		}
	}

	sEllipse ellipse;
	double cost = ransac_ellipse_fitting (data, no_data, ellipse, 50);

	CDC *pDC = GetDC();

	pDC->Rectangle (0, 0, 520, 520);

	for(int i = 0; i<no_data; i++) {
		double d = compute_distance (ellipse, data[i]);

		pDC->SetPixel((int)data[i].x, (int)data[i].y, d < 50 ? RGB(255,0,0) : RGB(0,0,255));
	}

	pDC->SelectStockObject (NULL_BRUSH);

	if (100. < cost) {
		EllipseRot (pDC, ellipse);

		char text[256];
		sprintf (text, "Center = (%d, %d), Axis Length = (%d, %d); Angle = %d", 
			(int)ellipse.cx, (int)ellipse.cy, (int)ellipse.w, (int)ellipse.h, (int)(ellipse.theta*180/M_PI));
		pDC->TextOutA (10, 10, text, strlen(text));

		sprintf (text, "cost = %g", cost);
		pDC->TextOutA (10, 30, text, strlen(text));
	}

	delete [] data;
}
