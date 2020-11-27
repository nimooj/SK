// CPose2ResultDialog.cpp : implementation file
//

#include "pch.h"
#include "SizeKorea-2020.h"
#include "CPose2ResultDialog.h"
#include "afxdialogex.h"

#include "EgSizer.h"
#include "MainFrm.h"

#include "Pose2Measures.h"

extern EgSizer theSizer;
extern std::vector<std::string> ResultPose2;


// CPose2ResultDialog dialog

IMPLEMENT_DYNAMIC(CPose2ResultDialog, CDialogEx)

CPose2ResultDialog::CPose2ResultDialog(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_POSE2_RESULT_DIALOG, pParent)
	, m_bPose2ShowPath1(FALSE)
	, m_Pose2Result1(_T(""))
	, m_bPose2ShowPath2(FALSE)
	, m_bPose2ShowPath3(FALSE)
	, m_bPose2ShowPath4(FALSE)
	, m_bPose2ShowPath5(FALSE)
	, m_bPose2ShowPath6(FALSE)
	, m_bPose2ShowPath7(FALSE)
	, m_bPose2ShowPath8(FALSE)
	, m_bPose2ShowPath9(FALSE)
	, m_bPose2ShowPath10(FALSE)
	, m_bPose2ShowPath11(FALSE)
	, m_bPose2ShowPath12(FALSE)
	, m_bPose2ShowPath13(FALSE)
	, m_bPose2ShowPath14(FALSE)
	, m_bPose2ShowPath15(FALSE)
	, m_bPose2ShowPath16(FALSE)
	, m_bPose2ShowPath17(FALSE)
	, m_bPose2ShowPath18(FALSE)
	, m_bPose2ShowPath19(FALSE)
	, m_bPose2ShowPath20(FALSE)
	, m_bPose2ShowPath21(FALSE)
	, m_bPose2ShowPath22(FALSE)
	, m_bPose2ShowPath23(FALSE)
	, m_bPose2ShowPath24(FALSE)
	, m_bPose2ShowPath25(FALSE)
	, m_bPose2ShowPath26(FALSE)
	, m_bPose2ShowPath27(FALSE)
	, m_bPose2ShowPath28(FALSE)
	, m_bPose2ShowPath29(FALSE)
	, m_bPose2ShowPath30(FALSE)
	, m_bPose2ShowPath31(FALSE)
	, m_bPose2ShowPath32(FALSE)
	, m_bPose2ShowPath33(FALSE)
	, m_bPose2ShowPath34(FALSE)
	, m_bPose2ShowPath35(FALSE)
	, m_bPose2ShowPath36(FALSE)
	, m_bPose2ShowPath37(FALSE)
	, m_bPose2ShowPath38(FALSE)
	, m_bPose2ShowPath39(FALSE)
	, m_bPose2ShowPath40(FALSE)
	, m_bPose2ShowPath41(FALSE)
	, m_bPose2ShowPath42(FALSE)
	, m_bPose2ShowPath43(FALSE)
	, m_bPose2ShowPath44(FALSE)
	, m_bPose2ShowPath45(FALSE)
	, m_bPose2ShowPath46(FALSE)
	, m_bPose2ShowPath47(FALSE)
	, m_bPose2ShowPath48(FALSE)
	, m_bPose2ShowPath49(FALSE)
	, m_bPose2ShowPath50(FALSE)
	, m_bPose2ShowPath51(FALSE)
	, m_bPose2ShowPath52(FALSE)
	, m_bPose2ShowPath53(FALSE)
	, m_bPose2ShowPath54(FALSE)
	, m_bPose2ShowPath55(FALSE)
	, m_bPose2ShowPath56(FALSE)
	, m_bPose2ShowPath57(FALSE)
	, m_bPose2ShowPath58(FALSE)
	, m_bPose2ShowPath59(FALSE)
	, m_bPose2ShowPath60(FALSE)
	, m_bPose2ShowPath61(FALSE)
	, m_bPose2ShowPath62(FALSE)
	, m_bPose2ShowPath63(FALSE)
	, m_bPose2ShowPath64(FALSE)
	, m_bPose2ShowPath65(FALSE)
	, m_bPose2ShowPath66(FALSE)
	, m_bPose2ShowPath67(FALSE)
	, m_bPose2ShowPath68(FALSE)
	, m_bPose2ShowPath69(FALSE)
	, m_bPose2ShowPath70(FALSE)
	, m_bPose2ShowPath71(FALSE)
	, m_bPose2ShowPath72(FALSE)
	, m_bPose2ShowPath73(FALSE)
	, m_bPose2ShowPath74(FALSE)
	, m_bPose2ShowPath75(FALSE)
	, m_bPose2ShowPath76(FALSE)
	, m_bPose2ShowPath77(FALSE)
	, m_bPose2ShowPath78(FALSE)
	, m_bPose2ShowPath79(FALSE)
	, m_Pose2Result2(_T(""))
	, m_Pose2Result3(_T(""))
	, m_Pose2Result4(_T(""))
	, m_Pose2Result5(_T(""))
	, m_Pose2Result6(_T(""))
	, m_Pose2Result7(_T(""))
	, m_Pose2Result8(_T(""))
	, m_Pose2Result9(_T(""))
	, m_Pose2Result10(_T(""))
	, m_Pose2Result11(_T(""))
	, m_Pose2Result12(_T(""))
	, m_Pose2Result13(_T(""))
	, m_Pose2Result14(_T(""))
	, m_Pose2Result15(_T(""))
	, m_Pose2Result16(_T(""))
	, m_Pose2Result17(_T(""))
	, m_Pose2Result18(_T(""))
	, m_Pose2Result19(_T(""))
	, m_Pose2Result20(_T(""))
		, m_Pose2Result21(_T(""))
		, m_Pose2Result22(_T(""))
		, m_Pose2Result23(_T(""))
		, m_Pose2Result24(_T(""))
		, m_Pose2Result25(_T(""))
		, m_Pose2Result27(_T(""))
		, m_Pose2Result28(_T(""))
		, m_Pose2Result29(_T(""))
		, m_Pose2Result30(_T(""))
		, m_Pose2Result31(_T(""))
		, m_Pose2Result32(_T(""))
		, m_Pose2Result33(_T(""))
		, m_Pose2Result34(_T(""))
		, m_Pose2Result35(_T(""))
		, m_Pose2Result36(_T(""))
		, m_Pose2Result37(_T(""))
		, m_Pose2Result38(_T(""))
		, m_Pose2Result39(_T(""))
		, m_Pose2Result40(_T(""))
		, m_Pose2Result41(_T(""))
		, m_Pose2Result42(_T(""))
		, m_Pose2Result43(_T(""))
		, m_Pose2Result44(_T(""))
		, m_Pose2Result45(_T(""))
		, m_Pose2Result46(_T(""))
		, m_Pose2Result47(_T(""))
		, m_Pose2Result48(_T(""))
		, m_Pose2Result49(_T(""))
		, m_Pose2Result50(_T(""))
		, m_Pose2Result51(_T(""))
		, m_Pose2Result52(_T(""))
		, m_Pose2Result53(_T(""))
		, m_Pose2Result54(_T(""))
		, m_Pose2Result55(_T(""))
		, m_Pose2Result56(_T(""))
		, m_Pose2Result57(_T(""))
		, m_Pose2Result58(_T(""))
		, m_Pose2Result59(_T(""))
		, m_Pose2Result60(_T(""))
		, m_Pose2Result61(_T(""))
		, m_Pose2Result62(_T(""))
		, m_Pose2Result63(_T(""))
		, m_Pose2Result64(_T(""))
		, m_Pose2Result65(_T(""))
		, m_Pose2Result66(_T(""))
		, m_Pose2Result67(_T(""))
		, m_Pose2Result68(_T(""))
		, m_Pose2Result69(_T(""))
		, m_Pose2Result70(_T(""))
		, m_Pose2Result71(_T(""))
		, m_Pose2Result72(_T(""))
		, m_Pose2Result73(_T(""))
		, m_Pose2Result74(_T(""))
		, m_Pose2Result75(_T(""))
		, m_Pose2Result76(_T(""))
		, m_Pose2Result77(_T(""))
		, m_Pose2Result78(_T(""))
		, m_Pose2Result79(_T(""))
	{

}

CPose2ResultDialog::~CPose2ResultDialog()
{
}

