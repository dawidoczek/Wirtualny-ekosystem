#ifdef _WIN32
#include "PDcurses.h"
#elif defined(__linux__) || defined(__APPLE__)
#include <ncurses.h>
#endif

#include "main_menu.h"

#include <algorithm>
#include <cctype>
#include <limits>
#include <stdexcept>
#include <string>
#include <thread>
#include <chrono>

bool ncursesAktywne = false;
// https://patorjk.com/software/taag/
// const char* tytulASCII[] = {
//     " __        __  ______  _______  ________  __    __  ______   __        __    __  __      __  ",
//     "|  \\  _  |  \\|      \\|        \\|        \\|  \\  |  \\ /      \\ |  \\      |  \\  |  \\|  \\    /  \\ ",
//     "| $$ / \\ | $$ \\$$$$$$| $$$$$$$\\\\$$$$$$$$| $$  | $$|  $$$$$$\\| $$      | $$\\ | $$ \\$$\\  /  $$ ",
//     "| $$/  $\\| $$  | $$  | $$__| $$  | $$   | $$  | $$| $$__| $$| $$      | $$$\\| $$  \\$$\\/  $$  ",
//     "| $$  $$$\\ $$  | $$  | $$    $$  | $$   | $$  | $$| $$    $$| $$      | $$$$\\ $$   \\$$  $$   ",
//     "| $$ $$\\$$\\$$  | $$  | $$$$$$$\\  | $$   | $$  | $$| $$$$$$$$| $$      | $$\\$$ $$    \\$$$$    ",
//     "| $$$$  \\$$$$ _| $$_ | $$  | $$  | $$   | $$__/ $$| $$  | $$| $$_____ | $$ \\$$$$    | $$     ",
//     "| $$$    \\$$$|   $$ \\| $$  | $$  | $$    \\$$    $$| $$  | $$| $$     \\| $$  \\$$$    | $$     ",
//     " \\$$      \\$$ \\$$$$$$ \\$$   \\$$   \\$$     \\$$$$$$  \\$$   \\$$ \\$$$$$$$$ \\$$   \\$$     \\$$     ",
//     "                                                                                             ",
//     "                                                                                             ",
//     "                                                                                             ",
//     " ________  __    __   ______    ______  __      __   ______  ________  ________  __       __ ",
//     "|        \\|  \\  /  \\ /      \\  /      \\|  \\    /  \\ /      \\|        \\|        \\|  \\     /  \\",
//     "| $$$$$$$$| $$ /  $$|  $$$$$$\\|  $$$$$$\\\\$$\\  /  $$|  $$$$$$\\\\$$$$$$$$| $$$$$$$$| $$\\   /  $$",
//     "| $$__    | $$/  $$ | $$  | $$| $$___\\$$ \\$$\\/  $$ | $$___\\$$  | $$   | $$__    | $$$\\ /  $$$",
//     "| $$  \\   | $$  $$  | $$  | $$ \\$$    \\   \\$$  $$   \\$$    \\   | $$   | $$  \\   | $$$$\\  $$$$",
//     "| $$$$$   | $$$$$\\  | $$  | $$ _\\$$$$$$\\   \\$$$$    _\\$$$$$$\\  | $$   | $$$$$   | $$\\$$ $$ $$",
//     "| $$_____ | $$ \\$$\\ | $$__/ $$|  \\__| $$   | $$    |  \\__| $$  | $$   | $$_____ | $$ \\$$$| $$",
//     "| $$     \\| $$  \\$$\\ \\$$    $$ \\$$    $$   | $$     \\$$    $$  | $$   | $$     \\| $$  \\$ | $$",
//     " \\$$$$$$$$ \\$$   \\$$  \\$$$$$$  \\$$$$$$      \\$$      \\$$$$$$    \\$$    \\$$$$$$$$ \\$$      \\$$",
//     "                                                                                             ",
//     "                                                                                             ",
//     "                                                                                             "
// };

