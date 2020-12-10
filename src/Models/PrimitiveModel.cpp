#include "PrimitiveModel.h"

namespace Azul {
    PrimitiveModel::PrimitiveModel(Name nameIn)
        :Model(nameIn)
    {
    }

    Vect PrimitiveModel::CalcNormalVect(const Vect& x, const Vect& y, const Vect& z)
    {
        Vect vect1 = y - x;
        Vect vect2 = z - x;
        return vect1.cross(vect2).getNorm();
    }

}
