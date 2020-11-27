#pragma once

#include "SizeKorea.h"
#include "SizeKoreaUtil.h"

#include "Pose1Landmarks.h"
#include "Pose1Measures.h"

void Pose1_FindTopHeadPt(EgMesh *pMesh);

void Pose1_MakeSilhouette(EgMesh *pMesh);

void Pose1_FindFneckPt(EgMesh *pMesh);
void Pose1_FindNosePt(EgMesh *pMesh);
void Pose1_FindJawPt(EgMesh *pMesh);
void Pose1_FindHipnCalfExtrudePt(EgMesh *pMesh);
void Pose1_FindUnderHipPt(EgMesh *pMesh);
void Pose1_FindStomachExtrudePt(EgMesh *pMesh);
void Pose1_FindCrotchPt(EgMesh *pMesh);
void Pose1_FindArmpitPt(EgMesh *pMesh);
void Pose1_FindShoulderPt(EgMesh *pMesh);

void Pose1_FindSideNeckPt(EgMesh *pMesh);
void Pose1_FindBackNeckPt(EgMesh *pMesh);

void Pose1_FindArmpitWall(EgMesh *pMesh);
void Pose1_FindWaistPt(EgMesh *pMesh);

void Pose1_FindKneePt(EgMesh *pMesh);

void Pose1_FindAnklePt(EgMesh *pMesh);

void Pose1_FindNavelPt(EgMesh *pMesh);
void Pose1_FindHandPt(EgMesh *pMesh);

void Pose1_FindBustPt(EgMesh *pMesh);
void Pose1_FindUnderBustPt(EgMesh *pMesh);

void Pose1_FindWristPt(EgMesh *pMesh);
void Pose1_FindElbowPt(EgMesh *pMesh);

void Pose1_FindEyePt(EgMesh *pMesh);

void Pose1_FindTopHipPt(EgMesh *pMesh);
void Pose1_FindUpperHipPt(EgMesh *pMesh);
void Pose1_FindMidRiffPt(EgMesh *pMesh);

void Pose1_FindBackExtrudePt(EgMesh *pMesh);


void Pose1_ModifyArmpitWithShoudler(EgMesh *pMesh);;

