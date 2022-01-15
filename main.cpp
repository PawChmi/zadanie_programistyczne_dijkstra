/** @file */
//Importowanie bibliotek
#include <iostream>
#include <fstream>
#include <string.h>
#include <string>
#include <limits>
#include <map>
#include <set>
#include <iterator>
#include "dijkstra.h"

int main ( int argc, char **argv )
{
    std::string graphName, taskName, outputName;
    //wczytanie podanych parametrów
    bool verbose = false;
    if ( readParameters ( argc, argv, graphName, taskName, outputName, verbose ) ) {
        helpMessage();
        return 1;
    }
    //wczytywanie grafu
    setSt nodeList;
    graph nodes = readGraph ( graphName, nodeList );
    if(nodes.empty()){
        std::cerr << "Graf jest pusty." << std::endl;
        return 1;
    }
    //wczytywanie wierzchołków
    vSt toCheck = readTask ( taskName );
    if ( verbose ) printInput ( graphName, taskName, outputName, nodes, toCheck );
    std::ofstream outputStream ( outputName );
    if ( outputStream ) {
        for ( auto startingNode : toCheck ) {//dla każdego z zadanych wierzchołków
            if ( verbose ) std::cout << "Wierzchołek startowy: "<< startingNode <<std::endl;
            outputStream << "Wierzchołek startowy: "<< startingNode <<std::endl;
            if ( nodes.find ( startingNode ) ==nodes.end() ) { //sprawdzamy czy taki wierzchołek w ogóle istnieje
                if ( verbose ) std::cout << "Brak wierzchołka "<< startingNode <<" w grafie"<<std::endl;
                outputStream << "Brak wierzchołka "<< startingNode <<" w grafie"<<std::endl;
            } else {
                mapStSt previous;       //mapa poprzedników
                mapStDb distance;      //mapa odległości
                setSt checked = {};   // zbiór odwiedzonych wierzchołków
                vSt remaining = {};  // wektor wierzchołków nieodwiedzonych
                //wektor zastosowany z uwagi na łatwiejsze sortowanie
                //przygotowanie map odległości i poprzedników
                prepareValues ( startingNode, nodeList,distance, previous,  remaining );
                //wyszukanie najkrótszych ścieżek
                dijkstra ( remaining, checked, distance, previous, nodes );
                //wypisanie wyników dla każdego wierzchołka w grafie
                for ( auto node:checked ) {
                    
                    writeResults ( distance, previous, node, startingNode, outputStream, verbose );
                }
            }
        }
        outputStream.close();
    } else {
        std::cerr << "Błąd w otwieraniu pliku wyjściowego"<<std::endl;
        return 1;
    }
    return 0; //program wykonał się poprawnie
}

