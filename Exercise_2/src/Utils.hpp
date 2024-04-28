#pragma once

#include <iostream>
#include "PolygonalMesh.hpp"

using namespace std;

namespace PolygonalLibrary {
/// Import the polygonal mesh and test if the mesh is correct
/// mesh: a PolygonalMesh struct
/// return the result of the reading, true if is success, false otherwise
/// the output shows all the markers =! 0 and their Id
bool ImportMesh(const string &filepath,
                PolygonalMesh& mesh);

/// Import the Cell0D properties from Cell0Ds.csv file
/// mesh: a PolygonalMesh struct
/// return the result of the reading, true if is success, false otherwise
bool ImportCell0Ds(const string &filename,
                   PolygonalMesh& mesh);

/// Import the Cell1D properties from Cell1Ds.csv file
/// mesh: a PolygonalMesh struct
/// return the result of the reading, true if is success, false otherwise
bool ImportCell1Ds(const string &filename,
                   PolygonalMesh& mesh);

/// Import the Cell2D properties from Cell2Ds.csv file
/// mesh: a PolygonalMesh struct
/// return the result of the reading, true if is success, false otherwise
bool ImportCell2Ds(const string &filename,
                   PolygonalMesh& mesh);

/// Test if the length every edge in cell1D is not 0
/// mesh: a PolygonalMesh struct, tol: tolerance for the length
/// return the result of the reading, true if there aren't, false otherwise
bool CheckNonZeroEdgeLength(PolygonalMesh& mesh, double tol);

/// Test if the Area of every polygon in cell2D is not 0
/// mesh: a PolygonalMesh struct, tol: tolerance for the area
/// return the result of the reading, true if there aren't, false otherwise
/// the output shows also the total area of the mesh (it should be = 1)
bool CheckNonZeroPolygonArea(PolygonalMesh& mesh, double tol);

}
