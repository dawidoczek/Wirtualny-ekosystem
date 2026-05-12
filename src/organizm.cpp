#include "organizm.h"


bool Organizm::posilek()
{
    if (glodny())
    {
        licznikPosilkow++;
        return true;
    }
    else
        return false;
}
bool Organizm::potomek()
{
    if (paczkujacy())
    {
        licznikPosilkow -= kosztPotomka;
        return true;
    }
    else
        return false;
}
Organizm::Organizm(unsigned short dlugoscZycia,
                   unsigned short limitPosilkow,
                   unsigned short kosztPotomka) : limitPosilkow(limitPosilkow),
                                                  kosztPotomka(kosztPotomka),
                                                  licznikZycia(dlugoscZycia),
                                                  licznikPosilkow(0)
{
}



void drukujOrganizm(const Organizm &o)
{
    cout << "licznikZycia: "
         << o.stanLicznikaZycia()
         << " licznikPosilkow: "
         << o.stanLicznikaPosilkow() << endl
         << "limitPosilkow: "
         << o.limitPosilkow
         << " kosztPotomka: "
         << o.kosztPotomka << endl
         << "głodny: " << o.glodny()
         << " pączkujący: "
         << o.paczkujacy() << endl
         << endl;
}

void Organizm::krokSymulacji()
{
    if (zywy())
        licznikZycia--;
}
