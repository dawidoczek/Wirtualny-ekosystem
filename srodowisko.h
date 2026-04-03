#pragma once
#include <fstream>
#include <sstream>
#include "osobniki.h"
#include "ustawienia.h"

class Nisza;
class Mieszkaniec;
class Sasiedztwo;

class Srodowisko
{

public:

    const unsigned int wiersze, kolumny;
    const unsigned long liczbaNisz;

private:
    Nisza **nisza;
        

public:
    Srodowisko(unsigned int _wiersze,
               unsigned int _kolumny);
    static Srodowisko czytajZPliku(std::string nazwaPliku);
    ~Srodowisko();
    void operator++(int) { wykonajKrokSymulacji(); }

    bool operator!() { return !martwy(); }

    void lokuj(Mieszkaniec *mieszkaniec,
               unsigned int wiersz, unsigned int kolumna);

    Sasiedztwo ustalSasiedztwo(unsigned int wiersz,
                               unsigned int kolumna) const;

    unsigned long liczba(RodzajMieszkanca rodzaj) const;

    bool martwy();

    void wykonajSkok(unsigned int wiersz,
                     unsigned int kolumna);

    void wykonajAkcje(unsigned int wiersz,
                      unsigned int kolumna);

    void wykonajKrokSymulacji();

    std::string doTekstu() const;
};
