#pragma once
#include "XQuadTreeIndex.h"


// XSpreatController에서 Texture를 생성하고
// XMultiTextureRender에서는 4장의 Alpha Blending된 텍스처를 혼합해서 Render
class XMultiTextureRender:public XQuadTreeIndex
{
public:

public:
	XMultiTextureRender();
	virtual ~XMultiTextureRender();
};

