#pragma once
#include "ustawienia.h"

class Sasiedztwo
{

private:
    // tu dodałem czesc implementacji 3d
    // 26 bo 9 na dole, 9 na gorze i 8 na tym samym poziomie
    // na marginesie naco nam tablica 2d to nw
    RodzajMieszkanca sasiad[26];

    RodzajMieszkanca *elementWewnetrzny(
        Polozenie polozenie);

    static Polozenie losujPolozenie();

public:
    Sasiedztwo(RodzajMieszkanca rodzaj = NIEZNANE);

    void okreslSasiada(Polozenie polozenie,
                       RodzajMieszkanca rodzaj);

    RodzajMieszkanca ktoJestSasiadem(Polozenie polozenie);

    unsigned short ile(RodzajMieszkanca rodzaj) const;

    Polozenie losujSasiada(RodzajMieszkanca rodzaj);
    // tu dodałem czesc implementacji 3d
    static void zmienIdeksyWgPolozenia(
        Polozenie polozenie, long &wiersz, long &kolumna, long &glebokosc);

    static void zmienIdeksyWgPolozenia(
        Polozenie polozenie, unsigned int &wiersz,
        unsigned int &kolumna,
        unsigned int &glebokosc);
};