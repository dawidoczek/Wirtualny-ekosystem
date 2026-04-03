#pragma once
#pragma once
#include <algorithm>
#include <random>
#include <vector>

class Indeks2D
{
public:
    unsigned int wiersz, kolumna;
    Indeks2D(unsigned int _w = 0, unsigned int _k = 0)
        : wiersz(_w), kolumna(_k) {}
};

class Indeks3D
{
public:
    unsigned int wiersz, kolumna, glebokosc;
    Indeks3D(unsigned int _w = 0, unsigned int _k = 0, unsigned int _g = 0)
        : wiersz(_w), kolumna(_k), glebokosc(_g) {}
};

typedef std::vector<Indeks2D> WektorIndeksow2D;
typedef std::vector<Indeks3D> WektorIndeksow3D;
class GeneratorLosowy
{
private:
    static std::random_device device;
    GeneratorLosowy() {}

public:
    static unsigned short losujPomiedzy(unsigned short min, unsigned short max);
    static WektorIndeksow2D indeksyLosowe(unsigned int wiersze, unsigned int kolumny);
    static WektorIndeksow3D indeksyLosowe(unsigned int wiersze, unsigned int kolumny, unsigned int glebokosc);
    static long losujPomiedzy(long min, long max);
    static int losujOdZeraDo(int max);
};

typedef GeneratorLosowy GEN;