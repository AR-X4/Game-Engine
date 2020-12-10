

#ifndef NULL_MODEL_H
#define NULL_MODEL_H

#include "Model.h"

namespace Azul
{
	class NullModel : public Model
	{
	public:
		NullModel(const char* const modelFileName);
		virtual ~NullModel();

		// tells the compiler do not create or allow it to be used, c++11
		NullModel(const NullModel&) = delete;
		NullModel& operator=(const NullModel& other) = delete;




	};

}

#endif

// --- End of File ---
