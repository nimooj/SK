// CPose1ResultDialog.cpp : implementation file
//

#include "pch.h"
#include "SizeKorea-2020.h"
#include "CPose1ResultDialog.h"
#include "afxdialogex.h"
#include "EgSizer.h"
#include "MainFrm.h"

#include "Pose1Measures.h"


extern EgSizer theSizer;
extern std::vector<std::string> ResultPose1;


// CPose1ResultDialog dialog

IMPLEMENT_DYNAMIC(CPose1ResultDialog, CDialogEx)

CPose1ResultDialog::CPose1ResultDialog(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_POSE1_RESULT_DIALOG, pParent)
	, m_bPose1ShowPath1(FALSE)
	, m_bPose1ShowPath2(FALSE)
	, m_bPose1ShowPath3(FALSE)
	, m_bPose1ShowPath4(FALSE)
	, m_bPose1ShowPath5(FALSE)
	, m_bPose1ShowPath6(FALSE)
	, m_bPose1ShowPath7(FALSE)
	, m_bPose1ShowPath8(FALSE)
	, m_bPose1ShowPath9(FALSE)
	, m_bPose1ShowPath10(FALSE)
	, m_bPose1ShowPath11(FALSE)
	, m_bPose1ShowPath12(FALSE)
	, m_bPose1ShowPath13(FALSE)
	, m_bPose1ShowPath14(FALSE)
	, m_bPose1ShowPath15(FALSE)
	, m_bPose1ShowPath16(FALSE)
	, m_bPose1ShowPath17(FALSE)
	, m_bPose1ShowPath18(FALSE)
	, m_bPose1ShowPath19(FALSE)
	, m_bPose1ShowPath20(FALSE)
	, m_bPose1ShowPath21(FALSE)
	, m_bPose1ShowPath22(FALSE)
	, m_bPose1ShowPath23(FALSE)
	, m_bPose1ShowPath24(FALSE)
	, m_bPose1ShowPath25(FALSE)
	, m_bPose1ShowPath26(FALSE)
	, m_bPose1ShowPath27(FALSE)
	, m_bPose1ShowPath28(FALSE)
	, m_bPose1ShowPath29(FALSE)
	, m_bPose1ShowPath30(FALSE)
	, m_bPose1ShowPath31(FALSE)
	, m_bPose1ShowPath32(FALSE)
	, m_bPose1ShowPath33(FALSE)
	, m_bPose1ShowPath34(FALSE)
	, m_bPose1ShowPath35(FALSE)
	, m_bPose1ShowPath36(FALSE)
	, m_bPose1ShowPath37(FALSE)
	, m_bPose1ShowPath38(FALSE)
	, m_bPose1ShowPath39(FALSE)
	, m_bPose1ShowPath40(FALSE)
	, m_bPose1ShowPath41(FALSE)
	, m_bPose1ShowPath42(FALSE)
	, m_bPose1ShowPath43(FALSE)
	, m_bPose1ShowPath44(FALSE)
	, m_bPose1ShowPath45(FALSE)
	, m_bPose1ShowPath46(FALSE)
	, m_bPose1ShowPath47(FALSE)
	, m_bPose1ShowPath48(FALSE)
	, m_bPose1ShowPath49(FALSE)
	, m_bPose1ShowPath50(FALSE)
	, m_bPose1ShowPath51(FALSE)
	, m_bPose1ShowPath52(FALSE)
	, m_bPose1ShowPath53(FALSE)
	, m_bPose1ShowPath54(FALSE)
	, m_bPose1ShowPath55(FALSE)
	, m_bPose1ShowPath56(FALSE)
	, m_bPose1ShowPath57(FALSE)
	, m_bPose1ShowPath58(FALSE)
	, m_bPose1ShowPath59(FALSE)
	, m_bPose1ShowPath60(FALSE)
	, m_bPose1ShowPath61(FALSE)
	, m_bPose1ShowPath62(FALSE)
	, m_bPose1ShowPath63(FALSE)
	, m_bPose1ShowPath64(FALSE)
	, m_bPose1ShowPath65(FALSE)
{

}

CPose1ResultDialog::~CPose1ResultDialog()
{
}

