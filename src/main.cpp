#include "main_menu.h"

int main()
{
    UstawieniaSymulacji::pobierzUstawienia().wczytajZPliku("ustawienia.txt");
    KonfiguracjaStartowa konfiguracja;

    uruchomInterfejsNcurses();

    while (true)
    {
        WynikMenuGlownego wybor = pokazMenuGlowne();

        if (wybor == WynikMenuGlownego::StartSymulacji)
            uruchomSymulacje(konfiguracja);
        else if (wybor == WynikMenuGlownego::Ustawienia)
            pokazMenuUstawien(konfiguracja);
        else
            break;
    }

    zamknijInterfejsNcurses();

    return 0;
}