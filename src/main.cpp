#include "srodowisko.h"
#include "osobniki.h"
#include "generatorLosowy.h"

int rysuj3d(Srodowisko &ekoSystem);

static void losowoRozmiesc(Srodowisko &ekoSystem,
                           unsigned int ile,
                           RodzajMieszkanca rodzaj)
{
    unsigned int rozmieszczono = 0;
    unsigned int proby = 0;
    unsigned int limitProb = ile * 200 + 1000;

    while (rozmieszczono < ile && proby < limitProb)
    {
        proby++;
        unsigned int w = GeneratorLosowy::losujPomiedzy(0L, static_cast<long>(ekoSystem.wiersze) - 1L);
        unsigned int k = GeneratorLosowy::losujPomiedzy(0L, static_cast<long>(ekoSystem.kolumny) - 1L);
        unsigned int g = GeneratorLosowy::losujPomiedzy(0L, static_cast<long>(ekoSystem.glebokosc) - 1L);

        if (ekoSystem.rodzajNiszy(w, k, g) != PUSTKA)
            continue;

        if (rodzaj == GLON)
            ekoSystem.lokuj(new Glon(), w, k, g);
        else if (rodzaj == GRZYB)
            ekoSystem.lokuj(new Grzyb(), w, k, g);
        else if (rodzaj == BAKTERIA)
            ekoSystem.lokuj(new Bakteria(), w, k, g);

        rozmieszczono++;
    }
}

int main()
{
    Srodowisko ekoSystem(5, 5, 5);

    losowoRozmiesc(ekoSystem, 20, GLON);
    losowoRozmiesc(ekoSystem, 20, GRZYB);
    losowoRozmiesc(ekoSystem, 20, BAKTERIA);

    return rysuj3d(ekoSystem);
}