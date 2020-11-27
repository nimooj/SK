#pragma once

#define _PRINT_MSG

#ifdef _PRINT_MSG
#define print_msg(...) fprintf(stderr, __VA_ARGS__)
#else
#define print_msg(...) {}
#endif

// ���� E�� ���� ������ �� ������ ���ϴ� ��ũ��
#define SV(E)	(E)->m_pVert
#define EV(E)	(E)->m_pNext->m_pVert

// ���� V�� ��ġ�� ���ϴ� ��ũ��
#define POS(V)	(V)->m_Pos

// ���� E�� ���Ե� �ﰢ���� ���ϴ� ��ũ��
#define FACE(E)	(E)->m_pFace

// �ﰢ�� F�� ������ ���ϴ� ��ũ��
#define V0(F)	(F)->m_pEdge->m_pVert
#define V1(F)	(F)->m_pEdge->m_pNext->m_pVert
#define V2(F)	(F)->m_pEdge->m_pNext->m_pNext->m_pVert

// �ﰢ�� F�� ������ ��ǥ�� ���ϴ� ��ũ��
#define P0(F)	(F)->m_pEdge->m_pVert->m_Pos
#define P1(F)	(F)->m_pEdge->m_pNext->m_pVert->m_Pos
#define P2(F)	(F)->m_pEdge->m_pNext->m_pNext->m_pVert->m_Pos

// �ﰢ�� F�� ������ ���ϴ� ��ũ��
#define E0(F)	(F)->m_pEdge
#define E1(F)	(F)->m_pEdge->m_pNext
#define E2(F)	(F)->m_pEdge->m_pNext->m_pNext

// ���� E1, E2�� ����Ʈ ���������� Ȯ���ϴ� ��ũ��
#define IS_MATE_EDGE(E1, E2) (((E1)->m_pVert) == ((E2)->m_pNext->m_pVert))

// ������ ������ ������ ���ϴ� ��ũ��
#define NUM(List) ((int)(List.size()))








