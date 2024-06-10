//============================================
//
//	Æ€ [aim.h]
//	Author:‹Ø“÷š¬Š}Œ´š‹Ø“÷
//
//============================================

#include "main.h"
#include "objectBillboard.h"


// Æ€ƒNƒ‰ƒX
class CAim : public CObjectBillboard
{
public:
	CAim(int nPriority = 6);
	~CAim();

	static CAim* Create();

	HRESULT Init();
	void Uninit();
	void Update();
	void Draw();

private:
	void CursorMove();
	void CursorColor();
	void CursorLimit();
	void Limit(D3DXVECTOR3* rot, D3DXVECTOR3* move);

	struct SInfo
	{
		D3DXVECTOR3 rot;
		D3DXVECTOR3 rotOld;
		D3DXVECTOR3 posOld;
	};

	SInfo m_info;
};