#ifndef _MEASURE_FUNC_1_H
#define _MEASURE_FUNC_1_H

#include "point3f.h"

// ���� �׸��  1 ~ 20
extern void GetResultPose2_1_20( float *Result, GVector3f *Points, int NumPt, int *IdxList, int NumIdx, GLandMark *LandMark, GLandMark *OldLandMark);

// 21 : ��ѷ�.
extern void GetResultPose2_21( float *Result, GVector3f *Points, int NumPt, int *IdxList, int NumIdx, GLandMark *LandMark, GLandMark *OldLandMark, GObList<GVector3f> *CvxPt );

// 22 : ��صѷ�. 
// 68 : ��(��)�ʺ�.
extern void GetResultPose2_22_68( float *Result, GVector3f *Points, int NumPt, int *IdxList, int NumIdx, GLandMark *LandMark, GLandMark *LandMarkHelp, GObList<GVector3f> *CvxPt, GObList<GVector3f> *CvxPt2, GObList<GVector3f> *CvxPt3 );
extern void GetResultPose2_22_68_a( float *Result, GVector3f *Points, int NumPt, int *IdxList, int NumIdx, GLandMark *LandMark, GLandMark *LandMarkHelp, GObList<GVector3f> *CvxPt, GObList<GVector3f> *CvxPt2, GObList<GVector3f> *CvxPt3 );

// 25 : �ܵ���ѷ�. 
extern void GetResultPose2_25( float *Result, GVector3f *Points, int NumPt, int *IdxList, int NumIdx, GLandMark *LandMark, GLandMark *LandMarkHelp, GLandMark *OldLandMark, GObList<GVector3f> *CvxPt );
extern void GetResultPose2_25_a( float *Result, GVector3f *Points, int NumPt, int *IdxList, int NumIdx, GLandMark *LandMark, GLandMark *LandMarkHelp, GLandMark *OldLandMark, GObList<GVector3f> *CvxPt );

// 88 : �ܵ���ѷ� Left
extern void GetResultPose2_88( float *Result, GVector3f *Points, int NumPt, int *IdxList, int NumIdx, GLandMark *LandMark, GLandMark *LandMarkHelp, GLandMark *OldLandMark, GObList<GVector3f> *CvxPt );

// 26 : ���� �ѷ�. 
// 70 : ���� �ʺ�. 
// 79 : ���� �β�.
extern void GetResultPose2_26_70_79( float *Result, GVector3f *Points, int NumPt, int *IdxList, int NumIdx, GLandMark *LandMark, GLandMark *LandMarkHelp, GLandMark *OldLandMark, GObList<GVector3f> *CvxPt0, GObList<GVector3f> *CvxPt1, GObList<GVector3f> *CvxPt2 );
extern void GetResultPose2_26_70_79_a( float *Result, GVector3f *Points, int NumPt, int *IdxList, int NumIdx, GLandMark *LandMark, GLandMark *LandMarkHelp, GLandMark *OldLandMark, GObList<GVector3f> *CvxPt0, GObList<GVector3f> *CvxPt1, GObList<GVector3f> *CvxPt2 );


// 27 : ������ �ѷ�.
// 77 : ������ ���̼������.
// 71 : ������ �ʺ�.
// 80 : ������ �β�.
extern void GetResultPose2_27_77_71_80( float *Result, GVector3f *Points, int NumPt, int *IdxList, int NumIdx, GLandMark *LandMark,  GLandMark *LandMarkHelp, GLandMark *OldLandMark, GObList<GVector3f> *CvxPt0, GObList<GVector3f> *CvxPt1, GObList<GVector3f> *CvxPt2, GObList<GVector3f> *CvxPt3 );

// 28 : ������ �Ʒ� �ѷ�.
// 72 : ������ �Ʒ� �ʺ�.
// 81 : ������ �Ʒ� �β�.
extern void GetResultPose2_28_72_81( float *Result, GVector3f *Points, int NumPt, int *IdxList, int NumIdx, GLandMark *LandMark,  GLandMark *LandMarkHelp, GLandMark *OldLandMark, GObList<GVector3f> *CvxPt0, GObList<GVector3f> *CvxPt1, GObList<GVector3f> *CvxPt2 );

// 29 : �㸮�ѷ�.
// 58 : �㸮 �ʺ�.
// 66 : �㸮 �β�.
extern void GetResultPose2_29_73_82( float *Result, GVector3f *Points, int NumPt, int *IdxList, int NumIdx, GLandMark *LandMark, GLandMark *LandMarkHelp, GLandMark *OldLandMark, GObList<GVector3f> *CvxPt0, GObList<GVector3f> *CvxPt1, GObList<GVector3f> *CvxPt2 );

