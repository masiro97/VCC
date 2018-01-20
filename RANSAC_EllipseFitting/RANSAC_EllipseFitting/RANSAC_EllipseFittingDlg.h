
// RANSAC_EllipseFittingDlg.h : header file
//

#pragma once
#include "afxwin.h"


struct sEllipse;

// CRANSAC_EllipseFittingDlg dialog
class CRANSAC_EllipseFittingDlg : public CDialog
{
// Construction
public:
	CRANSAC_EllipseFittingDlg(CWnd* pParent = NULL);	// standard constructor

	void EllipseRot (CDC *pDC, sEllipse &ellipse);

// Dialog Data
	enum { IDD = IDD_RANSAC_EllipseFitting_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonEllipseFitting();
};
