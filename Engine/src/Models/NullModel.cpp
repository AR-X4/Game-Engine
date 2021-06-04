

#include "NullModel.h"

namespace Azul
{
	NullModel::NullModel(const char* const modelFileName)
		: Model(Name::NULLMODEL)
	{
		AZUL_UNUSED_VAR(modelFileName);
	}

	NullModel::~NullModel()
	{
		// remove anything dynamic here
	}


}

// --- End of File ---