void CPose1ResultDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_POSE1_RESULT1, m_Pose1Result1);
	DDX_Text(pDX, IDC_POSE1_RESULT2, m_Pose1Result2);
	DDX_Text(pDX, IDC_POSE1_RESULT3, m_Pose1Result3);
	DDX_Text(pDX, IDC_POSE1_RESULT4, m_Pose1Result4);
	DDX_Text(pDX, IDC_POSE1_RESULT5, m_Pose1Result5);
	DDX_Text(pDX, IDC_POSE1_RESULT6, m_Pose1Result6);
	DDX_Text(pDX, IDC_POSE1_RESULT7, m_Pose1Result7);
	DDX_Text(pDX, IDC_POSE1_RESULT8, m_Pose1Result8);
	DDX_Text(pDX, IDC_POSE1_RESULT9, m_Pose1Result9);
	DDX_Text(pDX, IDC_POSE1_RESULT10, m_Pose1Result10);
	DDX_Text(pDX, IDC_POSE1_RESULT11, m_Pose1Result11);
	DDX_Text(pDX, IDC_POSE1_RESULT12, m_Pose1Result12);
	DDX_Text(pDX, IDC_POSE1_RESULT13, m_Pose1Result13);
	DDX_Text(pDX, IDC_POSE1_RESULT14, m_Pose1Result14);
	DDX_Text(pDX, IDC_POSE1_RESULT15, m_Pose1Result15);
	DDX_Text(pDX, IDC_POSE1_RESULT16, m_Pose1Result16);
	DDX_Text(pDX, IDC_POSE1_RESULT17, m_Pose1Result17);
	DDX_Text(pDX, IDC_POSE1_RESULT18, m_Pose1Result18);
	DDX_Text(pDX, IDC_POSE1_RESULT19, m_Pose1Result19);
	DDX_Text(pDX, IDC_POSE1_RESULT20, m_Pose1Result20);
	DDX_Text(pDX, IDC_POSE1_RESULT21, m_Pose1Result21);
	DDX_Text(pDX, IDC_POSE1_RESULT22, m_Pose1Result22);
	DDX_Text(pDX, IDC_POSE1_RESULT23, m_Pose1Result23);
	DDX_Text(pDX, IDC_POSE1_RESULT24, m_Pose1Result24);
	DDX_Text(pDX, IDC_POSE1_RESULT25, m_Pose1Result25);
	DDX_Text(pDX, IDC_POSE1_RESULT26, m_Pose1Result26);
	DDX_Text(pDX, IDC_POSE1_RESULT27, m_Pose1Result27);
	DDX_Text(pDX, IDC_POSE1_RESULT28, m_Pose1Result28);
	DDX_Text(pDX, IDC_POSE1_RESULT29, m_Pose1Result29);
	DDX_Text(pDX, IDC_POSE1_RESULT30, m_Pose1Result30);
	DDX_Text(pDX, IDC_POSE1_RESULT31, m_Pose1Result31);
	DDX_Text(pDX, IDC_POSE1_RESULT32, m_Pose1Result32);
	DDX_Text(pDX, IDC_POSE1_RESULT33, m_Pose1Result33);
	DDX_Text(pDX, IDC_POSE1_RESULT34, m_Pose1Result34);
	DDX_Text(pDX, IDC_POSE1_RESULT35, m_Pose1Result35);
	DDX_Text(pDX, IDC_POSE1_RESULT36, m_Pose1Result36);
	DDX_Text(pDX, IDC_POSE1_RESULT37, m_Pose1Result37);
	DDX_Text(pDX, IDC_POSE1_RESULT38, m_Pose1Result38);
	DDX_Text(pDX, IDC_POSE1_RESULT39, m_Pose1Result39);
	DDX_Text(pDX, IDC_POSE1_RESULT40, m_Pose1Result40);
	DDX_Text(pDX, IDC_POSE1_RESULT41, m_Pose1Result41);
	DDX_Text(pDX, IDC_POSE1_RESULT42, m_Pose1Result42);
	DDX_Text(pDX, IDC_POSE1_RESULT43, m_Pose1Result43);
	DDX_Text(pDX, IDC_POSE1_RESULT44, m_Pose1Result44);
	DDX_Text(pDX, IDC_POSE1_RESULT45, m_Pose1Result45);
	DDX_Text(pDX, IDC_POSE1_RESULT46, m_Pose1Result46);
	DDX_Text(pDX, IDC_POSE1_RESULT47, m_Pose1Result47);
	DDX_Text(pDX, IDC_POSE1_RESULT48, m_Pose1Result48);
	DDX_Text(pDX, IDC_POSE1_RESULT49, m_Pose1Result49);
	DDX_Text(pDX, IDC_POSE1_RESULT50, m_Pose1Result50);
	DDX_Text(pDX, IDC_POSE1_RESULT51, m_Pose1Result51);
	DDX_Text(pDX, IDC_POSE1_RESULT52, m_Pose1Result52);
	DDX_Text(pDX, IDC_POSE1_RESULT53, m_Pose1Result53);
	DDX_Text(pDX, IDC_POSE1_RESULT54, m_Pose1Result54);
	DDX_Text(pDX, IDC_POSE1_RESULT55, m_Pose1Result55);
	DDX_Text(pDX, IDC_POSE1_RESULT56, m_Pose1Result56);
	DDX_Text(pDX, IDC_POSE1_RESULT57, m_Pose1Result57);
	DDX_Text(pDX, IDC_POSE1_RESULT58, m_Pose1Result58);
	DDX_Text(pDX, IDC_POSE1_RESULT59, m_Pose1Result59);
	DDX_Text(pDX, IDC_POSE1_RESULT60, m_Pose1Result60);
	DDX_Text(pDX, IDC_POSE1_RESULT61, m_Pose1Result61);
	DDX_Text(pDX, IDC_POSE1_RESULT62, m_Pose1Result62);
	DDX_Text(pDX, IDC_POSE1_RESULT63, m_Pose1Result63);
	DDX_Text(pDX, IDC_POSE1_RESULT64, m_Pose1Result64);
	DDX_Text(pDX, IDC_POSE1_RESULT65, m_Pose1Result65);
	DDX_Check(pDX, IDC_POSE1_SHOW_PATH1, m_bPose1ShowPath1);
	DDX_Check(pDX, IDC_POSE1_SHOW_PATH2, m_bPose1ShowPath2);
	DDX_Check(pDX, IDC_POSE1_SHOW_PATH3, m_bPose1ShowPath3);
	DDX_Check(pDX, IDC_POSE1_SHOW_PATH4, m_bPose1ShowPath4);
	DDX_Check(pDX, IDC_POSE1_SHOW_PATH5, m_bPose1ShowPath5);
	DDX_Check(pDX, IDC_POSE1_SHOW_PATH6, m_bPose1ShowPath6);
	DDX_Check(pDX, IDC_POSE1_SHOW_PATH7, m_bPose1ShowPath7);
	DDX_Check(pDX, IDC_POSE1_SHOW_PATH8, m_bPose1ShowPath8);
	DDX_Check(pDX, IDC_POSE1_SHOW_PATH9, m_bPose1ShowPath9);
	DDX_Check(pDX, IDC_POSE1_SHOW_PATH10, m_bPose1ShowPath10);
	DDX_Check(pDX, IDC_POSE1_SHOW_PATH11, m_bPose1ShowPath11);
	DDX_Check(pDX, IDC_POSE1_SHOW_PATH12, m_bPose1ShowPath12);
	DDX_Check(pDX, IDC_POSE1_SHOW_PATH13, m_bPose1ShowPath13);
	DDX_Check(pDX, IDC_POSE1_SHOW_PATH14, m_bPose1ShowPath14);
	DDX_Check(pDX, IDC_POSE1_SHOW_PATH15, m_bPose1ShowPath15);
	DDX_Check(pDX, IDC_POSE1_SHOW_PATH16, m_bPose1ShowPath16);
	DDX_Check(pDX, IDC_POSE1_SHOW_PATH17, m_bPose1ShowPath17);
	DDX_Check(pDX, IDC_POSE1_SHOW_PATH18, m_bPose1ShowPath18);
	DDX_Check(pDX, IDC_POSE1_SHOW_PATH19, m_bPose1ShowPath19);
	DDX_Check(pDX, IDC_POSE1_SHOW_PATH20, m_bPose1ShowPath20);
	DDX_Check(pDX, IDC_POSE1_SHOW_PATH21, m_bPose1ShowPath21);
	DDX_Check(pDX, IDC_POSE1_SHOW_PATH22, m_bPose1ShowPath22);
	DDX_Check(pDX, IDC_POSE1_SHOW_PATH23, m_bPose1ShowPath23);
	DDX_Check(pDX, IDC_POSE1_SHOW_PATH24, m_bPose1ShowPath24);
	DDX_Check(pDX, IDC_POSE1_SHOW_PATH25, m_bPose1ShowPath25);
	DDX_Check(pDX, IDC_POSE1_SHOW_PATH26, m_bPose1ShowPath26);
	DDX_Check(pDX, IDC_POSE1_SHOW_PATH27, m_bPose1ShowPath27);
	DDX_Check(pDX, IDC_POSE1_SHOW_PATH28, m_bPose1ShowPath28);
	DDX_Check(pDX, IDC_POSE1_SHOW_PATH29, m_bPose1ShowPath29);
	DDX_Check(pDX, IDC_POSE1_SHOW_PATH30, m_bPose1ShowPath30);
	DDX_Check(pDX, IDC_POSE1_SHOW_PATH31, m_bPose1ShowPath31);
	DDX_Check(pDX, IDC_POSE1_SHOW_PATH32, m_bPose1ShowPath32);
	DDX_Check(pDX, IDC_POSE1_SHOW_PATH33, m_bPose1ShowPath33);
	DDX_Check(pDX, IDC_POSE1_SHOW_PATH34, m_bPose1ShowPath34);
	DDX_Check(pDX, IDC_POSE1_SHOW_PATH35, m_bPose1ShowPath35);
	DDX_Check(pDX, IDC_POSE1_SHOW_PATH36, m_bPose1ShowPath36);
	DDX_Check(pDX, IDC_POSE1_SHOW_PATH37, m_bPose1ShowPath37);
	DDX_Check(pDX, IDC_POSE1_SHOW_PATH38, m_bPose1ShowPath38);
	DDX_Check(pDX, IDC_POSE1_SHOW_PATH39, m_bPose1ShowPath39);
	DDX_Check(pDX, IDC_POSE1_SHOW_PATH40, m_bPose1ShowPath40);
	DDX_Check(pDX, IDC_POSE1_SHOW_PATH41, m_bPose1ShowPath41);
	DDX_Check(pDX, IDC_POSE1_SHOW_PATH42, m_bPose1ShowPath42);
	DDX_Check(pDX, IDC_POSE1_SHOW_PATH43, m_bPose1ShowPath43);
	DDX_Check(pDX, IDC_POSE1_SHOW_PATH44, m_bPose1ShowPath44);
	DDX_Check(pDX, IDC_POSE1_SHOW_PATH45, m_bPose1ShowPath45);
	DDX_Check(pDX, IDC_POSE1_SHOW_PATH46, m_bPose1ShowPath46);
	DDX_Check(pDX, IDC_POSE1_SHOW_PATH47, m_bPose1ShowPath47);
	DDX_Check(pDX, IDC_POSE1_SHOW_PATH48, m_bPose1ShowPath48);
	DDX_Check(pDX, IDC_POSE1_SHOW_PATH49, m_bPose1ShowPath49);
	DDX_Check(pDX, IDC_POSE1_SHOW_PATH50, m_bPose1ShowPath50);
	DDX_Check(pDX, IDC_POSE1_SHOW_PATH51, m_bPose1ShowPath51);
	DDX_Check(pDX, IDC_POSE1_SHOW_PATH52, m_bPose1ShowPath52);
	DDX_Check(pDX, IDC_POSE1_SHOW_PATH53, m_bPose1ShowPath53);
	DDX_Check(pDX, IDC_POSE1_SHOW_PATH54, m_bPose1ShowPath54);
	DDX_Check(pDX, IDC_POSE1_SHOW_PATH55, m_bPose1ShowPath55);
	DDX_Check(pDX, IDC_POSE1_SHOW_PATH56, m_bPose1ShowPath56);
	DDX_Check(pDX, IDC_POSE1_SHOW_PATH57, m_bPose1ShowPath57);
	DDX_Check(pDX, IDC_POSE1_SHOW_PATH58, m_bPose1ShowPath58);
	DDX_Check(pDX, IDC_POSE1_SHOW_PATH59, m_bPose1ShowPath59);
	DDX_Check(pDX, IDC_POSE1_SHOW_PATH60, m_bPose1ShowPath60);
	DDX_Check(pDX, IDC_POSE1_SHOW_PATH61, m_bPose1ShowPath61);
	DDX_Check(pDX, IDC_POSE1_SHOW_PATH62, m_bPose1ShowPath62);
	DDX_Check(pDX, IDC_POSE1_SHOW_PATH63, m_bPose1ShowPath63);
	DDX_Check(pDX, IDC_POSE1_SHOW_PATH64, m_bPose1ShowPath64);
	DDX_Check(pDX, IDC_POSE1_SHOW_PATH65, m_bPose1ShowPath65);
}


