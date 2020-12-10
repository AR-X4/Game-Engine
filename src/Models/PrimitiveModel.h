#ifndef PRIMITIVE_MODEL_H
#define PRIMITIVE_MODEL_H


#include "Model.h"

namespace Azul {

    class PrimitiveModel : public Model
    {

    public:
        PrimitiveModel(Name Namein);
        PrimitiveModel(const PrimitiveModel&) = delete;
        PrimitiveModel& operator = (PrimitiveModel&) = delete;
        virtual ~PrimitiveModel() = default;


    protected:
        // load VAO
        virtual void privCreateVAO(const char* const pModelFileName) = 0;

        Vect CalcNormalVect(const Vect& x, const Vect& y, const Vect& z);


    };
}
#endif