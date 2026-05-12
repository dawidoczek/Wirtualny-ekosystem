#pragma once
#include "ustawienia.h"

class Sasiedztwo
{

private:
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

    static void zmienIdeksyWgPolozenia(
        Polozenie polozenie, long &wiersz, long &kolumna, long &glebokosc);

    static void zmienIdeksyWgPolozenia(
        Polozenie polozenie, unsigned int &wiersz,
        unsigned int &kolumna,
        unsigned int &glebokosc);
};