// https://patorjk.com/software/taag/#p=display&f=Graffiti&t=Type+Something+&x=none&v=4&h=4&w=80&we=false
const char* tytulASCII[] = {
    "  888       888 8888888 8888888b. 88888888888 888     888       d8888 888      888b    888 Y88b   d88P   ",
    "  888   o   888   888   888   Y88b    888     888     888      d88888 888      8888b   888  Y88b d88P     ",
    "  888  d8b  888   888   888    888    888     888     888     d88P888 888      88888b  888   Y88o88P      ",
    "  888 d888b 888   888   888   d88P    888     888     888    d88P 888 888      888Y88b 888    Y888P       ",
    "  888d88888b888   888   8888888P\"     888     888     888   d88P  888 888      888 Y88b888     888        ",
    "  88888P Y88888   888   888 T88b      888     888     888  d88P   888 888      888  Y88888     888        ",
    "  8888P   Y8888   888   888  T88b     888     Y88b. .d88P d8888888888 888      888   Y8888     888        ",
    "  888P     Y888 8888888 888   T88b    888      \"Y88888P\" d88P     888 88888888 888    Y888     888        ",
    "                                                                                                        ",
    "                                                                                                        ",
    "                                                                                                        ",
    "8888888888 888    d8P   .d88888b.   .d8888b. Y88b   d88P  .d8888b. 88888888888 8888888888 888b     d888 ",
    "888        888   d8P   d88P\" \"Y88b d88P  Y88b Y88b d88P  d88P  Y88b    888     888        8888b   d8888 ",
    "888        888  d8P    888     888 Y88b.       Y88o88P   Y88b.         888     888        88888b.d88888 ",
    "8888888    888d88K     888     888  \"Y888b.     Y888P     \"Y888b.      888     8888888    888Y88888P888 ",
    "888        8888888b    888     888     \"Y88b.    888         \"Y88b.    888     888        888 Y888P 888 ",
    "888        888  Y88b   888     888       \"888    888           \"888    888     888        888  Y8P  888 ",
    "888        888   Y88b  Y88b. .d88P Y88b  d88P    888     Y88b  d88P    888     888        888   \"   888 ",
    "8888888888 888    Y88b  \"Y88888P\"   \"Y8888P\"     888      \"Y8888P\"     888     8888888888 888       888 ",
    "                                                                                                        ",
    "                                                                                                        ",
    "                                                                                                        "
};

const int liczbaLiniiTytulu = sizeof(tytulASCII) / sizeof(tytulASCII[0]);
int liczbakolorow = 6;

std::string obetnij(std::string tekst)
{
    auto poczatek = std::find_if_not(tekst.begin(), tekst.end(), [](unsigned char znak)
                                     { return std::isspace(znak); });
    auto koniec = std::find_if_not(tekst.rbegin(), tekst.rend(), [](unsigned char znak)
                                   { return std::isspace(znak); })
                      .base();

    if (poczatek >= koniec)
        return {};

    return std::string(poczatek, koniec);
}

void pokazKomunikat(const std::string &tekst)
{
    int max_y, max_x;
    getmaxyx(stdscr, max_y, max_x);
    mvprintw(max_y - 2, 2, "%s", tekst.c_str());
    clrtoeol();
    mvprintw(max_y - 1, 2, "Nacisnij dowolny klawisz...");
    clrtoeol();
    refresh();
    getch();
}

bool pobierzTekst(const std::string &prompt, std::string &out)
{
    int max_y, max_x;
    getmaxyx(stdscr, max_y, max_x);

    echo();
    curs_set(1);

    move(max_y - 2, 2);
    clrtoeol();
    mvprintw(max_y - 2, 2, "%s", prompt.c_str());

    move(max_y - 1, 2);
    clrtoeol();

    char buffer[128] = {};
    int wynik = getnstr(buffer, static_cast<int>(sizeof(buffer) - 1));

    noecho();
    curs_set(0);

    if (wynik == ERR)
        return false;

    out = buffer;
    return true;
}

template <typename T>
bool pobierzLiczbe(const std::string &prompt, T &out, bool wymaganaDodatnia)
{
    std::string tekst;
    if (!pobierzTekst(prompt, tekst))
        return false;

    tekst = obetnij(tekst);

    try
    {
        if (tekst.empty())
            throw std::invalid_argument("pusty tekst");

        size_t pozycja = 0;
        unsigned long wartosc = std::stoul(tekst, &pozycja);
        if (pozycja != tekst.size())
            throw std::invalid_argument("smieci po liczbie");
        if (wymaganaDodatnia && wartosc == 0)
            throw std::out_of_range("zero");
        if (wartosc > static_cast<unsigned long>(std::numeric_limits<T>::max()))
            throw std::out_of_range("za duza liczba");

        out = static_cast<T>(wartosc);
        return true;
    }
    catch (...)
    {
        pokazKomunikat("Nieprawidlowa liczba.");
        return false;
    }
}