BEGIN_MESSAGE_MAP(CPose1ResultDialog, CDialogEx)
	ON_BN_CLICKED(IDC_POSE1_SHOW_PATH1, &CPose1ResultDialog::OnBnClickedPose1ShowPath1)
	ON_BN_CLICKED(IDC_POSE1_SHOW_PATH2, &CPose1ResultDialog::OnBnClickedPose1ShowPath2)
	ON_BN_CLICKED(IDC_POSE1_SHOW_PATH3, &CPose1ResultDialog::OnBnClickedPose1ShowPath3)
	ON_BN_CLICKED(IDC_POSE1_SHOW_PATH4, &CPose1ResultDialog::OnBnClickedPose1ShowPath4)
	ON_BN_CLICKED(IDC_POSE1_SHOW_PATH5, &CPose1ResultDialog::OnBnClickedPose1ShowPath5)
	ON_BN_CLICKED(IDC_POSE1_SHOW_PATH6, &CPose1ResultDialog::OnBnClickedPose1ShowPath6)
	ON_BN_CLICKED(IDC_POSE1_SHOW_PATH7, &CPose1ResultDialog::OnBnClickedPose1ShowPath7)
	ON_BN_CLICKED(IDC_POSE1_SHOW_PATH8, &CPose1ResultDialog::OnBnClickedPose1ShowPath8)
	ON_BN_CLICKED(IDC_POSE1_SHOW_PATH9, &CPose1ResultDialog::OnBnClickedPose1ShowPath9)
	ON_BN_CLICKED(IDC_POSE1_SHOW_PATH10, &CPose1ResultDialog::OnBnClickedPose1ShowPath10)
	ON_BN_CLICKED(IDC_POSE1_SHOW_PATH11, &CPose1ResultDialog::OnBnClickedPose1ShowPath11)
	ON_BN_CLICKED(IDC_POSE1_SHOW_PATH12, &CPose1ResultDialog::OnBnClickedPose1ShowPath12)
	ON_BN_CLICKED(IDC_POSE1_SHOW_PATH13, &CPose1ResultDialog::OnBnClickedPose1ShowPath13)
	ON_BN_CLICKED(IDC_POSE1_SHOW_PATH14, &CPose1ResultDialog::OnBnClickedPose1ShowPath14)
	ON_BN_CLICKED(IDC_POSE1_SHOW_PATH15, &CPose1ResultDialog::OnBnClickedPose1ShowPath15)
	ON_BN_CLICKED(IDC_POSE1_SHOW_PATH16, &CPose1ResultDialog::OnBnClickedPose1ShowPath16)
	ON_BN_CLICKED(IDC_POSE1_SHOW_PATH17, &CPose1ResultDialog::OnBnClickedPose1ShowPath17)
	ON_BN_CLICKED(IDC_POSE1_SHOW_PATH18, &CPose1ResultDialog::OnBnClickedPose1ShowPath18)
	ON_BN_CLICKED(IDC_POSE1_SHOW_PATH19, &CPose1ResultDialog::OnBnClickedPose1ShowPath19)
	ON_BN_CLICKED(IDC_POSE1_SHOW_PATH20, &CPose1ResultDialog::OnBnClickedPose1ShowPath20)
	ON_BN_CLICKED(IDC_POSE1_SHOW_PATH21, &CPose1ResultDialog::OnBnClickedPose1ShowPath21)
	ON_BN_CLICKED(IDC_POSE1_SHOW_PATH22, &CPose1ResultDialog::OnBnClickedPose1ShowPath22)
	ON_BN_CLICKED(IDC_POSE1_SHOW_PATH23, &CPose1ResultDialog::OnBnClickedPose1ShowPath23)
	ON_BN_CLICKED(IDC_POSE1_SHOW_PATH24, &CPose1ResultDialog::OnBnClickedPose1ShowPath24)
	ON_BN_CLICKED(IDC_POSE1_SHOW_PATH25, &CPose1ResultDialog::OnBnClickedPose1ShowPath25)
	ON_BN_CLICKED(IDC_POSE1_SHOW_PATH26, &CPose1ResultDialog::OnBnClickedPose1ShowPath26)
	ON_BN_CLICKED(IDC_POSE1_SHOW_PATH27, &CPose1ResultDialog::OnBnClickedPose1ShowPath27)
	ON_BN_CLICKED(IDC_POSE1_SHOW_PATH28, &CPose1ResultDialog::OnBnClickedPose1ShowPath28)
	ON_BN_CLICKED(IDC_POSE1_SHOW_PATH29, &CPose1ResultDialog::OnBnClickedPose1ShowPath29)
	ON_BN_CLICKED(IDC_POSE1_SHOW_PATH30, &CPose1ResultDialog::OnBnClickedPose1ShowPath30)
	ON_BN_CLICKED(IDC_POSE1_SHOW_PATH31, &CPose1ResultDialog::OnBnClickedPose1ShowPath31)
	ON_BN_CLICKED(IDC_POSE1_SHOW_PATH32, &CPose1ResultDialog::OnBnClickedPose1ShowPath32)
	ON_BN_CLICKED(IDC_POSE1_SHOW_PATH33, &CPose1ResultDialog::OnBnClickedPose1ShowPath33)
	ON_BN_CLICKED(IDC_POSE1_SHOW_PATH34, &CPose1ResultDialog::OnBnClickedPose1ShowPath34)
	ON_BN_CLICKED(IDC_POSE1_SHOW_PATH35, &CPose1ResultDialog::OnBnClickedPose1ShowPath35)
	ON_BN_CLICKED(IDC_POSE1_SHOW_PATH36, &CPose1ResultDialog::OnBnClickedPose1ShowPath36)
	ON_BN_CLICKED(IDC_POSE1_SHOW_PATH37, &CPose1ResultDialog::OnBnClickedPose1ShowPath37)
	ON_BN_CLICKED(IDC_POSE1_SHOW_PATH38, &CPose1ResultDialog::OnBnClickedPose1ShowPath38)
	ON_BN_CLICKED(IDC_POSE1_SHOW_PATH39, &CPose1ResultDialog::OnBnClickedPose1ShowPath39)
	ON_BN_CLICKED(IDC_POSE1_SHOW_PATH40, &CPose1ResultDialog::OnBnClickedPose1ShowPath40)