void CPose2ResultDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_POSE2_SHOW_PATH1, m_bPose2ShowPath1);
	DDX_Text(pDX, IDC_POSE2_RESULT1, m_Pose2Result1);
	DDX_Check(pDX, IDC_POSE2_SHOW_PATH2, m_bPose2ShowPath2);
	DDX_Check(pDX, IDC_POSE2_SHOW_PATH3, m_bPose2ShowPath3);
	DDX_Check(pDX, IDC_POSE2_SHOW_PATH4, m_bPose2ShowPath4);
	DDX_Check(pDX, IDC_POSE2_SHOW_PATH5, m_bPose2ShowPath5);
	DDX_Check(pDX, IDC_POSE2_SHOW_PATH6, m_bPose2ShowPath6);
	DDX_Check(pDX, IDC_POSE2_SHOW_PATH7, m_bPose2ShowPath7);
	DDX_Check(pDX, IDC_POSE2_SHOW_PATH8, m_bPose2ShowPath8);
	DDX_Check(pDX, IDC_POSE2_SHOW_PATH9, m_bPose2ShowPath9);
	DDX_Check(pDX, IDC_POSE2_SHOW_PATH10, m_bPose2ShowPath10);
	DDX_Check(pDX, IDC_POSE2_SHOW_PATH11, m_bPose2ShowPath11);
	DDX_Check(pDX, IDC_POSE2_SHOW_PATH12, m_bPose2ShowPath12);
	DDX_Check(pDX, IDC_POSE2_SHOW_PATH13, m_bPose2ShowPath13);
	DDX_Check(pDX, IDC_POSE2_SHOW_PATH14, m_bPose2ShowPath14);
	DDX_Check(pDX, IDC_POSE2_SHOW_PATH15, m_bPose2ShowPath15);
	DDX_Check(pDX, IDC_POSE2_SHOW_PATH16, m_bPose2ShowPath16);
	DDX_Check(pDX, IDC_POSE2_SHOW_PATH17, m_bPose2ShowPath17);
	DDX_Check(pDX, IDC_POSE2_SHOW_PATH18, m_bPose2ShowPath18);
	DDX_Check(pDX, IDC_POSE2_SHOW_PATH19, m_bPose2ShowPath19);
	DDX_Check(pDX, IDC_POSE2_SHOW_PATH20, m_bPose2ShowPath20);
	DDX_Check(pDX, IDC_POSE2_SHOW_PATH21, m_bPose2ShowPath21);
	DDX_Check(pDX, IDC_POSE2_SHOW_PATH22, m_bPose2ShowPath22);
	DDX_Check(pDX, IDC_POSE2_SHOW_PATH23, m_bPose2ShowPath23);
	DDX_Check(pDX, IDC_POSE2_SHOW_PATH24, m_bPose2ShowPath24);
	DDX_Check(pDX, IDC_POSE2_SHOW_PATH25, m_bPose2ShowPath25);
	DDX_Check(pDX, IDC_POSE2_SHOW_PATH26, m_bPose2ShowPath26);
	DDX_Check(pDX, IDC_POSE2_SHOW_PATH27, m_bPose2ShowPath27);
	DDX_Check(pDX, IDC_POSE2_SHOW_PATH28, m_bPose2ShowPath28);
	DDX_Check(pDX, IDC_POSE2_SHOW_PATH29, m_bPose2ShowPath29);
	DDX_Check(pDX, IDC_POSE2_SHOW_PATH30, m_bPose2ShowPath30);
	DDX_Check(pDX, IDC_POSE2_SHOW_PATH31, m_bPose2ShowPath31);
	DDX_Check(pDX, IDC_POSE2_SHOW_PATH32, m_bPose2ShowPath32);
	DDX_Check(pDX, IDC_POSE2_SHOW_PATH33, m_bPose2ShowPath33);
	DDX_Check(pDX, IDC_POSE2_SHOW_PATH34, m_bPose2ShowPath34);
	DDX_Check(pDX, IDC_POSE2_SHOW_PATH35, m_bPose2ShowPath35);
	DDX_Check(pDX, IDC_POSE2_SHOW_PATH36, m_bPose2ShowPath36);
	DDX_Check(pDX, IDC_POSE2_SHOW_PATH37, m_bPose2ShowPath37);
	DDX_Check(pDX, IDC_POSE2_SHOW_PATH38, m_bPose2ShowPath38);
	DDX_Check(pDX, IDC_POSE2_SHOW_PATH39, m_bPose2ShowPath39);
	DDX_Check(pDX, IDC_POSE2_SHOW_PATH40, m_bPose2ShowPath40);
	DDX_Check(pDX, IDC_POSE2_SHOW_PATH41, m_bPose2ShowPath41);
	DDX_Check(pDX, IDC_POSE2_SHOW_PATH42, m_bPose2ShowPath42);
	DDX_Check(pDX, IDC_POSE2_SHOW_PATH43, m_bPose2ShowPath43);
	DDX_Check(pDX, IDC_POSE2_SHOW_PATH44, m_bPose2ShowPath44);
	DDX_Check(pDX, IDC_POSE2_SHOW_PATH45, m_bPose2ShowPath45);
	DDX_Check(pDX, IDC_POSE2_SHOW_PATH46, m_bPose2ShowPath46);
	DDX_Check(pDX, IDC_POSE2_SHOW_PATH47, m_bPose2ShowPath47);
	DDX_Check(pDX, IDC_POSE2_SHOW_PATH48, m_bPose2ShowPath48);
	DDX_Check(pDX, IDC_POSE2_SHOW_PATH49, m_bPose2ShowPath49);
	DDX_Check(pDX, IDC_POSE2_SHOW_PATH50, m_bPose2ShowPath50);
	DDX_Check(pDX, IDC_POSE2_SHOW_PATH51, m_bPose2ShowPath51);
	DDX_Check(pDX, IDC_POSE2_SHOW_PATH52, m_bPose2ShowPath52);
	DDX_Check(pDX, IDC_POSE2_SHOW_PATH53, m_bPose2ShowPath53);
	DDX_Check(pDX, IDC_POSE2_SHOW_PATH54, m_bPose2ShowPath54);
	DDX_Check(pDX, IDC_POSE2_SHOW_PATH55, m_bPose2ShowPath55);
	DDX_Check(pDX, IDC_POSE2_SHOW_PATH56, m_bPose2ShowPath56);
	DDX_Check(pDX, IDC_POSE2_SHOW_PATH57, m_bPose2ShowPath57);
	DDX_Check(pDX, IDC_POSE2_SHOW_PATH58, m_bPose2ShowPath58);
	DDX_Check(pDX, IDC_POSE2_SHOW_PATH59, m_bPose2ShowPath59);
	DDX_Check(pDX, IDC_POSE2_SHOW_PATH60, m_bPose2ShowPath60);
	DDX_Check(pDX, IDC_POSE2_SHOW_PATH61, m_bPose2ShowPath61);
	DDX_Check(pDX, IDC_POSE2_SHOW_PATH62, m_bPose2ShowPath62);
	DDX_Check(pDX, IDC_POSE2_SHOW_PATH63, m_bPose2ShowPath63);
	DDX_Check(pDX, IDC_POSE2_SHOW_PATH64, m_bPose2ShowPath64);
	DDX_Check(pDX, IDC_POSE2_SHOW_PATH65, m_bPose2ShowPath65);
	DDX_Check(pDX, IDC_POSE2_SHOW_PATH66, m_bPose2ShowPath66);
	DDX_Check(pDX, IDC_POSE2_SHOW_PATH67, m_bPose2ShowPath67);
	DDX_Check(pDX, IDC_POSE2_SHOW_PATH68, m_bPose2ShowPath68);
	DDX_Check(pDX, IDC_POSE2_SHOW_PATH69, m_bPose2ShowPath69);
	DDX_Check(pDX, IDC_POSE2_SHOW_PATH70, m_bPose2ShowPath70);
	DDX_Check(pDX, IDC_POSE2_SHOW_PATH71, m_bPose2ShowPath71);
	DDX_Check(pDX, IDC_POSE2_SHOW_PATH72, m_bPose2ShowPath72);
	DDX_Check(pDX, IDC_POSE2_SHOW_PATH73, m_bPose2ShowPath73);
	DDX_Check(pDX, IDC_POSE2_SHOW_PATH74, m_bPose2ShowPath74);
	DDX_Check(pDX, IDC_POSE2_SHOW_PATH75, m_bPose2ShowPath75);
	DDX_Check(pDX, IDC_POSE2_SHOW_PATH76, m_bPose2ShowPath76);
	DDX_Check(pDX, IDC_POSE2_SHOW_PATH77, m_bPose2ShowPath77);
	DDX_Check(pDX, IDC_POSE2_SHOW_PATH78, m_bPose2ShowPath78);
	DDX_Check(pDX, IDC_POSE2_SHOW_PATH79, m_bPose2ShowPath79);
	DDX_Text(pDX, IDC_POSE2_RESULT2, m_Pose2Result2);
	DDX_Text(pDX, IDC_POSE2_RESULT3, m_Pose2Result3);
	DDX_Text(pDX, IDC_POSE2_RESULT4, m_Pose2Result4);
	DDX_Text(pDX, IDC_POSE2_RESULT5, m_Pose2Result5);
	DDX_Text(pDX, IDC_POSE2_RESULT6, m_Pose2Result6);
	DDX_Text(pDX, IDC_POSE2_RESULT7, m_Pose2Result7);
	DDX_Text(pDX, IDC_POSE2_RESULT8, m_Pose2Result8);
	DDX_Text(pDX, IDC_POSE2_RESULT9, m_Pose2Result9);
	DDX_Text(pDX, IDC_POSE2_RESULT10, m_Pose2Result10);
	DDX_Text(pDX, IDC_POSE2_RESULT11, m_Pose2Result11);
	DDX_Text(pDX, IDC_POSE2_RESULT12, m_Pose2Result12);
	DDX_Text(pDX, IDC_POSE2_RESULT13, m_Pose2Result13);
	DDX_Text(pDX, IDC_POSE2_RESULT14, m_Pose2Result14);
	DDX_Text(pDX, IDC_POSE2_RESULT15, m_Pose2Result15);
	DDX_Text(pDX, IDC_POSE2_RESULT16, m_Pose2Result16);
	DDX_Text(pDX, IDC_POSE2_RESULT17, m_Pose2Result17);
	DDX_Text(pDX, IDC_POSE2_RESULT18, m_Pose2Result18);
	DDX_Text(pDX, IDC_POSE2_RESULT19, m_Pose2Result19);
	DDX_Text(pDX, IDC_POSE2_RESULT20, m_Pose2Result20);
	DDX_Text(pDX, IDC_POSE2_RESULT21, m_Pose2Result21);
	DDX_Text(pDX, IDC_POSE2_RESULT22, m_Pose2Result22);
	DDX_Text(pDX, IDC_POSE2_RESULT23, m_Pose2Result23);
	DDX_Text(pDX, IDC_POSE2_RESULT24, m_Pose2Result24);
	DDX_Text(pDX, IDC_POSE2_RESULT25, m_Pose2Result25);
	DDX_Text(pDX, IDC_POSE2_RESULT26, m_Pose2Result26);
	DDX_Text(pDX, IDC_POSE2_RESULT27, m_Pose2Result27);
	DDX_Text(pDX, IDC_POSE2_RESULT28, m_Pose2Result28);
	DDX_Text(pDX, IDC_POSE2_RESULT29, m_Pose2Result29);
	DDX_Text(pDX, IDC_POSE2_RESULT30, m_Pose2Result30);
	DDX_Text(pDX, IDC_POSE2_RESULT31, m_Pose2Result31);
	DDX_Text(pDX, IDC_POSE2_RESULT32, m_Pose2Result32);
	DDX_Text(pDX, IDC_POSE2_RESULT33, m_Pose2Result33);
	DDX_Text(pDX, IDC_POSE2_RESULT34, m_Pose2Result34);
	DDX_Text(pDX, IDC_POSE2_RESULT35, m_Pose2Result35);
	DDX_Text(pDX, IDC_POSE2_RESULT36, m_Pose2Result36);
	DDX_Text(pDX, IDC_POSE2_RESULT37, m_Pose2Result37);
	DDX_Text(pDX, IDC_POSE2_RESULT38, m_Pose2Result38);
	DDX_Text(pDX, IDC_POSE2_RESULT39, m_Pose2Result39);
	DDX_Text(pDX, IDC_POSE2_RESULT40, m_Pose2Result40);
	DDX_Text(pDX, IDC_POSE2_RESULT41, m_Pose2Result41);
	DDX_Text(pDX, IDC_POSE2_RESULT42, m_Pose2Result42);
	DDX_Text(pDX, IDC_POSE2_RESULT43, m_Pose2Result43);
	DDX_Text(pDX, IDC_POSE2_RESULT44, m_Pose2Result44);
	DDX_Text(pDX, IDC_POSE2_RESULT45, m_Pose2Result45);
	DDX_Text(pDX, IDC_POSE2_RESULT46, m_Pose2Result46);
	DDX_Text(pDX, IDC_POSE2_RESULT47, m_Pose2Result47);
	DDX_Text(pDX, IDC_POSE2_RESULT48, m_Pose2Result48);
	DDX_Text(pDX, IDC_POSE2_RESULT49, m_Pose2Result49);
	DDX_Text(pDX, IDC_POSE2_RESULT50, m_Pose2Result50);
	DDX_Text(pDX, IDC_POSE2_RESULT51, m_Pose2Result51);
	DDX_Text(pDX, IDC_POSE2_RESULT52, m_Pose2Result52);
	DDX_Text(pDX, IDC_POSE2_RESULT53, m_Pose2Result53);
	DDX_Text(pDX, IDC_POSE2_RESULT54, m_Pose2Result54);
	DDX_Text(pDX, IDC_POSE2_RESULT55, m_Pose2Result55);
	DDX_Text(pDX, IDC_POSE2_RESULT56, m_Pose2Result56);
	DDX_Text(pDX, IDC_POSE2_RESULT57, m_Pose2Result57);
	DDX_Text(pDX, IDC_POSE2_RESULT58, m_Pose2Result58);
	DDX_Text(pDX, IDC_POSE2_RESULT59, m_Pose2Result59);
	DDX_Text(pDX, IDC_POSE2_RESULT60, m_Pose2Result60);
	DDX_Text(pDX, IDC_POSE2_RESULT61, m_Pose2Result61);
	DDX_Text(pDX, IDC_POSE2_RESULT62, m_Pose2Result62);
	DDX_Text(pDX, IDC_POSE2_RESULT63, m_Pose2Result63);
	DDX_Text(pDX, IDC_POSE2_RESULT64, m_Pose2Result64);
	DDX_Text(pDX, IDC_POSE2_RESULT65, m_Pose2Result65);
	DDX_Text(pDX, IDC_POSE2_RESULT66, m_Pose2Result66);
	DDX_Text(pDX, IDC_POSE2_RESULT67, m_Pose2Result67);
	DDX_Text(pDX, IDC_POSE2_RESULT68, m_Pose2Result68);
	DDX_Text(pDX, IDC_POSE2_RESULT69, m_Pose2Result69);
	DDX_Text(pDX, IDC_POSE2_RESULT70, m_Pose2Result70);
	DDX_Text(pDX, IDC_POSE2_RESULT71, m_Pose2Result71);
	DDX_Text(pDX, IDC_POSE2_RESULT72, m_Pose2Result72);
	DDX_Text(pDX, IDC_POSE2_RESULT73, m_Pose2Result73);
	DDX_Text(pDX, IDC_POSE2_RESULT74, m_Pose2Result74);
	DDX_Text(pDX, IDC_POSE2_RESULT75, m_Pose2Result75);
	DDX_Text(pDX, IDC_POSE2_RESULT76, m_Pose2Result76);
	DDX_Text(pDX, IDC_POSE2_RESULT77, m_Pose2Result77);
	DDX_Text(pDX, IDC_POSE2_RESULT78, m_Pose2Result78);
	DDX_Text(pDX, IDC_POSE2_RESULT79, m_Pose2Result79);
}


