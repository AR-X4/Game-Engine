#ifndef DIAMOND_MODEL_H
#define DIAMOND_MODEL_H

#include "PrimitiveModel.h"

namespace Azul
{



	class DiamondModel : public PrimitiveModel
	{
	public:


		DiamondModel(const char* const pModelFileName);

		DiamondModel() = delete;
		DiamondModel(const DiamondModel&) = delete;
		DiamondModel& operator = (DiamondModel&) = delete;
		virtual ~DiamondModel();

	private:
		virtual void privCreateVAO(const char* const pModelFileName) override;

		void CreateNormalData();

		// Data

	};
}

#endif

// --- End of File ---
