#ifndef SPHERE_MODEL_H
#define SPHERE_MODEL_H

#include "PrimitiveModel.h"

namespace Azul
{



	class SphereModel : public PrimitiveModel
	{
	public:


		SphereModel(const char* const pModelFileName);

		SphereModel() = delete;
		SphereModel(const SphereModel&) = delete;
		SphereModel& operator = (SphereModel&) = delete;
		virtual ~SphereModel();

	private:
		virtual void privCreateVAO(const char* const pModelFileName) override;

		void CreateSphere();

		// Data

	};
}

#endif

// --- End of File ---
