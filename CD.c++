#include <bits/stdc++.h>
#include <conio.h>
#include <windows.h>

using namespace std;

#define MAX_LENGTH 1000

const char DIR_UP = 'U';
const char DIR_DOWN = 'D';
const char DIR_LEFT = 'L';
const char DIR_RIGHT = 'R';

int consoleWidth, consoleHeight;

void initScreen() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hConsole, &csbi);
    consoleHeight = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
    consoleWidth = csbi.srWindow.Right - csbi.srWindow.Left + 1;
}

void hideCursor() {
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO info;
    info.bVisible = false;
    info.dwSize = 1;
    SetConsoleCursorInfo(consoleHandle, &info);
}

struct Point {
    int x, y;
    Point(int x = 0, int y = 0) : x(x), y(y) {}
};

class Snake {
    vector<Point> body;
    char direction;

public:
    Snake(int x, int y) {
        body.push_back(Point(x, y));
        direction = DIR_RIGHT;
    }

    void changeDirection(char newDirection) {
        if ((newDirection == DIR_UP && direction != DIR_DOWN) ||
            (newDirection == DIR_DOWN && direction != DIR_UP) ||
            (newDirection == DIR_LEFT && direction != DIR_RIGHT) ||
            (newDirection == DIR_RIGHT && direction != DIR_LEFT)) {
            direction = newDirection;
        }
    }

    bool move(Point food) {
        for (int i = body.size() - 1; i > 0; i--) {
            body[i] = body[i - 1];
        }
        switch (direction) {
            case DIR_UP: body[0].y--; break;
            case DIR_DOWN: body[0].y++; break;
            case DIR_LEFT: body[0].x--; break;
            case DIR_RIGHT: body[0].x++; break;
        }

        for (size_t i = 1; i < body.size(); i++) {
            if (body[0].x == body[i].x && body[0].y == body[i].y) return false;
        }

        if (body[0].x < 0 || body[0].x >= consoleWidth ||
            body[0].y < 0 || body[0].y >= consoleHeight) return false;

        if (body[0].x == food.x && body[0].y == food.y) {
            body.push_back(Point());
            return true;
        }
        return true;
    }

    vector<Point> getBody() { return body; }
};

class Board {
    Snake *snake;
    Point food;
    int score;

public:
    Board() { reset(); }
    ~Board() { delete snake; }

    void reset() {
        delete snake;
        snake = new Snake(10, 10);
        score = 0;
        spawnFood();
    }

    void spawnFood() {
        food = Point(1 + rand() % (consoleWidth - 2), 1 + rand() % (consoleHeight - 2));
    }

    void draw() {
        system("cls");
        for (const Point &p : snake->getBody()) {
            gotoxy(p.x, p.y); cout << "O";
        }
        gotoxy(food.x, food.y); cout << "o";
        gotoxy(consoleWidth / 2, 0); cout << "Score: " << score;
    }

    void gotoxy(int x, int y) {
        COORD coord = { (SHORT)x, (SHORT)y };
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
    }

    bool update() {
        if (!snake->move(food)) return false;
        if (food.x == snake->getBody()[0].x && food.y == snake->getBody()[0].y) {
            score++;
            spawnFood();
        }
        return true;
    }

    void getInput() {
        if (_kbhit()) {
            int key = _getch();
            if (key == 224) {
                key = _getch();
                switch (key) {
                    case 72: snake->changeDirection(DIR_UP); break;
                    case 80: snake->changeDirection(DIR_DOWN); break;
                    case 75: snake->changeDirection(DIR_LEFT); break;
                    case 77: snake->changeDirection(DIR_RIGHT); break;
                }
            }
        }
    }

    int getScore() { return score; }
};

void playGame() {
    srand(time(0));
    initScreen();
    hideCursor();
    Board *board = new Board();

    while (true) {
        while (board->update()) {
            board->getInput();
            board->draw();
            Sleep(100);
        }
        system("cls");
        cout << "Game Over!\nFinal Score: " << board->getScore() << "\n";
        cout << "Press 'R' to Restart or 'Q' to Quit...\n";
        while (true) {
            char choice = _getch();
            if (choice == 'R' || choice == 'r') {
                board->reset();
                break;
            } else if (choice == 'Q' || choice == 'q') {
                delete board;
                return;
            }
        }
    }
}

int main() {
    playGame();
    return 0;
}
