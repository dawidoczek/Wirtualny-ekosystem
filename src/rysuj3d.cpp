#ifdef _WIN32
    // jak się okazuje, bibloteka na której bazuje cały projekt,
    // jest wbudowana tylko w Maca...
    // na linuxie mozna latwo odpalic pobrać
    // ale na windowsie to katorga.
    // więc załączam biblioteke PDCurses.a, wraz z plikiem nagłówkowym
    // prosze sobie poradzić.
    #include "PDcurses.h"
#elif defined(__linux__) || defined(__APPLE__)
    #include <ncurses.h>
#endif

#include <cmath>
#include <vector>
#include <algorithm>
#include <thread>
#include <chrono>
#include <string>
#include "srodowisko.h"
// inspiracje
// https://www.a1k0n.net/2011/07/20/donut-math.html
// https://github.com/alt44s/cube-spin

struct Point3D
{
    double x, y, z;
    int color;
    char symbol;
};

struct ViewState
{
    bool obrotAutomatyczny = true;
    bool obwodka = false;
    double angle_x = 0.0;
    double angle_y = 0.0;
    double angle_z = 0.0;
    double offset_x = 0.0;
    double offset_y = 0.0;
    double offset_z = 0.0;
};

// source https://github.com/alt44s/cube-spin/blob/main/cube.py
Point3D rotate_x(double x, double y, double z, double angle, int color, char symbol)
{
    double cos_a = cos(angle);
    double sin_a = sin(angle);
    // tak jak https://www.a1k0n.net/2011/07/20/donut-math.html
    // x' = x
    // y' = y *cos(alpha) - z * sin(alpha)
    // z' = y * sin(alpha) + z * cos(alpha)
    return {x, y * cos_a - z * sin_a, y * sin_a + z * cos_a, color, symbol};
}

Point3D rotate_y(double x, double y, double z, double angle, int color, char symbol)
{
    double cos_a = cos(angle);
    double sin_a = sin(angle);
    // x' = x *cos(alpha) + z * sin(alpha)
    // y' = y
    // z' = -x * sin(alpha) + z * cos(alpha)
    return {x * cos_a + z * sin_a, y, -x * sin_a + z * cos_a, color, symbol};
}

Point3D rotate_z(double x, double y, double z, double angle, int color, char symbol)
{
    double cos_a = cos(angle);
    double sin_a = sin(angle);
    // x' = x *cos(alpha) - y * sin(alpha)
    // y' = x * sin(alpha) + y * cos(alpha)
    // z' = z
    return {x * cos_a - y * sin_a, x * sin_a + y * cos_a, z, color, symbol};
}

// Funkcja rzutowania na ekran 2D
// blizsze elementy kamery sa wieksze, dalsze mniejsze
// xyz, rozmiar wyswietlania, srodek ekranu,
// maksymalna skala, wyjscie x i y
void project(double x, double y, double z, double display_size,
             int center_x, int center_y, double max_scale,
             int &out_x, int &out_y)
{
    double camera_distance = max_scale + 4.5; // offset 4.5 na sztywno
    double factor = display_size / (z + camera_distance);

    out_x = static_cast<int>(x * factor * 2 + center_x);
    out_y = static_cast<int>(-y * factor + center_y);
}

void drawStatsPanel(const Srodowisko &ekoSystem, int max_y)
{
    if (max_y <= 0)
        return;

    int y = 1;
    // int mvprintw(int, int, const char *, ...)
    //              y    x    format        args
    auto printLine = [&](const char *format, auto... args)
    {
        if (y < max_y)
            mvprintw(y++, 2, format, args...);
    };

    // attr on, ustawia kolor i pogrubienie, a attroff wyłącza
    attron(A_BOLD | COLOR_PAIR(7));
    printLine("STATYSTYKI");
    printLine("TURA: %lu", ekoSystem.numerTury);
    printLine("GLON  : %lu", ekoSystem.liczba(GLON));
    printLine("GRZYB : %lu", ekoSystem.liczba(GRZYB));
    printLine("BAKT  : %lu", ekoSystem.liczba(BAKTERIA));
    printLine("TRUP  : %lu", ekoSystem.liczba(TRUP));
    attroff(A_BOLD | COLOR_PAIR(7));
    y++;
    printLine("SPACJA - inspekcja");
    printLine("ENTER - krok symulacji");
    printLine("strzalki - przesun");
    printLine("shift+strzalki - obrot");
    printLine("n - obwodka ekosystemu");
    printLine("q - wyjscie");
}
// przeksztalc punkt
Point3D transformPoint(double x, double y, double z, const ViewState &widok)
{
    x += widok.offset_x;
    y += widok.offset_y;
    z += widok.offset_z;

    Point3D r = rotate_x(x, y, z, widok.angle_x, 7, '#');
    r = rotate_y(r.x, r.y, r.z, widok.angle_y, r.color, r.symbol);
    r = rotate_z(r.x, r.y, r.z, widok.angle_z, r.color, r.symbol);
    return r;
}
// funkcja rysująca linię według algorytmu Bresenhama
// https://pl.wikipedia.org/wiki/Algorytm_Bresenhama
// https://github.com/tashi-2004/2D-Line-Drawing-Algorithms/blob/main/Header.h
void drawLine2D(int x0, int y0, int x1, int y1, int max_y, int max_x, int statsWidth)
{
    // czary
    int dx = std::abs(x1 - x0);
    int dy = std::abs(y1 - y0);
    int xi = (x0 < x1) ? 1 : -1;
    int yi = (y0 < y1) ? 1 : -1;
    int err = dx - dy;

    while (true)
    {
        if (x0 < max_x && y0 >= 0)
            mvaddch(y0, x0, '#');

        if (x0 == x1 && y0 == y1)
            break;

        int ai = 2 * err;
        if (ai > -dy)
        {
            err -= dy;
            x0 += xi;
        }
        if (ai < dx)
        {
            err += dx;
            y0 += yi;
        }
    }
}

