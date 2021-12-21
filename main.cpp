// Dijkstra
//  Napisać  program,  do  znajdowania  najkrótszych  ścieżek  między  zadanym
// wierzchołkiem  grafu  a  wszystkimi  pozostałymi  wierzchołkami. W programie
// powinien zostać zastosowany algorytm Dijkstry.
// Plik z grafem ma następującą postać:
// • każda krawędź jest podana w osobnej linii,
// • przykład  krawędzi  skierowanej:  4  ->  5  :  54.4  oznacza  krawędź
// skierowaną od wierzchołka 4 do 5,
// • waga (długość) krawędzi wynosi 54.4,
// • przykład krawędzi nieskierowanej: 3  -  6  :  12.5  oznacza krawędź
// nieskierowaną między wierzchołkami 3 i 6 o wadze (długości) 12.5,
// • w  pliku mogą wystąpić puste linie  oraz  dodatkowe  (nadmiarowe)  znaki
// białe.
// Przykładowy plik z grafem:
// 3 -> 2 : 54.5
// 12 -> 3 : 4.56
// 2 -> 5 : 34.65
// 5 -> 3 : 2.4
// 3 -> 12 : 1.0
// Drugim plikiem wejściowym programu, jest plik z numerami wierzchołków, dla
// których chcemy wyznaczyć najkrótsze odległości do pozostałych wierzchołków.
// Przykładowy plik:
// 2 6 12
// W pliku wynikowym zostaną zapisane trasy o minimalnej długości dla zadanych
// wierzchołków, np.
// Wierzchołek startowy: 2
// 2 -> 5 -> 3 : 37.05
// 2 -> 5 : 34.65
// 2 -> 5 -> 3 -> 12 : 38.05
// Wierzchołek startowy: 6
// Brak wierzchołka 6 w grafie
// Wierzchołek startowy: 12
// 12 -> 3 : 4.56
// 12 -> 3 -> 2 : 59.06
// 12 -> 3 -> 2 -> 5 : 93.76
// Program  uruchamiany  jest  z  linii  poleceń  z  wykorzystaniem  następujących
// przełączników (kolejność przełączników jest dowolna):
// -g plik wejściowy z grafem
// -w plik w3jściowy z wierzchołkami
// -o plik wyjściowy z wynikami

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
//#include "dijkstra.cpp"


