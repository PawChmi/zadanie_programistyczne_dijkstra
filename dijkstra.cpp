#include "dijkstra.h"
#include <string>
#include <vector>
#include <iostream>
#include <limits>
#include <string.h>
#include <algorithm>

void helpMessage() {
    std::cout << "Ten program znajduje najkrótszą drogę miedzy wierzchołkami wykorzystując algorytm Dijksry" << std::endl;
    std::cout << "Program przyjmuje następujące argumenty:"<<std::endl;
    std::cout << "-g \'nazwa pliku\' plik wejściowy zawierający graf."<<std::endl;
    std::cout << "-w \'nazwa pliku\' plik wejściowy zawierający listę wierzchołków do zbadania."<<std::endl;
    std::cout << "-o \'nazwa pliku\' plik wyjściowy."<<std::endl;
    std::cout << "-v Verbose, tryb wypisywania dodatkowych informacji."<<std::endl;
    std::cout << "-h wypisuje tą wiadomość pomocy."<<std::endl;
}

void printInput ( std::string &gName, std::string &wName, std::string &oName, graph &connections, vSt &tasks ) {
    std::cout << "graf: " << gName << std::endl;
    std::cout << "wierzchołki: " << wName << std::endl;
    std::cout << "plik wyjsciowy: " << oName << std::endl;
    std::cout << "Dane wprowadzone do programu:"<<std::endl;
    for ( const auto& node : connections ) {
        std::cout <<"nazwa "<< node.first << ":" <<std::endl;
        for ( const auto&  neighbour : node.second ) {
            std::cout << "\t-> " << neighbour.first << ": " << neighbour.second << std::endl;
        }
    }
    std::cout <<"wierzchołki do sprawdzenia:"<<std::endl;
    for ( auto x : tasks ) {
        std::cout << x<< " ";
    }
    std::cout << std::endl<< "----------"<<std::endl;

}

std::string traceback ( mapStSt& prev, std::string end, std::string start ) {
    if ( start == end )
        return start;
    else
        return traceback ( prev, prev[end], start ) +" -> "+ end;

}

vSt readTask ( const std::string filename ) {
    std::ifstream inputStream ( filename );
    vSt out;
    if ( inputStream ) {
        std::string node;
        while ( inputStream>>node ) {
            out.push_back ( node );
        }
        inputStream.close();
        return out;
    } else {
        std::cerr << "Błąd w otwieraniu pliku "<< filename <<std::endl;
        return out;
    }

}

graph readGraph ( const std::string filename, setSt& list ) {
    graph temp;

    std::ifstream graphStream ( filename );
    if ( graphStream ) {
        std::string start, connection, end, separator;
        double weight;
        int lines = 1;
        while ( graphStream >> start >> connection >> end >> separator >> weight ) {
            if(separator == ":") { //prosta weryfikacja poprawności wprowadzonych danych
                lines++;
                list.insert ( start ); //dodajemy do zbioru wierzchołek początkowy i końcowy
                list.insert ( end ); //ponieważ może być wierzchołek zdefiniwowany tylko jako ywkońcowy
                temp[start][end] = weight;
                if ( connection == "-" ) { // jeżeli krawędź nieskierowana
                    temp[end][start] = weight;//to połączenie w drugą stronę też istnieje
                }
            } else {
                std::cout << "Błąd wczytywania danych w połączeniu " << lines<<std::endl;
                graph error;
                return error;
            }

        }
        graphStream.close();
    } else {
        std::cerr << "Błąd w otwieraniu pliku "<<filename << std::endl;
    }
    return temp;
}

void prepareValues ( std::string &start, setSt& nList, mapStDb &dist, mapStSt &prev, vSt &remain ) {

    for ( auto node = nList.begin(); node != nList.end(); ++node) {
        prev[*node] = ""; //poprzednik jako niezdefiniowany
        remain.push_back ( *node);
        dist[*node] = std::numeric_limits<double>::infinity(); //ustaw odległość na nieskończoność

    }

    dist[start] = 0; // ustaw odległość początkowego na 0
}