BEGIN_MESSAGE_MAP(CPose2ResultDialog, CDialogEx)
	ON_BN_CLICKED(IDC_POSE2_SHOW_PATH1, &CPose2ResultDialog::OnBnClickedPose2ShowPath1)
	ON_BN_CLICKED(IDC_POSE2_SHOW_PATH2, &CPose2ResultDialog::OnBnClickedPose2ShowPath2)
	ON_BN_CLICKED(IDC_POSE2_SHOW_PATH3, &CPose2ResultDialog::OnBnClickedPose2ShowPath3)
	ON_BN_CLICKED(IDC_POSE2_SHOW_PATH4, &CPose2ResultDialog::OnBnClickedPose2ShowPath4)
	ON_BN_CLICKED(IDC_POSE2_SHOW_PATH5, &CPose2ResultDialog::OnBnClickedPose2ShowPath5)
	ON_BN_CLICKED(IDC_POSE2_SHOW_PATH6, &CPose2ResultDialog::OnBnClickedPose2ShowPath6)
	ON_BN_CLICKED(IDC_POSE2_SHOW_PATH7, &CPose2ResultDialog::OnBnClickedPose2ShowPath7)
	ON_BN_CLICKED(IDC_POSE2_SHOW_PATH8, &CPose2ResultDialog::OnBnClickedPose2ShowPath8)
	ON_BN_CLICKED(IDC_POSE2_SHOW_PATH9, &CPose2ResultDialog::OnBnClickedPose2ShowPath9)
	ON_BN_CLICKED(IDC_POSE2_SHOW_PATH10, &CPose2ResultDialog::OnBnClickedPose2ShowPath10)
	ON_BN_CLICKED(IDC_POSE2_SHOW_PATH11, &CPose2ResultDialog::OnBnClickedPose2ShowPath11)
	ON_BN_CLICKED(IDC_POSE2_SHOW_PATH12, &CPose2ResultDialog::OnBnClickedPose2ShowPath12)
	ON_BN_CLICKED(IDC_POSE2_SHOW_PATH13, &CPose2ResultDialog::OnBnClickedPose2ShowPath13)
	ON_BN_CLICKED(IDC_POSE2_SHOW_PATH14, &CPose2ResultDialog::OnBnClickedPose2ShowPath14)
	ON_BN_CLICKED(IDC_POSE2_SHOW_PATH15, &CPose2ResultDialog::OnBnClickedPose2ShowPath15)
	ON_BN_CLICKED(IDC_POSE2_SHOW_PATH16, &CPose2ResultDialog::OnBnClickedPose2ShowPath16)
	ON_BN_CLICKED(IDC_POSE2_SHOW_PATH17, &CPose2ResultDialog::OnBnClickedPose2ShowPath17)
	ON_BN_CLICKED(IDC_POSE2_SHOW_PATH18, &CPose2ResultDialog::OnBnClickedPose2ShowPath18)
	ON_BN_CLICKED(IDC_POSE2_SHOW_PATH19, &CPose2ResultDialog::OnBnClickedPose2ShowPath19)
	ON_BN_CLICKED(IDC_POSE2_SHOW_PATH20, &CPose2ResultDialog::OnBnClickedPose2ShowPath20)
	ON_BN_CLICKED(IDC_POSE2_SHOW_PATH21, &CPose2ResultDialog::OnBnClickedPose2ShowPath21)
	ON_BN_CLICKED(IDC_POSE2_SHOW_PATH22, &CPose2ResultDialog::OnBnClickedPose2ShowPath22)
	ON_BN_CLICKED(IDC_POSE2_SHOW_PATH23, &CPose2ResultDialog::OnBnClickedPose2ShowPath23)
	ON_BN_CLICKED(IDC_POSE2_SHOW_PATH24, &CPose2ResultDialog::OnBnClickedPose2ShowPath24)
	ON_BN_CLICKED(IDC_POSE2_SHOW_PATH25, &CPose2ResultDialog::OnBnClickedPose2ShowPath25)
	ON_BN_CLICKED(IDC_POSE2_SHOW_PATH26, &CPose2ResultDialog::OnBnClickedPose2ShowPath26)
	ON_BN_CLICKED(IDC_POSE2_SHOW_PATH27, &CPose2ResultDialog::OnBnClickedPose2ShowPath27)
	ON_BN_CLICKED(IDC_POSE2_SHOW_PATH28, &CPose2ResultDialog::OnBnClickedPose2ShowPath28)
	ON_BN_CLICKED(IDC_POSE2_SHOW_PATH29, &CPose2ResultDialog::OnBnClickedPose2ShowPath29)
	ON_BN_CLICKED(IDC_POSE2_SHOW_PATH30, &CPose2ResultDialog::OnBnClickedPose2ShowPath30)
	ON_BN_CLICKED(IDC_POSE2_SHOW_PATH31, &CPose2ResultDialog::OnBnClickedPose2ShowPath31)
	ON_BN_CLICKED(IDC_POSE2_SHOW_PATH32, &CPose2ResultDialog::OnBnClickedPose2ShowPath32)
	ON_BN_CLICKED(IDC_POSE2_SHOW_PATH33, &CPose2ResultDialog::OnBnClickedPose2ShowPath33)
	ON_BN_CLICKED(IDC_POSE2_SHOW_PATH34, &CPose2ResultDialog::OnBnClickedPose2ShowPath34)
	ON_BN_CLICKED(IDC_POSE2_SHOW_PATH35, &CPose2ResultDialog::OnBnClickedPose2ShowPath35)
	ON_BN_CLICKED(IDC_POSE2_SHOW_PATH36, &CPose2ResultDialog::OnBnClickedPose2ShowPath36)
	ON_BN_CLICKED(IDC_POSE2_SHOW_PATH37, &CPose2ResultDialog::OnBnClickedPose2ShowPath37)
	ON_BN_CLICKED(IDC_POSE2_SHOW_PATH38, &CPose2ResultDialog::OnBnClickedPose2ShowPath38)
	ON_BN_CLICKED(IDC_POSE2_SHOW_PATH39, &CPose2ResultDialog::OnBnClickedPose2ShowPath39)
	ON_BN_CLICKED(IDC_POSE2_SHOW_PATH40, &CPose2ResultDialog::OnBnClickedPose2ShowPath40)
	ON_BN_CLICKED(IDC_POSE2_SHOW_PATH41, &CPose2ResultDialog::OnBnClickedPose2ShowPath41)
	ON_BN_CLICKED(IDC_POSE2_SHOW_PATH42, &CPose2ResultDialog::OnBnClickedPose2ShowPath42)
	ON_BN_CLICKED(IDC_POSE2_SHOW_PATH43, &CPose2ResultDialog::OnBnClickedPose2ShowPath43)
	ON_BN_CLICKED(IDC_POSE2_SHOW_PATH44, &CPose2ResultDialog::OnBnClickedPose2ShowPath44)
	ON_BN_CLICKED(IDC_POSE2_SHOW_PATH45, &CPose2ResultDialog::OnBnClickedPose2ShowPath45)
	ON_BN_CLICKED(IDC_POSE2_SHOW_PATH46, &CPose2ResultDialog::OnBnClickedPose2ShowPath46)
	ON_BN_CLICKED(IDC_POSE2_SHOW_PATH47, &CPose2ResultDialog::OnBnClickedPose2ShowPath47)
	ON_BN_CLICKED(IDC_POSE2_SHOW_PATH48, &CPose2ResultDialog::OnBnClickedPose2ShowPath48)
	ON_BN_CLICKED(IDC_POSE2_SHOW_PATH49, &CPose2ResultDialog::OnBnClickedPose2ShowPath49)
	ON_BN_CLICKED(IDC_POSE2_SHOW_PATH50, &CPose2ResultDialog::OnBnClickedPose2ShowPath50)
	ON_BN_CLICKED(IDC_POSE2_SHOW_PATH51, &CPose2ResultDialog::OnBnClickedPose2ShowPath51)
	ON_BN_CLICKED(IDC_POSE2_SHOW_PATH52, &CPose2ResultDialog::OnBnClickedPose2ShowPath52)
	ON_BN_CLICKED(IDC_POSE2_SHOW_PATH53, &CPose2ResultDialog::OnBnClickedPose2ShowPath53)
	ON_BN_CLICKED(IDC_POSE2_SHOW_PATH54, &CPose2ResultDialog::OnBnClickedPose2ShowPath54)
	ON_BN_CLICKED(IDC_POSE2_SHOW_PATH55, &CPose2ResultDialog::OnBnClickedPose2ShowPath55)
	ON_BN_CLICKED(IDC_POSE2_SHOW_PATH56, &CPose2ResultDialog::OnBnClickedPose2ShowPath56)
	ON_BN_CLICKED(IDC_POSE2_SHOW_PATH57, &CPose2ResultDialog::OnBnClickedPose2ShowPath57)
	ON_BN_CLICKED(IDC_POSE2_SHOW_PATH58, &CPose2ResultDialog::OnBnClickedPose2ShowPath58)
	ON_BN_CLICKED(IDC_POSE2_SHOW_PATH59, &CPose2ResultDialog::OnBnClickedPose2ShowPath59)
	ON_BN_CLICKED(IDC_POSE2_SHOW_PATH60, &CPose2ResultDialog::OnBnClickedPose2ShowPath60)
	ON_BN_CLICKED(IDC_POSE2_SHOW_PATH61, &CPose2ResultDialog::OnBnClickedPose2ShowPath61)
	ON_BN_CLICKED(IDC_POSE2_SHOW_PATH62, &CPose2ResultDialog::OnBnClickedPose2ShowPath62)
	ON_BN_CLICKED(IDC_POSE2_SHOW_PATH63, &CPose2ResultDialog::OnBnClickedPose2ShowPath63)
	ON_BN_CLICKED(IDC_POSE2_SHOW_PATH64, &CPose2ResultDialog::OnBnClickedPose2ShowPath64)
	ON_BN_CLICKED(IDC_POSE2_SHOW_PATH65, &CPose2ResultDialog::OnBnClickedPose2ShowPath65)
	ON_BN_CLICKED(IDC_POSE2_SHOW_PATH66, &CPose2ResultDialog::OnBnClickedPose2ShowPath66)
	ON_BN_CLICKED(IDC_POSE2_SHOW_PATH67, &CPose2ResultDialog::OnBnClickedPose2ShowPath67)
	ON_BN_CLICKED(IDC_POSE2_SHOW_PATH68, &CPose2ResultDialog::OnBnClickedPose2ShowPath68)
	ON_BN_CLICKED(IDC_POSE2_SHOW_PATH69, &CPose2ResultDialog::OnBnClickedPose2ShowPath69)
	ON_BN_CLICKED(IDC_POSE2_SHOW_PATH70, &CPose2ResultDialog::OnBnClickedPose2ShowPath70)
	ON_BN_CLICKED(IDC_POSE2_SHOW_PATH71, &CPose2ResultDialog::OnBnClickedPose2ShowPath71)
	ON_BN_CLICKED(IDC_POSE2_SHOW_PATH72, &CPose2ResultDialog::OnBnClickedPose2ShowPath72)
	ON_BN_CLICKED(IDC_POSE2_SHOW_PATH73, &CPose2ResultDialog::OnBnClickedPose2ShowPath73)
	ON_BN_CLICKED(IDC_POSE2_SHOW_PATH74, &CPose2ResultDialog::OnBnClickedPose2ShowPath74)
	ON_BN_CLICKED(IDC_POSE2_SHOW_PATH75, &CPose2ResultDialog::OnBnClickedPose2ShowPath75)
	ON_BN_CLICKED(IDC_POSE2_SHOW_PATH76, &CPose2ResultDialog::OnBnClickedPose2ShowPath76)
	ON_BN_CLICKED(IDC_POSE2_SHOW_PATH77, &CPose2ResultDialog::OnBnClickedPose2ShowPath77)
	ON_BN_CLICKED(IDC_POSE2_SHOW_PATH78, &CPose2ResultDialog::OnBnClickedPose2ShowPath78)
	ON_BN_CLICKED(IDC_POSE2_SHOW_PATH79, &CPose2ResultDialog::OnBnClickedPose2ShowPath79)
	ON_BN_CLICKED(IDC_POSE2_SHOW_PATH_ALL, &CPose2ResultDialog::OnBnClickedPose2ShowPathAll)
	ON_BN_CLICKED(IDC_POSE2_SHOW_PATH_NONE, &CPose2ResultDialog::OnBnClickedPose2ShowPathNone)
	ON_BN_CLICKED(IDC_POSE2_EXPORT_RESULT, &CPose2ResultDialog::OnBnClickedPose2ExportResult)
