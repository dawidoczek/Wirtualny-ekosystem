#pragma once
#include <iostream>
#include "sasiedztwo.h"
#include "mieszkaniec.h"
using namespace std;
class Organizm
{
public:
    const unsigned short limitPosilkow;
    const unsigned short kosztPotomka;

private:
    unsigned short licznikZycia;
    unsigned short licznikPosilkow;

public:
    Organizm(unsigned short dlugoscZycia,
             unsigned short limitPosilkow,
             unsigned short kosztPotomka);

    bool zywy() const
    {
        return licznikZycia > 0;
    }

    bool glodny() const
    {
        return zywy() && licznikPosilkow < limitPosilkow;
    }

    bool paczkujacy() const
    {
        return zywy() && licznikPosilkow > kosztPotomka;
    }

    unsigned short stanLicznikaZycia() const
    {
        return licznikZycia;
    }

    unsigned short stanLicznikaPosilkow() const
    {
        return licznikPosilkow;
    }

    bool posilek();
    bool potomek();

    void krokSymulacji();
};


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