bool pobierzZnak(const std::string &prompt, char &out)
{
    std::string tekst;
    if (!pobierzTekst(prompt, tekst))
        return false;

    if (tekst.empty())
    {
        pokazKomunikat("Wpisz jeden znak.");
        return false;
    }

    out = tekst[0];
    return true;
}

void rysujMenuGlowne(int zaznaczony)
{
    clear();

    int max_x = getmaxx(stdscr);

    // const char *tytul = "WIRTUALNY EKOSYSTEM";
    const char *opis = "Strzalki: wybor | Enter: zatwierdz | q: wyjscie";
    int dlugoscLinii = std::strlen(tytulASCII[0]);
    int start_x = std::max(2, (max_x - dlugoscLinii) / 2);
    for (int i = 0; i < liczbaLiniiTytulu; ++i)
    {
        // Obliczamy rzeczywistą długość aktualnej linii
        int dlugoscObecnejLinii = std::strlen(tytulASCII[i]);
        
        for (int j = 0; j < dlugoscObecnejLinii; ++j)
        {
            // Magia gradientu: (wiersz + kolumna) modulo ilość kolorów
            // Dzięki temu kolory przesuwają się ukośnie!
            int indeksKoloru = (((i * 2) + j) % liczbakolorow) + 1;

            // Włączamy odpowiedni kolor z palety
            attron(COLOR_PAIR(indeksKoloru));
            
            // Wypisujemy pojedynczy znak w wyliczonym miejscu (X i Y rosną)
            mvaddch(2 + i, start_x + j, tytulASCII[i][j]);
            
            // Wyłączamy kolor (dobra praktyka)
            attroff(COLOR_PAIR(indeksKoloru));
        }
    }
    mvprintw(liczbaLiniiTytulu+2, std::max(2, (max_x - static_cast<int>(std::string(opis).size())) / 2), "%s", opis);

    const char *opcje[] = {"Start symulacji", "Ustawienia", "Wyjscie"};
    for (int i = 0; i < 3; ++i)
    {
        int y = liczbaLiniiTytulu+5 + i * 2;
        int x = std::max(4, (max_x - static_cast<int>(std::string(opcje[i]).size())) / 2);
        if (i == zaznaczony)
            attron(A_REVERSE);
        mvprintw(y, x, "%s", opcje[i]);
        if (i == zaznaczony)
            attroff(A_REVERSE);
    }

    refresh();
}

void rysujMenuUstawien(const KonfiguracjaStartowa &konfiguracja,
                       const UstawieniaSymulacji &ustawienia,
                       int zaznaczony)
{
    clear();

    int max_y, max_x;
    getmaxyx(stdscr, max_y, max_x);

    mvprintw(1, 2, "USTAWIENIA");

    const int startY = 3;
    int indeks = 0;
    auto wiersz = [&](const std::string &etykieta, const std::string &wartosc)
    {
        if (indeks == zaznaczony)
            attron(A_REVERSE);
        mvprintw(startY + indeks, 2, "%-28s %s", etykieta.c_str(), wartosc.c_str());
        if (indeks == zaznaczony)
            attroff(A_REVERSE);
        ++indeks;
    };

    wiersz("Rozmiar: wiersze", std::to_string(konfiguracja.wiersze));
    wiersz("Rozmiar: kolumny", std::to_string(konfiguracja.kolumny));
    wiersz("Rozmiar: glebokosc", std::to_string(konfiguracja.glebokosc));
    wiersz("Start: glony", std::to_string(konfiguracja.glony));
    wiersz("Start: grzyby", std::to_string(konfiguracja.grzyby));
    wiersz("Start: bakterie", std::to_string(konfiguracja.bakterie));
    wiersz("znakGlon", std::string(1, ustawienia.znakGlon));
    wiersz("znakGrzyb", std::string(1, ustawienia.znakGrzyb));
    wiersz("znakBakteria", std::string(1, ustawienia.znakBakteria));
    wiersz("znakTrup", std::string(1, ustawienia.znakTrup));
    wiersz("znakNieokreslony", std::string(1, ustawienia.znakNieokreslony));
    wiersz("glonZycieMin", std::to_string(ustawienia.glonZycieMin));
    wiersz("glonZycieMax", std::to_string(ustawienia.glonZycieMax));
    wiersz("glonKosztPotomka", std::to_string(ustawienia.glonKosztPotomka));
    wiersz("glonLimitPosilkow", std::to_string(ustawienia.glonLimitPosilkow));
    wiersz("grzybZycieMin", std::to_string(ustawienia.grzybZycieMin));
    wiersz("grzybZycieMax", std::to_string(ustawienia.grzybZycieMax));
    wiersz("grzybKosztPotomka", std::to_string(ustawienia.grzybKosztPotomka));
    wiersz("grzybLimitPosilkow", std::to_string(ustawienia.grzybLimitPosilkow));
    wiersz("bakteriaZycieMin", std::to_string(ustawienia.bakteriaZycieMin));
    wiersz("bakteriaZycieMax", std::to_string(ustawienia.bakteriaZycieMax));
    wiersz("bakteriaKosztPotomka", std::to_string(ustawienia.bakteriaKosztPotomka));
    wiersz("bakteriaLimitPosilkow", std::to_string(ustawienia.bakteriaLimitPosilkow));
    wiersz("ZAPISZ USTAWIENIA", "[Enter]");

    mvprintw(max_y - 2, 2, "Zmiany zaczna dzialac przy nastepnym starcie symulacji.");
    refresh();
}

