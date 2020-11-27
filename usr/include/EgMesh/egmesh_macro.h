#pragma once

#define _PRINT_MSG

#ifdef _PRINT_MSG
#define print_msg(...) fprintf(stderr, __VA_ARGS__)
#else
#define print_msg(...) {}
#endif

// 에지 E의 시작 정점과 끝 정점을 구하는 매크로
#define SV(E)	(E)->m_pVert
#define EV(E)	(E)->m_pNext->m_pVert

// 정점 V의 위치를 구하는 매크로
#define POS(V)	(V)->m_Pos

// 에지 E가 포함된 삼각형을 구하는 매크로
#define FACE(E)	(E)->m_pFace

// 삼각형 F의 정점을 구하는 매크로
#define V0(F)	(F)->m_pEdge->m_pVert
#define V1(F)	(F)->m_pEdge->m_pNext->m_pVert
#define V2(F)	(F)->m_pEdge->m_pNext->m_pNext->m_pVert

// 삼각형 F의 정점의 좌표를 구하는 매크로
#define P0(F)	(F)->m_pEdge->m_pVert->m_Pos
#define P1(F)	(F)->m_pEdge->m_pNext->m_pVert->m_Pos
#define P2(F)	(F)->m_pEdge->m_pNext->m_pNext->m_pVert->m_Pos

// 삼각형 F의 에지를 구하는 매크로
#define E0(F)	(F)->m_pEdge
#define E1(F)	(F)->m_pEdge->m_pNext
#define E2(F)	(F)->m_pEdge->m_pNext->m_pNext

// 에지 E1, E2가 메이트 에지인지를 확인하는 매크로
#define IS_MATE_EDGE(E1, E2) (((E1)->m_pVert) == ((E2)->m_pNext->m_pVert))

// 벡터의 원소의 개수를 구하는 매크로
#define NUM(List) ((int)(List.size()))








