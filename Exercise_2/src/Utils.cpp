#include "Utils.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>

namespace PolygonalLibrary {

bool ImportCell0Ds(const string &filename,
                   PolygonalMesh& mesh)
{

    ifstream file;
    file.open(filename);

    if(file.fail())
        return false;

    list<string> listLines;
    string line;
    while (getline(file, line))
    {
        std::replace(line.begin(),line.end(),';',' '); // mi serve per comoità non avere come separatore il ;
        listLines.push_back(line);
    }

    file.close();

    listLines.pop_front(); // rimuovo prima riga (intestazione)

    mesh.NumberCell0D = listLines.size(); //ogni riga rappresenta una cella

    if (mesh.NumberCell0D == 0)
    {
        cerr << "There is no cell 0D" << endl;
        return false;
    }

    mesh.Cell0DId.reserve(mesh.NumberCell0D);
    mesh.Cell0DCoordinates.reserve(mesh.NumberCell0D);

    for (const string& line : listLines)
    {
        istringstream converter(line);

        unsigned int id;
        unsigned int marker;
        Vector2d coord;

        converter >>  id >> marker >> coord(0) >> coord(1);

        mesh.Cell0DId.push_back(id);
        mesh.Cell0DCoordinates.push_back(coord);

        if( marker != 0)
        {

            auto ret = mesh.Cell0DMarkers.insert({marker, {id}});
            if(!ret.second)
                (ret.first)->second.push_back(id);
        }

    }
    file.close();
    return true;

}

bool ImportCell1Ds(const string &filename,
                   PolygonalMesh& mesh)
{

    ifstream file;
    file.open(filename);

    if(file.fail())
        return false;

    list<string> listLines;
    string line;
    while (getline(file, line))
    {
        std::replace(line.begin(),line.end(),';',' ');
        listLines.push_back(line);
    }

    file.close();

    listLines.pop_front();

    mesh.NumberCell1D = listLines.size();

    if (mesh.NumberCell1D == 0)
    {
        cerr << "There is no cell 1D" << endl;
        return false;
    }

    mesh.Cell1DId.reserve(mesh.NumberCell1D);
    mesh.Cell1DVertices.reserve(mesh.NumberCell1D);

    for (const string& line : listLines)
    {
        istringstream converter(line);

        unsigned int id;
        unsigned int marker;
        Vector2i vertices;

        converter >>  id >> marker >> vertices(0) >> vertices(1);

        mesh.Cell1DId.push_back(id);
        mesh.Cell1DVertices.push_back(vertices);

        if( marker != 0)
        {
            auto ret = mesh.Cell1DMarkers.insert({marker, {id}});
            if(!ret.second)
                (ret.first)->second.push_back(id);
        }
    }

    file.close();

    return true;
}

bool ImportCell2Ds(const string &filename,
                   PolygonalMesh& mesh)
{

    ifstream file;
    file.open(filename);

    if(file.fail())
        return false;

    list<string> listLines;
    string line;
    while (getline(file, line))
    {
        std::replace(line.begin(),line.end(),';',' ');
        listLines.push_back(line);
    }
    file.close();

    listLines.pop_front();

    mesh.NumberCell2D = listLines.size();

    if (mesh.NumberCell2D == 0)
    {
        cerr << "There is no cell 2D" << endl;
        return false;
    }

    mesh.Cell2DId.reserve(mesh.NumberCell2D);
    mesh.Cell2DNumVertices.reserve(mesh.NumberCell2D);
    mesh.Cell2DVertices.reserve(mesh.NumberCell2D);
    mesh.Cell2DNumEdges.reserve(mesh.NumberCell2D);
    mesh.Cell2DEdges.reserve(mesh.NumberCell2D);

    for (const string& line : listLines)
    {
        istringstream converter(line);

        unsigned int id;
        unsigned int marker;
        unsigned int num_vertices;
        unsigned int num_edges;

        converter >>  id;
        converter >> marker;

        converter >> num_vertices;
        vector<unsigned int> vertices(num_vertices);
        for(unsigned int i = 0; i < num_vertices; i++)
            converter >> vertices[i];

        converter >> num_edges;
        vector<unsigned int> edges(num_edges);
        for(unsigned int i = 0; i < num_edges; i++)
            converter >> edges[i];

        mesh.Cell2DId.push_back(id);
        mesh.Cell2DNumVertices.push_back(num_vertices);
        mesh.Cell2DVertices.push_back(vertices);
        mesh.Cell2DNumEdges.push_back(num_edges);
        mesh.Cell2DEdges.push_back(edges);
    }

    return true;
}

bool ImportMesh(const string& filepath,
                PolygonalMesh& mesh)
{

    if(!ImportCell0Ds(filepath + "/Cell0Ds.csv",
                       mesh))
    {
        return false;
    }
    else
    {
        cout << "Cell0D marker:" << endl;
        for(auto it = mesh.Cell0DMarkers.begin(); it != mesh.Cell0DMarkers.end(); it++)
        {
            cout << "key:\t" << it -> first << "\t values:";
            for(const unsigned int id : it -> second)
                cout << "\t" << id;

            cout << endl;
        }
    }

    if(!ImportCell1Ds(filepath + "/Cell1Ds.csv",
                       mesh))
    {
        return false;
    }
    else
    {
        cout << "Cell1D marker:" << endl;
        for(auto it = mesh.Cell1DMarkers.begin(); it != mesh.Cell1DMarkers.end(); it++)
        {
            cout << "key:\t" << it -> first << "\t values:";
            for(const unsigned int id : it -> second)
                cout << "\t" << id;

            cout << endl;
        }
    }
    if(!ImportCell2Ds(filepath + "/Cell2Ds.csv",
                       mesh))
    {
        return false;
    }
    return true;
}


bool CheckNonZeroEdgeLength(PolygonalMesh& mesh, double tol)
{
    for (const auto& cell1D : mesh.Cell1DVertices) {
        Vector2d origin = mesh.Cell0DCoordinates[cell1D(0)];
        Vector2d end = mesh.Cell0DCoordinates[cell1D(1)];
        Vector2d edge = end - origin;

        if (edge.norm() <= max(numeric_limits<double>::epsilon(), tol)) {
            return false; // Se la norma del lato è 0, restituisci false
        }
    }
    return true; // Se tutte le lunghezze dei lati sono non nulle, restituisci true
}


bool CheckNonZeroPolygonArea(PolygonalMesh& mesh,double tol)
{
    double Area_Finale = 0;
    for (const auto& cell2D : mesh.Cell2DVertices) {
        if (cell2D.size() < 3) {
            return false; // Se una cella 2D ha meno di 3 vertici, non è un poligono valido
        }

        unsigned int n = cell2D.size();
        double area = 0.0;
        list<Vector2d> listVectors; // Creo una lista che contenga le coordinate di tutti i vertici del poligono
        for (unsigned int i = 0; i < n; ++i) {
            listVectors.push_back(mesh.Cell0DCoordinates[cell2D[i]]);
        }
        auto it = listVectors.begin(); // Iteratore al primo elemento
        auto jt = next(it); // Iteratore al secondo elemento

        for (unsigned int i = 0; i < n; ++i, ++it, ++jt) {
            if (jt == listVectors.end()) // Se l'iteratore jt raggiunge la fine della lista, torna all'inizio
                jt = listVectors.begin();

            area += (it->x() * jt->y() - jt->x() * it->y()); // (Xi * Yi+1) - (Xi+1 * Yi)
        }

        double areaTotale = abs(area)*0.5; // Per trovare l'area dei triangoli devo ancora moltiplicare per 1/2 e prendere il valore positivo
        Area_Finale += areaTotale;
        if (areaTotale <= max(numeric_limits<double>::epsilon(), tol*tol)) {
            return false; // Se l'area del poligono è più piccola di una certa tolleranza, restituisci false
        }
    }
    cout << "Area totale della somma di tutte le aree dei poligoni: " << Area_Finale << endl;
    return true; // Se tutte le aree dei poligoni sono diverse da zero, restituisci true
}

}
