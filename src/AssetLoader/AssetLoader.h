#ifndef ASSET_LOADER_H
#define ASSET_LOADER_H

#include "Models//Model.h"

namespace Azul {

	class AssetLoader
	{
	public:


		static Model* LoadModel(const char* const pkgName);
		static void LoadAnim(const char* const pkgName);

	};
}
#endif