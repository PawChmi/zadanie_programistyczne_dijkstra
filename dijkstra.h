#ifndef dijkstra_h
#define dijkstra_h

#include <vector>
#include <string>
#include <map>

typedef std::pair<double, std::string> connection;

/**
 * Funkcja wypisuje wiadomość pomocy ma standardowe wyjście. Jeżeli parametry wejściowe są niepoprawne lub użyty zostanie argument "-h"
*/
bool sortByDistance(const std::string &p1,const std::string &p2, std::map<std::string, double> &m);

/**
 * Funkcja wypisuje wiadomość pomocy ma standardowe wyjście. Jeżeli parametry wejściowe są niepoprawne lub użyty zostanie argument "-h"
*/
void helpMessage();

/**
 * Funkcja wypisuje wiadomość pomocy ma standardowe wyjście. Jeżeli parametry wejściowe są niepoprawne lub użyty zostanie argument "-h"
*/
std::string traceback(std::map<std::string, std::string> &m, std::string start, std::string end);

#endif
