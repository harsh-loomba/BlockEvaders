#define startLength 4;
#define BLUE 0x187F
#define RED 0xb800
#define YELLOW 0xf600
#define GREEN 0x07F0
#define BLACK 0x0000
#define TEST1 0x07fa
#define WHITE 0xFFFF
#define frameUp 10
#define frameDown 220
#define frameRight 310
#define frameLeft 10
#define maxSize 200
#define velocity 3
#define squareSize 2
#define PUSHBUTTONS ((volatile long *)0xFF200050)

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// sleep function

void sleep(int delay_seconds)
{
    time_t start_time = time(NULL);
    while (difftime(time(NULL), start_time) < delay_seconds)
    {
        // do nothing
    }
}

// writing and clearing functions

void write_pixel(int x, int y, short colour)
{
    volatile short *vga_addr = (volatile short *)(0x08000000 + (y << 10) + (x << 1));
    *vga_addr = colour;
}

void write_char(int x, int y, char c)
{
    volatile char *character_buffer = (char *)(0x09000000 + (y << 7) + x);
    *character_buffer = c;
}

void clear_screen_notchar()
{
    int x, y;
    for (x = 0; x < 320; x++)
    {
        for (y = 0; y < 240; y++)
        {
            write_pixel(x, y, BLACK);
        }
    }
}

void clear_screen_char()
{
    int x, y;
    for (x = 0; x < 80; x++)
    {
        for (y = 0; y < 60; y++)
        {
            write_char(x, y, 0);
        }
    }
}

void drawSquare(int cornerX, int cornerY, int size, short color) // draw a square of given size at given position whith color
{
    int i, j;
    for (i = cornerX; i < cornerX + size; i++)
    {
        for (j = cornerY; j < cornerY + size; j++)
        {
            write_pixel(i, j, color);
        }
    }
}

void draw_ver_line(int cornerX, int cornerY, int size, short color) // draw a square of given size at given position whith color
{
    int i, j;
    for (i = cornerX; i < cornerX + 1; i++)
    {
        for (j = cornerY; j < cornerY + size; j++)
        {
            write_pixel(i, j, color);
        }
    }
}

