#include "generatorLosowy.h"
#include <algorithm>
#include <iostream>

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


    // błąd, losujOdZeraDo(5) powinno zwracać 0, 1, 2, 3 lub 4, a nie 5
    // std::random_shuffle zakłada że funkcja zwraca wartosc 
    // z zakresu [0, max), a nie [0, max], więc powinno być max-1
    // std::uniform_int_distribution<int> dist(0, max);
    std::uniform_int_distribution<int> dist(0, max-1);
    return dist(GeneratorLosowy::device);
}

WektorIndeksow2D GeneratorLosowy::indeksyLosowe(unsigned int wiersze, unsigned int kolumny)
{
    WektorIndeksow2D indeksy;
    for (unsigned int w = 0; w < wiersze; w++)
        for (unsigned int k = 0; k < kolumny; k++)
            indeksy.push_back(Indeks2D(w, k));
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
// depraction
    std::random_shuffle(indeksy.begin(), indeksy.end(),
                        GeneratorLosowy::losujOdZeraDo);
#pragma clang diagnostic pop

    return indeksy;
}

WektorIndeksow3D GeneratorLosowy::indeksyLosowe(unsigned int wiersze, unsigned int kolumny, unsigned int glebokosc)
{
    WektorIndeksow3D indeksy;
    for (unsigned int w = 0; w < wiersze; w++)
        for (unsigned int k = 0; k < kolumny; k++)
            for (unsigned int g = 0; g < glebokosc; g++)
                indeksy.push_back(Indeks3D(w, k, g));

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
    std::random_shuffle(indeksy.begin(), indeksy.end(),
                        GeneratorLosowy::losujOdZeraDo);
#pragma clang diagnostic pop

    return indeksy;
}
