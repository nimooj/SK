#ifndef _MEASURE_FUNC_1_H
#define _MEASURE_FUNC_1_H

#include "point3f.h"

// 높이 항목들  1 ~ 20
extern void GetResultPose2_1_20( float *Result, GVector3f *Points, int NumPt, int *IdxList, int NumIdx, GLandMark *LandMark, GLandMark *OldLandMark);

// 21 : 목둘레.
extern void GetResultPose2_21( float *Result, GVector3f *Points, int NumPt, int *IdxList, int NumIdx, GLandMark *LandMark, GLandMark *OldLandMark, GObList<GVector3f> *CvxPt );

// 22 : 목밑둘레. 
// 68 : 목(밑)너비.
extern void GetResultPose2_22_68( float *Result, GVector3f *Points, int NumPt, int *IdxList, int NumIdx, GLandMark *LandMark, GLandMark *LandMarkHelp, GObList<GVector3f> *CvxPt, GObList<GVector3f> *CvxPt2, GObList<GVector3f> *CvxPt3 );
extern void GetResultPose2_22_68_a( float *Result, GVector3f *Points, int NumPt, int *IdxList, int NumIdx, GLandMark *LandMark, GLandMark *LandMarkHelp, GObList<GVector3f> *CvxPt, GObList<GVector3f> *CvxPt2, GObList<GVector3f> *CvxPt3 );

// 25 : 겨드랑둘레. 
extern void GetResultPose2_25( float *Result, GVector3f *Points, int NumPt, int *IdxList, int NumIdx, GLandMark *LandMark, GLandMark *LandMarkHelp, GLandMark *OldLandMark, GObList<GVector3f> *CvxPt );
extern void GetResultPose2_25_a( float *Result, GVector3f *Points, int NumPt, int *IdxList, int NumIdx, GLandMark *LandMark, GLandMark *LandMarkHelp, GLandMark *OldLandMark, GObList<GVector3f> *CvxPt );

// 88 : 겨드랑둘레 Left
extern void GetResultPose2_88( float *Result, GVector3f *Points, int NumPt, int *IdxList, int NumIdx, GLandMark *LandMark, GLandMark *LandMarkHelp, GLandMark *OldLandMark, GObList<GVector3f> *CvxPt );

// 26 : 가슴 둘레. 
// 70 : 가슴 너비. 
// 79 : 가슴 두께.
extern void GetResultPose2_26_70_79( float *Result, GVector3f *Points, int NumPt, int *IdxList, int NumIdx, GLandMark *LandMark, GLandMark *LandMarkHelp, GLandMark *OldLandMark, GObList<GVector3f> *CvxPt0, GObList<GVector3f> *CvxPt1, GObList<GVector3f> *CvxPt2 );
extern void GetResultPose2_26_70_79_a( float *Result, GVector3f *Points, int NumPt, int *IdxList, int NumIdx, GLandMark *LandMark, GLandMark *LandMarkHelp, GLandMark *OldLandMark, GObList<GVector3f> *CvxPt0, GObList<GVector3f> *CvxPt1, GObList<GVector3f> *CvxPt2 );


// 27 : 젖가슴 둘레.
// 77 : 젖꼭지 사이수평길이.
// 71 : 젖가슴 너비.
// 80 : 젖가슴 두께.
extern void GetResultPose2_27_77_71_80( float *Result, GVector3f *Points, int NumPt, int *IdxList, int NumIdx, GLandMark *LandMark,  GLandMark *LandMarkHelp, GLandMark *OldLandMark, GObList<GVector3f> *CvxPt0, GObList<GVector3f> *CvxPt1, GObList<GVector3f> *CvxPt2, GObList<GVector3f> *CvxPt3 );

// 28 : 젖가슴 아래 둘레.
// 72 : 젖가슴 아래 너비.
// 81 : 젖가슴 아래 두께.
extern void GetResultPose2_28_72_81( float *Result, GVector3f *Points, int NumPt, int *IdxList, int NumIdx, GLandMark *LandMark,  GLandMark *LandMarkHelp, GLandMark *OldLandMark, GObList<GVector3f> *CvxPt0, GObList<GVector3f> *CvxPt1, GObList<GVector3f> *CvxPt2 );

// 29 : 허리둘레.
// 58 : 허리 너비.
// 66 : 허리 두께.
extern void GetResultPose2_29_73_82( float *Result, GVector3f *Points, int NumPt, int *IdxList, int NumIdx, GLandMark *LandMark, GLandMark *LandMarkHelp, GLandMark *OldLandMark, GObList<GVector3f> *CvxPt0, GObList<GVector3f> *CvxPt1, GObList<GVector3f> *CvxPt2 );

