#include "main_menu.h"
#ifdef _WIN32
    #include <stdlib.h>
#else
    #include <cstdlib>
#endif // _WIN32

int main()
{
    UstawieniaSymulacji::pobierzUstawienia().wczytajZPliku("ustawienia.txt");
    KonfiguracjaStartowa konfiguracja;
    // kolorki
    #ifdef _WIN32
        _putenv("TERM=xterm-256color");
    #else
        setenv("TERM", "xterm-256color", 1);
    #endif // _WIN32
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
