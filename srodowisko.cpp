#include "srodowisko.h"
#include <fstream>
#include <sstream>
#include "generatorLosowy.h"
#include "osobniki.h"
#include "nisza.h"

unsigned long Srodowisko::aktualnaTura = 0;

unsigned long Srodowisko::pobierzAktualnaTure()
{
    return aktualnaTura;
}

Srodowisko Srodowisko::czytajZPliku(std::string nazwaPliku)
{
    std::ifstream plik(nazwaPliku);

    std::stringstream tekst("");
    if(plik)
    {
        tekst << plik.rdbuf();
        plik.close();
    }

    std::string zapis = tekst.str();

    unsigned int wiersze = 0, kolumny = 0;
    bool pierwszaLinia = true;
    for (char c : zapis)
    {
        if (c != '\n')
        {
            if (pierwszaLinia && c != ' ')
                kolumny++;
        }
        else
        {
            pierwszaLinia = false;
            if (c == '\n')
                wiersze++;
        }
    }

    Srodowisko srodowisko(wiersze, kolumny, 1);

    char glon = UstawieniaSymulacji ::pobierzUstawienia().znakGlon;
    char grzyb = UstawieniaSymulacji ::pobierzUstawienia().znakGrzyb;
    char bakteria = UstawieniaSymulacji ::pobierzUstawienia().znakBakteria;
    char pusta = UstawieniaSymulacji ::pobierzUstawienia().znakPustaNisza;

    char znak;
    for (unsigned int w = 0; w < wiersze; w++)
    {
        getline(tekst, zapis);
        for (unsigned int k = 0; k < 2 * kolumny; k += 2)
        {

            znak = k < zapis.size() ? zapis[k] : pusta;

            if (znak == glon)
                srodowisko.lokuj(new Glon(), w, k / 2, 0);
            else if (znak == grzyb)
                srodowisko.lokuj(new Grzyb(), w, k / 2, 0);
            else if (znak == bakteria)
                srodowisko.lokuj(new Bakteria(), w, k / 2, 0);
        }
    }

    return srodowisko;
}

Srodowisko::~Srodowisko()
{
    for (unsigned int i = 0; i < wiersze; i++)
    {
        for (unsigned int j = 0; j < kolumny; j++)
            delete[] nisza[i][j];
        delete[] nisza[i];
    }
    delete[] nisza;
}

void Srodowisko::lokuj(Mieszkaniec *mieszkaniec,
                       unsigned int wiersz, unsigned int kolumna,
                       unsigned int g)
{
    if (wiersz < wiersze && kolumna < kolumny && g < glebokosc)
        nisza[wiersz][kolumna][g].przyjmijLokatora(mieszkaniec);
}

Sasiedztwo Srodowisko::ustalSasiedztwo(
    unsigned int wiersz, unsigned int kolumna, unsigned int g) const
{
    Sasiedztwo sasiedztwo(SCIANA);

    long wiersz1, kolumna1, g1;

    for (Polozenie p : {P, PG, G, LG, L, LD, D, PD, GORA, DOL})
    {
        wiersz1 = wiersz;
        kolumna1 = kolumna;
        g1 = g;

        Sasiedztwo::
            zmienIdeksyWgPolozenia(p, wiersz1, kolumna1, g1);

        if (wiersz1 >= 0 &&
            wiersz1 < wiersze &&
            kolumna1 >= 0 &&
            kolumna1 < kolumny &&
            g1 >= 0 &&
            g1 < glebokosc)
        {
            sasiedztwo.okreslSasiada(p,
                                     nisza[wiersz1][kolumna1][g1].ktoTuMieszka());
        }
    }

    return sasiedztwo;
}
unsigned long Srodowisko::
    liczba(RodzajMieszkanca rodzaj) const
{
    unsigned long licznik = 0;

    for (unsigned int w = 0; w < wiersze; w++)
        for (unsigned int k = 0; k < kolumny; k++)
            for (unsigned int g = 0; g < glebokosc; g++)
                if (nisza[w][k][g].ktoTuMieszka() == rodzaj)
                    licznik++;

    return licznik;
}
bool Srodowisko::martwy()
{
    return liczba(GLON) + liczba(GRZYB) + liczba(BAKTERIA) == 0;
}
void Srodowisko::wykonajSkok(unsigned int wiersz,
                             unsigned int kolumna,
                             unsigned int g)
{

    if (!nisza[wiersz][kolumna][g].lokatorZywy() ||
        nisza[wiersz][kolumna][g].ktoTuMieszka() == GLON)
        return;

    Sasiedztwo sasiedztwo =
        ustalSasiedztwo(wiersz, kolumna, g);

    if (sasiedztwo.ile(PUSTKA) > 0)
    {

        Polozenie polozenie =
            sasiedztwo.losujSasiada(PUSTKA);

        unsigned int wiersz1 = wiersz, kolumna1 = kolumna, g1 = g;

        Sasiedztwo::zmienIdeksyWgPolozenia(polozenie,
                                           wiersz1, kolumna1, g1);

        nisza[wiersz1][kolumna1][g1] = nisza[wiersz][kolumna][g];
    }
}