int main ( int argc, char **argv )
{
        std::vector<std::string>toCheck;
        std::map<std::string, std::map<std::string, double>> nodes;
        std::string graph, input, output;
        bool verbose = false;
        //wcyztanie podanych parametrów 
        for ( int i = 1; i < argc-1; i++ ) {
                if ( not strcmp ( argv[i],"-g" ) ) { //plik z grafem
                        graph = argv[i+1];
                } else if ( not strcmp ( argv[i],"-w" ) ) { //plik z wierzchołkami
                        input = argv[i+1];
                } else if ( not strcmp ( argv[i],"-o" ) ) { //plik wyjściowy
                        output = argv[i+1];
                } else if ( not strcmp ( argv[i],"-v" ) ) { //verbose, wypisuj doatkowe informacje
                        verbose = true;
                } else if ( not strcmp ( argv[i],"-h" ) ) { //pomoc
                        helpMessage();
                        return 0;
                }
        }
        if(not strcmp ( argv[argc-1],"-v" ))verbose = true;
        
        if ( output == "" ) {
                //zwraca błąd jeżeli nie podano pliku wyjściowego
                helpMessage();
                std::cerr << "Nie podano pliku wyjściowego"<<std::endl;
                return 1;
        }
        //wczytywanie grafu
        std::set<std::string> nodeList;
        std::ifstream graphStream ( graph );
        std::string poczatek, polaczenie, koniec, dwukropek;
        double waga;
        if ( verbose ) std::cout<<"graf: " << graph << std::endl;
        if ( graphStream ) {
                while ( graphStream >> poczatek >> polaczenie >> koniec >> dwukropek >> waga ) {
                        nodeList.insert ( poczatek );
                        nodes[poczatek][koniec] = waga;
                        if ( polaczenie == "->" )
                                nodes[poczatek][koniec] = waga;
                }
                graphStream.close();
        } else {
                //zwraca błąd jeżeli nie podano pliku wejściowego z grafem
                helpMessage();
                std::cerr << "Nie podano pliku z grafem"<<std::endl;
                return 1;
        }

        //wczytywanie wierzchołków
        
        std::ifstream inputStream ( input );
        if ( verbose ) std::cout<<"wierzchołki: "  << input << std::endl;
        if ( inputStream ) {
                std::string wierzcholek;
                while ( inputStream>>wierzcholek ) {
                        toCheck.push_back ( wierzcholek );
                }
                inputStream.close();
        } else {
                //zwraca błąd jeżeli nie podano pliku wejściowego z wierzchołkami
                helpMessage();
                std::cerr << "Nie podano pliku z wierzchołkami"<<std::endl;
                return 1;
        }

        std::ofstream outputStream ( output );
        if ( outputStream.is_open() ) {
                if ( verbose ) std::cout<<"plik wyjsciowy: " << output << std::endl;
                for ( auto startingNode : toCheck ) {//dla każdego z zadanych wierzchołków
                        std::cout << "Wierzchołek startowy: "<< startingNode <<std::endl;
                        outputStream << "Wierzchołek startowy: "<< startingNode <<std::endl;
                        if ( nodes.find ( startingNode ) ==nodes.end() ) {
                                std::cout << "Brak wierzchołka "<< startingNode <<" w grafie"<<std::endl;
                                outputStream << "Brak wierzchołka "<< startingNode <<" w grafie"<<std::endl;
                        } else {
                                std::map<std::string, std::string> previous;
                                std::map<std::string, double> dists;
                                std::vector<connection> distances;
                                std::set<std::string> S = {};
                                std::vector<std::string> Q = {};
                                //przygotowanie
                                for ( auto node = nodeList.begin(); node != nodeList.end(); ++node ) {
                                        previous[*node] = "";
                                        Q.push_back ( *node );
                                        if ( *node != startingNode ) {
                                                dists[*node] = std::numeric_limits<double>::infinity();
                                        } else {
                                                dists[*node] = 0;
                                        }
                                }
                                while ( Q.size() ) {
                                        std::sort ( Q.begin(), Q.end(), [&dists] ( std::string x, std::string y ) {
                                                return sortByDistance ( x, y, dists );
                                        } );
                                        std::string closest = Q.back();
                                        Q.pop_back();
                                        S.insert ( closest );
                                        for ( auto compNode : nodes[closest] ) {
                                                if ( std::find ( Q.begin(), Q.end(), compNode.first ) !=Q.end() ) {
                                                        if ( dists[compNode.first]>dists[closest] + compNode.second ) {
                                                                dists[compNode.first] = dists[closest]+ compNode.second;
                                                        }
                                                        previous[compNode.first] = closest;
                                                }
                                        }
                                }
                                for ( auto x:S ) {
                                        if ( dists[x] ) {
                                                std::cout << traceback ( previous, x, startingNode );
                                                outputStream << traceback ( previous, x, startingNode );
                                                std::cout << " : " << dists[x] <<std::endl;
                                                outputStream << " : " << dists[x] <<std::endl;
                                        }
                                }
                        }

                        /*
                                                                                std::cout << " -> " << shrtPath.points[i];
                                                                                outputStream << " -> " << shrtPath.points[i];

                                                                        std::cout <<" : "<< shrtPath.lenght<<std::endl;
                                                                        outputStream <<" : "<< shrtPath.lenght<<std::endl;

                                                                        std::cout << "Nie istnieje ścieżka do "<<Pair.first<<std::endl;
                                                                        outputStream << "Nie istnieje ścieżka do "<<Pair.first<<std::endl;*/
                }

                outputStream.close();
        }


        if ( verbose ) {
                std::cout << "Dane wprowadzone do programu:"<<std::endl;
                for ( const auto& P : nodes ) {
                        std::cout <<"nazwa "<< P.first << ":" <<std::endl;

                        for ( const auto&  Q : P.second ) {
                                std::cout << "\twierzchołek: " << Q.first << "; waga: " << Q.second << std::endl;
                        }
                }
                std::cout <<"wierzchołki do sprawdzenia:"<<std::endl;
                for ( auto x : toCheck ) {
                        std::cout << "\t"<< x;
                }
                std::cout << std::endl;
        }
        return 0;
}

