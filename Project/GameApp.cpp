/*************************************************************************//*!

					@file	GameApp.cpp
					@brief	基本ゲームアプリ。

															@author	濱田　享
															@date	2014.05.14
*//**************************************************************************/

//INCLUDE
#include	"GameApp.h"
#include    "Card.h"
#include    "GameDefine.h"
#include    "Player.h"

CCard       g_Card;

CTexturePtr g_pTexture{ nullptr };

CPlayer     g_Player;

/*************************************************************************//*!
		@brief			アプリケーションの初期化
		@param			None

		@return			TRUE		成功<br>
						それ以外	失敗、エラーコードが戻り値となる
*//**************************************************************************/
MofBool CGameApp::Initialize(void) {

	CUtilities::SetCurrentDirectory("Resource");

	if (!CTextureAsset::Load("cards", "cards.png"))
	{
		return FALSE;
	}

	g_pTexture = TextureAsset("cards");

	g_Card.Shuffle();

	return TRUE;
}
/*************************************************************************//*!
		@brief			アプリケーションの更新
		@param			None

		@return			TRUE		成功<br>
						それ以外	失敗、エラーコードが戻り値となる
*//**************************************************************************/
MofBool CGameApp::Update(void) {
	//キーの更新
	g_pInput->RefreshKey();

	if (g_pInput->IsKeyPush(MOFKEY_1))
	{
		for (int i = 0; i < 5; i++)
		{
			int size = g_Card.m_Cards.size();
			g_Player.SetCard(i, g_Card.m_Cards[size - 1]);
			g_Card.m_Cards.pop_back();
		}
	}

	return TRUE;
}
/*************************************************************************//*!
		@brief			アプリケーションの描画
		@param			None

		@return			TRUE		成功<br>
						それ以外	失敗、エラーコードが戻り値となる
*//**************************************************************************/
MofBool CGameApp::Render(void) {
	//描画開始
	g_pGraphics->RenderStart();
	//画面のクリア
	g_pGraphics->ClearTarget(0.05f, 0.6f, 0.12f, 0.0f, 1.0f, 0);

	CRectangle rect = CCard::GetCardRect();
	rect.Translation(Vector2(CardSizeW * 1, CardSizeH * 4));
	for (int i = 0; i < g_Card.m_Cards.size(); i++)
	{
		g_pTexture->Render(g_pGraphics->GetTargetWidth() * 0.5f, g_pGraphics->GetTargetHeight() * 0.5f - i, rect, TEXALIGN_CENTERCENTER);
	}

	//描画の終了
	g_pGraphics->RenderEnd();
	return TRUE;
}
/*************************************************************************//*!
		@brief			アプリケーションの解放
		@param			None

		@return			TRUE		成功<br>
						それ以外	失敗、エラーコードが戻り値となる
*//**************************************************************************/
MofBool CGameApp::Release(void) {

	CTextureAsset::Release();
	
	return TRUE;
}