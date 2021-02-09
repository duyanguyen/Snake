#include <iostream>
#include <string>
#include <windows.h>
#include <conio.h>

using namespace std;

constexpr int height{ 20 }, width{ 30 };
int headX, headY, targetX, targetY, nTail, score, highScore;
int tailX[100], tailY[100];
bool gameOver, quit;
enum class direction {STOP, LEFT, RIGHT, UP, DOWN};
direction dir;

void hidecursor()
{
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO info;
    info.dwSize = 100;
    info.bVisible = FALSE;
    SetConsoleCursorInfo(consoleHandle, &info);
}

void setup()
{
    //Initialize parameters for a new game
    hidecursor();
    gameOver = false;
    quit = false;
    targetX = (rand() % (height-2))+1;
    targetY = (rand() % (width-2))+1;
    headX = height / 2;
    headY = height / 2;
    dir = direction::STOP;
    nTail = 0;
    score = 0;
}

void instruction()
{
    cout << "INSTRUCTIONS \n";
    cout << "NEW GAME   : N \n";
    cout << "MOVE LEFT  : A \n";
    cout << "MOVE RIGHT : D \n";
    cout << "MOVE UP    : W \n";
    cout << "MOVE DOWN  : S \n";
    cout << "QUIT       : Q \n";
}

void draw()
{
    system("cls");
    instruction();
    for (int i = 0; i < height; i++)
    {
        cout << endl;
        //Draw top and bottom walls
        if (i == 0 || i == height-1)
        {
            cout << string(width, '#');
        }
        else
        {
            for (int j = 0; j < width; j++)
            {
                //Draw fruit, head, tail and side walls
                if (i == targetX && j == targetY)
                {
                    cout << '*';
                }
                else if (i == headX && j == headY)
                {
                    cout << '@';
                }
                else if (j == 0 || j == width - 1)
                {
                    cout << '#';
                }
                else
                {
                    bool drawEmpty{ true };
                    for (int k = 0; k < nTail; k++)
                    {
                        if (i == tailX[k] && j == tailY[k])
                        {
                            cout << 'x';
                            drawEmpty = false;
                        }
                    }
                    if (drawEmpty)
                    {
                        cout << ' ';
                    }
                }
            }
        }
    }
    cout << endl;
    cout << "Score: " << score << string(width/4, ' ') + "High score: " << highScore;
}

void input()
{
    //Collecting user directional input
    if (_kbhit())
    {
        switch (_getch())
        {
        case 'a':
            dir = direction::LEFT;
            break;
        case 'd':
            dir = direction::RIGHT;
            break;
        case 'w':
            dir = direction::UP;
            break;
        case 's':
            dir = direction::DOWN;
            break;
        case 'n':
            setup();
            break;
        case 'q':
            gameOver = true;
            quit = true;
            break;
        }
    }
}

void logic()
{
    //Update tail coordinate array following new head position
    int prevX = tailX[0], prevY = tailY[0];
    int tempX, tempY;
    tailX[0] = headX;
    tailY[0] = headY;
    for (int i = 1; i < nTail; i++)
    {
        tempX = tailX[i];
        tempY = tailY[i];        
        tailX[i] = prevX;
        tailY[i] = prevY;
        prevX = tempX;
        prevY = tempY;
    }

    //Update head coordinate with new user input
    switch (dir)
    {
    case direction::LEFT:
        headY--;
        break;
    case direction::RIGHT:
        headY++;
        break;
    case direction::UP:
        headX--;
        break;
    case direction::DOWN:
        headX++;
        break;
    case direction::STOP:
        break;
    }

    //Hit a wall
    if (headX <= 0 || headX >= height-1 || headY <= 0 || headY >= width-1)
    {
        gameOver = true;
    }
    
    //Hit its tail
    for (int k = 0; k < nTail; k++)
    {
        if (headX == tailX[k] && headY == tailY[k])
        {
            gameOver = true;
            break;
        }
    }
    //Eating a fruit
    if (headX == targetX && headY == targetY)
    {
        targetX = (rand() % (height - 2))+1;
        targetY = (rand() % (width - 2))+1;
        score++;
        nTail++;
    }
}

int main()
{
    while (!quit)
    {
        setup();
        while (!gameOver)
        {
            draw();
            input();
            logic();
            Sleep(50);
        }
        if (score > highScore)
        {
            highScore = score;
        }
    }
    return 0;
}