// 30 : 배둘레.
// 74 : 배너비.
// 83 : 배두께.
extern void GetResultPose2_30_74_83( float *Result, GVector3f *Points, int NumPt, int *IdxList, int NumIdx, GLandMark *LandMark, GLandMark *LandMarkHelp, GLandMark *OldLandMark, GObList<GVector3f> *CvxPt0, GObList<GVector3f> *CvxPt1, GObList<GVector3f> *CvxPt2 );

// 31 : 배꼽수준허리둘레
// 75 : 배꼽수준허리너비
// 84 : 배꼽수준허리두께
extern void GetResultPose2_31_75_84( float *Result, GVector3f *Points, int NumPt, int *IdxList, int NumIdx, GLandMark *LandMark, GLandMark *LandMarkHelp, GLandMark *OldLandMark, GObList<GVector3f> *CvxPt0, GObList<GVector3f> *CvxPt1, GObList<GVector3f> *CvxPt2 );

// 32 : 엉덩이둘레.
// 76 : 엉덩이너비.
// 85 : 엉덩이두께.
extern void GetResultPose2_32_76_85( float *Result, GVector3f *Points, int NumPt, int *IdxList, int NumIdx, GLandMark *LandMark, GLandMark *LandMarkHelp, GLandMark *OldLandMark, GObList<GVector3f> *CvxPt0, GObList<GVector3f> *CvxPt1, GObList<GVector3f> *CvxPt2 );

// 33 : 넙다리 둘레.
extern void GetResultPose2_33( float *Result, GVector3f *Points, int NumPt, int *IdxList, int NumIdx, GLandMark *LandMark, GLandMark *OldLandMark, GObList<GVector3f> *CvxPt);

// 34 : 무릎둘레.
extern void GetResultPose2_34( float *Result, GVector3f *Points, int NumPt, int *IdxList, int NumIdx, GLandMark *LandMark, GLandMark *OldLandMark, GObList<GVector3f> *CvxPt );

// 35 : 장딴지 둘레.
extern void GetResultPose2_35( float *Result, GVector3f *Points, int NumPt, int *IdxList, int NumIdx, GLandMark *LandMark, GLandMark *OldLandMark, GObList<GVector3f> *CvxPt );

// 36 : 종아리최소 둘레.
extern void GetResultPose2_36( float *Result, GVector3f *Points, int NumPt, int *IdxList, int NumIdx, GLandMark *LandMark, GLandMark *OldLandMark, GObList<GVector3f> *CvxPt );

// 37 : 발목최대둘레	
extern void GetResultPose2_37( float *Result, GVector3f *Points, int NumPt, int *IdxList, int NumIdx, GLandMark *LandMark, GLandMark *OldLandMark, GObList<GVector3f> *CvxPt );

// 38 : 편위팔 둘레.
extern void GetResultPose2_38( float *Result, GVector3f *Points, int NumPt, int *IdxList, int NumIdx, GLandMark *LandMark, GLandMark *LandMarkHelp, GLandMark *OldLandMark, GObList<GVector3f> *CvxPt );

// 39 : 편팔꿈치둘레 64: 팔안쪽길이.
extern void GetResultPose2_39_64( float *Result, GVector3f *Points, int NumPt, int *IdxList, int NumIdx, GLandMark *LandMark,GLandMark *LandMarkHelp,  GLandMark *OldLandMark, GObList<GVector3f> *CvxPt0, GObList<GVector3f> *CvxPt1 );

// 40 : 손목 둘레
extern void GetResultPose2_40( float *Result, GVector3f *Points, int NumPt, int *IdxList, int NumIdx, GLandMark *LandMark, GLandMark *LandMarkHelp, GLandMark *OldLandMark, GObList<GVector3f> *CvxPt );

// 41 : 몸통세로둘레.
extern void GetResultPose2_41( float *Result, GVector3f *Points, int NumPt, int *IdxList, int NumIdx, GLandMark *LandMark, GLandMark *OldLandMark, GObList<GVector3f> *CvxPt );

// 42 : 앞중심길이.
extern void GetResultPose2_42( float *Result, GVector3f *Points, int NumPt, int *IdxList, int NumIdx, GLandMark *LandMark, GLandMark *OldLandMark, GObList<GVector3f> *CvxPt );

// 44 : 어깨 길이.
extern void GetResultPose2_44( float *Result, GVector3f *Points, int NumPt, int *IdxList, int NumIdx, GLandMark *LandMark, GLandMark *OldLandMark, GObList<GVector3f> *CvxPt );

// 49 : 목옆젖꼭지길이.
extern void GetResultPose2_49( float *Result, GVector3f *Points, int NumPt, int *IdxList, int NumIdx, GLandMark *LandMark, GLandMark *LandMarkHelp, GObList<GVector3f> *CvxPt );

// 50 : 목옆젖꼭지허리둘레선길이.
extern void GetResultPose2_50( float *Result, GVector3f *Points, int NumPt, int *IdxList, int NumIdx, GLandMark *LandMark, GLandMark *OldLandMark, GObList<GVector3f> *CvxPt);

