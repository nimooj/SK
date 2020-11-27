#pragma once

/*!	\brief	엉덩이돌출점(수준)을 찾는 함수 */
EgVertex *SK_Pose5_Buttock_Protrusion(EgMesh *pMesh);

/*!	\brief	배돌출점을 찾는 함수 */
EgVertex *SK_Pose5_Abdominal_Protrusion(EgMesh *pMesh);

/*!	\brief	엉덩이최대둘레수준(점)을 찾는 함수 */
EgVertex *SK_Pose5_Buttock_Max_Perimeter(EgMesh *pMesh, std::vector<EgPos> &Path);

/*!	\brief	오른쪽 주먹가운데점을 찾는 함수 */
EgVertex *SK_Pose5_Right_Fist_Center(EgMesh *pMesh);

/*!	\brief	왼쪽 주먹가운데점을 찾는 함수 */
EgVertex *SK_Pose5_Left_Fist_Center(EgMesh *pMesh);

/*!	\brief	왼쪽 팔꿈치아래점을 찾는 함수 */
EgVertex *SK_Pose5_Bottom_Olecranon(EgMesh *pMesh);

/*!	\brief	왼쪽 팔꿈치뒷점을 찾는 함수 */
EgVertex *SK_Pose5_Rear_Olecranon(EgMesh *pMesh);

/*!	\brief	왼쪽 등부위돌출점 찾는 함수 */
EgVertex *SK_Pose5_Left_Back_Protrusion(EgMesh *pMesh);

/*!	\brief	오른쪽 어깨점을 찾는 함수 */
EgVertex *SK_Pose5_Right_Acromion(EgMesh *pMesh);