void edytujWybranyWiersz(int wybor,
                         KonfiguracjaStartowa &konfiguracja,
                         UstawieniaSymulacji &ustawienia)
{
    switch (wybor)
    {
    case 0:
        pobierzLiczbe("Nowa liczba wierszy (> 0): ", konfiguracja.wiersze, true);
        break;
    case 1:
        pobierzLiczbe("Nowa liczba kolumn (> 0): ", konfiguracja.kolumny, true);
        break;
    case 2:
        pobierzLiczbe("Nowa glebokosc (> 0): ", konfiguracja.glebokosc, true);
        break;
    case 3:
        pobierzLiczbe("Nowa liczba glonow (>= 0): ", konfiguracja.glony, false);
        break;
    case 4:
        pobierzLiczbe("Nowa liczba grzybow (>= 0): ", konfiguracja.grzyby, false);
        break;
    case 5:
        pobierzLiczbe("Nowa liczba bakterii (>= 0): ", konfiguracja.bakterie, false);
        break;
    case 6:
        pobierzZnak("Nowy znak glonu: ", ustawienia.znakGlon);
        break;
    case 7:
        pobierzZnak("Nowy znak grzyba: ", ustawienia.znakGrzyb);
        break;
    case 8:
        pobierzZnak("Nowy znak bakterii: ", ustawienia.znakBakteria);
        break;
    case 9:
        pobierzZnak("Nowy znak trupa: ", ustawienia.znakTrup);
        break;
    case 10:
        pobierzZnak("Nowy znak nieokreslony: ", ustawienia.znakNieokreslony);
        break;
    case 11:
        pobierzLiczbe("Nowe glonZycieMin: ", ustawienia.glonZycieMin, false);
        break;
    case 12:
        pobierzLiczbe("Nowe glonZycieMax: ", ustawienia.glonZycieMax, false);
        break;
    case 13:
        pobierzLiczbe("Nowe glonKosztPotomka: ", ustawienia.glonKosztPotomka, false);
        break;
    case 14:
        pobierzLiczbe("Nowe glonLimitPosilkow: ", ustawienia.glonLimitPosilkow, false);
        break;
    case 15:
        pobierzLiczbe("Nowe grzybZycieMin: ", ustawienia.grzybZycieMin, false);
        break;
    case 16:
        pobierzLiczbe("Nowe grzybZycieMax: ", ustawienia.grzybZycieMax, false);
        break;
    case 17:
        pobierzLiczbe("Nowe grzybKosztPotomka: ", ustawienia.grzybKosztPotomka, false);
        break;
    case 18:
        pobierzLiczbe("Nowe grzybLimitPosilkow: ", ustawienia.grzybLimitPosilkow, false);
        break;
    case 19:
        pobierzLiczbe("Nowe bakteriaZycieMin: ", ustawienia.bakteriaZycieMin, false);
        break;
    case 20:
        pobierzLiczbe("Nowe bakteriaZycieMax: ", ustawienia.bakteriaZycieMax, false);
        break;
    case 21:
        pobierzLiczbe("Nowe bakteriaKosztPotomka: ", ustawienia.bakteriaKosztPotomka, false);
        break;
    case 22:
        pobierzLiczbe("Nowe bakteriaLimitPosilkow: ", ustawienia.bakteriaLimitPosilkow, false);
        break;
    default:
        break;
    }
}

