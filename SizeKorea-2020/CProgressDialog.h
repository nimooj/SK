#pragma once


// CProgressDialog 대화 상자

class CProgressDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CProgressDialog)

public:
	CProgressDialog(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CProgressDialog();

	// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PROGRESS_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CProgressCtrl m_LandmarkProgress;
	CString m_Status;
};
