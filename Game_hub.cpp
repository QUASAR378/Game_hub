#include <iostream>
#include <conio.h>  // For _kbhit() and _getch()
#include <windows.h>  // For Sleep() and system("cls")
using namespace std;

const int width = 20;
const int height = 10;
int x, y, foodX, foodY, score;
int tailX[100], tailY[100]; // Arrays to store body positions
int nTail; // Length of the tail
bool gameOver;

enum Direction { STOP = 0, LEFT, RIGHT, UP, DOWN };
Direction dir;

void Setup() {
    gameOver = false;
    dir = STOP;
    x = width / 2;
    y = height / 2;
    foodX = rand() % width;
    foodY = rand() % height;
    score = 0;
    nTail = 0; // Initially no tail
}

void Draw() {
    system("cls"); // Clear console

    // Display score
    cout << "Score: " << score << endl;

    // Draw top border
    for (int i = 0; i < width + 2; i++)
        cout << "#";
    cout << endl;

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (j == 0) cout << "#"; // Left border

            if (i == y && j == x)
                cout << "O"; // Snake head
            else if (i == foodY && j == foodX)
                cout << "F"; // Food
            else {
                bool print = false;
                for (int k = 0; k < nTail; k++) {
                    if (tailX[k] == j && tailY[k] == i) {
                        cout << "o"; // Snake body
                        print = true;
                    }
                }
                if (!print)
                    cout << " ";
            }

            if (j == width - 1) cout << "#"; // Right border
        }
        cout << endl;
    }

    // Draw bottom border
    for (int i = 0; i < width + 2; i++)
        cout << "#";
    cout << endl;
}

void Input() {
    if (_kbhit()) {
        switch (_getch()) {
        case 'a': dir = LEFT; break;
        case 'd': dir = RIGHT; break;
        case 'w': dir = UP; break;
        case 's': dir = DOWN; break;
        case 'x': gameOver = true; break; // Exit game
        }
    }
}

void Logic() {
    // Move tail
    int prevX = tailX[0];
    int prevY = tailY[0];
    int prev2X, prev2Y;
    tailX[0] = x;
    tailY[0] = y;

    for (int i = 1; i < nTail; i++) {
        prev2X = tailX[i];
        prev2Y = tailY[i];
        tailX[i] = prevX;
        tailY[i] = prevY;
        prevX = prev2X;
        prevY = prev2Y;
    }

    // Move snake head
    switch (dir) {
    case LEFT: x--; break;
    case RIGHT: x++; break;
    case UP: y--; break;
    case DOWN: y++; break;
    }

    // Check for wall collision
    if (x < 0 || x >= width || y < 0 || y >= height)
        gameOver = true;

    // Check for self-collision
    for (int i = 0; i < nTail; i++) {
        if (tailX[i] == x && tailY[i] == y)
            gameOver = true;
    }

    // Check if food is eaten
    if (x == foodX && y == foodY) {
        score += 10;
        foodX = rand() % width;
        foodY = rand() % height;
        nTail++; // Increase tail size
    }
}

int main() {
    Setup();
    while (!gameOver) {
        Draw();
        Input();
        Logic();
        Sleep(100 - (score / 10)); // Speed increases with score
    }
    cout << "\nGame Over! Final Score: " << score << endl;
    return 0;
}
/*Snake Growth

The snake's body is stored in tailX[] and tailY[].
When the snake eats food, its tail grows.
The tail follows the head's previous position.
Self-Collision Detection

The game ends if the snake collides with its body.
Score Display

The current score is displayed at the top.
Score increases by 10 each time food is eaten.
Dynamic Difficulty Levels

Speed increases as the score increases (Sleep(100 - (score / 10))).
This means the snake moves faster as the game progresses.
Improved Graphics (Optional)

The ASCII graphics use O for the head, o for the body, and F for the food.
If you want a graphical version, you can use SFML or graphics.h.
*/