void drawEnvironmentBorder(const ViewState &widok,
                           double scale_x, double scale_y, double scale_z,
                           double display_size, int center_x, int center_y,
                           double max_scale, int max_y, int max_x, int statsWidth)
{
    const double xi = scale_x;
    const double yi = scale_y;
    const double sz = scale_z;
    // 2^3 = 8 wierzchołków sześcianu
    // kazdy ma 3 koordynaty, x, y, z
    const double corners[8][3] = {
        {-xi, -yi, -sz},
        {xi, -yi, -sz},
        {xi, yi, -sz},
        {-xi, yi, -sz},
        {-xi, -yi, sz},
        {xi, -yi, sz},
        {xi, yi, sz},
        {-xi, yi, sz}};

    const int edges[12][2] = {
        // tylna sciana
        {0, 1},
        {1, 2},
        {2, 3},
        {3, 0},
        // przednia sciana
        {4, 5},
        {5, 6},
        {6, 7},
        {7, 4},
        // łączące
        {0, 4},
        {1, 5},
        {2, 6},
        {3, 7}};

    int projected[8][2];
    for (int i = 0; i < 8; ++i)
    {
        // zrob na 3d
        Point3D r = transformPoint(corners[i][0], corners[i][1], corners[i][2], widok);
        // wyswietl na 2d
        project(r.x, r.y, r.z, display_size, center_x, center_y, max_scale, projected[i][0], projected[i][1]);
    }
    // kolorki znowu
    attron(COLOR_PAIR(7) | A_BOLD);
    for (int e = 0; e < 12; ++e)
    {
        int a = edges[e][0];
        int b = edges[e][1];
        // rysuj od wierzchloka do wierzchloka
        drawLine2D(projected[a][0], projected[a][1], projected[b][0], projected[b][1], max_y, max_x, statsWidth);
    }
    attroff(COLOR_PAIR(7) | A_BOLD);
}