//	ON_BN_CLICKED(IDC_POSE1_SHOW_PATH41, &CPose1ResultDialog::OnBnClickedPose1ShowPath41)
	ON_BN_CLICKED(IDC_POSE1_SHOW_PATH42, &CPose1ResultDialog::OnBnClickedPose1ShowPath42)
	ON_BN_CLICKED(IDC_POSE1_SHOW_PATH43, &CPose1ResultDialog::OnBnClickedPose1ShowPath43)
	ON_BN_CLICKED(IDC_POSE1_SHOW_PATH44, &CPose1ResultDialog::OnBnClickedPose1ShowPath44)
	ON_BN_CLICKED(IDC_POSE1_SHOW_PATH45, &CPose1ResultDialog::OnBnClickedPose1ShowPath45)
	ON_BN_CLICKED(IDC_POSE1_SHOW_PATH46, &CPose1ResultDialog::OnBnClickedPose1ShowPath46)
	ON_BN_CLICKED(IDC_POSE1_SHOW_PATH47, &CPose1ResultDialog::OnBnClickedPose1ShowPath47)
	ON_BN_CLICKED(IDC_POSE1_SHOW_PATH48, &CPose1ResultDialog::OnBnClickedPose1ShowPath48)
	ON_BN_CLICKED(IDC_POSE1_SHOW_PATH49, &CPose1ResultDialog::OnBnClickedPose1ShowPath49)
	ON_BN_CLICKED(IDC_POSE1_SHOW_PATH50, &CPose1ResultDialog::OnBnClickedPose1ShowPath50)
	ON_BN_CLICKED(IDC_POSE1_SHOW_PATH51, &CPose1ResultDialog::OnBnClickedPose1ShowPath51)
	ON_BN_CLICKED(IDC_POSE1_SHOW_PATH52, &CPose1ResultDialog::OnBnClickedPose1ShowPath52)
	ON_BN_CLICKED(IDC_POSE1_SHOW_PATH53, &CPose1ResultDialog::OnBnClickedPose1ShowPath53)
	ON_BN_CLICKED(IDC_POSE1_SHOW_PATH54, &CPose1ResultDialog::OnBnClickedPose1ShowPath54)
	ON_BN_CLICKED(IDC_POSE1_SHOW_PATH55, &CPose1ResultDialog::OnBnClickedPose1ShowPath55)
	ON_BN_CLICKED(IDC_POSE1_SHOW_PATH56, &CPose1ResultDialog::OnBnClickedPose1ShowPath56)
	ON_BN_CLICKED(IDC_POSE1_SHOW_PATH57, &CPose1ResultDialog::OnBnClickedPose1ShowPath57)
	ON_BN_CLICKED(IDC_POSE1_SHOW_PATH58, &CPose1ResultDialog::OnBnClickedPose1ShowPath58)
	ON_BN_CLICKED(IDC_POSE1_SHOW_PATH59, &CPose1ResultDialog::OnBnClickedPose1ShowPath59)
	ON_BN_CLICKED(IDC_POSE1_SHOW_PATH60, &CPose1ResultDialog::OnBnClickedPose1ShowPath60)
	ON_BN_CLICKED(IDC_POSE1_SHOW_PATH61, &CPose1ResultDialog::OnBnClickedPose1ShowPath61)
	ON_BN_CLICKED(IDC_POSE1_SHOW_PATH62, &CPose1ResultDialog::OnBnClickedPose1ShowPath62)
	ON_BN_CLICKED(IDC_POSE1_SHOW_PATH63, &CPose1ResultDialog::OnBnClickedPose1ShowPath63)
	ON_BN_CLICKED(IDC_POSE1_SHOW_PATH64, &CPose1ResultDialog::OnBnClickedPose1ShowPath64)
	ON_BN_CLICKED(IDC_POSE1_SHOW_PATH65, &CPose1ResultDialog::OnBnClickedPose1ShowPath65)
	ON_BN_CLICKED(IDC_POSE1_SHOW_PATH_ALL, &CPose1ResultDialog::OnBnClickedPose1ShowPathAll)
	ON_BN_CLICKED(IDC_POSE1_SHOW_PATH_NONE, &CPose1ResultDialog::OnBnClickedPose1ShowPathNone)
	ON_BN_CLICKED(IDC_POSE1_EXPORT_RESULT, &CPose1ResultDialog::OnBnClickedPose1ExportResult)
	ON_BN_CLICKED(IDC_POSE1_SHOW_PATH41, &CPose1ResultDialog::OnBnClickedPose1ShowPath41)
END_MESSAGE_MAP()

CSizeKorea2020View *CPose1ResultDialog::GetView() {
	CMainFrame *pFrame = (CMainFrame *)AfxGetMainWnd();
	return (CSizeKorea2020View *)pFrame->GetActiveView();
}

CSizeKorea2020Doc *CPose1ResultDialog::GetDocument() {
	CMainFrame *pFrame = (CMainFrame *)AfxGetMainWnd();
	return (CSizeKorea2020Doc *)pFrame->GetActiveDocument();
}