void uruchomInterfejsNcurses()
{
    if (ncursesAktywne)
        return;

    initscr();
    noecho();
    cbreak();
    keypad(stdscr, TRUE);
    curs_set(0);
    start_color();
    use_default_colors();
    
   if (COLORS >= 256) {
        int tecza256[] = {
            196, 202, 208, 214, 220, 226, // Czerwony -> Żółty
            190, 154, 118, 82,  46,       // Żółty -> Zielony
            47,  48,  49,  50,  51,       // Zielony -> Cyjan
            45,  39,  33,  27,  21,       // Cyjan -> Niebieski
            57,  93,  129, 165, 201,      // Niebieski -> Magenta
            199, 198, 197                 // Magenta -> Czerwony
        };
        
        liczbakolorow = sizeof(tecza256) / sizeof(tecza256[0]);

        // Rejestrujemy naszą nową, szeroką paletę w ncurses
        for (int i = 0; i < liczbakolorow; ++i) {
            init_pair(i + 1, tecza256[i], -1);
        }
    } 
    else {
        // Fallback: jeśli ktoś używa starego terminala, zostajemy przy 6 kolorach
        init_pair(1, COLOR_RED, -1);
        init_pair(2, COLOR_YELLOW, -1);
        init_pair(3, COLOR_GREEN, -1);
        init_pair(4, COLOR_CYAN, -1);
        init_pair(5, COLOR_BLUE, -1);
        init_pair(6, COLOR_MAGENTA, -1);
        liczbakolorow = 6;
    }
    ncursesAktywne = true;
}

void zamknijInterfejsNcurses()
{
    if (!ncursesAktywne)
        return;

    endwin();
    ncursesAktywne = false;
}

WynikMenuGlownego pokazMenuGlowne()
{
    int zaznaczony = 0;
    while (true)
    {
        rysujMenuGlowne(zaznaczony);

        int klawisz = getch();
        if (klawisz == KEY_UP)
            zaznaczony = (zaznaczony + 2) % 3;
        else if (klawisz == KEY_DOWN)
            zaznaczony = (zaznaczony + 1) % 3;
        else if (klawisz == 'q' || klawisz == 'Q' || klawisz == 27)
            return WynikMenuGlownego::Wyjscie;
        else if (klawisz == '\n' || klawisz == '\r' || klawisz == KEY_ENTER)
        {
            if (zaznaczony == 0)
                return WynikMenuGlownego::StartSymulacji;
            if (zaznaczony == 1)
                return WynikMenuGlownego::Ustawienia;
            return WynikMenuGlownego::Wyjscie;
        }
    }
}

void pokazMenuUstawien(KonfiguracjaStartowa &konfiguracja)
{
    UstawieniaSymulacji &ustawienia = UstawieniaSymulacji::pobierzUstawienia();

    const int liczbaPolozen = 24;
    int zaznaczony = 0;


    while (true)
    {
        rysujMenuUstawien(konfiguracja, ustawienia, zaznaczony);
        int klawisz = getch();
        if (klawisz == KEY_UP)
            zaznaczony = (zaznaczony + liczbaPolozen - 1) % liczbaPolozen;
        else if (klawisz == KEY_DOWN)
            zaznaczony = (zaznaczony + 1) % liczbaPolozen;
        else if (klawisz == 'q' || klawisz == 'Q' || klawisz == 27)
            return;
        else if (klawisz == '\n' || klawisz == '\r' || klawisz == KEY_ENTER || klawisz == ' ')
        {
            if (zaznaczony == 23)
            {
                if (ustawienia.zapiszDoPliku("ustawienia.txt"))
                    pokazKomunikat("Ustawienia zapisane do pliku!");
                else
                    pokazKomunikat("Blad: nie mozna zapisac pliku!");
                return;
            }
            else
                edytujWybranyWiersz(zaznaczony, konfiguracja, ustawienia);
        }
    }
}