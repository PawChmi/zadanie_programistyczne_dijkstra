#include "dijkstra.h"
#include <string>
#include <vector>
#include <iostream>


connection parseLineGraph(std::string line)
{
    int i = 0;
    connection newConnection;
    while(i<line.size())
    {
        if(line[i] != ' ') //ignorowanie spacji
        {
            int j = 1; //długość podciągu
            while(line[i+j] != ' '&& j<line.size())
            {
                j++;
            }//wydłużaj aż nie będzie spacją
            
            if (line.substr(i, j)=="-") // - oznacza krawędź nieskierowaną
            {
                newConnection.directed = false;
            }else if (line.substr(i, j)=="->")//oznacza krawędź skierowaną 
            {
                newConnection.directed = true; 
            }else if(line.substr(i, j)!=":")
            { //jeżeli ciąg znaków nie jest ":" to jest nazwą jednego z wierzchołków lub wagą krawędzi
                if(newConnection.origin == "") newConnection.origin = line.substr(i, j);
                else if(newConnection.destination == "") newConnection.destination = line.substr(i,j);
                else if(newConnection.weight == 0) newConnection.weight = std::stod(line.substr(i, j));
            }
            i += j; //zacznij od końca podciągu
        }else{ 
            i++;
        }
    }
    return newConnection;
}


std::vector<std::string> parseLineVertex(std::string line)
{
    int i = 0;
    std::vector<std::string> newVector;
    while(i<line.size())
    {
        if(line[i] != ' ') //ignorowanie spacji
        {
            int j = 1;//długość podciągu
            while(line[i+j] != ' '&& j<line.size())
            {
                j++;
            }//wydłużaj aż nie będzie spacją
            
            newVector.push_back(line.substr(i, j)); //dodaj podciągo do listy wierzchołków do sprawdzenia
            i += j; //przeskocz do końca podciągu
        }else{ 
            i++;
        }
    }
    return newVector;
}


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


path compareNeighbours( std::map<std::string, std::map<std::string, double>> n, std::string start, std::string end, path previous, std::map<std::string, double> d)
{
    
    if(start == end) //warunek powrotu z rekurencji
    {
        previous.points.push_back(start);
        previous.lenght = d[end];
        return previous;
    }

    for(auto node : n[start])//dla sąsiednich wierzchołków wierzchołka start
    {
        if(d.find(node.first)!=d.end()) //jeżeli wierzchołek jest w liście odległości (nie był sprawdzany)
        {
            if(d[start] + node.second < d[node.first] || d[node.first]<0) //jeżeli odległość do wierzchołka jest większa niż obecna odległość + odległość do tego wierzchołka (ewentulanie wierzchołek oddalony o nieskończoność)
            {
                d[node.first] = d[start] + node.second; //ustaw odległość do wierzchołka na sumę obecnej odlegóści i odległości do tego wierzchołka
            }
        }
    }
    previous.points.push_back(start); //dodaj obecny wierzchołek do ścieżki
    d.erase(start); //usuń obecny wierzchołek z listy odległości  wierzchołków
    previous = compareNeighbours(n, closest(d), end, previous, d); //wywołanie rekurencyjne
    
    return previous; //zwróć wartość funkcji rekurencyjnej
}

std::string closest(std::map<std::string, double> d)
{
    double shortest=0;
    std::string output;
    for(auto item : d)
    {
        if(item.second > 0)
        {
            if(item.second<shortest ||shortest == 0)
            {
                shortest = item.second;
                output = item.first;
            }
        }
    }
    return output;
}

path findShortestPath (std::map<std::string, std::map<std::string, double>> n, std::string start, std::string end )
{
    path currentPath;
    std::map<std::string, double> distances;
    for(auto x : n)
    {
        distances[x.first] = -1; //początkowe odległości do wszystkich punktów ustalone na -1, traktowane jak nieskończoność przy porównywaniu 
    }
    distances[start] = 0; //początkowa odległość ustalona na zero
    path temp;
    currentPath = compareNeighbours(n, start, end, temp, distances);
    return currentPath;
}
