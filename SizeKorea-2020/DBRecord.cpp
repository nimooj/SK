// DBRecord.cpp : implementation file
//

#include "pch.h"
#include "SizeKorea-2020.h"
#include "DBRecord.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDBRecord

IMPLEMENT_DYNAMIC(CDBRecord, CRecordset)

CDBRecord::CDBRecord(CDatabase* pdb)
	: CRecordset(pdb)
{
	//{{AFX_FIELD_INIT(CDBRecord)
	int i;	
	m_id = 0;
	m_column1 = 0;
	m_column2 = _T("");
	m_column3 = _T("");
	m_column4 = _T("");
	m_column5 = _T("");
	m_column6 = _T("");
	m_column7 = _T("");
	m_column8 = 0;
	m_column9 = 0;
	m_column10 = 0;
	m_column11 = 0;
	m_column12 = 0;
	m_column13 = 0;
	m_column14 = 0;
	m_column15 = 0;
	m_file1 = _T("");
	m_file2 = _T("");
	m_file3 = _T("");
	m_file4 = _T("");
	m_file5 = _T("");
	m_file6 = _T("");
	for (i = 0; i < 19; ++i) {
		m_human_size[i] = 0;
	}
	m_RI = 0.0;
	m_BMI = 0.0;
	for (i = 0; i < 63; ++i) {
		m_pose1_size[i] = 0;
	}
	for (i = 0; i < 39; ++i) {
		m_pose2_size[i] = 0;
	}
	for (i = 0; i < 34; ++i) {
		m_pose3_size[i] = 0;
	}
	for (i = 0; i < 9; ++i) {
		m_pose4_size[i] = 0;
	}
	for (i = 0; i < 50; ++i) {
		m_answer[i] = 0;
	}
	m_nFields = 238;

	//}}AFX_FIELD_INIT
	m_nDefaultType = snapshot;
}

void CDBRecord::InitData() {
	int i;	
	m_id = 0;
	m_column1 = 0;
	m_column2 = _T("");
	m_column3 = _T("");
	m_column4 = _T("");
	m_column5 = _T("");
	m_column6 = _T("");
	m_column7 = _T("");
	m_column8 = 2;
	m_column9 = 0;
	m_column10 = 3;
	m_column11 = 0;
	m_column12 = 2;
	m_column13 = 0;
	m_column14 = 0;
	m_column15 = 0;
	m_file1 = _T("");
	m_file2 = _T("");
	m_file3 = _T("");
	m_file4 = _T("");
	m_file5 = _T("");
	m_file6 = _T("");
	for (i = 0; i < 19; ++i) {
		m_human_size[i] = 0;
	}
	m_RI = 0.0;
	m_BMI = 0.0;
	for (i = 0; i < 63; ++i) {
		m_pose1_size[i] = 0;
	}
	for (i = 0; i < 39; ++i) {
		m_pose2_size[i] = 0;
	}
	for (i = 0; i < 34; ++i) {
		m_pose3_size[i] = 0;
	}
	for (i = 0; i < 9; ++i) {
		m_pose4_size[i] = 0;
	}
	for (i = 0; i < 50; ++i) {
		m_answer[i] = 0;
	}
}

CString CDBRecord::GetDefaultConnect()
{
	return _T("ODBC;DSN=HumanData");
}

CString CDBRecord::GetDefaultSQL()
{
	return _T("[HumanData]");
}

