#include "main_menu.h"
#include <cstdlib>
int main()
{
    UstawieniaSymulacji::pobierzUstawienia().wczytajZPliku("ustawienia.txt");
    KonfiguracjaStartowa konfiguracja;
    // kolorki
    setenv("TERM", "xterm-256color", 1);
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