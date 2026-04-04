```
    \   /\
    )  ( ')
    (  /  )
    \(__)|
    
By
Dawid Rej
```
# Symulacja Ekosystemu 3D (programowanie obiektowe C++)

![ekosystem2 3d](https://github.com/user-attachments/assets/6ad21c4b-baa7-4142-8d3e-10324fe795e0)



Projekt symulacji ekosystemu rozwinięty do wersji 3D z interaktywną wizualizacją w terminalu.

## INSPIRACJE:
- [donut c](https://www.a1k0n.net/2011/07/20/donut-math.html)
- [cube spin](https://github.com/alt44s/cube-spin)
- [2D-Line-Drawing-Algorithms](https://github.com/tashi-2004/2D-Line-Drawing-Algorithms/blob/main/Header.h)

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
- Wizualizacja została przeniesiona do renderera z biblioteki ncurses z auto obrotem bryły i panelem statystyk.
- Symulacja działa krokowo: obrót jest ciągły (co 50ms), ale krok świata wykonuje się po kliknieciu `ENTER`.
- Dodano tryb inspekcji (pauza obrotu, ręczne przesuwanie i obracanie widoku).
- Dodano przełączaną obwódkę 3D sześcianu środowiska (12 linii-krawędzi).
