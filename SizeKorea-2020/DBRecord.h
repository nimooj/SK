#if !defined(AFX_DBRECORD_H__B5C27C2E_293C_499D_BD72_E23B5873B869__INCLUDED_)
#define AFX_DBRECORD_H__B5C27C2E_293C_499D_BD72_E23B5873B869__INCLUDED_

#include <afxdb.h> // mj::For CResordSet
#include "SizeKorea.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DBRecord.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDBRecord recordset

class CDBRecord : public CRecordset
{
public:
	CDBRecord(CDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(CDBRecord)

// Field/Param Data
	//{{AFX_FIELD(CDBRecord, CRecordset)
	long	m_id;
	long	m_column1;
	CString	m_column2;
	CString	m_column3;
	CString	m_column4;
	CString	m_column5;
	CString	m_column6;
	CString	m_column7;
	BYTE	m_column8;
	BYTE	m_column9;
	BYTE	m_column10;
	BYTE	m_column11;
	BYTE	m_column12;
	int		m_column13;
	BYTE	m_column14;
	BYTE	m_column15;
	int		m_human_size[19];
	double	m_RI;
	double	m_BMI;
	CString m_file1;
	CString m_file2;
	CString m_file3;
	CString m_file4;
	CString m_file5;
	CString m_file6;
	int	m_pose1_size[63];
	int	m_pose2_size[39];
	int	m_pose3_size[34];
	int m_pose4_size[9];
	byte	m_answer[50];

	//}}AFX_FIELD


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDBRecord)
	public:
	virtual CString GetDefaultConnect();    // Default connection string
	virtual CString GetDefaultSQL();    // Default SQL for Recordset
	virtual void DoFieldExchange(CFieldExchange* pFX);  // RFX support
	//}}AFX_VIRTUAL

	void InitData(void);

// Implementation
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DBRECORD_H__B5C27C2E_293C_499D_BD72_E23B5873B869__INCLUDED_)
