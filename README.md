```
    \   /\
    )  ( ')
    (  /  )
    \(__)|
    
By
Dawid Rej
```
# Symulacja Ekosystemu 3D (programowanie obiektowe C++ LAB MDK )



Projekt symulacji ekosystemu rozwinięty do wersji 3D z interaktywną wizualizacją w terminalu.

## Sterowanie w wizualizacji

- `ENTER`: wykonuje jeden krok symulacji
- `SPACJA`: przełącza tryb auto-obrotu / tryb inspekcji
- `Strzałki` (w trybie inspekcji): przesuwają położenie ekosystemu
- `Shift + Strzałki` (w trybie inspekcji): obracają widok
- `N`: włącza/wyłącza obwódkę 3D (12 krawędzi sześcianu)
- `Q`: wyjście

## Co zmieniono względem projektu bazowego

- Środowisko zostało rozszerzone z 2D do 3D (wiersze, kolumny, głębokość).
- Generator indeksów i logika sąsiedztwa obsługują układ 3D.
- Wizualizacja została przeniesiona do renderera ncurses z obrotem bryły i panelem statystyk.
- Symulacja działa krokowo: obrót jest ciągły, ale krok świata wykonuje się po `ENTER`.
- Dodano tryb inspekcji (pauza obrotu, ręczne przesuwanie i obracanie widoku).
- Dodano przełączaną obwódkę 3D sześcianu środowiska (12 linii-krawędzi).
