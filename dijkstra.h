#ifndef dijkstra_h
#define dijkstra_h

#include <vector>
#include <string>
#include <map>

/**
 * Struktura opisuje krawędź grafu
 * origin - punkt początkowy
 * destination - punkt końcowy
 * directed - czy skierowana?
 * weight - waga (długość)
*/
struct connection
{
    std::string origin = "";
    std::string destination = "";
    bool directed;
    double weight = 0.0;
};


/**
 * Struktura opisuje ścieżkę przez kolejne wierzchołki
 * lenght - długość ścieżki
 * points - wektor zawierający kolejne wierzchołki
 */
struct path
{
    double lenght = 0;
    std::vector<std::string> points;
};

/**
Funkcja zwraca strukturę connection na podstawie podanego ciągu znaków
@param line string z którego należy odczytać krawędź grafu
*/
connection parseLineGraph(std::string line);

/**
Funkcja zwraca wektor ciągów znaków na podstawie podanego ciągu znaków
@param line string z którego należy odczytać szukane wierzchołki
*/
std::vector<std::string> parseLineVertex(std::string line);

/**
 * Funkcja zwraca strukturę path z najkrótszą ścieżką od wierzchołka start do wierzchołka end, w mapie wierzchołków n
 * @param n mapa gdzie:
 *         klucz: nazwa wierzchołka
 *         wartość: mapa gdzie:
 *               klucz: nazwa sąsiedniego wierzchołka
 *               wartość: odległość do sąsiedniego wierzchołka
 * @param start nazwa początkowego wierzchołka
 * @param end nazwa docelowego wierzchołka
*/
path findShortestPath(std::map<std::string, std::map<std::string, double>> n, std::string start, std::string end);

/**
 * Funkcja rekurencyjne, zwraca strukturę path powiększoną o samą siebie dla kolejnego wierzchołka, jeżeli wierzchołek szukany jest wierzchołkiem początkowym zwraca sam siebie oraz podaje odległośc ostatiego wierzchołka
 * @param n mapa gdzie:
 *         klucz: nazwa wierzchołka
 *         wartość: mapa gdzie:
 *               klucz: nazwa sąsiedniego wierzchołka
 *               wartość: odległość do sąsiedniego wierzchołka
 * @param start nazwa początkowego wierzchołka
 * @param end nazwa docelowego wierzchołka
 * @param previous struktura path zawierająca poprzednie wierzchołki na ścieżce
 * @param d mapa, gdzie:
 *          klucz: nazwa wierzchołka
 *          wartość: odległość od początkowego wierzchołka
 * Z mapy d wykluczane są już odwiedzone wierzchołki
*/
path compareNeighbours( std::map<std::string, std::map<std::string, double>> n, std::string start, std::string end, path previous, std::map<std::string, double> d);

/**
 * Funkcja zwraca nazwę najbliżeszego wierzchołkawedług mapy d
 * @param d mapa gdzie:
 *          klucz: nazwa wierzchołka
 *          wartość: odległość od początkowego wierzchołka
*/
std::string closest(std::map<std::string, double> d);

/**
 * Funkcja wypisuje wiadomość pomocy ma standardowe wyjście. Jeżeli parametry wejściowe są niepoprawne lub użyty zostanie argument "-h"
 */
void helpMessage();
#endif
