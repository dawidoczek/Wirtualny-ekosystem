#include <iostream>
#include <fstream>
using namespace std;
#include "osobniki.h"
#include "sasiedztwo.h"
#include "nisza.h"
#include "srodowisko.h"
string kolory[] = {"\e[0;32m", "\e[0;35m", "\e[0;36m", "\e[0;37m", "\e[0;33m", "\e[0;31m", "\e[0;37m"};

string koloruj(string tekst){

    for(unsigned long i=0; i<tekst.length(); i++){
        if(tekst[i] == UST.znakGlon)
            tekst.insert(i, kolory[0]);
        else if(tekst[i] == UST.znakGrzyb)
            tekst.insert(i, kolory[1]);
        else if(tekst[i] == UST.znakBakteria)
            tekst.insert(i, kolory[2]);
        else if(tekst[i] == UST.znakTrup)
            tekst.insert(i, kolory[5]);
        else if(tekst[i] == UST.znakPustaNisza)
            tekst.insert(i, kolory[3]);
        else if(tekst[i] == UST.znakNieokreslony)
            tekst.insert(i, kolory[6]);
        else
            tekst.insert(i, kolory[6]);

        i+=kolory[0].length();
    }
    return tekst;
            

}

string nazwaRodzaju(RodzajMieszkanca rodzaj)
{
    switch (rodzaj)
    {
    case GLON:
        return "GLON";
    case GRZYB:
        return "GRZYB";
    case BAKTERIA:
        return "BAKTERIA";
    case PUSTKA:
        return "PUSTKA";
    case SCIANA:
        return "ŚCIANA";
    case TRUP:
        return "TRUP";
    case NIEZNANE:
        return "NIEZNANE";
    }
}
void wyswietl(UstawieniaSymulacji &UST)
{
    cout << "Znak glon:" << UST.znakGlon
         << " zycieMin=" << UST.glonZycieMin
         << " zycieMax=" << UST.glonZycieMax << endl;
}
// int main()
// {
//     // Srodowisko ekoSystem=Srodowisko::czytajZPliku("start.txt");
//     Srodowisko ekoSystem(8, 12);

//     ekoSystem.lokuj(new Glon(), 0, 10);
//     ekoSystem.lokuj(new Glon(), 0, 8);
//     ekoSystem.lokuj(new Glon(), 1, 10);
//     ekoSystem.lokuj(new Glon(), 1, 13);
//     ekoSystem.lokuj(new Glon(), 3, 10);
//     ekoSystem.lokuj(new Grzyb(), 1, 11);
//     ekoSystem.lokuj(new Grzyb(), 0, 0);
//     ekoSystem.lokuj(new Bakteria(), 3, 3);
//     ekoSystem.lokuj(new Bakteria(), 2, 6);
//     ofstream plikWynikowy("symulacja.txt");
//     if (!plikWynikowy.is_open())
//         return 1;
//     string stanSymulacji;

//     unsigned long i = 0;

//     do
//     {
//         system("clear");
//         std::cout << "Krok symulacji: " << i << endl;
//         plikWynikowy << "Krok symulacji: " << i << endl;
//         stanSymulacji = ekoSystem.doTekstu();

//         std::cout << endl
//                   << koloruj(stanSymulacji) << endl;
//         plikWynikowy << stanSymulacji << endl;
//         std::cin.ignore(1);
//         // dodałej kolejnego cleara zeby nie bylo poprzedniego
//         system("clear");
//         ekoSystem++;
//         i++;
//     } while (i < 200 && !ekoSystem);

//     std::cout << endl;

//     plikWynikowy.close();
//     return 0;
// }

int main()
{
    UstawieniaSymulacji::pobierzUstawienia().wczytajZPliku("ustawienia.txt");

    Srodowisko ekoSystem = Srodowisko::czytajZPliku("start.txt");

    ofstream plikWynikowy("symulacja.txt");
    if (!plikWynikowy.is_open())
        return 1;
    string stanSymulacji;

    unsigned long i = 0;

    do
    {
        system("clear");
        cout << "Krok symulacji: " << i << endl;
        plikWynikowy << "Krok symulacji: " << i << endl;
        stanSymulacji = ekoSystem.doTekstu();

        cout << endl
             << koloruj(stanSymulacji) << endl;
        plikWynikowy << stanSymulacji << endl;
        cin.ignore(1);
        ekoSystem++;
        i++;
    } while (i < 200 && !ekoSystem);

    cout << endl;

    plikWynikowy.close();
    return 0;
}