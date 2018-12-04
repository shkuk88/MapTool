#pragma once
#include"XStd.h"

namespace SH
{
	D3DXVECTOR2					CreateUV(float x, float y, float TextureWidth, float TextureHeight);
	ID3D11ShaderResourceView*	CreateShaderResourceView(ID3D11Device* pDevice, const TCHAR* strFilePath);
}