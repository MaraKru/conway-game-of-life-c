#include <ncurses.h>
#include <stdio.h>

#define LY 0
#define HY 23
#define LX 0
#define RX 78
#define HEIGHT 25
#define WIDTH 80
#define MAX_SPEED 500
#define MIN_SPEED 50
#define MOVE_SPEED 50

void draw_border();
int user_desire(int cells[HEIGHT][WIDTH], int next[HEIGHT][WIDTH]);
void clear_cells(int cells[HEIGHT][WIDTH], int next[HEIGHT][WIDTH]);
void draw_cells(int cells[HEIGHT][WIDTH], int speed);
int load_file(int cells[HEIGHT][WIDTH], const char *filename);
int count_neighbors(int cells[HEIGHT][WIDTH], int y, int x);
void update(int cells[HEIGHT][WIDTH], int next[HEIGHT][WIDTH]);
void input_key(int *speed, int *exit_game, int keywords);

int main()
{
    int cells[HEIGHT][WIDTH];
    int next[HEIGHT][WIDTH];

    initscr();
    noecho();
    curs_set(0);
    nodelay(stdscr, TRUE);
    start_color();
    init_pair(1, COLOR_GREEN, COLOR_BLACK);

    clear_cells(cells, next);
    int speed = 100;
    int exit_game = 0;
    int flag = 0;
    if (user_desire(cells, next) != 0)
    {
        endwin();
        flag = 1;
    }
    while (!exit_game)
    {
        if (flag == 0)
        {
            draw_cells(cells, speed);
            update(cells, next);
            int ch = getch();
            if (ch != ERR)
            {
                input_key(&speed, &exit_game, ch);
            }
            napms(speed);
        }
    }
    endwin();
    return flag;
}

void input_key(int *speed, int *exit_game, int keywords)
{
    switch (keywords)
    {
    case 'a':
    case 'A':
        *speed = (*speed - MOVE_SPEED < MIN_SPEED) ? MIN_SPEED : *speed - MOVE_SPEED;
        break;

    case 'z':
    case 'Z':
        *speed = (*speed + MOVE_SPEED > MAX_SPEED) ? MAX_SPEED : *speed + MOVE_SPEED;
        break;
    case ' ':
        *exit_game = 1;
        break;
    }
}

int user_desire(int cells[HEIGHT][WIDTH], int next[HEIGHT][WIDTH])
{
    int tap;
    int flag = 0;
    echo();
    printw("Choose seed for generation (1-5): ");
    refresh();

    while ((tap = getch()) == ERR)
        ;
    tap -= '0';
    noecho();

    if (tap < 1 || tap > 5)
    {
        flag = 1;
    }
    else
    {
        const char *filename;
        switch (tap)
        {
        case 1:
            filename = "../patterns/pond.txt";
            break;
        case 2:
            filename = "../patterns/achim.txt";
            break;
        case 3:
            filename = "../patterns/turtle.txt";
            break;
        case 4:
            filename = "../patterns/random.txt";
            break;
        case 5:
            filename = "../patterns/orion.txt";
            break;
        }
        clear_cells(cells, next);
        if (load_file(cells, filename) != 0)
        {
            flag = 1;
        }
    }
    return flag;
}

void clear_cells(int cells[HEIGHT][WIDTH], int next[HEIGHT][WIDTH])
{
    for (int y = 0; y < HEIGHT; ++y)
    {
        for (int x = 0; x < WIDTH; ++x)
        {
            cells[y][x] = 0;
            next[y][x] = 0;
        }
    }
}

void draw_cells(int cells[HEIGHT][WIDTH], int speed)
{
    for (int x = 0; x < WIDTH; ++x)
    {
        mvaddch(LY, LX + 1 + x, '~');
    }

    for (int y = 0; y < HEIGHT; ++y)
    {
        mvaddch(LY + 1 + y, LX, '|');

        for (int x = 0; x < WIDTH; ++x)
        {
            if (cells[y][x])
            {
                attron(COLOR_PAIR(1));
                mvaddch(LY + 1 + y, LX + 1 + x, 'o');
                attroff(COLOR_PAIR(1));
            }
            else
            {
                mvaddch(LY + 1 + y, LX + 1 + x, ' ');
            }
        }

        mvaddch(LY + 1 + y, LX + 1 + WIDTH, '|');
    }

    for (int x = 0; x < WIDTH; ++x)
    {
        mvaddch(LY + 1 + HEIGHT, LX + 1 + x, '~');
    }
    mvprintw(HEIGHT + 2, 0,
             "Controls: SPACE - exit, A/Z - increase/decrease speed | Speed of cell colony development: %dms",
             speed);
}

int load_file(int cells[HEIGHT][WIDTH], const char *filename)
{
    FILE *file = fopen(filename, "r");
    if (!file)
    {
        printf("n/a");
        return 1;
    }

    for (int y = 0; y < HEIGHT; ++y)
    {
        char line[WIDTH + 2];
        if (!fgets(line, sizeof(line), file))
            break;

        for (int x = 0; x < WIDTH; ++x)
        {
            cells[y][x] = (line[x] == '*') ? 1 : 0;
        }
    }
    fclose(file);
    return 0;
}

int count_neighbors(int cells[HEIGHT][WIDTH], int y, int x)
{
    int count = 0;
    for (int dy = -1; dy <= 1; dy++)
    {
        for (int dx = -1; dx <= 1; dx++)
        {
            if (dx == 0 && dy == 0)
                continue;

            int ny = (y + dy + HEIGHT) % HEIGHT;
            int nx = (x + dx + WIDTH) % WIDTH;

            if (cells[ny][nx] == 1)
                count++;
        }
    }
    return count;
}

void update(int cells[HEIGHT][WIDTH], int next[HEIGHT][WIDTH])
{
    for (int y = 0; y < HEIGHT; y++)
    {
        for (int x = 0; x < WIDTH; x++)
        {
            int n = count_neighbors(cells, y, x);

            if (cells[y][x] == 1)
            {
                next[y][x] = (n == 2 || n == 3) ? 1 : 0;
            }
            else
            {
                next[y][x] = (n == 3) ? 1 : 0;
            }
        }
    }

    for (int y = 0; y < HEIGHT; y++)
        for (int x = 0; x < WIDTH; x++)
            cells[y][x] = next[y][x];
}