void CPose1ResultDialog::InitDialog()
{
	m_bPose1ShowPath1 = theSizer.m_bShowPaths[0];
	m_bPose1ShowPath2 = theSizer.m_bShowPaths[1];
	m_bPose1ShowPath3 = theSizer.m_bShowPaths[2];
	m_bPose1ShowPath4 = theSizer.m_bShowPaths[3];
	m_bPose1ShowPath5 = theSizer.m_bShowPaths[4];
	m_bPose1ShowPath6 = theSizer.m_bShowPaths[5];
	m_bPose1ShowPath7 = theSizer.m_bShowPaths[6];
	m_bPose1ShowPath8 = theSizer.m_bShowPaths[7];
	m_bPose1ShowPath9 = theSizer.m_bShowPaths[8];
	m_bPose1ShowPath10 = theSizer.m_bShowPaths[9];
	m_bPose1ShowPath11 = theSizer.m_bShowPaths[10];
	m_bPose1ShowPath12 = theSizer.m_bShowPaths[11];
	m_bPose1ShowPath13 = theSizer.m_bShowPaths[12];
	m_bPose1ShowPath14 = theSizer.m_bShowPaths[13];
	m_bPose1ShowPath15 = theSizer.m_bShowPaths[14];
	m_bPose1ShowPath16 = theSizer.m_bShowPaths[15];
	m_bPose1ShowPath17 = theSizer.m_bShowPaths[16];
	m_bPose1ShowPath18 = theSizer.m_bShowPaths[17];
	m_bPose1ShowPath19 = theSizer.m_bShowPaths[18];
	m_bPose1ShowPath20 = theSizer.m_bShowPaths[19];
	m_bPose1ShowPath21 = theSizer.m_bShowPaths[20];
	m_bPose1ShowPath22 = theSizer.m_bShowPaths[21];
	m_bPose1ShowPath23 = theSizer.m_bShowPaths[22];
	m_bPose1ShowPath24 = theSizer.m_bShowPaths[23];
	m_bPose1ShowPath25 = theSizer.m_bShowPaths[24];
	m_bPose1ShowPath26 = theSizer.m_bShowPaths[25];
	m_bPose1ShowPath27 = theSizer.m_bShowPaths[26];
	m_bPose1ShowPath28 = theSizer.m_bShowPaths[27];
	m_bPose1ShowPath29 = theSizer.m_bShowPaths[28];
	m_bPose1ShowPath30 = theSizer.m_bShowPaths[29];
	m_bPose1ShowPath31 = theSizer.m_bShowPaths[30];
	m_bPose1ShowPath32 = theSizer.m_bShowPaths[31];
	m_bPose1ShowPath33 = theSizer.m_bShowPaths[32];
	m_bPose1ShowPath34 = theSizer.m_bShowPaths[33];
	m_bPose1ShowPath35 = theSizer.m_bShowPaths[34];
	m_bPose1ShowPath36 = theSizer.m_bShowPaths[35];
	m_bPose1ShowPath37 = theSizer.m_bShowPaths[36];
	m_bPose1ShowPath38 = theSizer.m_bShowPaths[37];
	m_bPose1ShowPath39 = theSizer.m_bShowPaths[38];
	m_bPose1ShowPath40 = theSizer.m_bShowPaths[39];
	m_bPose1ShowPath41 = theSizer.m_bShowPaths[40];
	m_bPose1ShowPath42 = theSizer.m_bShowPaths[41];
	m_bPose1ShowPath43 = theSizer.m_bShowPaths[42];
	m_bPose1ShowPath44 = theSizer.m_bShowPaths[43];
	m_bPose1ShowPath45 = theSizer.m_bShowPaths[44];
	m_bPose1ShowPath46 = theSizer.m_bShowPaths[45];
	m_bPose1ShowPath47 = theSizer.m_bShowPaths[46];
	m_bPose1ShowPath48 = theSizer.m_bShowPaths[47];
	m_bPose1ShowPath49 = theSizer.m_bShowPaths[48];
	m_bPose1ShowPath50 = theSizer.m_bShowPaths[49];
	m_bPose1ShowPath51 = theSizer.m_bShowPaths[50];
	m_bPose1ShowPath52 = theSizer.m_bShowPaths[51];
	m_bPose1ShowPath53 = theSizer.m_bShowPaths[52];
	m_bPose1ShowPath54 = theSizer.m_bShowPaths[53];
	m_bPose1ShowPath55 = theSizer.m_bShowPaths[54];
	m_bPose1ShowPath56 = theSizer.m_bShowPaths[55];
	m_bPose1ShowPath57 = theSizer.m_bShowPaths[56];
	m_bPose1ShowPath58 = theSizer.m_bShowPaths[57];
	m_bPose1ShowPath59 = theSizer.m_bShowPaths[58];
	m_bPose1ShowPath60 = theSizer.m_bShowPaths[59];
	m_bPose1ShowPath61 = theSizer.m_bShowPaths[60];
	m_bPose1ShowPath62 = theSizer.m_bShowPaths[61];
	m_bPose1ShowPath63 = theSizer.m_bShowPaths[62];
	m_bPose1ShowPath64 = theSizer.m_bShowPaths[63];
	m_bPose1ShowPath65 = theSizer.m_bShowPaths[64];

	UpdateData(FALSE);
	GetView()->Invalidate(false);
}


void CPose1ResultDialog::OnBnClickedPose1ShowPath1()
{
	theSizer.m_bShowPaths[0] = !theSizer.m_bShowPaths[0];
	GetView()->Invalidate(FALSE);
}


void CPose1ResultDialog::OnBnClickedPose1ShowPath2()
{
	theSizer.m_bShowPaths[1] = !theSizer.m_bShowPaths[1];
	GetView()->Invalidate(FALSE);
}


void CPose1ResultDialog::OnBnClickedPose1ShowPath3()
{
	theSizer.m_bShowPaths[2] = !theSizer.m_bShowPaths[2];
	GetView()->Invalidate(FALSE);
}


void CPose1ResultDialog::OnBnClickedPose1ShowPath4()
{
	theSizer.m_bShowPaths[3] = !theSizer.m_bShowPaths[3];
	GetView()->Invalidate(FALSE);
}


void CPose1ResultDialog::OnBnClickedPose1ShowPath5()
{
	theSizer.m_bShowPaths[4] = !theSizer.m_bShowPaths[4];
	GetView()->Invalidate(FALSE);
}


void CPose1ResultDialog::OnBnClickedPose1ShowPath6()
{
	theSizer.m_bShowPaths[5] = !theSizer.m_bShowPaths[5];
	GetView()->Invalidate(FALSE);
}


void CPose1ResultDialog::OnBnClickedPose1ShowPath7()
{
	theSizer.m_bShowPaths[6] = !theSizer.m_bShowPaths[6];
	GetView()->Invalidate(FALSE);
}


void CPose1ResultDialog::OnBnClickedPose1ShowPath8()
{
	theSizer.m_bShowPaths[7] = !theSizer.m_bShowPaths[7];
	GetView()->Invalidate(FALSE);
}


void CPose1ResultDialog::OnBnClickedPose1ShowPath9()
{
	// TODO: Add your control notification handler code here
	theSizer.m_bShowPaths[8] = !theSizer.m_bShowPaths[8];
	GetView()->Invalidate(FALSE);
}


void CPose1ResultDialog::OnBnClickedPose1ShowPath10()
{
	// TODO: Add your control notification handler code here
	theSizer.m_bShowPaths[9] = !theSizer.m_bShowPaths[9];
	GetView()->Invalidate(FALSE);
}


