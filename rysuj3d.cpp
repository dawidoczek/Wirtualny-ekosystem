#include <ncurses.h>
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

struct Point3D {
    double x, y, z;
    int color;
    char symbol;
};

struct ViewState {
    bool obrotAutomatyczny = true;
    bool obwodka = false;
    double angle_x = 0.0;
    double angle_y = 0.0;
    double angle_z = 0.0;
    double offset_x = 0.0;
    double offset_y = 0.0;
    double offset_z = 0.0;
};

Point3D rotate_x(double x, double y, double z, double angle, int color, char symbol) {
    double cos_a = cos(angle);
    double sin_a = sin(angle);
    return {x, y * cos_a - z * sin_a, y * sin_a + z * cos_a, color, symbol};
}

Point3D rotate_y(double x, double y, double z, double angle, int color, char symbol) {
    double cos_a = cos(angle);
    double sin_a = sin(angle);
    return {x * cos_a + z * sin_a, y, -x * sin_a + z * cos_a, color, symbol};
}

Point3D rotate_z(double x, double y, double z, double angle, int color, char symbol) {
    double cos_a = cos(angle);
    double sin_a = sin(angle);
    return {x * cos_a - y * sin_a, x * sin_a + y * cos_a, z, color, symbol};
}

// Funkcja rzutowania na ekran 2D
void project(double x, double y, double z, double display_size, int center_x, int center_y, double max_scale, int &out_x, int &out_y) {
    double camera_distance = max_scale + 3.5;
    double factor = display_size / (z + camera_distance);
    
    out_x = static_cast<int>(x * factor * 2 + center_x);
    out_y = static_cast<int>(-y * factor + center_y);
}

void drawStatsPanel(const Srodowisko &ekoSystem, int max_y)
{
    int y = 1;
    attron(A_BOLD | COLOR_PAIR(7));
    mvprintw(y++, 2, "STATY STYKI");
    mvprintw(y++, 2, "GLON  : %lu", ekoSystem.liczba(GLON));
    mvprintw(y++, 2, "GRZYB : %lu", ekoSystem.liczba(GRZYB));
    mvprintw(y++, 2, "BAKT  : %lu", ekoSystem.liczba(BAKTERIA));
    mvprintw(y++, 2, "TRUP  : %lu", ekoSystem.liczba(TRUP));
    attroff(A_BOLD | COLOR_PAIR(7));

    if (y + 2 < max_y)
    {
        mvprintw(y + 1, 2, "SPACJA - inspekcja");
        mvprintw(y + 2, 2, "ENTER - krok symulacji");
        mvprintw(y + 3, 2, "strzalki - przesun");
        mvprintw(y + 4, 2, "shift+strzalki - obrot");
        mvprintw(y + 5, 2, "n - obwodka ekosystemu");
        mvprintw(y + 6, 2, "q - wyjscie");
    }
}

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

void drawLine2D(int x0, int y0, int x1, int y1, int max_y, int max_x, int statsWidth)
{
    int dx = std::abs(x1 - x0);
    int dy = std::abs(y1 - y0);
    int sx = (x0 < x1) ? 1 : -1;
    int sy = (y0 < y1) ? 1 : -1;
    int err = dx - dy;

    while (true)
    {
        if (x0 > statsWidth && x0 < max_x && y0 >= 0 && y0 < max_y)
            mvaddch(y0, x0, '#');

        if (x0 == x1 && y0 == y1)
            break;

        int e2 = 2 * err;
        if (e2 > -dy)
        {
            err -= dy;
            x0 += sx;
        }
        if (e2 < dx)
        {
            err += dx;
            y0 += sy;
        }
    }
}

