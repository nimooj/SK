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
	RFX_Long(pFX, _T("[��ȣ]"), m_column1);
	RFX_Text(pFX, _T("[��������Ͻð�]"), m_column2);
	RFX_Text(pFX, _T("[�ֹε�Ϲ�ȣ]"), m_column3);
	RFX_Text(pFX, _T("[�������]"), m_column4);
	RFX_Text(pFX, _T("[�����]"), m_column5);
	RFX_Text(pFX, _T("[�ְ�����]"), m_column6);
	RFX_Text(pFX, _T("[��������]"), m_column7);
	RFX_Byte(pFX, _T("[�ַξ��¼�]"), m_column8);
	RFX_Byte(pFX, _T("[�ʰ濬��]"), m_column9);
	RFX_Byte(pFX, _T("[��ȥ����]"), m_column10);
	RFX_Byte(pFX, _T("[����ڳ��]"), m_column11);	
	RFX_Byte(pFX, _T("[�Ȱ����뿩��]"), m_column12);
	RFX_Int(pFX, _T("[�Ź�ġ��]"), m_column13);
	RFX_Byte(pFX, _T("[�з�]"), m_column14);
	RFX_Byte(pFX, _T("[����]"), m_column15);
	RFX_Int(pFX, _T("[Ű]"), m_human_size[0]);
	RFX_Int(pFX, _T("[��ڳ���]"), m_human_size[1]);
	RFX_Int(pFX, _T("[������]"), m_human_size[2]);
	RFX_Int(pFX, _T("[��صѷ�]"), m_human_size[3]);
	RFX_Int(pFX, _T("[�����ѷ�]"), m_human_size[4]);
	RFX_Int(pFX, _T("[�������ѷ�]"), m_human_size[5]);
	RFX_Int(pFX, _T("[�������Ʒ��ѷ�]"), m_human_size[6]);
	RFX_Int(pFX, _T("[�㸮�ѷ�]"), m_human_size[7]);
	RFX_Int(pFX, _T("[��ѷ�]"), m_human_size[8]);
	RFX_Int(pFX, _T("[�����̵ѷ�]"), m_human_size[9]);
	RFX_Int(pFX, _T("[�ܵ���ѷ�]"), m_human_size[10]);
	RFX_Int(pFX, _T("[������ʻ��̱���]"), m_human_size[11]);
	RFX_Int(pFX, _T("[�ȱ���]"), m_human_size[12]);
	RFX_Int(pFX, _T("[�����]"), m_human_size[13]);
	RFX_Int(pFX, _T("[�ͱ������̸Ӹ�������]"), m_human_size[14]);
	RFX_Int(pFX, _T("[�ͱ������̳ʺ�]"), m_human_size[15]);
	RFX_Int(pFX, _T("[�͹����Ѹ��͹����Ѹ�����������]"), m_human_size[16]);
	RFX_Int(pFX, _T("[����������]"), m_human_size[17]);
	RFX_Int(pFX, _T("[������]"), m_human_size[18]);
	RFX_Double(pFX, _T("[RI]"), m_RI);
	RFX_Double(pFX, _T("[BMI]"), m_BMI);

	RFX_Text(pFX, _T("[Filename1]"), m_file1);
	RFX_Text(pFX, _T("[Filename2]"), m_file2);
	RFX_Text(pFX, _T("[Filename3]"), m_file3);
	RFX_Text(pFX, _T("[Filename4]"), m_file4);
	RFX_Text(pFX, _T("[Filename5]"), m_file5);
	RFX_Text(pFX, _T("[Filename6]"), m_file6);
	RFX_Int(pFX, _T("[��ѷ�1]"), m_pose1_size[0]);
	RFX_Int(pFX, _T("[��ڵѷ�1]"), m_pose1_size[1]);
	RFX_Int(pFX, _T("[��صѷ�1]"), m_pose1_size[2]);
	RFX_Int(pFX, _T("[�ܵ���ѷ�1]"), m_pose1_size[3]);
	RFX_Int(pFX, _T("[�����ѷ�1]"), m_pose1_size[4]);
	RFX_Int(pFX, _T("[�������ѷ�1]"), m_pose1_size[5]);
	RFX_Int(pFX, _T("[�������Ʒ��ѷ�1]"), m_pose1_size[6]);
	RFX_Int(pFX, _T("[�㸮�ѷ�1]"), m_pose1_size[7]);
	RFX_Int(pFX, _T("[��ż����㸮�ѷ�1]"), m_pose1_size[8]);
	RFX_Int(pFX, _T("[��ѷ�1]"), m_pose1_size[9]);
	RFX_Int(pFX, _T("[�Ҵٸ��ѷ�1]"), m_pose1_size[10]);
	RFX_Int(pFX, _T("[�Ҵٸ��߰��ѷ�1]"), m_pose1_size[11]);
	RFX_Int(pFX, _T("[�����ѷ�1]"), m_pose1_size[12]);
	RFX_Int(pFX, _T("[�����Ʒ��ѷ�1]"), m_pose1_size[13]);
	RFX_Int(pFX, _T("[������ѷ�1]"), m_pose1_size[14]);
	RFX_Int(pFX, _T("[���Ƹ��ּҵѷ�1]"), m_pose1_size[15]);
	RFX_Int(pFX, _T("[�ߵѷ�1]"), m_pose1_size[16]);
	RFX_Int(pFX, _T("[�߸��ִ�ѷ�1]"), m_pose1_size[17]);
	RFX_Int(pFX, _T("[�����ȵѷ�1]"), m_pose1_size[18]);
	RFX_Int(pFX, _T("[���Ȳ�ġ�ѷ�1]"), m_pose1_size[19]);
	RFX_Int(pFX, _T("[�Ʒ��ȵѷ�1]"), m_pose1_size[20]);
	RFX_Int(pFX, _T("[�ո�ѷ�1]"), m_pose1_size[21]);
	RFX_Int(pFX, _T("[���뼼�εѷ�1]"), m_pose1_size[22]);
	RFX_Int(pFX, _T("[���߽ɱ���1]"), m_pose1_size[23]);
	RFX_Int(pFX, _T("[��ż��ؾ��߽ɱ���1]"), m_pose1_size[24]);
	RFX_Int(pFX, _T("[������������1]"), m_pose1_size[25]);
	RFX_Int(pFX, _T("[���㸮�ѷ�������1]"), m_pose1_size[26]);
	RFX_Int(pFX, _T("[����������㸮�ѷ�������1]"), m_pose1_size[27]);
	RFX_Int(pFX, _T("[�������������1]"), m_pose1_size[28]);
	RFX_Int(pFX, _T("[��ڵ�����ܵ�����ر���1]"), m_pose1_size[29]);
	RFX_Int(pFX, _T("[�񿷵��㸮�ѷ�������1]"), m_pose1_size[30]);
	RFX_Int(pFX, _T("[�������㸮�ѷ�������1]"), m_pose1_size[31]);
	RFX_Int(pFX, _T("[�����1]"), m_pose1_size[32]);
	RFX_Int(pFX, _T("[��ż��ص����1]"), m_pose1_size[33]);
	RFX_Int(pFX, _T("[���յڱ���1]"), m_pose1_size[34]);
	RFX_Int(pFX, _T("[�Ҵٸ�����1]"), m_pose1_size[35]);
	RFX_Int(pFX, _T("[����������1]"), m_pose1_size[36]);
	RFX_Int(pFX, _T("[�ȱ���1]"), m_pose1_size[37]);
	RFX_Int(pFX, _T("[�ٸ�����������1]"), m_pose1_size[38]);
	RFX_Int(pFX, _T("[�Ⱦ��ʱ���1]"), m_pose1_size[39]);
	RFX_Int(pFX, _T("[���ȱ���1]"), m_pose1_size[40]);
	RFX_Int(pFX, _T("[�Ʒ��ȱ���1]"), m_pose1_size[41]);
	RFX_Int(pFX, _T("[��سʺ�1]"), m_pose1_size[42]);
	RFX_Int(pFX, _T("[�����ʺ�1]"), m_pose1_size[43]);
	RFX_Int(pFX, _T("[�������ʺ�1]"), m_pose1_size[44]);
	RFX_Int(pFX, _T("[�������Ʒ��ʺ�1]"), m_pose1_size[45]);
	RFX_Int(pFX, _T("[�㸮�ʺ�1]"), m_pose1_size[46]);
	RFX_Int(pFX, _T("[��ż����㸮�ʺ�1]"), m_pose1_size[47]);
	RFX_Int(pFX, _T("[��ʺ�1]"), m_pose1_size[48]);
	RFX_Int(pFX, _T("[�߳ʺ�1]"), m_pose1_size[49]);
	RFX_Int(pFX, _T("[���������̼������1]"), m_pose1_size[50]);
	RFX_Int(pFX, _T("[�ܵ���β�1]"), m_pose1_size[51]);
	RFX_Int(pFX, _T("[�����β�1]"), m_pose1_size[52]);
	RFX_Int(pFX, _T("[�������β�1]"), m_pose1_size[53]);
	RFX_Int(pFX, _T("[�������Ʒ��β�1]"), m_pose1_size[54]);
	RFX_Int(pFX, _T("[�㸮�β�1]"), m_pose1_size[55]);
	RFX_Int(pFX, _T("[��ż����㸮�β�1]"), m_pose1_size[56]);
	RFX_Int(pFX, _T("[��β�1]"), m_pose1_size[57]);
	RFX_Int(pFX, _T("[�����̵β�1]"), m_pose1_size[58]);
	RFX_Int(pFX, _T("[�������β�1]"), m_pose1_size[59]);
	RFX_Int(pFX, _T("[�������Ʒ��ѷ�F1]"), m_pose1_size[60]);
	RFX_Int(pFX, _T("[��ڼո���ʱ���1]"), m_pose1_size[61]);
	RFX_Int(pFX, _T("[�ܵ���ѷ���1]"), m_pose1_size[62]);

	RFX_Int(pFX, _T("[Ű2]"), m_pose2_size[0]);
	RFX_Int(pFX, _T("[������2]"), m_pose2_size[1]);
	RFX_Int(pFX, _T("[�������2]"), m_pose2_size[2]);
	RFX_Int(pFX, _T("[������ʳ���2]"), m_pose2_size[3]);
	RFX_Int(pFX, _T("[��ڳ���2]"), m_pose2_size[4]);
	RFX_Int(pFX, _T("[��ճ���2]"), m_pose2_size[5]);
	RFX_Int(pFX, _T("[�ܵ������2]"), m_pose2_size[6]);
	RFX_Int(pFX, _T("[����������2]"), m_pose2_size[7]);
	RFX_Int(pFX, _T("[�������Ʒ�����2]"), m_pose2_size[8]);
	RFX_Int(pFX, _T("[�㸮����2]"), m_pose2_size[9]);
	RFX_Int(pFX, _T("[��ż����㸮����2]"), m_pose2_size[10]);
	RFX_Int(pFX, _T("[�����2]"), m_pose2_size[11]);
	RFX_Int(pFX, _T("[���վ��������ó���2]"), m_pose2_size[12]);
	RFX_Int(pFX, _T("[�������ɼ�����2]"), m_pose2_size[13]);
	RFX_Int(pFX, _T("[�����̳���2]"), m_pose2_size[14]);
	RFX_Int(pFX, _T("[����������2]"), m_pose2_size[15]);
	RFX_Int(pFX, _T("[�Ҵٸ�����2]"), m_pose2_size[16]);
	RFX_Int(pFX, _T("[������2]"), m_pose2_size[17]);
	RFX_Int(pFX, _T("[���������2]"), m_pose2_size[18]);
	RFX_Int(pFX, _T("[�����̼�������2]"), m_pose2_size[19]);
	RFX_Int(pFX, _T("[�����������2]"), m_pose2_size[20]);
	RFX_Int(pFX, _T("[��������2]"), m_pose2_size[21]);
	RFX_Int(pFX, _T("[�������������2]"), m_pose2_size[22]);
	RFX_Int(pFX, _T("[���ʺ������2]"), m_pose2_size[23]);
	RFX_Int(pFX, _T("[�����Ȳ�ġ����2]"), m_pose2_size[24]);
	RFX_Int(pFX, _T("[�ճ�����2]"), m_pose2_size[25]);
	RFX_Int(pFX, _T("[���峡����2]"), m_pose2_size[26]);
	RFX_Int(pFX, _T("[�����̵ѷ�2]"), m_pose2_size[27]);
	RFX_Int(pFX, _T("[��ڿ��ݱ���2]"), m_pose2_size[28]);
	RFX_Int(pFX, _T("[�ѱ���2]"), m_pose2_size[29]);
	RFX_Int(pFX, _T("[�����̿�����2]"), m_pose2_size[30]);
	RFX_Int(pFX, _T("[�㸮�����ʺ������2]"), m_pose2_size[31]);
	RFX_Int(pFX, _T("[�ٸ����ʱ���2]"), m_pose2_size[32]);
	RFX_Int(pFX, _T("[�����ȱ���2]"), m_pose2_size[33]);
	RFX_Int(pFX, _T("[�ȱ�����ڼո���ʱ���2]"), m_pose2_size[34]);
	RFX_Int(pFX, _T("[�������������2]"), m_pose2_size[35]);
	RFX_Int(pFX, _T("[�����̳ʺ�2]"), m_pose2_size[36]);
	RFX_Int(pFX, _T("[�ճʺ�2]"), m_pose2_size[37]);
	RFX_Int(pFX, _T("[����ճ��������2]"), m_pose2_size[38]);
	RFX_Int(pFX, _T("[����Ű3]"), m_pose3_size[0]);
	RFX_Int(pFX, _T("[����������3]"), m_pose3_size[1]);
	RFX_Int(pFX, _T("[������ڳ���3]"), m_pose3_size[2]);
	RFX_Int(pFX, _T("[�����������3]"), m_pose3_size[3]);
	RFX_Int(pFX, _T("[�����Ȳ�ġ����3]"), m_pose3_size[4]);
	RFX_Int(pFX, _T("[������������3]"), m_pose3_size[5]);
	RFX_Int(pFX, _T("[�������ݳ���3]"), m_pose3_size[6]);
	RFX_Int(pFX, _T("[�������3]"), m_pose3_size[7]);
	RFX_Int(pFX, _T("[��ھ�����ʱ���3]"), m_pose3_size[8]);
	RFX_Int(pFX, _T("[������ʻ��̱���3]"), m_pose3_size[9]);
	RFX_Int(pFX, _T("[�ܵ���պ����̱��̾�ǰ3]"), m_pose3_size[10]);
	RFX_Int(pFX, _T("[�ܵ���ں����̱��̵�ǰ3]"), m_pose3_size[11]);
	RFX_Int(pFX, _T("[�ܵ�����������̱���3]"), m_pose3_size[12]);
	RFX_Int(pFX, _T("[�ܵ�����������̱���3]"), m_pose3_size[13]);
	RFX_Int(pFX, _T("[���ȼ�������3]"), m_pose3_size[14]);
	RFX_Int(pFX, _T("[�Ʒ��ȼ������3]"), m_pose3_size[15]);
	RFX_Int(pFX, _T("[�Ȳ�ġ�ո�������3]"), m_pose3_size[16]);
	RFX_Int(pFX, _T("[���������̿��ݼ������3]"), m_pose3_size[17]);
	RFX_Int(pFX, _T("[���������̹����������3]"), m_pose3_size[18]);
	RFX_Int(pFX, _T("[����ʺ�3]"), m_pose3_size[19]);
	RFX_Int(pFX, _T("[���Ȼ��̳ʺ�3]"), m_pose3_size[20]);
	RFX_Int(pFX, _T("[�Ȳ�ġ���̳ʺ�3]"), m_pose3_size[21]);
	RFX_Int(pFX, _T("[���������̳ʺ�3]"), m_pose3_size[22]);
	RFX_Int(pFX, _T("[�����Ҵٸ�����3]"), m_pose3_size[23]);
	RFX_Int(pFX, _T("[������β�3]"), m_pose3_size[24]);
	RFX_Int(pFX, _T("[���������̹�β�3]"), m_pose3_size[25]);
	RFX_Int(pFX, _T("[�����������3]"), m_pose3_size[26]);
	RFX_Int(pFX, _T("[���ʾ������3]"), m_pose3_size[27]);
	RFX_Int(pFX, _T("[������ʻ��̼������3]"), m_pose3_size[28]);
	RFX_Int(pFX, _T("[�ܵ���ѷ�F3]"), m_pose3_size[29]);
	RFX_Int(pFX, _T("[�����ѷ�F3]"), m_pose3_size[30]);
	RFX_Int(pFX, _T("[�����ʺ�F3]"), m_pose3_size[31]);
	RFX_Int(pFX, _T("[�����β�F3]"), m_pose3_size[32]);
	RFX_Int(pFX, _T("[������̱���3]"), m_pose3_size[33]);

	RFX_Int(pFX, _T("[�Ӹ��ѷ�4]"), m_pose4_size[0]);
	RFX_Int(pFX, _T("[����Ӹ���������4]"), m_pose4_size[1]);
	RFX_Int(pFX, _T("[�ͱ������̸Ӹ�����4]"), m_pose4_size[2]);
	RFX_Int(pFX, _T("[�ͱ��������γ�4]"), m_pose4_size[3]);
	RFX_Int(pFX, _T("[�ͱ������̰��󿬰�4]"), m_pose4_size[4]);
	RFX_Int(pFX, _T("[�ͱ��������ڻѸ�4]"), m_pose4_size[5]);
	RFX_Int(pFX, _T("[�ͱ��������ڹ�4]"), m_pose4_size[6]);
	RFX_Int(pFX, _T("[�������24]"), m_pose4_size[7]);
	RFX_Int(pFX, _T("[������ʳ���24]"), m_pose4_size[8]);

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
