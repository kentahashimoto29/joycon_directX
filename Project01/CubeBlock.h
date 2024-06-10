//============================================
//
//	キューブブロック [CubeBlock.h]
//	Author:sakamoto kai
//
//============================================
#ifndef _CUBEBLOCK_H_
#define _CUBEBLOCK_H_

#include "main.h"
#include "objmeshCube.h"

//オブジェクトメッシュフィールドクラス
class CCubeBlock : public CObjmeshCube
{
public:
	CCubeBlock(int nPriority = 3);
	~CCubeBlock();

	static CCubeBlock* Create(void);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	bool CollisionBlock(D3DXVECTOR3* pPos, D3DXVECTOR3 pPosOld, D3DXVECTOR3* pMove, D3DXVECTOR3* pObjMove, D3DXVECTOR3 Size, bool* bJump, COLLISION XYZ);
	bool CollisionBlockUp(D3DXVECTOR3* pPos, D3DXVECTOR3 pPosOld, D3DXVECTOR3* pMove, D3DXVECTOR3* pObjMove, D3DXVECTOR3 Size, bool* bJump, COLLISION XYZ);

protected:

private:

};
#endif