// 54 : 목뒤등뼈위겨드랑수준길이 진동깊이.
extern void GetResultPose2_54( float *Result, GVector3f *Points, int NumPt, int *IdxList, int NumIdx, GLandMark *LandMark, GLandMark *OldLandMark, GObList<GVector3f> *CvxPt );

// 55 : 목옆뒤허리둘레선길이(뒤길이)
extern void GetResultPose2_55( float *Result, GVector3f *Points, int NumPt, int *IdxList, int NumIdx, GLandMark *LandMark, GLandMark *OldLandMark, GObList<GVector3f> *CvxPt );

// 56 : 등길이.
extern void GetResultPose2_56( float *Result, GVector3f *Points, int NumPt, int *IdxList, int NumIdx, GLandMark *LandMark, GLandMark *OldLandMark, GObList<GVector3f> *CvxPt );

// 45 : 목뒤어깨가쪽길이
// 46 : 어깨가쪽사이길이-목뒤.
// 69 : 어깨 너비.
extern void GetResultPose2_45_46_69( float *Result, GVector3f *Points, int NumPt, int *IdxList, int NumIdx, GLandMark *LandMark, GLandMark *OldLandMark, GObList<GVector3f> *CvxPt0,  GObList<GVector3f> *CvxPt1,  GObList<GVector3f> *CvxPt2 );

// 47 : 겨드랑앞벽사이길이 앞품.
extern void GetResultPose2_47( float *Result, GVector3f *Points, int NumPt, int *IdxList, int NumIdx, GLandMark *LandMark, GLandMark *OldLandMark, GObList<GVector3f> *CvxPt );

// 48 : 겨드랑뒤벽사이길이 뒤품.
extern void GetResultPose2_48( float *Result, GVector3f *Points, int NumPt, int *IdxList, int NumIdx, GLandMark *LandMark, GLandMark *OldLandMark, GObList<GVector3f> *CvxPt );

// 59_60 : 샅앞뒤길이. 배꼽수준샅앞뒤길이
extern void GetResultPose2_59( float *Result, GVector3f *Points, int NumPt, int *IdxList, int NumIdx, GLandMark *LandMark, GLandMark *LandMarkHelp, GObList<GVector3f> *CvxPt );

// 61 : 허리옆가쪽복사길이(=기능적다리길이=바지길이)
// 62 : 다리길이
extern void GetResultPose2_61_62( float *Result, GVector3f *Points, int NumPt, int *IdxList, int NumIdx, GLandMark *LandMark, GLandMark *LandMarkHelp, GLandMark *OldLandMark, GObList<GVector3f> *CvxPt0, GObList<GVector3f> *CvxPt1 );


// 58 : 엉덩이(옆)길이  
extern void GetResultPose2_58( float *Result, GVector3f *Points, int NumPt, int *IdxList, int NumIdx, GLandMark *LandMark, GLandMark *m_LandMarkHelp, GObList<GVector3f> *CvxPt);


// 63 : 팔길이.
// 66 : 위팔길이.
// 65 : 목뒤손목안쪽길이
extern void GetResultPose2_63_66_65( float *Result, GVector3f *Points, int NumPt, int *IdxList, int NumIdx, GLandMark *LandMark, GLandMark *OldLandMark, GObList<GVector3f> *CvxPt0, GObList<GVector3f> *CvxPt1, GObList<GVector3f> *CvxPt2 );

// 68 ~ 76 : 목(밑)너비 ~ 엉덩이 너비

// 78: 겨드랑 두께.
extern void GetResultPose2_78( float *Result, GVector3f *Points, int NumPt, int *IdxList, int NumIdx, GLandMark *LandMark, GLandMark *OldLandMark, GObList<GVector3f> *CvxPt );

// 79~85 :  가슴두께~엉덩이두께

// 86 : 오른쪽 어깨기울기
extern void GetResultPose2_86( float *Result, GVector3f *Points, int NumPt, int *IdxList, int NumIdx, GLandMark *LandMark, GLandMark *OldLandMark, GObList<GVector3f> *CvxPt, GObList<GVector3f> *CvxPt1 );

// 75 ; 젖가슴아래 둘래(F)
//extern void GetResultPose2_89( float *Result, GVector3f *Points, int NumPt, int *IdxList, int NumIdx, GLandMark *LandMark, GLandMark *LandMarkHelp, GObList<GVector3f> *CvxPt );

// 90 : 겨드랑둘레(구벼젼)
extern void GetResultPose2_90( float *Result, GVector3f *Points, int NumPt, int *IdxList, int NumIdx, GLandMark *LandMark, GLandMark *OldLandMark, GObList<GVector3f> *CvxPt );


#else
#endif //_MEASURE_FUNC_1_H

