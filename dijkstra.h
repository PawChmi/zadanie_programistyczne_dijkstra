/** @file */
#ifndef dijkstra_h
#define dijkstra_h
#include <set>
#include <vector>
#include <string>
#include <unordered_map>
#include <fstream>

/** Typ graph przechowuje połączenia między wierzchołkami w mapie, w której do nazwy każdego wierzchołka przyporządkowana jest mapa odległości do wierzchołków, do których da się dojść z tego wierzchołka */
typedef std::unordered_map<std::string, std::unordered_map<std::string, double>> graph;

/** Zbiór zmiennych typu String */
typedef std::set<std::string> setSt;
/** Mapa String : double */
typedef std::unordered_map<std::string, double> mapStDb;
/** Mapa String : String */
typedef std::unordered_map<std::string, std::string> mapStSt;
/** Vector zmiennych typu String*/
typedef std::vector<std::string> vSt;

/**
 * Funkcja wypisuje wiadomość pomocy ma standardowe wyjście.
 * */
void helpMessage();

/**
 * Funkcja wypisuje nazwy plików wejściowych oraz pliku wyjściowego, 
 * a następnie wypisuje wczytany graf i listę wierzchołków, z których ma znaleźć ścieżki.
 * @param gName nazwa pliku z grafem
 * @param wName nazwa pliku z wierzchołkami
 * @param oName nazwa pliku wyjściowego
 * @param connections wczytany graf
 * @param tasks lista wierzchołków, z których należy znaleźć ścieżki
 *
 */
void printInput(std::string& gName, std::string& wName, std::string& oName, graph& connections, vSt& tasks);


/**
 * Funkcja zwraca ciąg znaków: kolejne punkty ścieżki od start do end połączone znakami  " -> ".
 * Funkcja działa rekurencyjnie.
 * Jeżeli start jest równe end, zwraca samą siebie
 * @param prev mapa String : String, w której klucz odnosi się do poprzedniego wierzchołka
 * @param end wierzchołek, do którego szukamy ścieżki
 * @param start wierzchołek, z którego zaczynamy wyznaczać ścieżkę
 * @return funkcja zwraca string opisujący ścieżkę od start do end
 */
std::string traceback(mapStSt& prev, std::string end, std::string start);

/**
 * Funkcja wczytuje z pliku o podanej nazwie, nazwy wierzchołków, dla których należy wyznaczyć najkrótsze ścieżki.
 * 
 * @param filename nazwa pliku wejściowego
 * @return wektor z nazwami wierzchołków, dla których należy znaleźć najkrótsze ścieżki
 */
vSt readTask(const std::string filename);

/**
 * Funkcja wczytuje dane grafu z pliku o podanej nazwie.
 * Funkcja zwraca listę wierzchołków w grafie do podanego zbioru ciągów znaków.
 * @param filename nazwa pliku wejściowego
 * @param[out] list zbiór String, do którego ma wpisać wierzchołki
 * @return mapa map odległości między wierzchołkami (obiekt typu graph)
 */
graph readGraph(const std::string filename, setSt& list);


/**
 * Funkcja przygotowuje podane kontenery do użycia w algorytmie Dijkstry.
 * @param start nazwa wierzchołka początkowego, którego odległość będzie równa 0
 * @param nList lista wierzchołków w grafie
 * @param[out] dist mapa, do której mają być wpisane odległości
 * @param[out] prev mapa, do której mają być wpisywane poprzedzające wierzchołki
 * @param[out] remain wektor, do którego mają być wpisane wierzchołki do sprawdzenia
 *
 */
void prepareValues(std::string& start, setSt& nList, mapStDb& dist, mapStSt& prev, vSt& remain);

/**
 * Funkcja wykonuje algorytm Dijkstry i wpisuje do podanych kontenerów odległości do wierzchołków oraz listę ich poprzedników.
 * @param remain wektor wierzchołków, które nie zostały odwiedzone
 * @param checked zbiór odwiedzonych wierzchołków
 * @param[out] dist mapa, w której zawarte są odległości
 * @param[out] prev mapa, do której mają być wpisywane poprzedzające wierzchołki
 * @param connections graph, w którym ma być szukana ścieżka
 *
 */
void applyDijkstra(vSt& remain, setSt& checked, mapStDb& dist, mapStSt& prev, graph& connections);
/**
 * Funkcja wypisuje do strumienia wyjściowego wyznaczoną najkrótszą ścieżkę.
 * @param dist  mapa odległości od wierzchołka startowego
 * @param prev  mapa poprzedników
 * @param node  wierzchołek, dla którego wypisujemy ścieżkę
 * @param startN wierzchołek początkowy
 * @param outStream strumień wyjściowy, do którego mamy wypisać dane
 * @param verb zmienna określająca, czy funkcja ma wypisywać dane na konsolę
 */
void writeResults(mapStDb& dist, mapStSt& prev, std::string& node, std::string& startNode, std::ofstream& outStream, bool verb = false);


/**
 * Funkcja wczytuje parametry wejściowe podane do programu.
 *
 * @param argc ilość podanych agrumentów
 * @param argv tablica podanych argumentów
 * @param[out] gName  nazwa pliku z grafem
 * @param[out] wName  nazwa pliku z wierzchołkami
 * @param[out] oName  nazwa pliku wyjściowego
 * @param[out] verb zmienna określająca, czy funkcja ma wypisywać dodatkowe dane w konsoli
 * @return funkcja zwraca wartość false jeżeli podane zostały wszystkie parametry lub true jeżeli jakiegoś brakuje
 */
bool readParameters(int& argc, char** argv, std::string& gName, std::string& wName, std::string& oName, bool& verb);



#endif
