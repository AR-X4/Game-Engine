#ifndef FBX_MODEL_H
#define FBX_MODEL_H

#include "Model.h"

namespace Azul
{
	class FBXModel : public Model
	{
    public:
        FBXModel(const Vert_xyzuvn* vertBuffer, const Tri_index* triBuffer, unsigned int _numVerts, unsigned int _numTris, Name nameIn = Name::UNINITIALIZED);
        FBXModel(const FBXModel&) = delete;
        FBXModel& operator = (FBXModel&) = delete;
        virtual ~FBXModel() = default;


    private:
        // load VAO
        void privCreateVAO(const Vert_xyzuvn* vertBuffer, const Tri_index* triBuffer);
	};
}
#endif