END_MESSAGE_MAP()

CSizeKorea2020View *CPose2ResultDialog::GetView() {
	CMainFrame *pFrame = (CMainFrame *)AfxGetMainWnd();
	return (CSizeKorea2020View *)pFrame->GetActiveView();
}

CSizeKorea2020Doc *CPose2ResultDialog::GetDocument() {
	CMainFrame *pFrame = (CMainFrame *)AfxGetMainWnd();
	return (CSizeKorea2020Doc *)pFrame->GetActiveDocument();
}

void CPose2ResultDialog::InitDialog()
{
	m_bPose2ShowPath1 = theSizer.m_bShowPaths[0];
	m_bPose2ShowPath2 = theSizer.m_bShowPaths[1];
	m_bPose2ShowPath3 = theSizer.m_bShowPaths[2];
	m_bPose2ShowPath4 = theSizer.m_bShowPaths[3];
	m_bPose2ShowPath5 = theSizer.m_bShowPaths[4];
	m_bPose2ShowPath6 = theSizer.m_bShowPaths[5];
	m_bPose2ShowPath7 = theSizer.m_bShowPaths[6];
	m_bPose2ShowPath8 = theSizer.m_bShowPaths[7];
	m_bPose2ShowPath9 = theSizer.m_bShowPaths[8];
	m_bPose2ShowPath10 = theSizer.m_bShowPaths[9];
	m_bPose2ShowPath11 = theSizer.m_bShowPaths[10];
	m_bPose2ShowPath12 = theSizer.m_bShowPaths[11];
	m_bPose2ShowPath13 = theSizer.m_bShowPaths[12];
	m_bPose2ShowPath14 = theSizer.m_bShowPaths[13];
	m_bPose2ShowPath15 = theSizer.m_bShowPaths[14];
	m_bPose2ShowPath16 = theSizer.m_bShowPaths[15];
	m_bPose2ShowPath17 = theSizer.m_bShowPaths[16];
	m_bPose2ShowPath18 = theSizer.m_bShowPaths[17];
	m_bPose2ShowPath19 = theSizer.m_bShowPaths[18];
	m_bPose2ShowPath20 = theSizer.m_bShowPaths[19];
	m_bPose2ShowPath21 = theSizer.m_bShowPaths[20];
	m_bPose2ShowPath22 = theSizer.m_bShowPaths[21];
	m_bPose2ShowPath23 = theSizer.m_bShowPaths[22];
	m_bPose2ShowPath24 = theSizer.m_bShowPaths[23];
	m_bPose2ShowPath25 = theSizer.m_bShowPaths[24];
	m_bPose2ShowPath26 = theSizer.m_bShowPaths[25];
	m_bPose2ShowPath27 = theSizer.m_bShowPaths[26];
	m_bPose2ShowPath28 = theSizer.m_bShowPaths[27];
	m_bPose2ShowPath29 = theSizer.m_bShowPaths[28];
	m_bPose2ShowPath30 = theSizer.m_bShowPaths[29];
	m_bPose2ShowPath31 = theSizer.m_bShowPaths[30];
	m_bPose2ShowPath32 = theSizer.m_bShowPaths[31];
	m_bPose2ShowPath33 = theSizer.m_bShowPaths[32];
	m_bPose2ShowPath34 = theSizer.m_bShowPaths[33];
	m_bPose2ShowPath35 = theSizer.m_bShowPaths[34];
	m_bPose2ShowPath36 = theSizer.m_bShowPaths[35];
	m_bPose2ShowPath37 = theSizer.m_bShowPaths[36];
	m_bPose2ShowPath38 = theSizer.m_bShowPaths[37];
	m_bPose2ShowPath39 = theSizer.m_bShowPaths[38];
	m_bPose2ShowPath40 = theSizer.m_bShowPaths[39];
	m_bPose2ShowPath41 = theSizer.m_bShowPaths[40];
	m_bPose2ShowPath42 = theSizer.m_bShowPaths[41];
	m_bPose2ShowPath43 = theSizer.m_bShowPaths[42];
	m_bPose2ShowPath44 = theSizer.m_bShowPaths[43];
	m_bPose2ShowPath45 = theSizer.m_bShowPaths[44];
	m_bPose2ShowPath46 = theSizer.m_bShowPaths[45];
	m_bPose2ShowPath47 = theSizer.m_bShowPaths[46];
	m_bPose2ShowPath48 = theSizer.m_bShowPaths[47];
	m_bPose2ShowPath49 = theSizer.m_bShowPaths[48];
	m_bPose2ShowPath50 = theSizer.m_bShowPaths[49];
	m_bPose2ShowPath51 = theSizer.m_bShowPaths[50];
	m_bPose2ShowPath52 = theSizer.m_bShowPaths[51];
	m_bPose2ShowPath53 = theSizer.m_bShowPaths[52];
	m_bPose2ShowPath54 = theSizer.m_bShowPaths[53];
	m_bPose2ShowPath55 = theSizer.m_bShowPaths[54];
	m_bPose2ShowPath56 = theSizer.m_bShowPaths[55];
	m_bPose2ShowPath57 = theSizer.m_bShowPaths[56];
	m_bPose2ShowPath58 = theSizer.m_bShowPaths[57];
	m_bPose2ShowPath59 = theSizer.m_bShowPaths[58];
	m_bPose2ShowPath60 = theSizer.m_bShowPaths[59];
	m_bPose2ShowPath61 = theSizer.m_bShowPaths[60];
	m_bPose2ShowPath62 = theSizer.m_bShowPaths[61];
	m_bPose2ShowPath63 = theSizer.m_bShowPaths[62];
	m_bPose2ShowPath64 = theSizer.m_bShowPaths[63];
	m_bPose2ShowPath65 = theSizer.m_bShowPaths[64];
	m_bPose2ShowPath66 = theSizer.m_bShowPaths[65];
	m_bPose2ShowPath67 = theSizer.m_bShowPaths[66];
	m_bPose2ShowPath68 = theSizer.m_bShowPaths[67];
	m_bPose2ShowPath69 = theSizer.m_bShowPaths[68];
	m_bPose2ShowPath70 = theSizer.m_bShowPaths[69];
	m_bPose2ShowPath71 = theSizer.m_bShowPaths[70];
	m_bPose2ShowPath72 = theSizer.m_bShowPaths[71];
	m_bPose2ShowPath73 = theSizer.m_bShowPaths[72];
	m_bPose2ShowPath74 = theSizer.m_bShowPaths[73];
	m_bPose2ShowPath75 = theSizer.m_bShowPaths[74];
	m_bPose2ShowPath76 = theSizer.m_bShowPaths[75];
	m_bPose2ShowPath77 = theSizer.m_bShowPaths[76];
	m_bPose2ShowPath78 = theSizer.m_bShowPaths[77];
	m_bPose2ShowPath79 = theSizer.m_bShowPaths[78];

	UpdateData(FALSE);
	GetView()->Invalidate(false);
}


