/** @file */
#ifndef dijkstra_h
#define dijkstra_h
#include <set>
#include <vector>
#include <string>
#include <map>
#include <fstream>

typedef std::map<std::string, std::map<std::string, double>> graph;
typedef std::set<std::string> setSt; //zbiór zmiennych String
typedef std::map<std::string, double> mapStDb; //mapa String : double
typedef std::map<std::string, std::string> mapStSt; //mapa String : String
typedef std::vector<std::string> vSt; //vector zmiennych String

/**
 * Funkcja wypisuje wiadomość pomocy ma standardowe wyjście. Jeżeli parametry wejściowe są niepoprawne lub użyty zostanie argument "-h"
 */
void helpMessage();

/**
 * Funkcja wypisuje nazwy plików wejściowych oraz pliku wyjściowego
 * następnie wypisuje wczytany graf i listę wierzchołków z których ma znaleźć ścieżki
 * @param g nazwa pliku z grafem
 * @param i nazwa pliku z wierzchołkami
 * @param o nazwa pliku wyjściowego
 * @param n wczytany graf
 * @param tC lista wierzchołków z których należy znaleźć ścieżki
 * 
 */
void printInput(std::string& g, std::string& i, std::string& o, graph& n, vSt& tC);


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
vSt readTask(const std::string filename);

/**
 * Funkcja zwraca graph wczytany z pliku o nazwie filename
 * zwraca również listę wierzchołków w grafie 
 * @param filename nazwa pliku wejściowego
 * @param list zbiór String do którego ma wpisać wierzchołki
 */
graph readGraph(const std::string filename, setSt& list);


/**
 * Funkcja przygotowuje podane kontenery do użycia w algorytmie dijkstry 
 * @param start nazwa wierzchołka początkowego, którego odległość będzie równa 0
 * @param nL lista wierzchołków w grafie
 * @param d mapa do której mają być wpisane odległości
 * @param p mapa do której mają być wpisywane poprzedające wierzchołki
 * @param r wektor do którego mają być wpisane wierzchołki do sprawdzenia
 * 
 */
void prepareValues(std::string &start, setSt &nL, mapStDb &d, mapStSt &p, vSt &r );

/**
 * Funkcja wykonuje algorytm dijkstry i wpisuje do podanych kontenerów odległości do wierzchołków, oraz listę ich poprzedników
 * @param Q werktor wierzchołków, które nie zostały odwiedzone
 * @param S zbiór odwiedzonych wierzchołków
 * @param d mapa w której zawarte są odległości
 * @param p mapa do której mają być wpisywane poprzedające wierzchołki
 * @param n graph w którym ma być szukana ścieżka
 * 
 */
void dijkstra(vSt &Q, setSt &S, mapStDb &d, mapStSt &p, graph &n);
/**
 * Funkcja wypisuje do strumienia wyjściowego wyznaczoną najkrótszą ścieżkę 
 * @param d  mapa odległości od wierzchołka startowego
 * @param p  mapa poprzedników
 * @param n  wierzchołek dla którego wypisujemy ścieżkę
 * @param startN wierzchołek początkowy
 * 
 */
void writeResults(mapStDb& d, mapStSt& p, std::string &n, std::string &startN, std::ofstream &S, bool V=false);


/**
 * Funkcja wczytuje parametry wejściowe podane do programu
 * Funkcja zwraca wartość true jeżeli jakieś dane są brakujące
 * @param argc ilość podanych agrumentów
 * @param argv tablica podanych argumentów
 * @param g  do niego zwróci nazwę pliku z grafem
 * @param inp  do niego zwróci nazwę pliku z wierzchołkami
 * @param out do niego zwróci nazwę pliku wyjściowego
 * @param v  do niego zwróci czy funkcja ma być verobse
 *
 */
bool readParameters(int& argc, char** argv, std::string& g, std::string& inp, std::string& out, bool &v);



#endif