void drawEnvironmentBorder(const ViewState &widok,
                           double scale_x, double scale_y, double scale_z,
                           double display_size, int center_x, int center_y,
                           double max_scale, int max_y, int max_x, int statsWidth)
{
    const double sx = scale_x;
    const double sy = scale_y;
    const double sz = scale_z;

    const double corners[8][3] = {
        {-sx, -sy, -sz},
        { sx, -sy, -sz},
        { sx,  sy, -sz},
        {-sx,  sy, -sz},
        {-sx, -sy,  sz},
        { sx, -sy,  sz},
        { sx,  sy,  sz},
        {-sx,  sy,  sz}
    };

    const int edges[12][2] = {
        {0, 1}, {1, 2}, {2, 3}, {3, 0},
        {4, 5}, {5, 6}, {6, 7}, {7, 4},
        {0, 4}, {1, 5}, {2, 6}, {3, 7}
    };

    int projected[8][2];
    for (int i = 0; i < 8; ++i)
    {
        Point3D r = transformPoint(corners[i][0], corners[i][1], corners[i][2], widok);
        project(r.x, r.y, r.z, display_size, center_x, center_y, max_scale, projected[i][0], projected[i][1]);
    }

    attron(COLOR_PAIR(7) | A_BOLD);
    for (int e = 0; e < 12; ++e)
    {
        int a = edges[e][0];
        int b = edges[e][1];
        drawLine2D(projected[a][0], projected[a][1], projected[b][0], projected[b][1], max_y, max_x, statsWidth);
    }
    attroff(COLOR_PAIR(7) | A_BOLD);
}

int rysuj3d(Srodowisko ekoSystem) {
    initscr();
    noecho();
    curs_set(0);
    nodelay(stdscr, TRUE);
    keypad(stdscr, TRUE);
    
    start_color();
    use_default_colors();
    init_pair(1, COLOR_GREEN, -1);   // Przód
    init_pair(2, COLOR_MAGENTA, -1); // Tył
    init_pair(3, COLOR_CYAN, -1);    // Lewo
    init_pair(4, COLOR_WHITE, -1);   // Prawo
    init_pair(5, COLOR_YELLOW, -1);  // Góra
    init_pair(6, COLOR_RED, -1);     // Dół
    init_pair(7, COLOR_WHITE, -1);   // Wnętrze

    int grid_x = static_cast<int>(ekoSystem.wiersze);
    int grid_y = static_cast<int>(ekoSystem.kolumny);
    int grid_z = static_cast<int>(ekoSystem.glebokosc);

    double scale_x = 1.0;
    double scale_y = 1.0;
    double scale_z = 1.0;

    double display_size = 150.0;

    double max_scale = std::max({scale_x, scale_y, scale_z});

    ViewState widok;

    while (true) {
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
        int statsWidth = 42;
        int center_x = (statsWidth + max_x) / 2;
        int center_y = max_y / 2;

        drawStatsPanel(ekoSystem, max_y);
        
        std::vector<Point3D> rotated_points;
        for (int i = 0; i < grid_x; ++i) {
            for (int j = 0; j < grid_y; ++j) {
                for (int k = 0; k < grid_z; ++k) {
                    RodzajMieszkanca rodzaj = ekoSystem.rodzajNiszy(i, j, k);
                    if (rodzaj == PUSTKA)
                        continue;

                    int color = 7;
                    if (rodzaj == GLON) color = 1;
                    else if (rodzaj == GRZYB) color = 2;
                    else if (rodzaj == BAKTERIA) color = 3;
                    else if (rodzaj == TRUP) color = 6;

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
        std::sort(rotated_points.begin(), rotated_points.end(), 
                  [](const Point3D& a, const Point3D& b) {
                      return a.z > b.z; 
                  });

        // Rysowanie
        for (const auto& p : rotated_points) {
            int px, py;
            project(p.x, p.y, p.z, display_size, center_x, center_y, max_scale, px, py);
            
            if (px >= 0 && px < max_x && py >= 0 && py < max_y) {
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
            widok.angle_x += 0.04;
            widok.angle_y += 0.03;
            widok.angle_z += 0.02;
        }

        if (wykonajKrok)
            ekoSystem++;
    
        refresh();
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }

    endwin();
    return 0;
}