void CPose1ResultDialog::OnBnClickedPose1ShowPath11()
{
	// TODO: Add your control notification handler code here
	theSizer.m_bShowPaths[10] = !theSizer.m_bShowPaths[10];
	GetView()->Invalidate(FALSE);
}


void CPose1ResultDialog::OnBnClickedPose1ShowPath12()
{
	// TODO: Add your control notification handler code here
	theSizer.m_bShowPaths[11] = !theSizer.m_bShowPaths[11];
	GetView()->Invalidate(FALSE);
}


void CPose1ResultDialog::OnBnClickedPose1ShowPath13()
{
	// TODO: Add your control notification handler code here
	theSizer.m_bShowPaths[12] = !theSizer.m_bShowPaths[12];
	GetView()->Invalidate(FALSE);
}


void CPose1ResultDialog::OnBnClickedPose1ShowPath14()
{
	// TODO: Add your control notification handler code here
	theSizer.m_bShowPaths[13] = !theSizer.m_bShowPaths[13];
	GetView()->Invalidate(FALSE);
}


void CPose1ResultDialog::OnBnClickedPose1ShowPath15()
{
	// TODO: Add your control notification handler code here
	theSizer.m_bShowPaths[14] = !theSizer.m_bShowPaths[14];
	GetView()->Invalidate(FALSE);
}


void CPose1ResultDialog::OnBnClickedPose1ShowPath16()
{
	// TODO: Add your control notification handler code here
	theSizer.m_bShowPaths[15] = !theSizer.m_bShowPaths[15];
	GetView()->Invalidate(FALSE);
}


void CPose1ResultDialog::OnBnClickedPose1ShowPath17()
{
	// TODO: Add your control notification handler code here
	theSizer.m_bShowPaths[16] = !theSizer.m_bShowPaths[16];
	GetView()->Invalidate(FALSE);
}


void CPose1ResultDialog::OnBnClickedPose1ShowPath18()
{
	// TODO: Add your control notification handler code here
	theSizer.m_bShowPaths[17] = !theSizer.m_bShowPaths[17];
	GetView()->Invalidate(FALSE);
}


void CPose1ResultDialog::OnBnClickedPose1ShowPath19()
{
	// TODO: Add your control notification handler code here
	theSizer.m_bShowPaths[18] = !theSizer.m_bShowPaths[18];
	GetView()->Invalidate(FALSE);
}


void CPose1ResultDialog::OnBnClickedPose1ShowPath20()
{
	// TODO: Add your control notification handler code here
	theSizer.m_bShowPaths[19] = !theSizer.m_bShowPaths[19];
	GetView()->Invalidate(FALSE);
}


void CPose1ResultDialog::OnBnClickedPose1ShowPath21()
{
	// TODO: Add your control notification handler code here
	theSizer.m_bShowPaths[20] = !theSizer.m_bShowPaths[20];
	GetView()->Invalidate(FALSE);
}


void CPose1ResultDialog::OnBnClickedPose1ShowPath22()
{
	// TODO: Add your control notification handler code here
	theSizer.m_bShowPaths[21] = !theSizer.m_bShowPaths[21];
	GetView()->Invalidate(FALSE);
}


void CPose1ResultDialog::OnBnClickedPose1ShowPath23()
{
	// TODO: Add your control notification handler code here
	theSizer.m_bShowPaths[22] = !theSizer.m_bShowPaths[22];
	GetView()->Invalidate(FALSE);
}


void CPose1ResultDialog::OnBnClickedPose1ShowPath24()
{
	// TODO: Add your control notification handler code here
	theSizer.m_bShowPaths[23] = !theSizer.m_bShowPaths[23];
	GetView()->Invalidate(FALSE);
}


void CPose1ResultDialog::OnBnClickedPose1ShowPath25()
{
	// TODO: Add your control notification handler code here
	theSizer.m_bShowPaths[24] = !theSizer.m_bShowPaths[24];
	GetView()->Invalidate(FALSE);
}


void CPose1ResultDialog::OnBnClickedPose1ShowPath26()
{
	// TODO: Add your control notification handler code here
	theSizer.m_bShowPaths[25] = !theSizer.m_bShowPaths[25];
	GetView()->Invalidate(FALSE);
}


void CPose1ResultDialog::OnBnClickedPose1ShowPath27()
{
	// TODO: Add your control notification handler code here
	theSizer.m_bShowPaths[26] = !theSizer.m_bShowPaths[26];
	GetView()->Invalidate(FALSE);
}


void CPose1ResultDialog::OnBnClickedPose1ShowPath28()
{
	// TODO: Add your control notification handler code here
	theSizer.m_bShowPaths[27] = !theSizer.m_bShowPaths[27];
	GetView()->Invalidate(FALSE);
}


void CPose1ResultDialog::OnBnClickedPose1ShowPath29()
{
	// TODO: Add your control notification handler code here
	theSizer.m_bShowPaths[28] = !theSizer.m_bShowPaths[28];
	GetView()->Invalidate(FALSE);
}


void CPose1ResultDialog::OnBnClickedPose1ShowPath30()
{
	// TODO: Add your control notification handler code here
	theSizer.m_bShowPaths[29] = !theSizer.m_bShowPaths[29];
	GetView()->Invalidate(FALSE);
}


void CPose1ResultDialog::OnBnClickedPose1ShowPath31()
{
	// TODO: Add your control notification handler code here
	theSizer.m_bShowPaths[30] = !theSizer.m_bShowPaths[30];
	GetView()->Invalidate(FALSE);
}


void CPose1ResultDialog::OnBnClickedPose1ShowPath32()
{
	// TODO: Add your control notification handler code here
	theSizer.m_bShowPaths[31] = !theSizer.m_bShowPaths[31];
	GetView()->Invalidate(FALSE);
}


void CPose1ResultDialog::OnBnClickedPose1ShowPath33()
{
	// TODO: Add your control notification handler code here
	theSizer.m_bShowPaths[32] = !theSizer.m_bShowPaths[32];
	GetView()->Invalidate(FALSE);
}


void CPose1ResultDialog::OnBnClickedPose1ShowPath34()
{
	// TODO: Add your control notification handler code here
	theSizer.m_bShowPaths[33] = !theSizer.m_bShowPaths[33];
	GetView()->Invalidate(FALSE);
}


void CPose1ResultDialog::OnBnClickedPose1ShowPath35()
{
	// TODO: Add your control notification handler code here
	theSizer.m_bShowPaths[34] = !theSizer.m_bShowPaths[34];
	GetView()->Invalidate(FALSE);
}


void CPose1ResultDialog::OnBnClickedPose1ShowPath36()
{
	// TODO: Add your control notification handler code here
	theSizer.m_bShowPaths[35] = !theSizer.m_bShowPaths[35];
	GetView()->Invalidate(FALSE);
}


void CPose1ResultDialog::OnBnClickedPose1ShowPath37()
{
	// TODO: Add your control notification handler code here
	theSizer.m_bShowPaths[36] = !theSizer.m_bShowPaths[36];
	GetView()->Invalidate(FALSE);
}


