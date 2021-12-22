#ifndef dijkstra_h
#define dijkstra_h
#include <set>
#include <vector>
#include <string>
#include <map>
#include <fstream>

typedef std::set<std::string> setSt; //zbirór zmiennych String
typedef std::map<std::string, double> mapStDb; //mapa String : double
typedef std::map<std::string, std::string> mapStSt; //mapa String : String
typedef std::vector<std::string> vSt; //vector zmiennych String

/**
 * Funkcja wypisuje wiadomość pomocy ma standardowe wyjście. Jeżeli parametry wejściowe są niepoprawne lub użyty zostanie argument "-h"
*/
void helpMessage();

/**
 * Funkcja zwraca ciąg znaków: kolejne punkty ścieżki od start do end połączone znakami  " -> "
 * Funkcja działa reukrencyjnie.
 * Jeżeli start jest równe end to zwraca samą siebie
 * @param m mapa String : String w której klucz odnosi się do poprzedniego wierzchołka
 * @param end wierzchołek do którego szukamy ścieżki
 * @param  start wierzchoł z którego zaczynamy wyznaczać ścieżkę
*/
std::string traceback(mapStSt &m, std::string end, std::string start);

/**
 * Funkcja zwraca wektor ciągów znaków, które określają dla jakich wierzchołków należy znaleźć najkrótsze ścieżki
 * @param filename nazwa pliku wejściowego
*/
vSt readTask(std::string filename);

#endif