// CPose2ResultDialog message handlers


void CPose2ResultDialog::OnBnClickedPose2ShowPath1()
{
	// TODO: Add your control notification handler code here
	theSizer.m_bShowPaths[0] = !theSizer.m_bShowPaths[0];
	GetView()->Invalidate(FALSE);
}


void CPose2ResultDialog::OnBnClickedPose2ShowPath2()
{
	// TODO: Add your control notification handler code here
	theSizer.m_bShowPaths[1] = !theSizer.m_bShowPaths[1];
	GetView()->Invalidate(FALSE);
}


void CPose2ResultDialog::OnBnClickedPose2ShowPath3()
{
	// TODO: Add your control notification handler code here
	theSizer.m_bShowPaths[2] = !theSizer.m_bShowPaths[2];
	GetView()->Invalidate(FALSE);
}


void CPose2ResultDialog::OnBnClickedPose2ShowPath4()
{
	// TODO: Add your control notification handler code here
	theSizer.m_bShowPaths[3] = !theSizer.m_bShowPaths[3];
	GetView()->Invalidate(FALSE);
}


void CPose2ResultDialog::OnBnClickedPose2ShowPath5()
{
	// TODO: Add your control notification handler code here
	theSizer.m_bShowPaths[4] = !theSizer.m_bShowPaths[4];
	GetView()->Invalidate(FALSE);
}


