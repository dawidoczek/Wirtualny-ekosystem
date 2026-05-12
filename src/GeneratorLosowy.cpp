#include "generatorLosowy.h"
#include <algorithm>
#include <iostream>
// https://www.cplusplus.com/reference/random/mt19937/
std::mt19937& GeneratorLosowy::generatorLosowyEngine()
{
    static std::mt19937 engine{std::random_device{}()};
    return engine;
}

std::random_device GeneratorLosowy::device;


unsigned short GeneratorLosowy::losujPomiedzy(unsigned short min, unsigned short max)
{
    if (min > max)
    {
        unsigned short t = min;
        min = max;
        max = t;
    }
    std::uniform_int_distribution<unsigned short>
        dist(min, max);

    return dist(GeneratorLosowy::device);
}
long GeneratorLosowy::losujPomiedzy(long min, long max)
{
    if (min > max)
    {
        long t = min;
        min = max;
        max = t;
    }

    std::uniform_int_distribution<long> dist(min, max);
    return dist(GeneratorLosowy::device);
}
int GeneratorLosowy::losujOdZeraDo(int max)
{
    if (max <= 0)
        return 0;

    std::uniform_int_distribution<int> dist(0, max-1);
    return dist(GeneratorLosowy::device);
}

WektorIndeksow2D GeneratorLosowy::indeksyLosowe(unsigned int wiersze, unsigned int kolumny)
{
    WektorIndeksow2D indeksy;
    for (unsigned int w = 0; w < wiersze; w++)
        for (unsigned int k = 0; k < kolumny; k++)
            indeksy.push_back(Indeks2D(w, k));
    std::shuffle(indeksy.begin(), indeksy.end(), generatorLosowyEngine());

    return indeksy;
}

WektorIndeksow3D GeneratorLosowy::indeksyLosowe(unsigned int wiersze, unsigned int kolumny, unsigned int glebokosc)
{
    WektorIndeksow3D indeksy;
    for (unsigned int w = 0; w < wiersze; w++)
        for (unsigned int k = 0; k < kolumny; k++)
            for (unsigned int g = 0; g < glebokosc; g++)
                indeksy.push_back(Indeks3D(w, k, g));

    std::shuffle(indeksy.begin(), indeksy.end(), generatorLosowyEngine());

    return indeksy;
}