void CPose1ResultDialog::OnBnClickedPose1ShowPath38()
{
	// TODO: Add your control notification handler code here
	theSizer.m_bShowPaths[37] = !theSizer.m_bShowPaths[37];
	GetView()->Invalidate(FALSE);
}


void CPose1ResultDialog::OnBnClickedPose1ShowPath39()
{
	// TODO: Add your control notification handler code here
	theSizer.m_bShowPaths[38] = !theSizer.m_bShowPaths[38];
	GetView()->Invalidate(FALSE);
}


void CPose1ResultDialog::OnBnClickedPose1ShowPath40()
{
	// TODO: Add your control notification handler code here
	theSizer.m_bShowPaths[39] = !theSizer.m_bShowPaths[39];
	GetView()->Invalidate(FALSE);
}


void CPose1ResultDialog::OnBnClickedPose1ShowPath41()
{
	// TODO: Add your control notification handler code here
	theSizer.m_bShowPaths[40] = !theSizer.m_bShowPaths[40];
	GetView()->Invalidate(FALSE);
}


void CPose1ResultDialog::OnBnClickedPose1ShowPath42()
{
	// TODO: Add your control notification handler code here
	theSizer.m_bShowPaths[41] = !theSizer.m_bShowPaths[41];
	GetView()->Invalidate(FALSE);
}


void CPose1ResultDialog::OnBnClickedPose1ShowPath43()
{
	// TODO: Add your control notification handler code here
	theSizer.m_bShowPaths[42] = !theSizer.m_bShowPaths[42];
	GetView()->Invalidate(FALSE);
}


void CPose1ResultDialog::OnBnClickedPose1ShowPath44()
{
	// TODO: Add your control notification handler code here
	theSizer.m_bShowPaths[43] = !theSizer.m_bShowPaths[43];
	GetView()->Invalidate(FALSE);
}


void CPose1ResultDialog::OnBnClickedPose1ShowPath45()
{
	// TODO: Add your control notification handler code here
	theSizer.m_bShowPaths[44] = !theSizer.m_bShowPaths[44];
	GetView()->Invalidate(FALSE);
}


void CPose1ResultDialog::OnBnClickedPose1ShowPath46()
{
	// TODO: Add your control notification handler code here
	theSizer.m_bShowPaths[45] = !theSizer.m_bShowPaths[45];
	GetView()->Invalidate(FALSE);
}


void CPose1ResultDialog::OnBnClickedPose1ShowPath47()
{
	// TODO: Add your control notification handler code here
	theSizer.m_bShowPaths[46] = !theSizer.m_bShowPaths[46];
	GetView()->Invalidate(FALSE);
}


void CPose1ResultDialog::OnBnClickedPose1ShowPath48()
{
	// TODO: Add your control notification handler code here
	theSizer.m_bShowPaths[47] = !theSizer.m_bShowPaths[47];
	GetView()->Invalidate(FALSE);
}


void CPose1ResultDialog::OnBnClickedPose1ShowPath49()
{
	// TODO: Add your control notification handler code here
	theSizer.m_bShowPaths[48] = !theSizer.m_bShowPaths[48];
	GetView()->Invalidate(FALSE);
}


void CPose1ResultDialog::OnBnClickedPose1ShowPath50()
{
	// TODO: Add your control notification handler code here
	theSizer.m_bShowPaths[49] = !theSizer.m_bShowPaths[49];
	GetView()->Invalidate(FALSE);
}


void CPose1ResultDialog::OnBnClickedPose1ShowPath51()
{
	// TODO: Add your control notification handler code here
	theSizer.m_bShowPaths[50] = !theSizer.m_bShowPaths[50];
	GetView()->Invalidate(FALSE);
}


void CPose1ResultDialog::OnBnClickedPose1ShowPath52()
{
	// TODO: Add your control notification handler code here
	theSizer.m_bShowPaths[51] = !theSizer.m_bShowPaths[51];
	GetView()->Invalidate(FALSE);
}


void CPose1ResultDialog::OnBnClickedPose1ShowPath53()
{
	// TODO: Add your control notification handler code here
	theSizer.m_bShowPaths[52] = !theSizer.m_bShowPaths[52];
	GetView()->Invalidate(FALSE);
}


void CPose1ResultDialog::OnBnClickedPose1ShowPath54()
{
	// TODO: Add your control notification handler code here
	theSizer.m_bShowPaths[53] = !theSizer.m_bShowPaths[53];
	GetView()->Invalidate(FALSE);
}


void CPose1ResultDialog::OnBnClickedPose1ShowPath55()
{
	// TODO: Add your control notification handler code here
	theSizer.m_bShowPaths[54] = !theSizer.m_bShowPaths[54];
	GetView()->Invalidate(FALSE);
}


void CPose1ResultDialog::OnBnClickedPose1ShowPath56()
{
	// TODO: Add your control notification handler code here
	theSizer.m_bShowPaths[55] = !theSizer.m_bShowPaths[55];
	GetView()->Invalidate(FALSE);
}


void CPose1ResultDialog::OnBnClickedPose1ShowPath57()
{
	// TODO: Add your control notification handler code here
	theSizer.m_bShowPaths[56] = !theSizer.m_bShowPaths[56];
	GetView()->Invalidate(FALSE);
}


void CPose1ResultDialog::OnBnClickedPose1ShowPath58()
{
	// TODO: Add your control notification handler code here
	theSizer.m_bShowPaths[57] = !theSizer.m_bShowPaths[57];
	GetView()->Invalidate(FALSE);
}


void CPose1ResultDialog::OnBnClickedPose1ShowPath59()
{
	// TODO: Add your control notification handler code here
	theSizer.m_bShowPaths[58] = !theSizer.m_bShowPaths[58];
	GetView()->Invalidate(FALSE);
}


void CPose1ResultDialog::OnBnClickedPose1ShowPath60()
{
	// TODO: Add your control notification handler code here
	theSizer.m_bShowPaths[59] = !theSizer.m_bShowPaths[59];
	GetView()->Invalidate(FALSE);
}


void CPose1ResultDialog::OnBnClickedPose1ShowPath61()
{
	// TODO: Add your control notification handler code here
	theSizer.m_bShowPaths[60] = !theSizer.m_bShowPaths[60];
	GetView()->Invalidate(FALSE);
}


void CPose1ResultDialog::OnBnClickedPose1ShowPath62()
{
	// TODO: Add your control notification handler code here
	theSizer.m_bShowPaths[61] = !theSizer.m_bShowPaths[61];
	GetView()->Invalidate(FALSE);
}


void CPose1ResultDialog::OnBnClickedPose1ShowPath63()
{
	// TODO: Add your control notification handler code here
	theSizer.m_bShowPaths[62] = !theSizer.m_bShowPaths[62];
	GetView()->Invalidate(FALSE);
}


void CPose1ResultDialog::OnBnClickedPose1ShowPath64()
{
	// TODO: Add your control notification handler code here
	theSizer.m_bShowPaths[63] = !theSizer.m_bShowPaths[63];
	GetView()->Invalidate(FALSE);
}


void CPose1ResultDialog::OnBnClickedPose1ShowPath65()
{
	// TODO: Add your control notification handler code here
	theSizer.m_bShowPaths[64] = !theSizer.m_bShowPaths[64];
	GetView()->Invalidate(FALSE);
}



