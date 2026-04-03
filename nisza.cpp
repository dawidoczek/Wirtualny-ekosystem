#include "nisza.h"
#include "srodowisko.h"

Nisza::Nisza() : lokator(nullptr), ostatniaTuraObslugi(0) {}

Nisza::Nisza(Nisza &innaNisza)
{

    if (innaNisza.zajeta())
    {
        lokator = innaNisza.lokator;
        innaNisza.lokator = nullptr;
        ostatniaTuraObslugi = innaNisza.ostatniaTuraObslugi;
    }
    else
    {
        lokator = nullptr;
        ostatniaTuraObslugi = innaNisza.ostatniaTuraObslugi;
    }
}

Nisza::~Nisza()
{
    if (lokator != nullptr)
        delete lokator;
}

Nisza &Nisza::operator=(Nisza &innaNisza)
{
    Mieszkaniec *tmp = lokator;
    lokator = innaNisza.lokator;
    innaNisza.lokator = tmp;
    unsigned long t = ostatniaTuraObslugi;
    ostatniaTuraObslugi = innaNisza.ostatniaTuraObslugi;
    innaNisza.ostatniaTuraObslugi = t;
    return *this;
}
// tutaj jest błąd a nawet 2, brak nawiasu zamykającego i
// chcemy zmienic oryginalny wskaznik, a nie jego kopie, 
// więc powinno być Mieszkaniec *&lokatorBezdomny
// void Nisza::przyjmijLokatora(Mieszkaniec *lokatorBezdomny
void Nisza::przyjmijLokatora(Mieszkaniec *&lokatorBezdomny)
{
    if (!zajeta())
    {
        // ustawia nam null ptr na kopie, a nie orginal
        lokator = lokatorBezdomny;
        lokatorBezdomny = nullptr;
        ostatniaTuraObslugi = Srodowisko::pobierzAktualnaTure();
    }
}

Mieszkaniec *Nisza::oddajLokatora()
{
    Mieszkaniec *tmp = nullptr;

    if (zajeta())
    {
        tmp = lokator;
        lokator = nullptr;
    }

    return tmp;
}

bool Nisza::lokatorZywy() const
{
    if (zajeta())
    {
        RodzajMieszkanca r = lokator->kimJestes();
        return r == GLON || r == GRZYB || r == BAKTERIA;
    }
    else
        return false;
}

char Nisza::jakiSymbol() const
{
    if (!zajeta())
        return UstawieniaSymulacji::
            pobierzUstawienia()
                .znakPustaNisza;
    else
        return lokator->jakiSymbol();
}

bool Nisza::juzObsluzonaWTurze() const
{
    return ostatniaTuraObslugi == Srodowisko::pobierzAktualnaTure();
}

void Nisza::oznaczObslugeWTurze()
{
    ostatniaTuraObslugi = Srodowisko::pobierzAktualnaTure();
}