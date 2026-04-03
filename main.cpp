#include <iostream>
#include <fstream>
using namespace std;
#include "osobniki.h"
#include "sasiedztwo.h"
#include "nisza.h"
#include "srodowisko.h"

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
//                   << stanSymulacji << endl;
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
             << stanSymulacji << endl;
        plikWynikowy << stanSymulacji << endl;
        cin.ignore(1);
        ekoSystem++;
        i++;
    } while (i < 200 && !ekoSystem);

    cout << endl;

    plikWynikowy.close();
    return 0;
}