void CDBRecord::DoFieldExchange(CFieldExchange* pFX)
{
	//{{AFX_FIELD_MAP(CDBRecord)
	pFX->SetFieldType(CFieldExchange::outputColumn);
	RFX_Long(pFX, _T("[ID]"), m_id);
	RFX_Long(pFX, _T("[번호]"), m_column1);
	RFX_Text(pFX, _T("[측정년월일시각]"), m_column2);
	RFX_Text(pFX, _T("[주민등록번호]"), m_column3);
	RFX_Text(pFX, _T("[측정장소]"), m_column4);
	RFX_Text(pFX, _T("[출생지]"), m_column5);
	RFX_Text(pFX, _T("[주거주지]"), m_column6);
	RFX_Text(pFX, _T("[현거주지]"), m_column7);
	RFX_Byte(pFX, _T("[주로쓰는손]"), m_column8);
	RFX_Byte(pFX, _T("[초경연령]"), m_column9);
	RFX_Byte(pFX, _T("[결혼여부]"), m_column10);
	RFX_Byte(pFX, _T("[출산자녀수]"), m_column11);	
	RFX_Byte(pFX, _T("[안경착용여부]"), m_column12);
	RFX_Int(pFX, _T("[신발치수]"), m_column13);
	RFX_Byte(pFX, _T("[학력]"), m_column14);
	RFX_Byte(pFX, _T("[직업]"), m_column15);
	RFX_Int(pFX, _T("[키]"), m_human_size[0]);
	RFX_Int(pFX, _T("[목뒤높이]"), m_human_size[1]);
	RFX_Int(pFX, _T("[샅높이]"), m_human_size[2]);
	RFX_Int(pFX, _T("[목밑둘레]"), m_human_size[3]);
	RFX_Int(pFX, _T("[가슴둘레]"), m_human_size[4]);
	RFX_Int(pFX, _T("[젖가슴둘레]"), m_human_size[5]);
	RFX_Int(pFX, _T("[젖가슴아래둘레]"), m_human_size[6]);
	RFX_Int(pFX, _T("[허리둘레]"), m_human_size[7]);
	RFX_Int(pFX, _T("[배둘레]"), m_human_size[8]);
	RFX_Int(pFX, _T("[엉덩이둘레]"), m_human_size[9]);
	RFX_Int(pFX, _T("[겨드랑둘레]"), m_human_size[10]);
	RFX_Int(pFX, _T("[어깨가쪽사이길이]"), m_human_size[11]);
	RFX_Int(pFX, _T("[팔길이]"), m_human_size[12]);
	RFX_Int(pFX, _T("[등길이]"), m_human_size[13]);
	RFX_Int(pFX, _T("[귀구슬사이머리위길이]"), m_human_size[14]);
	RFX_Int(pFX, _T("[귀구슬사이너비]"), m_human_size[15]);
	RFX_Int(pFX, _T("[귀바퀴뿌리귀바퀴뿌리뒤직선길이]"), m_human_size[16]);
	RFX_Int(pFX, _T("[발직선길이]"), m_human_size[17]);
	RFX_Int(pFX, _T("[몸무게]"), m_human_size[18]);
	RFX_Double(pFX, _T("[RI]"), m_RI);
	RFX_Double(pFX, _T("[BMI]"), m_BMI);

	RFX_Text(pFX, _T("[Filename1]"), m_file1);
	RFX_Text(pFX, _T("[Filename2]"), m_file2);
	RFX_Text(pFX, _T("[Filename3]"), m_file3);
	RFX_Text(pFX, _T("[Filename4]"), m_file4);
	RFX_Text(pFX, _T("[Filename5]"), m_file5);
	RFX_Text(pFX, _T("[Filename6]"), m_file6);
	RFX_Int(pFX, _T("[목둘레1]"), m_pose1_size[0]);
	RFX_Int(pFX, _T("[목뒤둘레1]"), m_pose1_size[1]);
	RFX_Int(pFX, _T("[목밑둘레1]"), m_pose1_size[2]);
	RFX_Int(pFX, _T("[겨드랑둘레1]"), m_pose1_size[3]);
	RFX_Int(pFX, _T("[가슴둘레1]"), m_pose1_size[4]);
	RFX_Int(pFX, _T("[젖가슴둘레1]"), m_pose1_size[5]);
	RFX_Int(pFX, _T("[젖가슴아래둘레1]"), m_pose1_size[6]);
	RFX_Int(pFX, _T("[허리둘레1]"), m_pose1_size[7]);
	RFX_Int(pFX, _T("[배꼽수준허리둘레1]"), m_pose1_size[8]);
	RFX_Int(pFX, _T("[배둘레1]"), m_pose1_size[9]);
	RFX_Int(pFX, _T("[넙다리둘레1]"), m_pose1_size[10]);
	RFX_Int(pFX, _T("[넙다리중간둘레1]"), m_pose1_size[11]);
	RFX_Int(pFX, _T("[무릎둘레1]"), m_pose1_size[12]);
	RFX_Int(pFX, _T("[무릎아래둘레1]"), m_pose1_size[13]);
	RFX_Int(pFX, _T("[장딴지둘레1]"), m_pose1_size[14]);
	RFX_Int(pFX, _T("[종아리최소둘레1]"), m_pose1_size[15]);
	RFX_Int(pFX, _T("[발둘레1]"), m_pose1_size[16]);
	RFX_Int(pFX, _T("[발목최대둘레1]"), m_pose1_size[17]);
	RFX_Int(pFX, _T("[편위팔둘레1]"), m_pose1_size[18]);
	RFX_Int(pFX, _T("[편팔꿈치둘레1]"), m_pose1_size[19]);
	RFX_Int(pFX, _T("[아래팔둘레1]"), m_pose1_size[20]);
	RFX_Int(pFX, _T("[손목둘레1]"), m_pose1_size[21]);
	RFX_Int(pFX, _T("[몸통세로둘레1]"), m_pose1_size[22]);
	RFX_Int(pFX, _T("[앞중심길이1]"), m_pose1_size[23]);
	RFX_Int(pFX, _T("[배꼽수준앞중심길이1]"), m_pose1_size[24]);
	RFX_Int(pFX, _T("[목옆젖꼭지길이1]"), m_pose1_size[25]);
	RFX_Int(pFX, _T("[목옆허리둘레선길이1]"), m_pose1_size[26]);
	RFX_Int(pFX, _T("[목뒤젖꼭지허리둘레선길이1]"), m_pose1_size[27]);
	RFX_Int(pFX, _T("[목뒤젖꼭지길이1]"), m_pose1_size[28]);
	RFX_Int(pFX, _T("[목뒤등뼈위겨드랑수준길이1]"), m_pose1_size[29]);
	RFX_Int(pFX, _T("[목옆뒤허리둘레선길이1]"), m_pose1_size[30]);
	RFX_Int(pFX, _T("[젖꼭지허리둘레선길이1]"), m_pose1_size[31]);
	RFX_Int(pFX, _T("[등길이1]"), m_pose1_size[32]);
	RFX_Int(pFX, _T("[배꼽수준등길이1]"), m_pose1_size[33]);
	RFX_Int(pFX, _T("[샅앞뒤길이1]"), m_pose1_size[34]);
	RFX_Int(pFX, _T("[넙다리길이1]"), m_pose1_size[35]);
	RFX_Int(pFX, _T("[발직선길이1]"), m_pose1_size[36]);
	RFX_Int(pFX, _T("[팔길이1]"), m_pose1_size[37]);
	RFX_Int(pFX, _T("[다리벌린샅높이1]"), m_pose1_size[38]);
	RFX_Int(pFX, _T("[팔안쪽길이1]"), m_pose1_size[39]);
	RFX_Int(pFX, _T("[위팔길이1]"), m_pose1_size[40]);
	RFX_Int(pFX, _T("[아래팔길이1]"), m_pose1_size[41]);
	RFX_Int(pFX, _T("[목밑너비1]"), m_pose1_size[42]);
	RFX_Int(pFX, _T("[가슴너비1]"), m_pose1_size[43]);
	RFX_Int(pFX, _T("[젖가슴너비1]"), m_pose1_size[44]);
	RFX_Int(pFX, _T("[젖가슴아래너비1]"), m_pose1_size[45]);
	RFX_Int(pFX, _T("[허리너비1]"), m_pose1_size[46]);
	RFX_Int(pFX, _T("[배꼽수준허리너비1]"), m_pose1_size[47]);
	RFX_Int(pFX, _T("[배너비1]"), m_pose1_size[48]);
	RFX_Int(pFX, _T("[발너비1]"), m_pose1_size[49]);
	RFX_Int(pFX, _T("[젖꼭지사이수평길이1]"), m_pose1_size[50]);
	RFX_Int(pFX, _T("[겨드랑두께1]"), m_pose1_size[51]);
	RFX_Int(pFX, _T("[가슴두께1]"), m_pose1_size[52]);
	RFX_Int(pFX, _T("[젖가슴두께1]"), m_pose1_size[53]);
	RFX_Int(pFX, _T("[젖가슴아래두께1]"), m_pose1_size[54]);
	RFX_Int(pFX, _T("[허리두께1]"), m_pose1_size[55]);
	RFX_Int(pFX, _T("[배꼽수준허리두께1]"), m_pose1_size[56]);
	RFX_Int(pFX, _T("[배두께1]"), m_pose1_size[57]);
	RFX_Int(pFX, _T("[엉덩이두께1]"), m_pose1_size[58]);
	RFX_Int(pFX, _T("[벽면몸통두께1]"), m_pose1_size[59]);
	RFX_Int(pFX, _T("[젖가슴아래둘래F1]"), m_pose1_size[60]);
	RFX_Int(pFX, _T("[목뒤손목안쪽길이1]"), m_pose1_size[61]);
	RFX_Int(pFX, _T("[겨드랑둘레구1]"), m_pose1_size[62]);

	RFX_Int(pFX, _T("[키2]"), m_pose2_size[0]);
	RFX_Int(pFX, _T("[눈높이2]"), m_pose2_size[1]);
	RFX_Int(pFX, _T("[어깨높이2]"), m_pose2_size[2]);
	RFX_Int(pFX, _T("[어깨가쪽높이2]"), m_pose2_size[3]);
	RFX_Int(pFX, _T("[목뒤높이2]"), m_pose2_size[4]);
	RFX_Int(pFX, _T("[목앞높이2]"), m_pose2_size[5]);
	RFX_Int(pFX, _T("[겨드랑높이2]"), m_pose2_size[6]);
	RFX_Int(pFX, _T("[젖가슴높이2]"), m_pose2_size[7]);
	RFX_Int(pFX, _T("[젖가슴아래높이2]"), m_pose2_size[8]);
	RFX_Int(pFX, _T("[허리높이2]"), m_pose2_size[9]);
	RFX_Int(pFX, _T("[배꼽수준허리높이2]"), m_pose2_size[10]);
	RFX_Int(pFX, _T("[배높이2]"), m_pose2_size[11]);
	RFX_Int(pFX, _T("[위앞엉덩뼈가시높이2]"), m_pose2_size[12]);
	RFX_Int(pFX, _T("[엉덩뼈능선높이2]"), m_pose2_size[13]);
	RFX_Int(pFX, _T("[엉덩이높이2]"), m_pose2_size[14]);
	RFX_Int(pFX, _T("[볼기고랑높이2]"), m_pose2_size[15]);
	RFX_Int(pFX, _T("[넙다리높이2]"), m_pose2_size[16]);
	RFX_Int(pFX, _T("[샅높이2]"), m_pose2_size[17]);
	RFX_Int(pFX, _T("[장딴지높이2]"), m_pose2_size[18]);
	RFX_Int(pFX, _T("[엉덩이수직길이2]"), m_pose2_size[19]);
	RFX_Int(pFX, _T("[몸통수직길이2]"), m_pose2_size[20]);
	RFX_Int(pFX, _T("[무릎높이2]"), m_pose2_size[21]);
	RFX_Int(pFX, _T("[무릎뼈가운데높이2]"), m_pose2_size[22]);
	RFX_Int(pFX, _T("[가쪽복사높이2]"), m_pose2_size[23]);
	RFX_Int(pFX, _T("[굽힌팔꿈치높이2]"), m_pose2_size[24]);
	RFX_Int(pFX, _T("[손끝높이2]"), m_pose2_size[25]);
	RFX_Int(pFX, _T("[손쥔끝높이2]"), m_pose2_size[26]);
	RFX_Int(pFX, _T("[엉덩이둘레2]"), m_pose2_size[27]);
	RFX_Int(pFX, _T("[목뒤오금길이2]"), m_pose2_size[28]);
	RFX_Int(pFX, _T("[총길이2]"), m_pose2_size[29]);
	RFX_Int(pFX, _T("[엉덩이옆길이2]"), m_pose2_size[30]);
	RFX_Int(pFX, _T("[허리옆가쪽복사길이2]"), m_pose2_size[31]);
	RFX_Int(pFX, _T("[다리가쪽길이2]"), m_pose2_size[32]);
	RFX_Int(pFX, _T("[굽힌팔길이2]"), m_pose2_size[33]);
	RFX_Int(pFX, _T("[팔굽힌목뒤손목안쪽길이2]"), m_pose2_size[34]);
	RFX_Int(pFX, _T("[벽면어깨수평길이2]"), m_pose2_size[35]);
	RFX_Int(pFX, _T("[엉덩이너비2]"), m_pose2_size[36]);
	RFX_Int(pFX, _T("[손너비2]"), m_pose2_size[37]);
	RFX_Int(pFX, _T("[벽면손끝수평길이2]"), m_pose2_size[38]);
	RFX_Int(pFX, _T("[앉은키3]"), m_pose3_size[0]);
	RFX_Int(pFX, _T("[앉은눈높이3]"), m_pose3_size[1]);
	RFX_Int(pFX, _T("[앉은목뒤높이3]"), m_pose3_size[2]);
	RFX_Int(pFX, _T("[앉은어깨높이3]"), m_pose3_size[3]);
	RFX_Int(pFX, _T("[앉은팔꿈치높이3]"), m_pose3_size[4]);
	RFX_Int(pFX, _T("[앉은무릎높이3]"), m_pose3_size[5]);
	RFX_Int(pFX, _T("[앉은오금높이3]"), m_pose3_size[6]);
	RFX_Int(pFX, _T("[어깨길이3]"), m_pose3_size[7]);
	RFX_Int(pFX, _T("[목뒤어깨가쪽길이3]"), m_pose3_size[8]);
	RFX_Int(pFX, _T("[어깨가쪽사이길이3]"), m_pose3_size[9]);
	RFX_Int(pFX, _T("[겨드랑앞벽사이길이앞품3]"), m_pose3_size[10]);
	RFX_Int(pFX, _T("[겨드랑뒤벽사이길이뒤품3]"), m_pose3_size[11]);
	RFX_Int(pFX, _T("[겨드랑앞접힘사이길이3]"), m_pose3_size[12]);
	RFX_Int(pFX, _T("[겨드랑뒤접힘사이길이3]"), m_pose3_size[13]);
	RFX_Int(pFX, _T("[위팔수직길이3]"), m_pose3_size[14]);
	RFX_Int(pFX, _T("[아래팔수평길이3]"), m_pose3_size[15]);
	RFX_Int(pFX, _T("[팔꿈치손목수평길이3]"), m_pose3_size[16]);
	RFX_Int(pFX, _T("[앉은엉덩이오금수평길이3]"), m_pose3_size[17]);
	RFX_Int(pFX, _T("[앉은엉덩이무릎수평길이3]"), m_pose3_size[18]);
	RFX_Int(pFX, _T("[어깨너비3]"), m_pose3_size[19]);
	RFX_Int(pFX, _T("[위팔사이너비3]"), m_pose3_size[20]);
	RFX_Int(pFX, _T("[팔꿈치사이너비3]"), m_pose3_size[21]);
	RFX_Int(pFX, _T("[앉은엉덩이너비3]"), m_pose3_size[22]);
	RFX_Int(pFX, _T("[앉은넙다리높이3]"), m_pose3_size[23]);
	RFX_Int(pFX, _T("[앉은배두께3]"), m_pose3_size[24]);
	RFX_Int(pFX, _T("[앉은엉덩이배두께3]"), m_pose3_size[25]);
	RFX_Int(pFX, _T("[오른어깨기울기3]"), m_pose3_size[26]);
	RFX_Int(pFX, _T("[왼쪽어깨기울기3]"), m_pose3_size[27]);
	RFX_Int(pFX, _T("[어깨가쪽사이수평길이3]"), m_pose3_size[28]);
	RFX_Int(pFX, _T("[겨드랑둘레F3]"), m_pose3_size[29]);
	RFX_Int(pFX, _T("[가슴둘레F3]"), m_pose3_size[30]);
	RFX_Int(pFX, _T("[가슴너비F3]"), m_pose3_size[31]);
	RFX_Int(pFX, _T("[가슴두께F3]"), m_pose3_size[32]);
	RFX_Int(pFX, _T("[어깨사이길이3]"), m_pose3_size[33]);

	RFX_Int(pFX, _T("[머리둘레4]"), m_pose4_size[0]);
	RFX_Int(pFX, _T("[눈살머리마루뒤통수4]"), m_pose4_size[1]);
	RFX_Int(pFX, _T("[귀구슬사이머리마루4]"), m_pose4_size[2]);
	RFX_Int(pFX, _T("[귀구슬사이턱끝4]"), m_pose4_size[3]);
	RFX_Int(pFX, _T("[귀구슬사이갑상연골4]"), m_pose4_size[4]);
	RFX_Int(pFX, _T("[귀구슬사이코뿌리4]"), m_pose4_size[5]);
	RFX_Int(pFX, _T("[귀구슬사이코밑4]"), m_pose4_size[6]);
	RFX_Int(pFX, _T("[어깨높이24]"), m_pose4_size[7]);
	RFX_Int(pFX, _T("[어깨가쪽높이24]"), m_pose4_size[8]);

	RFX_Byte(pFX, _T("[answer1]"), m_answer[0]);
	RFX_Byte(pFX, _T("[answer2]"), m_answer[1]);
	RFX_Byte(pFX, _T("[answer3]"), m_answer[2]);
	RFX_Byte(pFX, _T("[answer4]"), m_answer[3]);
	RFX_Byte(pFX, _T("[answer5]"), m_answer[4]);
	RFX_Byte(pFX, _T("[answer6]"), m_answer[5]);
	RFX_Byte(pFX, _T("[answer7]"), m_answer[6]);
	RFX_Byte(pFX, _T("[answer8]"), m_answer[7]);
	RFX_Byte(pFX, _T("[answer9]"), m_answer[8]);
	RFX_Byte(pFX, _T("[answer10]"), m_answer[9]);
	RFX_Byte(pFX, _T("[answer11]"), m_answer[10]);
	RFX_Byte(pFX, _T("[answer12]"), m_answer[11]);
	RFX_Byte(pFX, _T("[answer13]"), m_answer[12]);
	RFX_Byte(pFX, _T("[answer14]"), m_answer[13]);
	RFX_Byte(pFX, _T("[answer15]"), m_answer[14]);
	RFX_Byte(pFX, _T("[answer16]"), m_answer[15]);
	RFX_Byte(pFX, _T("[answer17]"), m_answer[16]);
	RFX_Byte(pFX, _T("[answer18]"), m_answer[17]);
	RFX_Byte(pFX, _T("[answer19]"), m_answer[18]);
	RFX_Byte(pFX, _T("[answer20]"), m_answer[19]);
	RFX_Byte(pFX, _T("[answer21]"), m_answer[20]);
	RFX_Byte(pFX, _T("[answer22]"), m_answer[21]);
	RFX_Byte(pFX, _T("[answer23]"), m_answer[22]);
	RFX_Byte(pFX, _T("[answer24]"), m_answer[23]);
	RFX_Byte(pFX, _T("[answer25]"), m_answer[24]);
	RFX_Byte(pFX, _T("[answer26]"), m_answer[25]);
	RFX_Byte(pFX, _T("[answer27]"), m_answer[26]);
	RFX_Byte(pFX, _T("[answer28]"), m_answer[27]);
	RFX_Byte(pFX, _T("[answer29]"), m_answer[28]);
	RFX_Byte(pFX, _T("[answer30]"), m_answer[29]);
	RFX_Byte(pFX, _T("[answer31]"), m_answer[30]);
	RFX_Byte(pFX, _T("[answer32]"), m_answer[31]);
	RFX_Byte(pFX, _T("[answer33]"), m_answer[32]);
	RFX_Byte(pFX, _T("[answer34]"), m_answer[33]);
	RFX_Byte(pFX, _T("[answer35]"), m_answer[34]);
	RFX_Byte(pFX, _T("[answer36]"), m_answer[35]);
	RFX_Byte(pFX, _T("[answer37]"), m_answer[36]);
	RFX_Byte(pFX, _T("[answer38]"), m_answer[37]);
	RFX_Byte(pFX, _T("[answer39]"), m_answer[38]);
	RFX_Byte(pFX, _T("[answer40]"), m_answer[39]);
	RFX_Byte(pFX, _T("[answer41]"), m_answer[40]);
	RFX_Byte(pFX, _T("[answer42]"), m_answer[41]);
	RFX_Byte(pFX, _T("[answer43]"), m_answer[42]);
	RFX_Byte(pFX, _T("[answer44]"), m_answer[43]);
	RFX_Byte(pFX, _T("[answer45]"), m_answer[44]);
	RFX_Byte(pFX, _T("[answer46]"), m_answer[45]);
	RFX_Byte(pFX, _T("[answer47]"), m_answer[46]);
	RFX_Byte(pFX, _T("[answer48]"), m_answer[47]);
	RFX_Byte(pFX, _T("[answer49]"), m_answer[48]);
	RFX_Byte(pFX, _T("[answer50]"), m_answer[49]);
	
	//}}AFX_FIELD_MAP
}

/////////////////////////////////////////////////////////////////////////////
// CDBRecord diagnostics

#ifdef _DEBUG
void CDBRecord::AssertValid() const
{
	CRecordset::AssertValid();
}

void CDBRecord::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG
