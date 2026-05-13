#pragma once

#include "srodowisko.h"
#include "ustawienia.h"

enum class WynikMenuGlownego
{
    StartSymulacji,
    Ustawienia,
    Wyjscie
};

WynikMenuGlownego pokazMenuGlowne();
void pokazMenuUstawien(KonfiguracjaStartowa &konfiguracja);
void uruchomInterfejsNcurses();
void zamknijInterfejsNcurses();