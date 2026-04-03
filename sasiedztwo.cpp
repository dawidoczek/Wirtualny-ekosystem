#include "sasiedztwo.h"
#include "generatorLosowy.h"

RodzajMieszkanca *Sasiedztwo::
    elementWewnetrzny(Polozenie polozenie)
{
    switch (polozenie)
    {
    case P:
        return &sasiad[0];
    case PG:
        return &sasiad[1];
    case G:
        return &sasiad[2];
    case LG:
        return &sasiad[3];
    case L:
        return &sasiad[4];
    case LD:
        return &sasiad[5];
    case D:
        return &sasiad[6];
    case PD:
        return &sasiad[7];
    case GORA:
        return &sasiad[8];
    case DOL:
        return &sasiad[9];
    case NIGDZIE:
        return nullptr;
    }
    return nullptr;
}
Polozenie Sasiedztwo::losujPolozenie()
{
    unsigned short min = 0, max = 9;
    return static_cast<Polozenie>(GeneratorLosowy::losujPomiedzy(min, max));
}
Sasiedztwo::Sasiedztwo(RodzajMieszkanca rodzaj)
{
    for (int i = 0; i < 10; ++i)
        sasiad[i] = rodzaj;
}
void Sasiedztwo::okreslSasiada(Polozenie polozenie, RodzajMieszkanca rodzaj)
{
    if (polozenie != NIGDZIE)
        *elementWewnetrzny(polozenie) = rodzaj;
}
RodzajMieszkanca Sasiedztwo::
    ktoJestSasiadem(Polozenie polozenie)
{
    if (polozenie != NIGDZIE)
        return *elementWewnetrzny(polozenie);
    else
        return NIEZNANE;
}
unsigned short Sasiedztwo::
    ile(RodzajMieszkanca rodzaj) const
{
    unsigned short licznik = 0;

    for (int i = 0; i < 10; ++i)
        if (sasiad[i] == rodzaj)
            licznik++;

    return licznik;
}
Polozenie Sasiedztwo::
    losujSasiada(RodzajMieszkanca rodzaj)
{
    if (ile(rodzaj) == 0)
        return NIGDZIE;
    else
    {
        Polozenie polozenie = Sasiedztwo::losujPolozenie();

        while (ktoJestSasiadem(polozenie) != rodzaj)
            polozenie = Sasiedztwo::losujPolozenie();

        return polozenie;
    }
}

void Sasiedztwo::
    zmienIdeksyWgPolozenia(Polozenie polozenie,
                           long &wiersz, long &kolumna, long &glebokosc)
{
    if (polozenie == PG || polozenie == P || polozenie == PD)
        kolumna++;
    // błąd, brak operatora logicznego OR
    // else if (polozenie == LG || polozenie == L | polozenie == LD)
    else if (polozenie == LG || polozenie == L || polozenie == LD)
        kolumna--;

    if (polozenie == LG || polozenie == G || polozenie == PG)
        wiersz--;
    else if (polozenie == LD || polozenie == D || polozenie == PD)
        wiersz++;

    if (polozenie == GORA)
        glebokosc++;
    else if (polozenie == DOL)
        glebokosc--;
}
void Sasiedztwo::
    zmienIdeksyWgPolozenia(Polozenie polozenie,
                           unsigned int &wiersz, unsigned int &kolumna,
                           unsigned int &glebokosc)
{
    long w = static_cast<long>(wiersz);
    long k = static_cast<long>(kolumna);
    long g = static_cast<long>(glebokosc);

    zmienIdeksyWgPolozenia(polozenie, w, k, g);

    wiersz = static_cast<unsigned int>(w);
    kolumna = static_cast<unsigned int>(k);
    glebokosc = static_cast<unsigned int>(g);
}