void Srodowisko::wykonajAkcje(unsigned int wiersz,
                              unsigned int kolumna,
                              unsigned int g)
{
    if (!nisza[wiersz][kolumna][g].lokatorZywy())
        return;

    Sasiedztwo sasiedztwo = ustalSasiedztwo(wiersz, kolumna, g);


    ZamiarMieszkanca zamiar =
        nisza[wiersz][kolumna][g].aktywujLokatora(sasiedztwo);

    unsigned int wiersz1 = wiersz, kolumna1 = kolumna, g1 = g;

    Sasiedztwo::zmienIdeksyWgPolozenia(zamiar.gdzie,
                                       wiersz1, kolumna1, g1);

    Mieszkaniec *m = nullptr;

    switch (zamiar.akcja)
    {
    case NIC:
        wykonajSkok(wiersz, kolumna, g);
        break;

    case POTOMEK:
        m = nisza[wiersz][kolumna][g].wypuscPotomka();
        nisza[wiersz1][kolumna1][g1].przyjmijLokatora(m);
        break;

    case POLOWANIE:
    case ROZKLAD:
        m = nisza[wiersz1][kolumna1][g1].oddajLokatora();
        nisza[wiersz][kolumna][g].przyjmijZdobycz(m);
        break;
    }
}
void Srodowisko::wykonajKrokSymulacji()
{
    numerTury++;
    aktualnaTura = numerTury;
    WektorIndeksow3D indeksyLosowe =
        GeneratorLosowy::indeksyLosowe(wiersze, kolumny, glebokosc);
    // błąd, organizm moze wykonac kilka ruchow w jednej turze
    // uznajmy ze bakteria na [0,0] wykona ruch na [1,1]
    // i potem przeiterujemy przez [1,1], to wykona ona kolejną akcje
    for (Indeks3D indeks : indeksyLosowe)
    {
        if (nisza[indeks.wiersz][indeks.kolumna][indeks.glebokosc].juzObsluzonaWTurze())
            continue;

        nisza[indeks.wiersz][indeks.kolumna][indeks.glebokosc].oznaczObslugeWTurze();
        wykonajAkcje(indeks.wiersz, indeks.kolumna, indeks.glebokosc);
    }
}

std::string Srodowisko::doTekstu() const
{
    std::string tekst = "";
    char sep = UstawieniaSymulacji::
                   pobierzUstawienia()
                       .znakSeparator;

    for (unsigned int g = 0; g < glebokosc; g++)
    {
        tekst += "Warstwa z=" + std::to_string(g) + "\n";

        for (unsigned int w = 0; w < wiersze; w++)
        {
            for (unsigned int k = 0; k < kolumny; k++)
            {
                if (k > 0)
                    tekst += sep;
                tekst += nisza[w][k][g].jakiSymbol();
            }

            tekst += '\n';
        }

        tekst += '\n';
    }

    tekst += "\n Glony * : " + std::to_string(liczba(GLON)) + "\n Grzyby # : " + std::to_string(liczba(GRZYB)) + "\nBakterie @ : " + std::to_string(liczba(BAKTERIA)) + "\n Martwe + : " + std::to_string(liczba(TRUP)) + '\n';

    return tekst;
    // kolejny błąd, zabrakło nawiasu zamykającego
}

Srodowisko::Srodowisko(unsigned int _wiersze,
                       unsigned int _kolumny,
                       unsigned int _glebokosc)
    : wiersze(_wiersze),
      kolumny(_kolumny),
      glebokosc(_glebokosc),
      liczbaNisz(_wiersze * _kolumny * _glebokosc)
{

    nisza = new Nisza **[wiersze];
    for (unsigned int i = 0; i < wiersze; i++)
    {
        nisza[i] = new Nisza *[kolumny];
        for (unsigned int j = 0; j < kolumny; j++)
            nisza[i][j] = new Nisza[glebokosc];
    }
}

char Srodowisko::symbolNiszy(unsigned int wiersz,
                             unsigned int kolumna,
                             unsigned int g) const
{
    if (wiersz >= wiersze || kolumna >= kolumny || g >= glebokosc)
        return UstawieniaSymulacji::pobierzUstawienia().znakNieokreslony;
    return nisza[wiersz][kolumna][g].jakiSymbol();
}

RodzajMieszkanca Srodowisko::rodzajNiszy(unsigned int wiersz,
                                         unsigned int kolumna,
                                         unsigned int g) const
{
    if (wiersz >= wiersze || kolumna >= kolumny || g >= glebokosc)
        return NIEZNANE;
    return nisza[wiersz][kolumna][g].ktoTuMieszka();
}

std::ostream &operator<<(std::ostream &strumien,
                         const Srodowisko &srodowisko)
{
    strumien << srodowisko.doTekstu();
    return strumien;
}