// 30 : ��ѷ�.
// 74 : ��ʺ�.
// 83 : ��β�.
extern void GetResultPose2_30_74_83( float *Result, GVector3f *Points, int NumPt, int *IdxList, int NumIdx, GLandMark *LandMark, GLandMark *LandMarkHelp, GLandMark *OldLandMark, GObList<GVector3f> *CvxPt0, GObList<GVector3f> *CvxPt1, GObList<GVector3f> *CvxPt2 );

// 31 : ��ż����㸮�ѷ�
// 75 : ��ż����㸮�ʺ�
// 84 : ��ż����㸮�β�
extern void GetResultPose2_31_75_84( float *Result, GVector3f *Points, int NumPt, int *IdxList, int NumIdx, GLandMark *LandMark, GLandMark *LandMarkHelp, GLandMark *OldLandMark, GObList<GVector3f> *CvxPt0, GObList<GVector3f> *CvxPt1, GObList<GVector3f> *CvxPt2 );

// 32 : �����̵ѷ�.
// 76 : �����̳ʺ�.
// 85 : �����̵β�.
extern void GetResultPose2_32_76_85( float *Result, GVector3f *Points, int NumPt, int *IdxList, int NumIdx, GLandMark *LandMark, GLandMark *LandMarkHelp, GLandMark *OldLandMark, GObList<GVector3f> *CvxPt0, GObList<GVector3f> *CvxPt1, GObList<GVector3f> *CvxPt2 );

// 33 : �Ҵٸ� �ѷ�.
extern void GetResultPose2_33( float *Result, GVector3f *Points, int NumPt, int *IdxList, int NumIdx, GLandMark *LandMark, GLandMark *OldLandMark, GObList<GVector3f> *CvxPt);

// 34 : �����ѷ�.
extern void GetResultPose2_34( float *Result, GVector3f *Points, int NumPt, int *IdxList, int NumIdx, GLandMark *LandMark, GLandMark *OldLandMark, GObList<GVector3f> *CvxPt );

// 35 : ����� �ѷ�.
extern void GetResultPose2_35( float *Result, GVector3f *Points, int NumPt, int *IdxList, int NumIdx, GLandMark *LandMark, GLandMark *OldLandMark, GObList<GVector3f> *CvxPt );

// 36 : ���Ƹ��ּ� �ѷ�.
extern void GetResultPose2_36( float *Result, GVector3f *Points, int NumPt, int *IdxList, int NumIdx, GLandMark *LandMark, GLandMark *OldLandMark, GObList<GVector3f> *CvxPt );

// 37 : �߸��ִ�ѷ�	
extern void GetResultPose2_37( float *Result, GVector3f *Points, int NumPt, int *IdxList, int NumIdx, GLandMark *LandMark, GLandMark *OldLandMark, GObList<GVector3f> *CvxPt );

// 38 : ������ �ѷ�.
extern void GetResultPose2_38( float *Result, GVector3f *Points, int NumPt, int *IdxList, int NumIdx, GLandMark *LandMark, GLandMark *LandMarkHelp, GLandMark *OldLandMark, GObList<GVector3f> *CvxPt );

// 39 : ���Ȳ�ġ�ѷ� 64: �Ⱦ��ʱ���.
extern void GetResultPose2_39_64( float *Result, GVector3f *Points, int NumPt, int *IdxList, int NumIdx, GLandMark *LandMark,GLandMark *LandMarkHelp,  GLandMark *OldLandMark, GObList<GVector3f> *CvxPt0, GObList<GVector3f> *CvxPt1 );

// 40 : �ո� �ѷ�
extern void GetResultPose2_40( float *Result, GVector3f *Points, int NumPt, int *IdxList, int NumIdx, GLandMark *LandMark, GLandMark *LandMarkHelp, GLandMark *OldLandMark, GObList<GVector3f> *CvxPt );

// 41 : ���뼼�εѷ�.
extern void GetResultPose2_41( float *Result, GVector3f *Points, int NumPt, int *IdxList, int NumIdx, GLandMark *LandMark, GLandMark *OldLandMark, GObList<GVector3f> *CvxPt );

// 42 : ���߽ɱ���.
extern void GetResultPose2_42( float *Result, GVector3f *Points, int NumPt, int *IdxList, int NumIdx, GLandMark *LandMark, GLandMark *OldLandMark, GObList<GVector3f> *CvxPt );

// 44 : ��� ����.
extern void GetResultPose2_44( float *Result, GVector3f *Points, int NumPt, int *IdxList, int NumIdx, GLandMark *LandMark, GLandMark *OldLandMark, GObList<GVector3f> *CvxPt );

// 49 : ������������.
extern void GetResultPose2_49( float *Result, GVector3f *Points, int NumPt, int *IdxList, int NumIdx, GLandMark *LandMark, GLandMark *LandMarkHelp, GObList<GVector3f> *CvxPt );

