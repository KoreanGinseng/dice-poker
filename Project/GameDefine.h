#pragma once

#include "MofSip.h"

using CTextureAsset = CAssetBase<std::string, CTexture>;

using CTexturePtr = std::shared_ptr<CTexture>;

#define TextureAsset(key) CTextureAsset::GetAsset(key)
