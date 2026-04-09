```
    \   /\
    )  ( ')
    (  /  )
     \(__)|
    
By
Dawid Rej
```
# Symulacja Ekosystemu (programowanie obiektowe C++ LAB MDK )

<img src="https://github.com/user-attachments/assets/83519708-b42a-4dd6-a1a4-4dabc6e3964e" width="200">

Prosty projekt symulacji ekosystemu na siatce 2D zrealizowany w ramach laboratoriów z Programowania Obiektowego.


## Wymagania

- kompilator C++
- `make`

## instalowanie
W konsoli wpisz 
```bash
git clone https://github.com/dawidoczek/Wirtualny-ekosystem.git
```

## Uruchamianie

```bash
make run
```

lub ręcznie

```bash
c++ -Wall -Wextra -g3 *.cpp -o output/main && ./output/main
```

## Struktura projektu

```text
/Wirtualny-ekosystem
├── main.cpp              # start programu i petla tur
├── srodowisko.h          # logika swiata i przebieg symulacji
├── nisza.h               # pojedyncze pole planszy
├── sasiedztwo.h          # analiza sasiedztwa i kierunki ruchu
├── mieszkaniec.h         # baza dla wszystkich bytow
├── organizm.h            # wspolna logika zycia organizmow
├── osobniki.h            # Glon, Grzyb, Bakteria
├── generatorLosowy.h     # losowania
├── ustawienia.h          # typy, enumy i konfiguracja
├── ustawienia.txt        # dane konfiguracyjne
├── start.txt             # dane wejsciowe
└── symulacja.txt         # dane wyjsciowe: zapis kolejnych tur
```

## Opis projektu

Aplikacja symuluje zamknięte środowisko (reprezentowane jako prostokątna siatka nisz/komórek), w którym żyją i oddziałują na siebie trzy gatunki wirtualnych organizmów:
* **Glony (`*`)** – samożywne organizmy prowadzące fotosyntezę.
* **Grzyby (`#`)** – każdy wie czym jest grzyb, żywi się szczątkami.
* **Bakterie (`@`)** – organizmy drapieżne, polujące w pierwszej kolejności na glony, a w przypadku ich braku – na inne bakterie.
* **Martwe organizmy (`+`)** – organizmy, które przekroczyły swój maksymalny wiek.
* **Puste nisze (`_`)** – niezajęte komórki środowiska.

Rozwój ekosystemu przebiega w dyskretnych krokach czasowych (turach). W każdej turze organizmy wykonują swoje funkcje życiowe na podstawie analizy swojego bezpośredniego sąsiedztwa (8 sąsiednich komórek).

## Funkcjonalności

* **Cykl życia organizmów:** Każdy organizm rodzi się, starzeje, odżywia, rozmnaża, porusza i ostatecznie umiera.
* **Złożone interakcje:** Organizmy reagują na otoczenie w zależności od swojego gatunku (np. bakterie szukają ofiar, grzyby szukają martwej materii organicznej, głodne organizmy się nie rozmnażają).
* **Losowość:** Wykorzystanie generatora liczb pseudolosowych (biblioteka `<random>`) do decydowania o ruchu, maksymalnym wieku organizmu czy kierunku rozmnażania.
* **Animacja konsolowa:** Przebieg symulacji jest wizualizowany w oknie terminala/konsoli z podsumowaniem statystyk populacji (liczba glonów, grzybów, bakterii oraz martwych ciał w danej turze).

## Wykorzystane technologie i koncepcje

Projekt został napisany w standardzie **C++** i silnie opiera się na zaawansowanych mechanizmach obiektowych:
* **Polimorfizm i funkcje wirtualne:** Abstrakcyjna klasa bazowa `Mieszkaniec` oraz hierarchia dziedziczenia (`Organizm` -> `Glon`, `Grzyb`, `Bakteria`).
* **Hermetyzacja:** Ścisłe ukrywanie danych w klasach (`Srodowisko`, `Nisza`, `Sasiedztwo`).
* **Wzorce projektowe:** Użycie wzorca **Singleton** do przechowywania globalnych parametrów symulacji (klasa `UstawieniaSymulacji`).
* **Zarządzanie pamięcią:** Dynamiczna alokacja pamięci dla dwuwymiarowej tablicy środowiska oraz bezpieczne zwalnianie wskaźników do obiektów.
* **Przeciążanie operatorów:** M.in. operator postinkrementacji `++` do wywoływania kolejnego kroku symulacji oraz operator wprowadzania do strumienia `<<` do wizualizacji.
* **STL:** Wykorzystanie standardowych kontenerów i narzędzi, takich jak `std::vector`, `std::string` czy algorytmów przetasowania.

# Architektura klas

Główne klasy wchodzące w skład aplikacji:
* `Srodowisko` – zarządza całą siatką symulacji oraz listą mieszkańców.
* `Nisza` – pojedyncza komórka środowiska, przechowująca wskaźnik do lokatora.
* `Sasiedztwo` – klasa pomocnicza analizująca 8 komórek wokół konkretnego organizmu.
* `Mieszkaniec` / `Organizm` – klasy bazowe definiujące podstawowe liczniki (życia, posiłków) i stany (żywy, głodny, pączkujący).
* `Glon`, `Grzyb`, `Bakteria` – klasy pochodne implementujące specyficzne zachowania (np. metodę wirtualną `wybierzAkcje()`).
* `GeneratorLosowy` – statyczna klasa pomocnicza ułatwiająca losowanie.
