#ifndef dijkstra_h
#define dijkstra_h

#include <vector>
#include <string>
#include <map>

/**
funkcja zwraca wektor taki że:
element 0 = węzeł poczatkowy
element 1 = 
element 0 = węzeł poczatkowy


*/
struct connection
{
    std::string origin = "";
    std::string destination = "";
    bool directed;
    double weight = 0.0;
};

struct node
{
    std::string label = "";
    std::map<std::string, double> neighbours = {};
    double value = 0.0;
    bool active = true;
};
struct path
{
    double lenght = 0;
    std::vector<std::string> points;
};
connection parseLineGraph(std::string line);

std::vector<std::string> parseLineVertex(std::string line);

node connectionToNode(std::string origin, connection C);

path findShortestPath(std::map<std::string, node> n, std::string start, std::string end);
std::string closest(std::map<std::string, double> d);
path compareNeighbours( std::map< std::string, node > n, std::string start, std::string end, path previous, std::map<std::string, double> d);

void helpMessage();
#endif
