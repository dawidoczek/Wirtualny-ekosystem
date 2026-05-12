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

    const unsigned int wiersze, kolumny, glebokosc;
    const unsigned long liczbaNisz;
    unsigned long numerTury = 0;
    static unsigned long aktualnaTura;

private:
    Nisza ***nisza;
        

public:
    Srodowisko(unsigned int _wiersze,
               unsigned int _kolumny,
               unsigned int _glebokosc = 1);
    static Srodowisko czytajZPliku(std::string nazwaPliku);
    ~Srodowisko();
    void operator++(int) { wykonajKrokSymulacji(); }

    bool operator!() { return !martwy(); }

    void lokuj(Mieszkaniec *mieszkaniec,
               unsigned int wiersz, unsigned int kolumna,
               unsigned int glebokosc = 0);

    Sasiedztwo ustalSasiedztwo(unsigned int wiersz,
                               unsigned int kolumna,
                               unsigned int glebokosc) const;

    unsigned long liczba(RodzajMieszkanca rodzaj) const;

    bool martwy();

    void wykonajSkok(unsigned int wiersz,
                     unsigned int kolumna,
                     unsigned int glebokosc);

    void wykonajAkcje(unsigned int wiersz,
                      unsigned int kolumna,
                      unsigned int glebokosc);

    void wykonajKrokSymulacji();

    std::string doTekstu() const;

    char symbolNiszy(unsigned int wiersz,
                     unsigned int kolumna,
                     unsigned int glebokosc) const;

    RodzajMieszkanca rodzajNiszy(unsigned int wiersz,
                                  unsigned int kolumna,
                                  unsigned int glebokosc) const;

    static unsigned long pobierzAktualnaTure();
};
