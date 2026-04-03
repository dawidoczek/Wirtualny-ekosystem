#pragma once
#include "sasiedztwo.h"
#include "mieszkaniec.h"
#include "ustawienia.h"
#include "srodowisko.h"

class Nisza
{
private:
    Mieszkaniec *lokator;
    unsigned long ostatniaTuraObslugi;

public:
    Nisza();
    Nisza(Nisza &innaNisza);
    ~Nisza();

    Nisza &operator=(Nisza &innaNisza);

    void przyjmijLokatora(Mieszkaniec *&lokatorBezdomny);

    Mieszkaniec *oddajLokatora();

    bool zajeta() const { return lokator != nullptr; }

    RodzajMieszkanca ktoTuMieszka()
    {
        return zajeta() ? lokator->kimJestes() : PUSTKA;
    }

    bool lokatorZywy() const;

    char jakiSymbol() const;

    bool juzObsluzonaWTurze() const;

    void oznaczObslugeWTurze();

private:
    // friend class Srodowisko;
    friend void Srodowisko::wykonajAkcje(unsigned int wiersz,unsigned int kolumna);
    ZamiarMieszkanca aktywujLokatora(
        Sasiedztwo sasiedztwo)
    {
        return lokator->wybierzAkcje(sasiedztwo);
    }

    Mieszkaniec *wypuscPotomka()
    {
        return lokator->dajPotomka();
    }

    void przyjmijZdobycz(Mieszkaniec *ofiara)
    {
        lokator->przyjmijZdobycz(ofiara);
    }
};

