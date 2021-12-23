/** @file */
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

void printInput ( std::string &g, std::string &i, std::string &o, graph &n, vSt &tC ) {
    std::cout << "graf: " << g << std::endl;
    std::cout << "wierzchołki: " << i << std::endl;
    std::cout << "plik wyjsciowy: " << o << std::endl;
    std::cout << "Dane wprowadzone do programu:"<<std::endl;
    for ( const auto& node : n ) {
        std::cout <<"nazwa "<< node.first << ":" <<std::endl;
        for ( const auto&  neighbour : node.second ) {
            std::cout << "\t-> " << neighbour.first << ": " << neighbour.second << std::endl;
        }
    }
    std::cout <<"wierzchołki do sprawdzenia:"<<std::endl;
    for ( auto x : tC ) {
        std::cout << x<< " ";
    }
    std::cout << std::endl<< "----------"<<std::endl;

}

std::string traceback ( mapStSt& m, std::string end, std::string start ) {
    if ( start == end )
        return start;
    else
        return traceback ( m, m[end], start ) +" -> "+ end;

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
        std::cerr << "Błąd w otwieraniu pliku z wierzchołkami"<<std::endl;
        return out;
    }

}

graph readGraph ( const std::string filename, setSt& list ) {
    graph temp;

    std::ifstream graphStream ( filename );
    if ( graphStream ) {
        std::string poczatek, polaczenie, koniec, dwukropek;
        double waga;
        while ( graphStream >> poczatek >> polaczenie >> koniec >> dwukropek >> waga ) {
            list.insert ( poczatek ); //dodajemy do zbioru wierzchołek początkowy i końcowy
            list.insert ( koniec ); //ponieważ może być wierzchołek zdefiniwowany tylko jako końcowy
            temp[poczatek][koniec] = waga;
            if ( polaczenie == "-" ) { // jeżeli krawędź nieskierowana
                temp[koniec][poczatek] = waga;//to połączenie w drugą stronę też istnieje
            }
        }
        graphStream.close();
    } else {
        std::cerr << "Błąd w otwieraniu pliku z grafem" << std::endl;
    }
    return temp;
}

void prepareValues ( std::string &start, setSt &nL, mapStSt &p, mapStDb &d, vSt &r ) {

    for ( auto node = nL.begin(); node != nL.end(); ++node ) {
        p[*node] = ""; //poprzednik jako niezdefiniowany
        r.push_back ( *node );
        d[*node] = std::numeric_limits<double>::infinity(); //ustaw odległość na nieskończoność
    }
    d[start] = 0; // ustaw odległość początkowego na 0
}

void dijkstra(vSt &Q, setSt &S, mapStDb &d, mapStSt &p, graph &n){
    while ( Q.size() ) { //dopóki nie wszystkie wierzchołki odwiedzone
        //sortowanie wektora Q według odległości
        std::sort ( Q.begin(), Q.end(), [&d] ( std::string el1, std::string el2 ) {
            return ( d[el1]>d[el2] );
        } );
        std::string closest = Q.back();//wybieramy wierzchołek o najmniejszej odległości
        Q.pop_back();
        S.insert ( closest ); //przenosimy go do listy sprawdzonych
        for ( auto compNode : n[closest] ) { //dla każdego z sąsiadujących wierzchołków
            if ( std::find ( Q.begin(), Q.end(), compNode.first ) !=Q.end() ) { //jeśli nie był jeszcze odwiedzony
                if ( d[compNode.first]>d[closest] + compNode.second ) {
                    //sprawdzamy czy droga do niego przez ten wierzchołek jest krótsza niż do tej pory
                    d[compNode.first] = d[closest]+ compNode.second;//jeżeli tak to ustalamy odległość na drogę przez ten wierzchołek
                }
                p[compNode.first] = closest; //i ustawiamy ten wierzchołek jako jego poprzednika
            }
        }
    }
    return;
}


void writeResults ( mapStDb& d,  mapStSt& p,  std::string& n,  std::string& startN, std::ofstream &S, bool V ) {
    if ( n!=startN ) {
        if ( d[n] == std::numeric_limits<double>::infinity() ) { //zabezpieczenie przed wierzchołkami izolowanymi
            if ( V ) std::cout << "Brak drogi do wierzchołka " << n << std::endl;
            S << "Brak drogi do wierzchołka " << n << std::endl;
        } else {
            if ( V ) std::cout << traceback ( p, n, startN );
            S << traceback ( p, n, startN );
            if ( V ) std::cout << " : " << d[n] << std::endl;
            S << " : " << d[n] << std::endl;
        }
    }

}

bool readParameters ( int& argc, char** argv, std::string& g, std::string& inp, std::string& out, bool &v ) {
    bool errors = false;
    for ( int i = 1; i < argc - 1; i++ ) {
        if ( not strcmp ( argv[i], "-g" ) ) { //plik z grafem
            g = argv[i + 1];
        } else if ( not strcmp ( argv[i], "-w" ) ) { //plik z wierzchołkami
            inp = argv[i + 1];
        } else if ( not strcmp ( argv[i], "-o" ) ) { //plik wyjściowy
            out = argv[i + 1];
        } else if ( not strcmp ( argv[i], "-v" ) ) { //verbose, wypisuj dodatkowe informacje
            v = true;
        } else if ( not strcmp ( argv[i], "-h" ) ) { //pomoc
            errors = true;
        }
    }
    if ( not strcmp ( argv[argc - 1], "-v" ) ) v = true; //verbose, wypisuj dodatkowe informacje
    if ( not strcmp ( argv[argc - 1], "-h" ) ) { //pomoc
        errors = true;
    };
    if ( inp == ""|| ( inp.size() ==2&&inp[0]=='-' ) ) {
        std::cerr << "Nie podano pliku wejściowego!"<<std::endl;
        errors = true;
    }
    if ( out == ""|| ( out.size() ==2&&out[0]=='-' ) ) {
        std::cerr << "Nie podano pliku wyjściowego!"<<std::endl;
        errors = true;
    }
    if ( g == ""|| ( g.size() ==2&&g[0]=='-' ) ) {
        std::cerr << "Nie podano pliku z grafem!"<<std::endl;
        errors = true;
    }

    return errors;
}