void applyDijkstra ( vSt &remain, setSt &checked, mapStDb &dist, mapStSt &prev, graph &connections ) {
    while ( remain.size() ) { //dopóki nie wszystkie wierzchołki odwiedzone
        //sortowanie wektora remain według odległości
        std::sort ( remain.begin(), remain.end(), [&dist] ( std::string el1, std::string el2 ) {
            return ( dist[el1]>dist[el2] );
        } );
        std::string closest = remain.back();//wybieramy wierzchołek o najmniejszej odległości
        remain.pop_back();
        checked.insert ( closest ); //przenosimy go do listy sprawdzonych
        for ( auto compNode : connections[closest] ) { //dla każdego z sąsiadujących wierzchołków
            if ( std::find ( remain.begin(), remain.end(), compNode.first ) !=remain.end() ) { //jeśli nie był jeszcze odwiedzony
                if ( dist[compNode.first]>dist[closest] + compNode.second ) {
                    //sprawdzamy czy droga do niego przez ten wierzchołek jest krótsza niż do tej pory
                    dist[compNode.first] = dist[closest]+ compNode.second;//jeżeli tak to ustalamy odległość na drogę przez ten wierzchołek
                    prev[compNode.first] = closest; //i ustawiamy ten wierzchołek jako jego poprzednika
                }

            }
        }
    }
    return;
}


void writeResults ( mapStDb& dist,  mapStSt& prev,  std::string& node,  std::string& startNode, std::ofstream &outStream, bool verb ) {
    if ( node!=startNode ) {
        if ( dist[node] == std::numeric_limits<double>::infinity() ) { //zabezpieczenie przed wierzchołkami izolowanymi
            if ( verb ) std::cout << "Brak drogi do wierzchołka " << node << std::endl;
            outStream << "Brak drogi do wierzchołka " << node << std::endl;
        } else {
            if ( verb ) std::cout << traceback ( prev, node, startNode );
            outStream << traceback ( prev, node, startNode );
            if ( verb ) std::cout << " : " << dist[node] << std::endl;
            outStream << " : " << dist[node] << std::endl;
        }
    }

}

bool readParameters ( int& argc, char** argv, std::string& gName, std::string& wName, std::string& oName, bool &verb ) {
    bool errors = false;
    for ( int i = 1; i < argc - 1; i++ ) {
        if ( not strcmp ( argv[i], "-g" ) ) { //plik z grafem
            gName = argv[i + 1];
        } else if ( not strcmp ( argv[i], "-w" ) ) { //plik z wierzchołkami
            wName = argv[i + 1];
        } else if ( not strcmp ( argv[i], "-o" ) ) { //plik wyjściowy
            oName = argv[i + 1];
        } else if ( not strcmp ( argv[i], "-v" ) ) { //verbose, wypisuj dodatkowe informacje
            verb = true;
        } else if ( not strcmp ( argv[i], "-h" ) ) { //pomoc
            errors = true;
        }
    }
    if ( not strcmp ( argv[argc - 1], "-v" ) ) verb = true; //verbose, wypisuj dodatkowe informacje
    if ( not strcmp ( argv[argc - 1], "-h" ) ) { //pomoc
        errors = true;
    };
    if ( wName == ""|| ( wName.size() ==2&&wName[0]=='-' ) ) {
        std::cerr << "Nie podano pliku wejściowego!"<<std::endl;
        errors = true;
    }
    if ( oName == ""|| ( oName.size() ==2&&oName[0]=='-' ) ) {
        std::cerr << "Nie podano pliku wyjściowego!"<<std::endl;
        errors = true;
    }
    if ( gName == ""|| ( gName.size() ==2&&gName[0]=='-' ) ) {
        std::cerr << "Nie podano pliku z grafem!"<<std::endl;
        errors = true;
    }

    return errors;
}