void CPose2ResultDialog::OnBnClickedPose2ShowPath6()
{
	// TODO: Add your control notification handler code here
	theSizer.m_bShowPaths[5] = !theSizer.m_bShowPaths[5];
	GetView()->Invalidate(FALSE);
}


void CPose2ResultDialog::OnBnClickedPose2ShowPath7()
{
	// TODO: Add your control notification handler code here
	theSizer.m_bShowPaths[6] = !theSizer.m_bShowPaths[6];
	GetView()->Invalidate(FALSE);
}


void CPose2ResultDialog::OnBnClickedPose2ShowPath8()
{
	// TODO: Add your control notification handler code here
	theSizer.m_bShowPaths[7] = !theSizer.m_bShowPaths[7];
	GetView()->Invalidate(FALSE);
}


void CPose2ResultDialog::OnBnClickedPose2ShowPath9()
{
	// TODO: Add your control notification handler code here
	theSizer.m_bShowPaths[8] = !theSizer.m_bShowPaths[8];
	GetView()->Invalidate(FALSE);
}


void CPose2ResultDialog::OnBnClickedPose2ShowPath10()
{
	// TODO: Add your control notification handler code here
	theSizer.m_bShowPaths[9] = !theSizer.m_bShowPaths[9];
	GetView()->Invalidate(FALSE);
}


void CPose2ResultDialog::OnBnClickedPose2ShowPath11()
{
	// TODO: Add your control notification handler code here
	theSizer.m_bShowPaths[10] = !theSizer.m_bShowPaths[10];
	GetView()->Invalidate(FALSE);
}


void CPose2ResultDialog::OnBnClickedPose2ShowPath12()
{
	// TODO: Add your control notification handler code here
	theSizer.m_bShowPaths[11] = !theSizer.m_bShowPaths[11];
	GetView()->Invalidate(FALSE);
}


void CPose2ResultDialog::OnBnClickedPose2ShowPath13()
{
	// TODO: Add your control notification handler code here
	theSizer.m_bShowPaths[12] = !theSizer.m_bShowPaths[12];
	GetView()->Invalidate(FALSE);
}


void CPose2ResultDialog::OnBnClickedPose2ShowPath14()
{
	// TODO: Add your control notification handler code here
	theSizer.m_bShowPaths[13] = !theSizer.m_bShowPaths[13];
	GetView()->Invalidate(FALSE);
}


void CPose2ResultDialog::OnBnClickedPose2ShowPath15()
{
	// TODO: Add your control notification handler code here
	theSizer.m_bShowPaths[14] = !theSizer.m_bShowPaths[14];
	GetView()->Invalidate(FALSE);
}


void CPose2ResultDialog::OnBnClickedPose2ShowPath16()
{
	// TODO: Add your control notification handler code here
	theSizer.m_bShowPaths[15] = !theSizer.m_bShowPaths[15];
	GetView()->Invalidate(FALSE);
}


void CPose2ResultDialog::OnBnClickedPose2ShowPath17()
{
	// TODO: Add your control notification handler code here
	theSizer.m_bShowPaths[16] = !theSizer.m_bShowPaths[16];
	GetView()->Invalidate(FALSE);
}


void CPose2ResultDialog::OnBnClickedPose2ShowPath18()
{
	// TODO: Add your control notification handler code here
	theSizer.m_bShowPaths[17] = !theSizer.m_bShowPaths[17];
	GetView()->Invalidate(FALSE);
}


void CPose2ResultDialog::OnBnClickedPose2ShowPath19()
{
	// TODO: Add your control notification handler code here
	theSizer.m_bShowPaths[18] = !theSizer.m_bShowPaths[18];
	GetView()->Invalidate(FALSE);
}


void CPose2ResultDialog::OnBnClickedPose2ShowPath20()
{
	// TODO: Add your control notification handler code here
	theSizer.m_bShowPaths[19] = !theSizer.m_bShowPaths[19];
	GetView()->Invalidate(FALSE);
}


void CPose2ResultDialog::OnBnClickedPose2ShowPath21()
{
	// TODO: Add your control notification handler code here
	theSizer.m_bShowPaths[20] = !theSizer.m_bShowPaths[20];
	GetView()->Invalidate(FALSE);
}


void CPose2ResultDialog::OnBnClickedPose2ShowPath22()
{
	// TODO: Add your control notification handler code here
	theSizer.m_bShowPaths[21] = !theSizer.m_bShowPaths[21];
	GetView()->Invalidate(FALSE);
}


void CPose2ResultDialog::OnBnClickedPose2ShowPath23()
{
	// TODO: Add your control notification handler code here
	theSizer.m_bShowPaths[22] = !theSizer.m_bShowPaths[22];
	GetView()->Invalidate(FALSE);
}


void CPose2ResultDialog::OnBnClickedPose2ShowPath24()
{
	// TODO: Add your control notification handler code here
	theSizer.m_bShowPaths[23] = !theSizer.m_bShowPaths[23];
	GetView()->Invalidate(FALSE);
}


void CPose2ResultDialog::OnBnClickedPose2ShowPath25()
{
	// TODO: Add your control notification handler code here
	theSizer.m_bShowPaths[24] = !theSizer.m_bShowPaths[24];
	GetView()->Invalidate(FALSE);
}


void CPose2ResultDialog::OnBnClickedPose2ShowPath26()
{
	// TODO: Add your control notification handler code here
	theSizer.m_bShowPaths[25] = !theSizer.m_bShowPaths[25];
	GetView()->Invalidate(FALSE);
}


void CPose2ResultDialog::OnBnClickedPose2ShowPath27()
{
	// TODO: Add your control notification handler code here
	theSizer.m_bShowPaths[26] = !theSizer.m_bShowPaths[26];
	GetView()->Invalidate(FALSE);
}


void CPose2ResultDialog::OnBnClickedPose2ShowPath28()
{
	// TODO: Add your control notification handler code here
	theSizer.m_bShowPaths[27] = !theSizer.m_bShowPaths[27];
	GetView()->Invalidate(FALSE);
}


void CPose2ResultDialog::OnBnClickedPose2ShowPath29()
{
	// TODO: Add your control notification handler code here
	theSizer.m_bShowPaths[28] = !theSizer.m_bShowPaths[28];
	GetView()->Invalidate(FALSE);
}


void CPose2ResultDialog::OnBnClickedPose2ShowPath30()
{
	// TODO: Add your control notification handler code here
	theSizer.m_bShowPaths[29] = !theSizer.m_bShowPaths[29];
	GetView()->Invalidate(FALSE);
}


void CPose2ResultDialog::OnBnClickedPose2ShowPath31()
{
	// TODO: Add your control notification handler code here
	theSizer.m_bShowPaths[30] = !theSizer.m_bShowPaths[30];
	GetView()->Invalidate(FALSE);
}


void CPose2ResultDialog::OnBnClickedPose2ShowPath32()
{
	// TODO: Add your control notification handler code here
	theSizer.m_bShowPaths[31] = !theSizer.m_bShowPaths[31];
	GetView()->Invalidate(FALSE);
}


void CPose2ResultDialog::OnBnClickedPose2ShowPath33()
{
	// TODO: Add your control notification handler code here
	theSizer.m_bShowPaths[32] = !theSizer.m_bShowPaths[32];
	GetView()->Invalidate(FALSE);
}


void CPose2ResultDialog::OnBnClickedPose2ShowPath34()
{
	// TODO: Add your control notification handler code here
	theSizer.m_bShowPaths[33] = !theSizer.m_bShowPaths[33];
	GetView()->Invalidate(FALSE);
}


void CPose2ResultDialog::OnBnClickedPose2ShowPath35()
{
	// TODO: Add your control notification handler code here
	theSizer.m_bShowPaths[34] = !theSizer.m_bShowPaths[34];
	GetView()->Invalidate(FALSE);
}


void CPose2ResultDialog::OnBnClickedPose2ShowPath36()
{
	// TODO: Add your control notification handler code here
	theSizer.m_bShowPaths[35] = !theSizer.m_bShowPaths[35];
	GetView()->Invalidate(FALSE);
}


void CPose2ResultDialog::OnBnClickedPose2ShowPath37()
{
	// TODO: Add your control notification handler code here
	theSizer.m_bShowPaths[36] = !theSizer.m_bShowPaths[36];
	GetView()->Invalidate(FALSE);
}


void CPose2ResultDialog::OnBnClickedPose2ShowPath38()
{
	// TODO: Add your control notification handler code here
	theSizer.m_bShowPaths[37] = !theSizer.m_bShowPaths[37];
	GetView()->Invalidate(FALSE);
}


void CPose2ResultDialog::OnBnClickedPose2ShowPath39()
{
	// TODO: Add your control notification handler code here
	theSizer.m_bShowPaths[38] = !theSizer.m_bShowPaths[38];
	GetView()->Invalidate(FALSE);
}


void CPose2ResultDialog::OnBnClickedPose2ShowPath40()
{
	// TODO: Add your control notification handler code here
	theSizer.m_bShowPaths[39] = !theSizer.m_bShowPaths[39];
	GetView()->Invalidate(FALSE);
}


