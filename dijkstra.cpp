#include "dijkstra.h"
#include <string>
#include <vector>
#include <iostream>




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
