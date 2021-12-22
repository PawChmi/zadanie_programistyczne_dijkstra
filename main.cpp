//Importowanie bibliotek
#include <iostream>
#include <fstream>
#include <string.h>
#include <string>
#include <limits>
#include <map>
#include <set>
#include <iterator>
#include <algorithm>
#include "dijkstra.h"

int main ( int argc, char **argv ) {
    std::string graphName, input, output;

    //wczytanie podanych parametrów    
    bool verbose = false;
    if (readParameters(argc, argv, graphName, input, output, verbose)) return 0;

    //wczytywanie grafu
    setSt nodeList;
    graph nodes = readGraph( graphName, nodeList );

    //wczytywanie wierzchołków
    vSt toCheck = readTask ( input );

    if ( verbose ) {
        std::cout << "graf: " << graphName << std::endl;
        std::cout << "wierzchołki: " << input << std::endl;
        std::cout << "plik wyjsciowy: " << output << std::endl;
        std::cout << "Dane wprowadzone do programu:"<<std::endl;
        for ( const auto& node : nodes ) {
            std::cout <<"nazwa "<< node.first << ":" <<std::endl;
            for ( const auto&  neighbour : node.second ) {
                std::cout << "\t-> " << neighbour.first << ": " << neighbour.second << std::endl;
            }
        }
        std::cout <<"wierzchołki do sprawdzenia:"<<std::endl;
        for ( auto x : toCheck ) {
            std::cout << x<< " ";
        }
        std::cout << std::endl;
    }
    std::ofstream outputStream ( output );
    if ( outputStream ) {
       
        for ( auto startingNode : toCheck ) {//dla każdego z zadanych wierzchołków
            std::cout << "Wierzchołek startowy: "<< startingNode <<std::endl;
            outputStream << "Wierzchołek startowy: "<< startingNode <<std::endl;
            if ( nodes.find ( startingNode ) ==nodes.end() ) { //sprawdzamy czy taki wierzchołek w ogóle istnieje
                std::cout << "Brak wierzchołka "<< startingNode <<" w grafie"<<std::endl;
                outputStream << "Brak wierzchołka "<< startingNode <<" w grafie"<<std::endl;
            } else {
                mapStSt previous;       //mapa poprzedników
                mapStDb distance;      //mapa odległości
                setSt checked = {};   // zbiór odwiedzonych wierzchołków
                vSt remaining = {};  // wektor wierzchołków nieodwiedzonych
                  //wektor zastosowany z uwagi na łatwiejsze sortowanie

                //przygotowanie map odległości i poprzedników
                for ( auto node = nodeList.begin(); node != nodeList.end(); ++node ) {
                    previous[*node] = ""; //poprzednik jako niezdefiniowany
                    remaining.push_back ( *node );
                    if ( *node != startingNode ) {
                        distance[*node] = std::numeric_limits<double>::infinity(); //ustaw odległość na nieskończoność
                    } else {
                        distance[*node] = 0; // ustaw odległośc początkowego na 0
                    }
                }
                while ( remaining.size() ) { //dopóki nie wszystkie wierzchołki odwiedzone
                    //sortowanie wektora remaining według odległości
                    std::sort ( remaining.begin(), remaining.end(), [&distance] ( std::string el1, std::string el2 ) {
                        return ( distance[el1]>distance[el2] );
                    } );

                    std::string closest = remaining.back();//wybieramy wierzchołek o najmniejszej odległości
                    remaining.pop_back();
                    checked.insert ( closest ); //przenosimy go do listy sprawdzonych

                    for ( auto compNode : nodes[closest] ) { //dla każdego z sąsiadujących wierzchołków
                        if ( std::find ( remaining.begin(), remaining.end(), compNode.first ) !=remaining.end() ) { //jeśli nie był jeszcze odwiedzony
                            if ( distance[compNode.first]>distance[closest] + compNode.second ) {
                                //sprawdzamy czy droga do niego przez ten wierzchołek jest krótsza niż do tej pory
                                distance[compNode.first] = distance[closest]+ compNode.second;//jeżeli tak to ustalamy odległość na drogę przez ten wierzchołek
                            }
                            previous[compNode.first] = closest; //i ustawiamy ten wierzchołek jako jego poprzednika
                        }
                    }
                }
                
                for ( auto node:checked ) {
                    writeResults(distance, previous, node, startingNode, outputStream, true);
                }
            }

        }

        outputStream.close();
    } else {
        //zwraca błąd jeżeli nie podano pliku wyjściowego
        helpMessage();
        std::cerr << "Nie podano pliku wyjściowego"<<std::endl;
        return 1;
    }


    return 0; //program wykonał się poprawnie
}