// 50 : ���������㸮�ѷ�������.
extern void GetResultPose2_50( float *Result, GVector3f *Points, int NumPt, int *IdxList, int NumIdx, GLandMark *LandMark, GLandMark *OldLandMark, GObList<GVector3f> *CvxPt);

// 54 : ��ڵ�����ܵ�����ر��� ��������.
extern void GetResultPose2_54( float *Result, GVector3f *Points, int NumPt, int *IdxList, int NumIdx, GLandMark *LandMark, GLandMark *OldLandMark, GObList<GVector3f> *CvxPt );

// 55 : �񿷵��㸮�ѷ�������(�ڱ���)
extern void GetResultPose2_55( float *Result, GVector3f *Points, int NumPt, int *IdxList, int NumIdx, GLandMark *LandMark, GLandMark *OldLandMark, GObList<GVector3f> *CvxPt );

// 56 : �����.
extern void GetResultPose2_56( float *Result, GVector3f *Points, int NumPt, int *IdxList, int NumIdx, GLandMark *LandMark, GLandMark *OldLandMark, GObList<GVector3f> *CvxPt );

// 45 : ��ھ�����ʱ���
// 46 : ������ʻ��̱���-���.
// 69 : ��� �ʺ�.
extern void GetResultPose2_45_46_69( float *Result, GVector3f *Points, int NumPt, int *IdxList, int NumIdx, GLandMark *LandMark, GLandMark *OldLandMark, GObList<GVector3f> *CvxPt0,  GObList<GVector3f> *CvxPt1,  GObList<GVector3f> *CvxPt2 );

// 47 : �ܵ���պ����̱��� ��ǰ.
extern void GetResultPose2_47( float *Result, GVector3f *Points, int NumPt, int *IdxList, int NumIdx, GLandMark *LandMark, GLandMark *OldLandMark, GObList<GVector3f> *CvxPt );

// 48 : �ܵ���ں����̱��� ��ǰ.
extern void GetResultPose2_48( float *Result, GVector3f *Points, int NumPt, int *IdxList, int NumIdx, GLandMark *LandMark, GLandMark *OldLandMark, GObList<GVector3f> *CvxPt );

// 59_60 : ���յڱ���. ��ż��ػ��յڱ���
extern void GetResultPose2_59( float *Result, GVector3f *Points, int NumPt, int *IdxList, int NumIdx, GLandMark *LandMark, GLandMark *LandMarkHelp, GObList<GVector3f> *CvxPt );

// 61 : �㸮�����ʺ������(=������ٸ�����=��������)
// 62 : �ٸ�����
extern void GetResultPose2_61_62( float *Result, GVector3f *Points, int NumPt, int *IdxList, int NumIdx, GLandMark *LandMark, GLandMark *LandMarkHelp, GLandMark *OldLandMark, GObList<GVector3f> *CvxPt0, GObList<GVector3f> *CvxPt1 );


// 58 : ������(��)����  
extern void GetResultPose2_58( float *Result, GVector3f *Points, int NumPt, int *IdxList, int NumIdx, GLandMark *LandMark, GLandMark *m_LandMarkHelp, GObList<GVector3f> *CvxPt);


// 63 : �ȱ���.
// 66 : ���ȱ���.
// 65 : ��ڼո���ʱ���
extern void GetResultPose2_63_66_65( float *Result, GVector3f *Points, int NumPt, int *IdxList, int NumIdx, GLandMark *LandMark, GLandMark *OldLandMark, GObList<GVector3f> *CvxPt0, GObList<GVector3f> *CvxPt1, GObList<GVector3f> *CvxPt2 );

// 68 ~ 76 : ��(��)�ʺ� ~ ������ �ʺ�

// 78: �ܵ�� �β�.
extern void GetResultPose2_78( float *Result, GVector3f *Points, int NumPt, int *IdxList, int NumIdx, GLandMark *LandMark, GLandMark *OldLandMark, GObList<GVector3f> *CvxPt );

// 79~85 :  �����β�~�����̵β�

// 86 : ������ �������
extern void GetResultPose2_86( float *Result, GVector3f *Points, int NumPt, int *IdxList, int NumIdx, GLandMark *LandMark, GLandMark *OldLandMark, GObList<GVector3f> *CvxPt, GObList<GVector3f> *CvxPt1 );

// 75 ; �������Ʒ� �ѷ�(F)
//extern void GetResultPose2_89( float *Result, GVector3f *Points, int NumPt, int *IdxList, int NumIdx, GLandMark *LandMark, GLandMark *LandMarkHelp, GObList<GVector3f> *CvxPt );

// 90 : �ܵ���ѷ�(������)
extern void GetResultPose2_90( float *Result, GVector3f *Points, int NumPt, int *IdxList, int NumIdx, GLandMark *LandMark, GLandMark *OldLandMark, GObList<GVector3f> *CvxPt );


#else
#endif //_MEASURE_FUNC_1_H