void draw_hor_line(int cornerX, int cornerY, int size, short color) // draw a square of given size at given position whith color
{
    int i, j;
    for (i = cornerX; i < cornerX + size; i++)
    {
        for (j = cornerY; j < cornerY + 1; j++)
        {
            write_pixel(i, j, color);
        }
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

typedef struct Block
{
    int x;
    int y;
    int size;
    int exists;
    int speed;
} block;

typedef struct Player
{
    int y;
    int height;
    int width;
} Player;

char grid[50][80], prev_grid[50][80];
block block_list[10] = {{0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}};
Player player = {39, 5, 3};

int min(int x, int y)
{
    if (x < y)
    {
        return x;
    }
    return y;
}

void clear_grid()
{
    for (int i = 1; i < 49; i++)
    {
        for (int j = 1; j < 79; j++)
        {
            grid[i][j] = ' ';
        }
    }

    for (int i = 1; i < 49; i++)
    {
        grid[i][0] = '*';
        grid[i][79] = '*';
    }

    for (int j = 0; j < 80; j++)
    {
        grid[0][j] = '*';
        grid[49][j] = '=';
        grid[48][j] = '=';
    }
}

void blit()
{
    // clear_screen_notchar();
    for (int i = 0; i < 50; i++)
    {
        char c;
        for (int j = 0; j < 80; j++)
        {
            if (prev_grid[i][j] != grid[i][j])
            {
                c = grid[i][j];

                switch (c)
                {
                case '*':
                    drawSquare(j * 4, i * 4, 4, RED);

                    if (j % 4 == 0 && i == 0)
                    {
                        draw_ver_line(j * 4, i * 4, 4, BLACK);
                    }
                    if (i % 4 == 0 && (j == 0 || j == 79))
                    {
                        draw_hor_line(j * 4, i * 4, 4, BLACK);
                    }
                    break;

                case '=':
                    drawSquare(j * 4, i * 4, 4, YELLOW);
                    break;

                case '0':
                    drawSquare(j * 4, i * 4, 4, 0xA44B);
                    break;

                case '1':
                    drawSquare(j * 4, i * 4, 4, 0xC636);
                    break;

                case '2':
                    drawSquare(j * 4, i * 4, 4, 0x8C4A);
                    break;

                case '3':
                    drawSquare(j * 4, i * 4, 4, 0xD368);
                    break;

                case '4':
                    drawSquare(j * 4, i * 4, 4, 0xCD85);
                    break;

                case '5':
                    drawSquare(j * 4, i * 4, 4, 0xF4A0);
                    break;

                case '6':
                    drawSquare(j * 4, i * 4, 4, 0x9552);
                    break;

                case '7':
                    drawSquare(j * 4, i * 4, 4, 0x8B45);
                    break;

                case '8':
                    drawSquare(j * 4, i * 4, 4, 0xF596);
                    break;

                case '9':
                    drawSquare(j * 4, i * 4, 4, 0xBC8F);
                    break;

                case 'P':
                    drawSquare(j * 4, i * 4, 4, WHITE);
                    break;

                default:
                    drawSquare(j * 4, i * 4, 4, 0x0000);
                    break;
                }
            }
        }
    }
}

void print_block(block B, int num)
{
    int x = B.x, y = B.y, size = B.size, exists = B.exists;

    if (exists == 1)
    {
        for (int i = x; i < x + size; i++)
        {
            for (int j = y; j < min(y + size, 79); j++)
            {
                grid[i][j] = '0' + num;
            }
        }
    }
}

void print_player()
{
    for (int i = 43; i < 43 + player.height; i++)
    {
        for (int j = player.y; j < player.y + player.width; j++)
        {
            grid[i][j] = 'P';
        }
    }
}

int check_collide(block B)
{
    if (B.exists == 1)
    {
        if (B.size + B.x > 43)
        {
            if ((B.y >= player.y) && (B.y < player.y + player.width))
            {
                return 1;
            }

            if ((B.y + B.size > player.y) && (B.y + B.size <= player.y + player.width))
            {
                return 1;
            }

            if ((B.y <= player.y) && (B.y + B.size >= player.y + player.width))
            {
                return 1;
            }
        }
    }
    return 0;
}

int display()
{
    int collide = 0;
    clear_grid();
    for (int i = 0; i < 10; i++)
    {
        if (block_list[i].exists == 1)
        {
            if (check_collide(block_list[i]) == 1)
            {
                collide = 1;
                block_list[i].exists = 0;
                continue;
            }
            print_block(block_list[i], i);
        }
    }
    print_player();
    blit();

    for (int i = 0; i < 50; i++)
    {
        for (int j = 0; j < 80; j++)
        {
            prev_grid[i][j] = grid[i][j];
        }
    }

    return collide;
}

void make_block(int y, int size, int num, int speed)
{
    if ((num >= 0) & (num < 10))
    {
        block B = {1, y, size, 1, speed};
        block_list[num] = B;
    }
}

void remove_block(int num)
{
    if ((num >= 0) & (num < 10))
    {
        block_list[num].exists = 0;
    }
}

void move_player(int inp)
{
    char c;
    if (inp == 1)
    {
        c = 'R';
    }
    else if (inp == 2)
    {
        c = 'L';
    }
    else
    {
        return;
    }
    if (c == 'L')
    {
        if (player.y > 1)
        {
            player.y--;
        }
    }
    if (c == 'R')
    {
        if (player.y < 80 - player.width - 1)
        {
            player.y++;
        }
    }
}

void set_player_y(int y)
{
    if (y < 1)
    {
        player.y = 1;
    }
    else if (y > 80 - player.width - 1)
    {
        player.y = 80 - player.width - 1;
    }
    else
    {
        player.y = y;
    }
}

void move_blocks(int *score, int *lvl_score)
{
    for (int i = 0; i < 10; i++)
    {
        if (block_list[i].exists == 1)
        {

            block_list[i].x += block_list[i].speed;

            if (block_list[i].x >= 49 - block_list[i].size)
            {
                block_list[i].exists = 0;
                *score += 10;
                *lvl_score += 10;
            }
        }
    }
}

// void delay(int frame_rate)
// {
//     int ct = 0;

//     while (ct < 1000000)
//     {
//         ct++;
//     }
// }

int main()
{
    clear_screen_notchar();
    clear_screen_char();

start_again:;
    // clear_screen_notchar();
    clear_screen_char();
    int lives = 3, collide = 0, level = 1, score = 0, max_size = 6, lvl_score = 0, max_speed = 3, sensitivity = 2, cont = 0, frame_rate = 500000000;

    time_t t;

    srand((unsigned)time(&t));

    display();

    while (lives > 0)
    {
        char *hw1 = "BLOCK EVADERS";
        int x = 33;
        while (*hw1)
        {
            write_char(x, 52, *hw1);
            x++;
            hw1++;
        }

        char *hw2 = "LIVES ";
        x = 15;
        int y = lives;
        char str[50];                  // allocate a buffer to hold the concatenated string
        sprintf(str, "%s %d", hw2, y); // format the string with hw2 and y

        // loop through the concatenated string and write each character
        char *p = str;
        while (*p)
        {
            write_char(x, 55, *p);
            x++;
            p++;
        }

        char *hw3 = "LEVEL ";
        x = 55;
        y = level;
        char str1[50];                  // allocate a buffer to hold the concatenated string
        sprintf(str1, "%s %d", hw3, y); // format the string with hw2 and y

        // loop through the concatenated string and write each character
        char *p1 = str1;
        while (*p1)
        {
            write_char(x, 55, *p1);
            x++;
            p1++;
        }

        if (cont == 0)
        {
            for (int i = 0; i < 3 + min(level, 7); i++)
            {
                if (block_list[i].exists == 0)
                {
                    make_block(1 + rand() % 78, 2 + rand() % max_size, i, 1 + rand() % max_speed);
                    break;
                }
            }
        }

        long PBval;

        PBval = *PUSHBUTTONS;
        move_player(PBval);

        if (cont >= sensitivity)
        {
            move_blocks(&score, &lvl_score);
            cont = 0;
        }
        else
        {
            cont++;
        }

        if ((lvl_score > 200) && (level < 15))
        {
            level++;
            collide = 0;
            lvl_score %= 200;

            if (level % 5 == 0)
            {
                lives++;
            }
        }

        collide = display();

        if (collide == 1)
        {
            lives--;
            collide = 0;

            for (int i = 0; i < 10; i++)
            {
                remove_block(i);
            }

            set_player_y(39);
        }

        // delay(frame_rate);
        int ct9 = 0;
        while (ct9 < 1000000)
        {
            ct9++;
        }
    }

    clear_screen_char();
    level = 1;
    char *hw1 = "GAME OVER!!!";
    int x = 35;
    while (*hw1)
    {
        write_char(x, 30, *hw1);
        x++;
        hw1++;
    }
    char *hw2 = "YOUR REPLAY WILL START SHORTLY";
    x = 35;
    while (*hw2)
    {
        write_char(x, 33, *hw2);
        x++;
        hw2++;
    }

    int ct = 0;

    while (ct < 100000000)
    {
        ct++;
    }
    goto start_again;

    return 0;
}