#include "dijkstra.h"
#include <string>
#include <vector>
#include <iostream>

connection parseLineGraph(std::string line)
{
    int i = 0;
    
    connection newConnection;
    while(i<line.size()){
        if(line[i] != ' ')
        {
            int j = 1;
            while(line[i+j] != ' '&& j<line.size()){
                j++;
            }
            if (line.substr(i, j)=="-")
            {
                newConnection.directed = false;
            }else if (line.substr(i, j)=="->")
            {
                newConnection.directed = true;
            }else if(line.substr(i, j)!=":"){
                if(newConnection.origin == "") newConnection.origin = line.substr(i, j);
                else if(newConnection.destination == "") newConnection.destination = line.substr(i,j);
                else if(newConnection.weight == 0) newConnection.weight = std::stod(line.substr(i, j));
            }
            i += j;
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
    while(i<line.size()){
        if(line[i] != ' ')
        {
            int j = 1;
            while(line[i+j] != ' '&& j<line.size()){
                j++;
            }
            newVector.push_back(line.substr(i, j));
            i += j;
        }else{ 
            i++;
        }
    }
    return newVector;
}


void helpMessage(){
    std::cout << "Ten program znajduje najkrótszą drogę miedzy wierzchołkami wykorzystując algorytm Dijksry" << std::endl;
    std::cout << "Program przyjmuje następujące argumenty:"<<std::endl;
    std::cout << "-g \'nazwa pliku\' plik wejściowy zawierający graf."<<std::endl;
    std::cout << "-w \'nazwa pliku\' plik wejściowy zawierający listę wierzchołków do zbadania."<<std::endl;
    std::cout << "-o \'nazwa pliku\' plik wyjściowy."<<std::endl;
    std::cout << "-v Verbose, tryb wypisywania dodatkowych informacji."<<std::endl;
    std::cout << "-h wypisuje tą wiadomość pomocy."<<std::endl;
}


node connectionToNode(std::string origin, connection C)
{
    node temp;
    temp.label = origin;
   
    if(origin == C.origin) temp.neighbours[C.destination] = C.weight;
    else temp.neighbours[C.origin] = C.weight;
    
    
    
    temp.value = -1;
    return temp;
}
path compareNeighbours( std::map< std::string, node > n, std::string start, std::string end, path previous, std::map<std::string, double> d){
    
    if(start == end){
        previous.points.push_back(start);
        previous.lenght = d[end];
        return previous;
    }

    n[start].active = false;
    for(auto Vert : n[start].neighbours){
        
        if(d.find(Vert.first)!=d.end()){             
            if(d[start] + Vert.second < d[Vert.first] || d[Vert.first]<0){
                d[Vert.first] = d[start] + Vert.second;
            }
        }
    }
    previous.points.push_back(start);
    d.erase(start);


    previous = compareNeighbours(n, closest(d), end, previous, d);
    return previous;
}

std::string closest(std::map<std::string, double> d){
    double shortest=0;
    std::string output;
    for(auto item : d){
        if(item.second > 0){
            if(item.second<shortest ||shortest == 0){
                shortest = item.second;
                output = item.first;
            }
        }
    }
    return output;
}

path findShortestPath ( std::map< std::string, node > n, std::string start, std::string end )
{
    path currentPath;
    std::map<std::string, double> distances;
    for(auto x : n){
        distances[x.first] = -1;
    }
    distances[start] = 0;
    path temp;
    currentPath = compareNeighbours(n, start, end, temp, distances);
    return currentPath;
}
