// Dijkstra 
//  Napisać  program,  do  znajdowania  najkrótszych  ścieżek  między  zadanym 
// wierzchołkiem  grafu  a  wszystkimi  pozostałymi  wierzchołkami. W programie 
// powinien zostać zastosowany algorytm Dijkstry.  
// Plik z grafem ma następującą postać: 
// • każda krawędź jest podana w osobnej linii, 
// • przykład  krawędzi  skierowanej:  4  ->  5  :  54.4  oznacza  krawędź 
// skierowaną od wierzchołka 4 do 5, 
// • waga (długość) krawędzi wynosi 54.4, 
// • przykład krawędzi nieskierowanej: 3  -  6  :  12.5  oznacza krawędź 
// nieskierowaną między wierzchołkami 3 i 6 o wadze (długości) 12.5, 
// • w  pliku mogą wystąpić puste linie  oraz  dodatkowe  (nadmiarowe)  znaki 
// białe. 
// Przykładowy plik z grafem: 
// 3 -> 2 : 54.5  
// 12 -> 3 : 4.56  
// 2 -> 5 : 34.65  
// 5 -> 3 : 2.4  
// 3 -> 12 : 1.0 
// Drugim plikiem wejściowym programu, jest plik z numerami wierzchołków, dla 
// których chcemy wyznaczyć najkrótsze odległości do pozostałych wierzchołków. 
// Przykładowy plik: 
// 2 6 12 
// W pliku wynikowym zostaną zapisane trasy o minimalnej długości dla zadanych 
// wierzchołków, np.  
// Wierzchołek startowy: 2  
// 2 -> 5 -> 3 : 37.05 
// 2 -> 5 : 34.65 
// 2 -> 5 -> 3 -> 12 : 38.05 
// Wierzchołek startowy: 6 
// Brak wierzchołka 6 w grafie 
// Wierzchołek startowy: 12 
// 12 -> 3 : 4.56 
// 12 -> 3 -> 2 : 59.06 
// 12 -> 3 -> 2 -> 5 : 93.76 
// Program  uruchamiany  jest  z  linii  poleceń  z  wykorzystaniem  następujących 
// przełączników (kolejność przełączników jest dowolna): 
// -g plik wejściowy z grafem 
// -w plik w3jściowy z wierzchołkami 
// -o plik wyjściowy z wynikami 

//Importowanie bibliotek
#include <iostream>
#include <fstream>
#include <string.h>
#include <string>
#include <map>
#include <iterator>
#include "dijkstra.h"

int main(int argc, char **argv) 
{
    std::vector<connection> connections;
    std::vector<std::string>toCheck;
    std::map<std::string, node> nodes;
    std::string graph, input, output;
    bool verbose = false;
    for (int i = 1; i < argc-1; i++)
    {

        if(not strcmp(argv[i],"-g")) //plik z grafe
        {
            graph = argv[i+1];
        }
        else if(not strcmp(argv[i],"-w")) //plik z wierzchołkami
        {
            input = argv[i+1];
        }
        else if(not strcmp(argv[i],"-o")) //plik wyjściowy
        {
            output = argv[i+1];
        }
        else if(not strcmp(argv[i],"-v")) //verbose, wypisuj doatkowe informacje
        {
            verbose = true;
        }
        else if(not strcmp(argv[i],"-h")) //pomoc
        {
            helpMessage();
            return 0;
        }
    }
    if(output == ""){
        helpMessage();
        std::cerr << "Nie podano pliku wyjściowego"<<std::endl;   
        return 1;
    }
    //wczytywanie grafu
    std::ifstream graphS (graph);
    std::string linia;
    if(graphS.is_open())
    {
        if(verbose)std::cout<<"graf: " << graph << std::endl;
        
        while(std::getline(graphS, linia)){
            if(linia.size()>1){
            connections.push_back(parseLineGraph(linia));
            }
            
        }
    graphS.close();
    }else{
        helpMessage();
        std::cerr << "Nie podano pliku z grafem"<<std::endl;
        return 1;
    }
    
    //wczytywanie wierzchołków
    std::ifstream inputS (input);
    if(inputS.is_open())
    {
        if(verbose)std::cout<<"wierzcholki: "  << input << std::endl;
        if(std::getline(inputS, linia)){
            toCheck = parseLineVertex(linia);
        } else { 
            std::cerr << "plik z wierzchołkami jest pusty"<<std::endl;
            return 2;
        }
        inputS.close();
    }else{
        helpMessage();
        std::cerr << "Nie podano pliku wejściowego"<<std::endl;   
        return 1;
    }



    for(connection w : connections){
        if(nodes.find(w.origin)==nodes.end()){ //nie ma jeszcze takiego wierzchołka, musimy go dodać
            nodes[w.origin] = connectionToNode(w.origin, w);
        }else {//jest już taki wierzchołek, wystarczy dodać sąsiada
            nodes[w.origin].neighbours[w.destination] = w.weight;
        }
        if(w.directed == 0){ // krawędź nieskierowana -> połączenie w drugą stronę też możliwe
            if(nodes.find(w.destination)==nodes.end()){ //nie ma jeszcze takiego wierzchołka, musimy go dodać
                nodes[w.destination] = connectionToNode(w.destination, w);
            }else { //jest już taki wierzchołek, wystarczy dodać sąsiada
                nodes[w.destination].neighbours[w.origin] = w.weight;
            }
        }
    }
    
    std::ofstream outputS (output);
    if(outputS.is_open())
    {
        if(verbose)std::cout<<"plik wyjsciowy: " << output << std::endl;
        for(auto W : toCheck){
            std::cout << "Wierzchołek startowy: "<< W <<std::endl;
            if(nodes.find(W)==nodes.end()){
                std::cout << "Brak wierzchołka "<< W <<" w grafie"<<std::endl;
            }else{
                for (const auto Pair : nodes){
                    if(Pair.first!=W){
                        path shrtPath = findShortestPath(nodes, W, Pair.first);
                        
                        for(auto x : shrtPath.points){
                            std::cout << "->" << x;
                        }std::cout <<":"<< shrtPath.lenght<<std::endl;
                    }
                }
            }
        }
        outputS.close();
    }
    
    
    if(verbose){
        std::cout <<"Wczytane dane:"<<std::endl;
        for(connection w : connections){
            std::cout << "z: "<< w.origin<< std::endl;
            std::cout << "do: "<< w.destination << std::endl;
            std::cout << "waga: "<< w.weight<< std::endl;
            std::cout << "skierowana "<< w.directed << std::endl;
        }
        for (const auto& [key, value] : nodes) {
            std::cout << key << ":" <<std::endl;
            std::cout <<"Nazwa: "<< value.label <<std::endl;
            std::cout << "Wartość "<< value.value <<"\nSąsiednie:\n";
            
            for(const auto [wierzch, waga] : value.neighbours){
                std::cout << "\twierzchołek: " << wierzch << "; waga: " << waga << std::endl;
            }
        }
        std::cout <<"wierzchołki do sprawdzenia:"<<std::endl;
        for(auto x : toCheck){
            std::cout << "\t"<< x << std::endl;
        }
    }
    return 0;
}
