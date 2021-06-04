#ifndef ASSET_LOADER_H
#define ASSET_LOADER_H

#include "Models/Model.h"
#include "Models/FbxModel.h"
#include "Models/FbxSkinnedModel.h"

namespace Azul {

	class AssetLoader
	{
	public:


		static FBXModel* LoadModel(const char* const pkgName);
		static FBXSkinnedModel* LoadAnimModel(const char* const pkgName);
		static void LoadAnim(const char* const pkgName);

	};
}
#endif