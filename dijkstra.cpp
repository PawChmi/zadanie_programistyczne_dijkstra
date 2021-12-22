#include "dijkstra.h"
#include <string>
#include <vector>
#include <iostream>
#include <limits>
#include <string.h>


/**
 * Funkcja wypisuje wiadomość pomocy ma standardowe wyjście. Jeżeli parametry wejściowe są niepoprawne lub użyty zostanie argument "-h"
*/
void helpMessage() {
    std::cout << "Ten program znajduje najkrótszą drogę miedzy wierzchołkami wykorzystując algorytm Dijksry" << std::endl;
    std::cout << "Program przyjmuje następujące argumenty:"<<std::endl;
    std::cout << "-g \'nazwa pliku\' plik wejściowy zawierający graf."<<std::endl;
    std::cout << "-w \'nazwa pliku\' plik wejściowy zawierający listę wierzchołków do zbadania."<<std::endl;
    std::cout << "-o \'nazwa pliku\' plik wyjściowy."<<std::endl;
    std::cout << "-v Verbose, tryb wypisywania dodatkowych informacji."<<std::endl;
    std::cout << "-h wypisuje tą wiadomość pomocy."<<std::endl;
}



std::string traceback ( mapStSt& m, std::string end, std::string start ) {
    if ( start == end )
        return start;
    else
        return traceback ( m, m[end], start ) +" -> "+ end;

}

vSt readTask ( std::string filename ) {
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
        //zwraca błąd jeżeli nie podano pliku wejściowego z wierzchołkami
        helpMessage();
        std::cerr << "Nie podano pliku z wierzchołkami"<<std::endl;
        return out;
    }

}

graph readGraph(std::string filename, setSt& list){
    graph temp;

    std::ifstream graphStream(filename);
    if (graphStream) {
        std::string poczatek, polaczenie, koniec, dwukropek;
        double waga;
        while (graphStream >> poczatek >> polaczenie >> koniec >> dwukropek >> waga) {
            list.insert(poczatek); //dodajemy do zbioru wierzchołek początkowy i końcowy
            list.insert(koniec);  //ponieważ może być wierzchołek zdefiniwowany tylko jako końcowy
            temp[poczatek][koniec] = waga;
            if (polaczenie == "-") { // jeżeli krawędź nieskierowana
                temp[koniec][poczatek] = waga;//to połączenie w drugą stronę też istnieje
            }
        }
        graphStream.close();
    }
    else {
        //zwraca błąd jeżeli nie podano pliku wejściowego z grafem
        helpMessage();
        std::cerr << "Nie podano pliku z grafem" << std::endl;
    }
    return temp;
}


void writeResults( mapStDb& d,  mapStSt& p,  std::string& n,  std::string& startN, std::ofstream &S, bool V ) {
    if (n!=startN) { 
        if (d[n] == std::numeric_limits<double>::infinity()) { //zabezpieczenie przed wierzchołkami izolowanymi
            if(V)std::cout << "Brak drogi do wierzchołka " << n << std::endl;
            S << "Brak drogi do wierzchołka " << n << std::endl;
        }
        else {
            if(V) std::cout << traceback(p, n, startN);
            S << traceback(p, n, startN);
            if(V) std::cout << " : " << d[n] << std::endl;
            S << " : " << d[n] << std::endl;
        }
    }

}

bool readParameters(int& argc, char** argv, std::string& g, std::string& inp, std::string& out, bool &v) {
    for (int i = 1; i < argc - 1; i++) {
        if (not strcmp(argv[i], "-g")) { //plik z grafem
            g = argv[i + 1];
        }
        else if (not strcmp(argv[i], "-w")) { //plik z wierzchołkami
            inp = argv[i + 1];
        }
        else if (not strcmp(argv[i], "-o")) { //plik wyjściowy
            out = argv[i + 1];
        }
        else if (not strcmp(argv[i], "-v")) { //verbose, wypisuj dodatkowe informacje
            v = true;
        }
        else if (not strcmp(argv[i], "-h")) { //pomoc
            helpMessage();
            return true;
        }
    }
    if (not strcmp(argv[argc - 1], "-v")) v = true; //verbose, wypisuj dodatkowe informacje
    if (not strcmp(argv[argc - 1], "-h")) {//pomoc
        helpMessage();
        return true;
    }; 

    
    return false;
}
