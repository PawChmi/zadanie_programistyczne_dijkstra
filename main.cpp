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

    std::map<std::string, mapStDb> nodes;
    std::string graph, input, output;
    bool verbose = false;
    //wczytanie podanych parametrów
    for ( int i = 1; i < argc-1; i++ ) {
        if ( not strcmp ( argv[i],"-g" ) ) { //plik z grafem
            graph = argv[i+1];
        } else if ( not strcmp ( argv[i],"-w" ) ) { //plik z wierzchołkami
            input = argv[i+1];
        } else if ( not strcmp ( argv[i],"-o" ) ) { //plik wyjściowy
            output = argv[i+1];
        } else if ( not strcmp ( argv[i],"-v" ) ) { //verbose, wypisuj dodatkowe informacje
            verbose = true;
        } else if ( not strcmp ( argv[i],"-h" ) ) { //pomoc
            helpMessage();
            return 0;
        }
    }
    if ( not strcmp ( argv[argc-1],"-v" ) ) verbose = true; //verbose, wypisuj dodatkowe informacje
    //wypisanie wprowadzonych danych jeżeli podano parametr verobse
    if (verbose) {
        std::cout << "plik wyjsciowy: " << output << std::endl;
        std::cout << "wierzchołki: " << input << std::endl;
        std::cout << "graf: " << graph << std::endl;
        std::cout << "Dane wprowadzone do programu:" << std::endl;
        for (const auto& node : nodes) {
            std::cout << "nazwa " << node.first << ":" << std::endl;
            for (const auto& neighbour : node.second) {
                std::cout << "\t-> " << neighbour.first << ": " << neighbour.second << std::endl;
            }
        }
        std::cout << "wierzchołki do sprawdzenia:" << std::endl;
        for (auto x : toCheck) {
            std::cout << x << " ";
        }
        std::cout << std::endl;
    }
    //wczytywanie grafu
   
    setSt nodeList;
    std::ifstream graphStream ( graph );
    if ( graphStream ) {
        std::string poczatek, polaczenie, koniec, dwukropek;
        double waga;
        while ( graphStream >> poczatek >> polaczenie >> koniec >> dwukropek >> waga ) {
            nodeList.insert ( poczatek ); //dodajemy do zbioru wierzchołek początkowy i końcowy
            nodeList.insert ( koniec );  //ponieważ może być wierzchołek zdefiniwowany tylko jako końcowy
            nodes[poczatek][koniec] = waga;
            if ( polaczenie == "-" ) { // jeżeli krawędź nieskierowana
                nodes[koniec][poczatek] = waga;//to połączenie w drugą stronę też istnieje
            }
        }
        graphStream.close();
    } else {
        //zwraca błąd jeżeli nie podano pliku wejściowego z grafem
        helpMessage();
        std::cerr << "Nie podano pliku z grafem"<<std::endl;
        return 1;
    }

    //wczytywanie wierzchołków
    
    vSt toCheck = readTask ( input );


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
                    if ( distance[node] ) {
                        if ( distance[node] == std::numeric_limits<double>::infinity() ) { //zabezpieczenie przed wierzchołkami izolowanymi
                            std::cout << "Brak drogi do wierzchołka " << node <<std::endl;
                        } else {
                            std::cout << traceback ( previous, node, startingNode );
                            outputStream << traceback ( previous, node, startingNode );
                            std::cout << " : " << distance[node] <<std::endl;
                            outputStream << " : " << distance[node] <<std::endl;
                        }
                    }
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

