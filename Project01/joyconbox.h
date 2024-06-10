#pragma once
//============================================
//
//	�L���[�u�X�s�� [CubeSpin.h]
//	Author:sakamoto kai
//
//============================================
#ifndef _JOYCONBOX_H_
#define _JOYCONBOX_H_

#include "main.h"
#include "objmeshCube.h"

class CObject3D;

//�I�u�W�F�N�g���b�V���t�B�[���h�N���X
class CJoyconBox : public CObject
{
public:
	CJoyconBox(int nPriority = 4);
	~CJoyconBox();

	static CJoyconBox* Create();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; }
	void SetMove(D3DXVECTOR3 move) { m_move = move; }
	void SetSpinDistance(float Dis) { m_fSpinDistance = Dis; }
	float GetSpinDistance(void) { return m_fSpinDistance; }
	void SetSpinDisMove(float Move) { m_fSpinDisMove = Move; }
	float GetSpinDisMove(void) { return m_fSpinDisMove; }
	void SetSpinSpeedX(float Speed) { m_fSpinSpeed.x = Speed; }
	void SetSpinSpeedY(float Speed) { m_fSpinSpeed.y = Speed; }
	void SetSpinSpeedZ(float Speed) { m_fSpinSpeed.z = Speed; }
	D3DXVECTOR3 GetSpinSpeed(void) { return m_fSpinSpeed; }
protected:

private:
	int m_nNumCube;				//�L���[�u�̐�
	D3DXVECTOR3 m_pos;			//�ʒu
	D3DXVECTOR3 m_move;			//�ړ���
	float m_fSpinDistance;		//��]�̒��S����̋���
	float m_fSpinDisMove;		//��]�̒��S����̋����̈ړ���
	D3DXVECTOR3 m_fSpinSpeed;	//��]���x
};
#endif