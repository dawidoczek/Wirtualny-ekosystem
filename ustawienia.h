#pragma once
#include <iostream>
#include <string>
#include <vector>
using namespace std;

/*
https://gist.github.com/JBlond/2fea43a3049b38287e5e9cefc87b2124
\e[0;31m	Red
\e[0;32m	Green
\e[0;33m	Yellow
\e[0;34m	Blue
\e[0;35m	Purple
\e[0;36m	Cyan
\e[0;37m	White
\e[0m	Reset
*/

enum RodzajMieszkanca
{
    GLON,
    GRZYB,
    BAKTERIA,
    PUSTKA,
    SCIANA,
    TRUP,
    NIEZNANE
};

enum AkcjaMieszkanca
{
    POTOMEK,
    POLOWANIE,
    ROZKLAD,
    NIC
};

enum Polozenie
{
    // Warstwa 0 (bieżący poziom)
    P = 0,
    PG = 1,
    G = 2,
    LG = 3,
    L = 4,
    LD = 5,
    D = 6,
    PD = 7,
    // Warstwa +1 (górna)
    P_GORA = 8,
    PG_GORA = 9,
    G_GORA = 10,
    LG_GORA = 11,
    L_GORA = 12,
    LD_GORA = 13,
    D_GORA = 14,
    PD_GORA = 15,
    GORA = 16,
    // Warstwa -1 (dolna)
    P_DOL = 17,
    PG_DOL = 18,
    G_DOL = 19,
    LG_DOL = 20,
    L_DOL = 21,
    LD_DOL = 22,
    D_DOL = 23,
    PD_DOL = 24,
    DOL = 25,
    NIGDZIE = 26
};

class UstawieniaSymulacji
{
public:
    const char
        znakGlon = '*',
        znakGrzyb = '#',
        znakBakteria = '@',
        znakTrup = '+',
        znakNieokreslony = '?',
        znakPustaNisza = '_',
        znakSeparator = ' ';

    unsigned short
        glonZycieMin = 5,
        glonZycieMax = 10,
        glonKosztPotomka = 2,
        glonLimitPosilkow = 6,

        grzybZycieMin = 40,
        grzybZycieMax = 60,
        grzybKosztPotomka = 3,
        grzybLimitPosilkow = 30,

        bakteriaZycieMin = 25,
        bakteriaZycieMax = 40,
        bakteriaKosztPotomka = 3,
        bakteriaLimitPosilkow = 10;

    bool poprawnyZnakNiszy(char znak) const
    {
        return znak == znakGlon ||
               znak == znakGrzyb ||
               znak == znakBakteria ||
               znak == znakTrup ||
               znak == znakPustaNisza;
    }

    bool poprawnySeparator(char znak) const
    {
        return znak == znakSeparator;
    }

private:
    UstawieniaSymulacji(){};
    UstawieniaSymulacji(UstawieniaSymulacji &);

public:
    static UstawieniaSymulacji &pobierzUstawienia()
    {
        static UstawieniaSymulacji ustawienia;
        return ustawienia;
    }
};


static char sep = UstawieniaSymulacji::
                      pobierzUstawienia()
                          .znakSeparator;