void CPose1ResultDialog::OnBnClickedPose1ShowPathAll()
{
	// TODO: Add your control notification handler code here
	m_bPose1ShowPath1 = true;
	m_bPose1ShowPath2 = true;
	m_bPose1ShowPath3 = true;
	m_bPose1ShowPath4 = true;
	m_bPose1ShowPath5 = true;
	m_bPose1ShowPath6 = true;
	m_bPose1ShowPath7 = true;
	m_bPose1ShowPath8 = true;
	m_bPose1ShowPath9 = true;
	m_bPose1ShowPath10 = true;
	m_bPose1ShowPath11 = true;
	m_bPose1ShowPath12 = true;
	m_bPose1ShowPath13 = true;
	m_bPose1ShowPath14 = true;
	m_bPose1ShowPath15 = true;
	m_bPose1ShowPath16 = true;
	m_bPose1ShowPath17 = true;
	m_bPose1ShowPath18 = true;
	m_bPose1ShowPath19 = true;
	m_bPose1ShowPath20 = true;
	m_bPose1ShowPath21 = true;
	m_bPose1ShowPath22 = true;
	m_bPose1ShowPath23 = true;
	m_bPose1ShowPath24 = true;
	m_bPose1ShowPath25 = true;
	m_bPose1ShowPath26 = true;
	m_bPose1ShowPath27 = true;
	m_bPose1ShowPath28 = true;
	m_bPose1ShowPath29 = true;
	m_bPose1ShowPath30 = true;
	m_bPose1ShowPath31 = true;
	m_bPose1ShowPath32 = true;
	m_bPose1ShowPath33 = true;
	m_bPose1ShowPath34 = true;
	m_bPose1ShowPath35 = true;
	m_bPose1ShowPath36 = true;
	m_bPose1ShowPath37 = true;
	m_bPose1ShowPath38 = true;
	m_bPose1ShowPath39 = true;
	m_bPose1ShowPath40 = true;
	m_bPose1ShowPath41 = true;
	m_bPose1ShowPath42 = true;
	m_bPose1ShowPath43 = true;
	m_bPose1ShowPath44 = true;
	m_bPose1ShowPath45 = true;
	m_bPose1ShowPath46 = true;
	m_bPose1ShowPath47 = true;
	m_bPose1ShowPath48 = true;
	m_bPose1ShowPath49 = true;
	m_bPose1ShowPath50 = true;
	m_bPose1ShowPath51 = true;
	m_bPose1ShowPath52 = true;
	m_bPose1ShowPath53 = true;
	m_bPose1ShowPath54 = true;
	m_bPose1ShowPath55 = true;
	m_bPose1ShowPath56 = true;
	m_bPose1ShowPath57 = true;
	m_bPose1ShowPath58 = true;
	m_bPose1ShowPath59 = true;
	m_bPose1ShowPath60 = true;
	m_bPose1ShowPath61 = true;
	m_bPose1ShowPath62 = true;
	m_bPose1ShowPath63 = true;
	m_bPose1ShowPath64 = true;
	m_bPose1ShowPath65 = true;

	for (int i = 0; i < NUM(theSizer.m_bShowPaths); ++i)
		theSizer.m_bShowPaths[i] = true;

	UpdateData(FALSE);
	GetView()->Invalidate(FALSE);
}


void CPose1ResultDialog::OnBnClickedPose1ShowPathNone()
{
	// TODO: Add your control notification handler code here
	m_bPose1ShowPath1 = false;
	m_bPose1ShowPath2 = false;
	m_bPose1ShowPath3 = false;
	m_bPose1ShowPath4 = false;
	m_bPose1ShowPath5 = false;
	m_bPose1ShowPath6 = false;
	m_bPose1ShowPath7 = false;
	m_bPose1ShowPath8 = false;
	m_bPose1ShowPath9 = false;
	m_bPose1ShowPath10 = false;
	m_bPose1ShowPath11 = false;
	m_bPose1ShowPath12 = false;
	m_bPose1ShowPath13 = false;
	m_bPose1ShowPath14 = false;
	m_bPose1ShowPath15 = false;
	m_bPose1ShowPath16 = false;
	m_bPose1ShowPath17 = false;
	m_bPose1ShowPath18 = false;
	m_bPose1ShowPath19 = false;
	m_bPose1ShowPath20 = false;
	m_bPose1ShowPath21 = false;
	m_bPose1ShowPath22 = false;
	m_bPose1ShowPath23 = false;
	m_bPose1ShowPath24 = false;
	m_bPose1ShowPath25 = false;
	m_bPose1ShowPath26 = false;
	m_bPose1ShowPath27 = false;
	m_bPose1ShowPath28 = false;
	m_bPose1ShowPath29 = false;
	m_bPose1ShowPath30 = false;
	m_bPose1ShowPath31 = false;
	m_bPose1ShowPath32 = false;
	m_bPose1ShowPath33 = false;
	m_bPose1ShowPath34 = false;
	m_bPose1ShowPath35 = false;
	m_bPose1ShowPath36 = false;
	m_bPose1ShowPath37 = false;
	m_bPose1ShowPath38 = false;
	m_bPose1ShowPath39 = false;
	m_bPose1ShowPath40 = false;
	m_bPose1ShowPath41 = false;
	m_bPose1ShowPath42 = false;
	m_bPose1ShowPath43 = false;
	m_bPose1ShowPath44 = false;
	m_bPose1ShowPath45 = false;
	m_bPose1ShowPath46 = false;
	m_bPose1ShowPath47 = false;
	m_bPose1ShowPath48 = false;
	m_bPose1ShowPath49 = false;
	m_bPose1ShowPath50 = false;
	m_bPose1ShowPath51 = false;
	m_bPose1ShowPath52 = false;
	m_bPose1ShowPath53 = false;
	m_bPose1ShowPath54 = false;
	m_bPose1ShowPath55 = false;
	m_bPose1ShowPath56 = false;
	m_bPose1ShowPath57 = false;
	m_bPose1ShowPath58 = false;
	m_bPose1ShowPath59 = false;
	m_bPose1ShowPath60 = false;
	m_bPose1ShowPath61 = false;
	m_bPose1ShowPath62 = false;
	m_bPose1ShowPath63 = false;
	m_bPose1ShowPath64 = false;
	m_bPose1ShowPath65 = false;

	for (int i = 0; i < NUM(theSizer.m_bShowPaths); ++i)
		theSizer.m_bShowPaths[i] = false;

	UpdateData(FALSE);
	GetView()->Invalidate(FALSE);
}


void CPose1ResultDialog::OnBnClickedPose1ExportResult()
{
	CString strFileName;
	LPCTSTR filter = "Measurement file(ascii)(*.csv)|*.csv||";
	CFileDialog FileDlg(false, NULL, NULL, OFN_HIDEREADONLY, filter);
	if (FileDlg.DoModal() != IDOK)
		return;

	strFileName = FileDlg.GetPathName(); // get file path.
	if (strFileName.Find(".csv") == -1)
		strFileName += ".csv";

	FILE *fp;
	fopen_s(&fp, strFileName, "w");
	int NumResults = (int)theSizer.m_Results.size();
	for (int i = 0; i < NumResults; i++)
		fprintf(fp, "%s,%.1lf\n", ResultPose1[i].c_str(), theSizer.m_Results[i]);
	fclose(fp);
}

