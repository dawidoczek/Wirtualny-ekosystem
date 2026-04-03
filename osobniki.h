#pragma once
#pragma once
#include "organizm.h"
#include "mieszkaniec.h"
#include "generatorLosowy.h"
#include "sasiedztwo.h"
#include "ustawienia.h"

static const UstawieniaSymulacji &UST = UstawieniaSymulacji::pobierzUstawienia();


class Glon : protected Organizm, public Mieszkaniec
{
public:
    Glon();
    virtual RodzajMieszkanca kimJestes() final;

    virtual ZamiarMieszkanca
    wybierzAkcje(Sasiedztwo sasiedztwo);

    virtual Mieszkaniec *dajPotomka();

    virtual void przyjmijZdobycz(Mieszkaniec *mieszkaniec);
};
class Grzyb : protected Organizm, public Mieszkaniec
{
public:
    Grzyb();
    virtual RodzajMieszkanca kimJestes() final;

    virtual ZamiarMieszkanca
    wybierzAkcje(Sasiedztwo sasiedztwo);

    virtual Mieszkaniec *dajPotomka();

    virtual void przyjmijZdobycz(Mieszkaniec *mieszkaniec);
};
class Bakteria : protected Organizm, public Mieszkaniec
{
public:
    Bakteria();
    virtual RodzajMieszkanca kimJestes() final;

    virtual ZamiarMieszkanca
    wybierzAkcje(Sasiedztwo sasiedztwo);

    virtual Mieszkaniec *dajPotomka();
    virtual void przyjmijZdobycz(Mieszkaniec *mieszkaniec);
};