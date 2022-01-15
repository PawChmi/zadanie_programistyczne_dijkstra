/** @file */
#ifndef dijkstra_h
#define dijkstra_h
#include <set>
#include <vector>
#include <string>
#include <map>
#include <fstream>

/** typ graph przechowuje połączenia między wierzchołkami w mapie, w której dla nazwy każdego wierzchołka przyporądkowana jest mapa odległości do wierzchołków, do których da się dojść z tego wierzchołka */
typedef std::map<std::string, std::map<std::string, double>> graph;

/** zbiór zmiennych String */
typedef std::set<std::string> setSt; 
/** mapa String : double */
typedef std::map<std::string, double> mapStDb; 
/** mapa String : String */
typedef std::map<std::string, std::string> mapStSt; 
/** vector zmiennych String*/
typedef std::vector<std::string> vSt; 

/**
 * Funkcja wypisuje wiadomość pomocy ma standardowe wyjście.
 * */
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
 * Funkcja działa rekurencyjnie.
 * Jeżeli start jest równe end to zwraca samą siebie
 * @param m mapa String : String w której klucz odnosi się do poprzedniego wierzchołka
 * @param end wierzchołek do którego szukamy ścieżki
 * @param  start wierzchołek z którego zaczynamy wyznaczać ścieżkę
 * @return funkcja zwraca string opisujący ścieżkę od start do end
 */
std::string traceback(mapStSt &m, std::string end, std::string start);

/**
 * Funkcja zwraca wektor ciągów znaków, które określają dla jakich wierzchołków należy znaleźć najkrótsze ścieżki
 * @param filename nazwa pliku wejściowego
 * @return wektor z nawami wierzchołków dla których należy znaleźc najkrótsze ścieżki
 */
vSt readTask(const std::string filename);

/**
 * Funkcja zwraca graph wczytany z pliku o nazwie filename
 * zwraca również listę wierzchołków w grafie 
 * @param filename nazwa pliku wejściowego
 * @param[out] list zbiór String do którego ma wpisać wierzchołki
 * @return mapa map odległośći między wierzchołkami
 */
graph readGraph(const std::string filename, setSt& list);


/**
 * Funkcja przygotowuje podane kontenery do użycia w algorytmie dijkstry 
 * @param start nazwa wierzchołka początkowego, którego odległość będzie równa 0
 * @param nL lista wierzchołków w grafie
 * @param[out] d mapa do której mają być wpisane odległości
 * @param[out] p mapa do której mają być wpisywane poprzedające wierzchołki
 * @param[out] r wektor do którego mają być wpisane wierzchołki do sprawdzenia
 * 
 */
void prepareValues(std::string &start, setSt &nL, mapStDb &d, mapStSt &p, vSt &r );

/**
 * Funkcja wykonuje algorytm dijkstry i wpisuje do podanych kontenerów odległości do wierzchołków, oraz listę ich poprzedników
 * @param Q wektor wierzchołków, które nie zostały odwiedzone
 * @param S zbiór odwiedzonych wierzchołków
 * @param[out] d mapa w której zawarte są odległości
 * @param[out] p mapa do której mają być wpisywane poprzedające wierzchołki
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
 * @param v czy funkcja ma wypisywać dane na konsnolę
 */
void writeResults(mapStDb& d, mapStSt& p, std::string &n, std::string &startN, std::ofstream &S, bool V=false);


/**
 * Funkcja wczytuje parametry wejściowe podane do programu
 * 
 * @param argc ilość podanych agrumentów
 * @param argv tablica podanych argumentów
 * @param[out] gName  nazwa pliku z grafem
 * @param[out] wName  nazwa pliku z wierzchołkami
 * @param[out] oName  nazwa pliku wyjściowego
 * @param[out] v boolean czy funkcja ma wypisywać dane w konsoli
 * @return funkcja zwraca wartość true jeżeli podane zostały wszystkie parametry lub false jeżeli jakiegoś brakuje
 */
bool readParameters(int& argc, char** argv, std::string& gName, std::string& wName, std::string& oName, bool &v);



#endif
