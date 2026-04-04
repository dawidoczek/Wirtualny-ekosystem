#pragma once
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <cctype>
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
    P = 0,
    PG = 1,
    G = 2,
    LG = 3,
    L = 4,
    LD = 5,
    D = 6,
    PD = 7,
    NIGDZIE = 8
};

class UstawieniaSymulacji
{
public:
    char
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

    bool wczytajZPliku(const string &nazwaPliku = "ustawienia.txt")
    {
        ifstream plik(nazwaPliku);
        if (!plik.is_open())
            return false;

        auto trim = [](string &s)
        {
            while (!s.empty() && isspace(static_cast<unsigned char>(s.front())))
                s.erase(s.begin());
            while (!s.empty() && isspace(static_cast<unsigned char>(s.back())))
                s.pop_back();
        };

        string klucz, wartosc;
        while (getline(plik, klucz, ';'))
        {
            if (!getline(plik, wartosc, ','))
                break;

            trim(klucz);
            trim(wartosc);
            if (klucz.empty() || wartosc.empty())
                continue;

            if (klucz == "znakGlon")
                znakGlon = wartosc[0];
            else if (klucz == "znakGrzyb")
                znakGrzyb = wartosc[0];
            else if (klucz == "znakBakteria")
                znakBakteria = wartosc[0];
            else if (klucz == "znakTrup")
                znakTrup = wartosc[0];
            else if (klucz == "znakNieokreslony")
                znakNieokreslony = wartosc[0];
            else if (klucz == "znakPustaNisza")
                znakPustaNisza = wartosc[0];
            else if (klucz == "znakSeparator")
                znakSeparator = wartosc[0];
            else if (klucz == "glonZycieMin")
                glonZycieMin = static_cast<unsigned short>(stoi(wartosc));
            else if (klucz == "glonZycieMax")
                glonZycieMax = static_cast<unsigned short>(stoi(wartosc));
            else if (klucz == "glonKosztPotomka")
                glonKosztPotomka = static_cast<unsigned short>(stoi(wartosc));
            else if (klucz == "glonLimitPosilkow")
                glonLimitPosilkow = static_cast<unsigned short>(stoi(wartosc));
            else if (klucz == "grzybZycieMin")
                grzybZycieMin = static_cast<unsigned short>(stoi(wartosc));
            else if (klucz == "grzybZycieMax")
                grzybZycieMax = static_cast<unsigned short>(stoi(wartosc));
            else if (klucz == "grzybKosztPotomka")
                grzybKosztPotomka = static_cast<unsigned short>(stoi(wartosc));
            else if (klucz == "grzybLimitPosilkow")
                grzybLimitPosilkow = static_cast<unsigned short>(stoi(wartosc));
            else if (klucz == "bakteriaZycieMin")
                bakteriaZycieMin = static_cast<unsigned short>(stoi(wartosc));
            else if (klucz == "bakteriaZycieMax")
                bakteriaZycieMax = static_cast<unsigned short>(stoi(wartosc));
            else if (klucz == "bakteriaKosztPotomka")
                bakteriaKosztPotomka = static_cast<unsigned short>(stoi(wartosc));
            else if (klucz == "bakteriaLimitPosilkow")
                bakteriaLimitPosilkow = static_cast<unsigned short>(stoi(wartosc));
        }

        plik.close();
        return true;
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