void CPose2ResultDialog::OnBnClickedPose2ShowPath41()
{
	// TODO: Add your control notification handler code here
	theSizer.m_bShowPaths[40] = !theSizer.m_bShowPaths[40];
	GetView()->Invalidate(FALSE);
}


void CPose2ResultDialog::OnBnClickedPose2ShowPath42()
{
	// TODO: Add your control notification handler code here
	theSizer.m_bShowPaths[41] = !theSizer.m_bShowPaths[41];
	GetView()->Invalidate(FALSE);
}


void CPose2ResultDialog::OnBnClickedPose2ShowPath43()
{
	// TODO: Add your control notification handler code here
	theSizer.m_bShowPaths[42] = !theSizer.m_bShowPaths[42];
	GetView()->Invalidate(FALSE);
}


void CPose2ResultDialog::OnBnClickedPose2ShowPath44()
{
	// TODO: Add your control notification handler code here
	theSizer.m_bShowPaths[43] = !theSizer.m_bShowPaths[43];
	GetView()->Invalidate(FALSE);
}


void CPose2ResultDialog::OnBnClickedPose2ShowPath45()
{
	// TODO: Add your control notification handler code here
	theSizer.m_bShowPaths[44] = !theSizer.m_bShowPaths[44];
	GetView()->Invalidate(FALSE);
}


void CPose2ResultDialog::OnBnClickedPose2ShowPath46()
{
	// TODO: Add your control notification handler code here
	theSizer.m_bShowPaths[45] = !theSizer.m_bShowPaths[45];
	GetView()->Invalidate(FALSE);
}


void CPose2ResultDialog::OnBnClickedPose2ShowPath47()
{
	// TODO: Add your control notification handler code here
	theSizer.m_bShowPaths[46] = !theSizer.m_bShowPaths[46];
	GetView()->Invalidate(FALSE);
}


void CPose2ResultDialog::OnBnClickedPose2ShowPath48()
{
	// TODO: Add your control notification handler code here
	theSizer.m_bShowPaths[47] = !theSizer.m_bShowPaths[47];
	GetView()->Invalidate(FALSE);
}


void CPose2ResultDialog::OnBnClickedPose2ShowPath49()
{
	// TODO: Add your control notification handler code here
	theSizer.m_bShowPaths[48] = !theSizer.m_bShowPaths[48];
	GetView()->Invalidate(FALSE);
}


void CPose2ResultDialog::OnBnClickedPose2ShowPath50()
{
	// TODO: Add your control notification handler code here
	theSizer.m_bShowPaths[49] = !theSizer.m_bShowPaths[49];
	GetView()->Invalidate(FALSE);
}


void CPose2ResultDialog::OnBnClickedPose2ShowPath51()
{
	// TODO: Add your control notification handler code here
	theSizer.m_bShowPaths[50] = !theSizer.m_bShowPaths[50];
	GetView()->Invalidate(FALSE);
}


void CPose2ResultDialog::OnBnClickedPose2ShowPath52()
{
	// TODO: Add your control notification handler code here
	theSizer.m_bShowPaths[51] = !theSizer.m_bShowPaths[51];
	GetView()->Invalidate(FALSE);
}


void CPose2ResultDialog::OnBnClickedPose2ShowPath53()
{
	// TODO: Add your control notification handler code here
	theSizer.m_bShowPaths[52] = !theSizer.m_bShowPaths[52];
	GetView()->Invalidate(FALSE);
}


void CPose2ResultDialog::OnBnClickedPose2ShowPath54()
{
	// TODO: Add your control notification handler code here
	theSizer.m_bShowPaths[53] = !theSizer.m_bShowPaths[53];
	GetView()->Invalidate(FALSE);
}


void CPose2ResultDialog::OnBnClickedPose2ShowPath55()
{
	// TODO: Add your control notification handler code here
	theSizer.m_bShowPaths[54] = !theSizer.m_bShowPaths[54];
	GetView()->Invalidate(FALSE);
}


void CPose2ResultDialog::OnBnClickedPose2ShowPath56()
{
	// TODO: Add your control notification handler code here
	theSizer.m_bShowPaths[55] = !theSizer.m_bShowPaths[55];
	GetView()->Invalidate(FALSE);
}


void CPose2ResultDialog::OnBnClickedPose2ShowPath57()
{
	// TODO: Add your control notification handler code here
	theSizer.m_bShowPaths[56] = !theSizer.m_bShowPaths[56];
	GetView()->Invalidate(FALSE);
}


void CPose2ResultDialog::OnBnClickedPose2ShowPath58()
{
	// TODO: Add your control notification handler code here
	theSizer.m_bShowPaths[57] = !theSizer.m_bShowPaths[57];
	GetView()->Invalidate(FALSE);
}


void CPose2ResultDialog::OnBnClickedPose2ShowPath59()
{
	// TODO: Add your control notification handler code here
	theSizer.m_bShowPaths[58] = !theSizer.m_bShowPaths[58];
	GetView()->Invalidate(FALSE);
}


void CPose2ResultDialog::OnBnClickedPose2ShowPath60()
{
	// TODO: Add your control notification handler code here
	theSizer.m_bShowPaths[59] = !theSizer.m_bShowPaths[59];
	GetView()->Invalidate(FALSE);
}


void CPose2ResultDialog::OnBnClickedPose2ShowPath61()
{
	// TODO: Add your control notification handler code here
	theSizer.m_bShowPaths[60] = !theSizer.m_bShowPaths[60];
	GetView()->Invalidate(FALSE);
}


void CPose2ResultDialog::OnBnClickedPose2ShowPath62()
{
	// TODO: Add your control notification handler code here
	theSizer.m_bShowPaths[61] = !theSizer.m_bShowPaths[61];
	GetView()->Invalidate(FALSE);
}


void CPose2ResultDialog::OnBnClickedPose2ShowPath63()
{
	// TODO: Add your control notification handler code here
	theSizer.m_bShowPaths[62] = !theSizer.m_bShowPaths[62];
	GetView()->Invalidate(FALSE);
}


void CPose2ResultDialog::OnBnClickedPose2ShowPath64()
{
	// TODO: Add your control notification handler code here
	theSizer.m_bShowPaths[63] = !theSizer.m_bShowPaths[63];
	GetView()->Invalidate(FALSE);
}


void CPose2ResultDialog::OnBnClickedPose2ShowPath65()
{
	// TODO: Add your control notification handler code here
	theSizer.m_bShowPaths[64] = !theSizer.m_bShowPaths[64];
	GetView()->Invalidate(FALSE);
}


void CPose2ResultDialog::OnBnClickedPose2ShowPath66()
{
	// TODO: Add your control notification handler code here
	theSizer.m_bShowPaths[65] = !theSizer.m_bShowPaths[65];
	GetView()->Invalidate(FALSE);
}


void CPose2ResultDialog::OnBnClickedPose2ShowPath67()
{
	// TODO: Add your control notification handler code here
	theSizer.m_bShowPaths[66] = !theSizer.m_bShowPaths[66];
	GetView()->Invalidate(FALSE);
}


void CPose2ResultDialog::OnBnClickedPose2ShowPath68()
{
	// TODO: Add your control notification handler code here
	theSizer.m_bShowPaths[67] = !theSizer.m_bShowPaths[67];
	GetView()->Invalidate(FALSE);
}


void CPose2ResultDialog::OnBnClickedPose2ShowPath69()
{
	// TODO: Add your control notification handler code here
	theSizer.m_bShowPaths[68] = !theSizer.m_bShowPaths[68];
	GetView()->Invalidate(FALSE);
}


void CPose2ResultDialog::OnBnClickedPose2ShowPath70()
{
	// TODO: Add your control notification handler code here
	theSizer.m_bShowPaths[69] = !theSizer.m_bShowPaths[69];
	GetView()->Invalidate(FALSE);
}


void CPose2ResultDialog::OnBnClickedPose2ShowPath71()
{
	// TODO: Add your control notification handler code here
	theSizer.m_bShowPaths[70] = !theSizer.m_bShowPaths[70];
	GetView()->Invalidate(FALSE);
}


void CPose2ResultDialog::OnBnClickedPose2ShowPath72()
{
	// TODO: Add your control notification handler code here
	theSizer.m_bShowPaths[71] = !theSizer.m_bShowPaths[71];
	GetView()->Invalidate(FALSE);
}


void CPose2ResultDialog::OnBnClickedPose2ShowPath73()
{
	// TODO: Add your control notification handler code here
	theSizer.m_bShowPaths[72] = !theSizer.m_bShowPaths[72];
	GetView()->Invalidate(FALSE);
}


void CPose2ResultDialog::OnBnClickedPose2ShowPath74()
{
	// TODO: Add your control notification handler code here
	theSizer.m_bShowPaths[73] = !theSizer.m_bShowPaths[73];
	GetView()->Invalidate(FALSE);
}


void CPose2ResultDialog::OnBnClickedPose2ShowPath75()
{
	// TODO: Add your control notification handler code here
	theSizer.m_bShowPaths[74] = !theSizer.m_bShowPaths[74];
	GetView()->Invalidate(FALSE);
}


