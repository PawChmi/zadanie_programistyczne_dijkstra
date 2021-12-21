#include "dijkstra.h"
#include <string>
#include <vector>
#include <iostream>



/**
 * Funkcja wypisuje wiadomość pomocy ma standardowe wyjście. Jeżeli parametry wejściowe są niepoprawne lub użyty zostanie argument "-h"
*/
void helpMessage()
{
    std::cout << "Ten program znajduje najkrótszą drogę miedzy wierzchołkami wykorzystując algorytm Dijksry" << std::endl;
    std::cout << "Program przyjmuje następujące argumenty:"<<std::endl;
    std::cout << "-g \'nazwa pliku\' plik wejściowy zawierający graf."<<std::endl;
    std::cout << "-w \'nazwa pliku\' plik wejściowy zawierający listę wierzchołków do zbadania."<<std::endl;
    std::cout << "-o \'nazwa pliku\' plik wyjściowy."<<std::endl;
    std::cout << "-v Verbose, tryb wypisywania dodatkowych informacji."<<std::endl;
    std::cout << "-h wypisuje tą wiadomość pomocy."<<std::endl;
}


/**
 * Funkcja zwraca wartość true jeżeli pierwsza z porównywanych ścieżek jest krótsza
 * @param p1 pierwsze połączenie 
 * @param p2 drugie połączenie 
*/
bool sortByDistance(const std::string &p1, const std::string &p2,  std::map<std::string, double> &m) {
    return (m[p1]>m[p2]);
}

std::string traceback(std::map<std::string, std::string> &m, std::string start, std::string end){
    if(start == end)
        return end;
    else
        return traceback(m, m[start], end) +" -> "+ start;
    
}
