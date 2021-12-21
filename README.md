# zadanie_programistyczne_dijkstra
Program znajduje najkrótszą drogę miedzy wierzchołkami wykorzystując algorytm Dijksry
Program przyjmuje następujące argumenty:
```-g 'nazwa pliku' plik wejściowy zawierający graf.
-w 'nazwa pliku' plik wejściowy zawierający listę wierzchołków do zbadania.
-o 'nazwa pliku' plik wyjściowy.
-v Verbose, tryb wypisywania dodatkowych informacji.
-h wypisuje wiadomość pomocy.
```
krawędzie w grafie reprezentowane są w następujący sposób:
- `"początek" -> "koniec" : <waga>`
dla krawędzi skierowanej, oraz 
- `"początek" - "koniec" : <waga>`
dla krawędzi nieskierowanej
