#include <iostream>
#include "PolygonalMesh.hpp"
#include "Utils.hpp"

using namespace std;
using namespace Eigen;
using namespace PolygonalLibrary;

int main()
{
    PolygonalMesh mesh;

    string filepath = "PolygonalMesh";
    if(!ImportMesh(filepath,
                    mesh))
    {
        return 1;
    }
    else if(!CheckNonZeroEdgeLength(mesh, 1e-16))
    {
        return 2;
    }
    else if(!CheckNonZeroPolygonArea(mesh, 1e-16))
    {
        return 3;
    }

    return 0;
}