int rysuj3d(Srodowisko &ekoSystem)
{
    nodelay(stdscr, TRUE);
    keypad(stdscr, TRUE);

    start_color();
    use_default_colors();
    init_pair(1, COLOR_GREEN, -1);   // Glon
    init_pair(2, COLOR_MAGENTA, -1); // Grzyb
    init_pair(3, COLOR_CYAN, -1);    // Bakteria
    init_pair(6, COLOR_RED, -1);     // Trup
    init_pair(7, COLOR_WHITE, -1);   // Interfejs i granice

    int grid_x = static_cast<int>(ekoSystem.wiersze);
    int grid_y = static_cast<int>(ekoSystem.kolumny);
    int grid_z = static_cast<int>(ekoSystem.glebokosc);
    // skale i display_size będą liczone dynamicznie po poznaniu rozmiaru terminala
    double scale_x = 1.0;
    double scale_y = 1.0;
    double scale_z = 1.0;
    double display_size = 50.0;
    double max_scale = 1.0;

    ViewState widok;

    while (!ekoSystem && ekoSystem.aktualnaTura < 400)
    {
        clear();

        int ch = getch();
        if (ch == 'q' || ch == 'Q')
            break;
        bool wykonajKrok = (ch == '\n' || ch == '\r' || ch == KEY_ENTER);

        if (ch == ' ')
            widok.obrotAutomatyczny = !widok.obrotAutomatyczny;

        if (ch == 'n' || ch == 'N')
            widok.obwodka = !widok.obwodka;

        if (!widok.obrotAutomatyczny)
        {
            if (ch == KEY_LEFT)
                widok.offset_x -= 0.08;
            else if (ch == KEY_RIGHT)
                widok.offset_x += 0.08;
            else if (ch == KEY_UP)
                widok.offset_y += 0.08;
            else if (ch == KEY_DOWN)
                widok.offset_y -= 0.08;
            else if (ch == KEY_SLEFT)
                widok.angle_y -= 0.05;
            else if (ch == KEY_SRIGHT)
                widok.angle_y += 0.05;
            else if (ch == KEY_SR)
                widok.angle_x -= 0.05;
            else if (ch == KEY_SF)
                widok.angle_x += 0.05;
        }

        int max_y, max_x;
        getmaxyx(stdscr, max_y, max_x);
        // wolna liczba kolumny dla statystyk
        int statsWidth = 42;
        int center_x = (statsWidth + max_x) / 2;
        int center_y = max_y / 2;

        drawStatsPanel(ekoSystem, max_y);

        // Oblicz dynamiczne skale na podstawie proporcji siatki
        int max_grid = std::max({grid_x, grid_y, grid_z});
        if (max_grid > 0)
        {
            scale_x = static_cast<double>(grid_x) / static_cast<double>(max_grid);
            scale_y = static_cast<double>(grid_y) / static_cast<double>(max_grid);
            scale_z = static_cast<double>(grid_z) / static_cast<double>(max_grid);
        }
        else
        {
            scale_x = scale_y = scale_z = 1.0;
        }

        max_scale = std::max({scale_x, scale_y, scale_z});

        // Obszar rysowania po odjęciu panelu statystyk i marginesu
        int avail_w = std::max(10, max_x - statsWidth - 4);
        int avail_h = std::max(6, max_y - 4);

        // Conservatywne wyliczenie display_size bazujące na najgorszym przypadku (z = -1)
        double camera_distance = max_scale + 3.5; // zgodnie z funkcją project
        double denom = camera_distance - 1.0;     // z = -1 -> z + camera_distance = camera_distance - 1
        double ds_x = (denom > 0.0) ? (static_cast<double>(avail_w) * denom / 4.0) : 10.0;
        double ds_y = (denom > 0.0) ? (static_cast<double>(avail_h) * denom / 2.0) : 10.0;

        // wybierz mniejszy z dopuszczalnych rozmiarów i dodaj mały margines (0.9)
        display_size = std::max(8.0, std::min(ds_x, ds_y));

        std::vector<Point3D> rotated_points;
        for (int i = 0; i < grid_x; ++i)
        {
            for (int j = 0; j < grid_y; ++j)
            {
                for (int k = 0; k < grid_z; ++k)
                {
                    RodzajMieszkanca rodzaj = ekoSystem.rodzajNiszy(i, j, k);
                    if (rodzaj == PUSTKA)
                        continue;
                    int color = 7;

                    switch (rodzaj)
                    {
                    case GLON:
                        color = 1;
                        break;
                    case GRZYB:
                        color = 2;
                        break;
                    case BAKTERIA:
                        color = 3;
                        break;
                    case TRUP:
                        color = 6;
                        break;
                    default:
                        break;
                    }

                    double x = (((i + 0.5) / grid_x) * 2.0 - 1.0) * scale_x;
                    double y = (((j + 0.5) / grid_y) * 2.0 - 1.0) * scale_y;
                    double z = (((k + 0.5) / grid_z) * 2.0 - 1.0) * scale_z;
                    char symbol = ekoSystem.symbolNiszy(i, j, k);

                    x += widok.offset_x;
                    y += widok.offset_y;
                    z += widok.offset_z;

                    Point3D r = rotate_x(x, y, z, widok.angle_x, color, symbol);
                    r = rotate_y(r.x, r.y, r.z, widok.angle_y, r.color, r.symbol);
                    r = rotate_z(r.x, r.y, r.z, widok.angle_z, r.color, r.symbol);
                    rotated_points.push_back(r);
                }
            }
        }

        // Algorytm malarza (sortowanie malejąco po osi Z)
        // udawana głębia
        // https://pl.wikipedia.org/wiki/Algorytm_malarza
        std::sort(rotated_points.begin(), rotated_points.end(),
                  [](const Point3D &a, const Point3D &b)
                  {
                      return a.z > b.z;
                  });

        // Rysowanie
        for (const auto &p : rotated_points)
        {
            int px, py;
            project(p.x, p.y, p.z, display_size, center_x, center_y, max_scale, px, py);

            if (px >= 0 && px < max_x && py >= 0 && py < max_y)
            {
                mvaddch(py, px, p.symbol | COLOR_PAIR(p.color));
            }
        }

        if (widok.obwodka)
            drawEnvironmentBorder(widok,
                                  scale_x, scale_y, scale_z,
                                  display_size, center_x, center_y,
                                  max_scale, max_y, max_x, statsWidth);

        if (widok.obrotAutomatyczny)
        {
            // na oko wartosci
            widok.angle_x += 0.04;
            widok.angle_y += 0.03;
            widok.angle_z += 0.02;
        }

        if (wykonajKrok)
            ekoSystem++;

        refresh();
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }

    nodelay(stdscr, FALSE);
    return 0;
}