void CPose2ResultDialog::OnBnClickedPose2ShowPath76()
{
	// TODO: Add your control notification handler code here
	theSizer.m_bShowPaths[75] = !theSizer.m_bShowPaths[75];
	GetView()->Invalidate(FALSE);
}


void CPose2ResultDialog::OnBnClickedPose2ShowPath77()
{
	// TODO: Add your control notification handler code here
	theSizer.m_bShowPaths[76] = !theSizer.m_bShowPaths[76];
	GetView()->Invalidate(FALSE);
}


void CPose2ResultDialog::OnBnClickedPose2ShowPath78()
{
	// TODO: Add your control notification handler code here
	theSizer.m_bShowPaths[77] = !theSizer.m_bShowPaths[77];
	GetView()->Invalidate(FALSE);
}


void CPose2ResultDialog::OnBnClickedPose2ShowPath79()
{
	// TODO: Add your control notification handler code here
	theSizer.m_bShowPaths[78] = !theSizer.m_bShowPaths[78];
	GetView()->Invalidate(FALSE);
}


void CPose2ResultDialog::OnBnClickedPose2ShowPathAll()
{
	// TODO: Add your control notification handler code here
	m_bPose2ShowPath1 = true;
	m_bPose2ShowPath2 = true;
	m_bPose2ShowPath3 = true;
	m_bPose2ShowPath4 = true;
	m_bPose2ShowPath5 = true;
	m_bPose2ShowPath6 = true;
	m_bPose2ShowPath7 = true;
	m_bPose2ShowPath8 = true;
	m_bPose2ShowPath9 = true;
	m_bPose2ShowPath10 = true;
	m_bPose2ShowPath11 = true;
	m_bPose2ShowPath12 = true;
	m_bPose2ShowPath13 = true;
	m_bPose2ShowPath14 = true;
	m_bPose2ShowPath15 = true;
	m_bPose2ShowPath16 = true;
	m_bPose2ShowPath17 = true;
	m_bPose2ShowPath18 = true;
	m_bPose2ShowPath19 = true;
	m_bPose2ShowPath20 = true;
	m_bPose2ShowPath21 = true;
	m_bPose2ShowPath22 = true;
	m_bPose2ShowPath23 = true;
	m_bPose2ShowPath24 = true;
	m_bPose2ShowPath25 = true;
	m_bPose2ShowPath26 = true;
	m_bPose2ShowPath27 = true;
	m_bPose2ShowPath28 = true;
	m_bPose2ShowPath29 = true;
	m_bPose2ShowPath30 = true;
	m_bPose2ShowPath31 = true;
	m_bPose2ShowPath32 = true;
	m_bPose2ShowPath33 = true;
	m_bPose2ShowPath34 = true;
	m_bPose2ShowPath35 = true;
	m_bPose2ShowPath36 = true;
	m_bPose2ShowPath37 = true;
	m_bPose2ShowPath38 = true;
	m_bPose2ShowPath39 = true;
	m_bPose2ShowPath40 = true;
	m_bPose2ShowPath41 = true;
	m_bPose2ShowPath42 = true;
	m_bPose2ShowPath43 = true;
	m_bPose2ShowPath44 = true;
	m_bPose2ShowPath45 = true;
	m_bPose2ShowPath46 = true;
	m_bPose2ShowPath47 = true;
	m_bPose2ShowPath48 = true;
	m_bPose2ShowPath49 = true;
	m_bPose2ShowPath50 = true;
	m_bPose2ShowPath51 = true;
	m_bPose2ShowPath52 = true;
	m_bPose2ShowPath53 = true;
	m_bPose2ShowPath54 = true;
	m_bPose2ShowPath55 = true;
	m_bPose2ShowPath56 = true;
	m_bPose2ShowPath57 = true;
	m_bPose2ShowPath58 = true;
	m_bPose2ShowPath59 = true;
	m_bPose2ShowPath60 = true;
	m_bPose2ShowPath61 = true;
	m_bPose2ShowPath62 = true;
	m_bPose2ShowPath63 = true;
	m_bPose2ShowPath64 = true;
	m_bPose2ShowPath65 = true;
	m_bPose2ShowPath66 = true;
	m_bPose2ShowPath67 = true;
	m_bPose2ShowPath68 = true;
	m_bPose2ShowPath69 = true;
	m_bPose2ShowPath70 = true;
	m_bPose2ShowPath71 = true;
	m_bPose2ShowPath72 = true;
	m_bPose2ShowPath73 = true;
	m_bPose2ShowPath74 = true;
	m_bPose2ShowPath75 = true;
	m_bPose2ShowPath76 = true;
	m_bPose2ShowPath77 = true;
	m_bPose2ShowPath78 = true;
	m_bPose2ShowPath79 = true;

	for (int i = 0; i < NUM(theSizer.m_bShowPaths); ++i)
		theSizer.m_bShowPaths[i] = true;

	UpdateData(FALSE);
	GetView()->Invalidate(FALSE);
}


void CPose2ResultDialog::OnBnClickedPose2ShowPathNone()
{
	// TODO: Add your control notification handler code here
	m_bPose2ShowPath1 = false;
	m_bPose2ShowPath2 = false;
	m_bPose2ShowPath3 = false;
	m_bPose2ShowPath4 = false;
	m_bPose2ShowPath5 = false;
	m_bPose2ShowPath6 = false;
	m_bPose2ShowPath7 = false;
	m_bPose2ShowPath8 = false;
	m_bPose2ShowPath9 = false;
	m_bPose2ShowPath10 = false;
	m_bPose2ShowPath11 = false;
	m_bPose2ShowPath12 = false;
	m_bPose2ShowPath13 = false;
	m_bPose2ShowPath14 = false;
	m_bPose2ShowPath15 = false;
	m_bPose2ShowPath16 = false;
	m_bPose2ShowPath17 = false;
	m_bPose2ShowPath18 = false;
	m_bPose2ShowPath19 = false;
	m_bPose2ShowPath20 = false;
	m_bPose2ShowPath21 = false;
	m_bPose2ShowPath22 = false;
	m_bPose2ShowPath23 = false;
	m_bPose2ShowPath24 = false;
	m_bPose2ShowPath25 = false;
	m_bPose2ShowPath26 = false;
	m_bPose2ShowPath27 = false;
	m_bPose2ShowPath28 = false;
	m_bPose2ShowPath29 = false;
	m_bPose2ShowPath30 = false;
	m_bPose2ShowPath31 = false;
	m_bPose2ShowPath32 = false;
	m_bPose2ShowPath33 = false;
	m_bPose2ShowPath34 = false;
	m_bPose2ShowPath35 = false;
	m_bPose2ShowPath36 = false;
	m_bPose2ShowPath37 = false;
	m_bPose2ShowPath38 = false;
	m_bPose2ShowPath39 = false;
	m_bPose2ShowPath40 = false;
	m_bPose2ShowPath41 = false;
	m_bPose2ShowPath42 = false;
	m_bPose2ShowPath43 = false;
	m_bPose2ShowPath44 = false;
	m_bPose2ShowPath45 = false;
	m_bPose2ShowPath46 = false;
	m_bPose2ShowPath47 = false;
	m_bPose2ShowPath48 = false;
	m_bPose2ShowPath49 = false;
	m_bPose2ShowPath50 = false;
	m_bPose2ShowPath51 = false;
	m_bPose2ShowPath52 = false;
	m_bPose2ShowPath53 = false;
	m_bPose2ShowPath54 = false;
	m_bPose2ShowPath55 = false;
	m_bPose2ShowPath56 = false;
	m_bPose2ShowPath57 = false;
	m_bPose2ShowPath58 = false;
	m_bPose2ShowPath59 = false;
	m_bPose2ShowPath60 = false;
	m_bPose2ShowPath61 = false;
	m_bPose2ShowPath62 = false;
	m_bPose2ShowPath63 = false;
	m_bPose2ShowPath64 = false;
	m_bPose2ShowPath65 = false;
	m_bPose2ShowPath66 = false;
	m_bPose2ShowPath67 = false;
	m_bPose2ShowPath68 = false;
	m_bPose2ShowPath69 = false;
	m_bPose2ShowPath70 = false;
	m_bPose2ShowPath71 = false;
	m_bPose2ShowPath72 = false;
	m_bPose2ShowPath73 = false;
	m_bPose2ShowPath74 = false;
	m_bPose2ShowPath75 = false;
	m_bPose2ShowPath76 = false;
	m_bPose2ShowPath77 = false;
	m_bPose2ShowPath78 = false;
	m_bPose2ShowPath79 = false;

	for (int i = 0; i < NUM(theSizer.m_bShowPaths); ++i)
		theSizer.m_bShowPaths[i] = false;

	UpdateData(FALSE);
	GetView()->Invalidate(FALSE);
}


void CPose2ResultDialog::OnBnClickedPose2ExportResult()
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
	_cprintf("%d %d\n", NumResults, ResultPose2.size());
	for (int i = 0; i < NumResults; i++)
		fprintf(fp, "%s,%.1lf\n", ResultPose2[i].c_str(), theSizer.m_Results[i]);
	fclose(fp);
}
