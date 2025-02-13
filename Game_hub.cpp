#include <SFML/Graphics.hpp>
#include <vector>
#include <cstdlib>
#include <ctime>

using namespace sf;

// Game Constants
const int cellSize = 20;
const int width = 30, height = 20;
const int winWidth = width * cellSize, winHeight = height * cellSize;

// Directions
enum Direction { UP, DOWN, LEFT, RIGHT };
Direction dir = RIGHT;

// Snake Data
std::vector<Vector2i> snake = { {5, 5}, {4, 5}, {3, 5} };
Vector2i food(rand() % width, rand() % height);
bool gameOver = false;
int score = 0;

// Move Snake Function
void moveSnake() {
    if (gameOver) return;

    Vector2i newHead = snake[0]; // Get head position

    // Move in current direction
    if (dir == UP) newHead.y--;
    else if (dir == DOWN) newHead.y++;
    else if (dir == LEFT) newHead.x--;
    else if (dir == RIGHT) newHead.x++;

    // Check collisions (wall or self)
    if (newHead.x < 0 || newHead.x >= width || newHead.y < 0 || newHead.y >= height) {
        gameOver = true;
        return;
    }
    for (auto segment : snake)
        if (segment == newHead) { gameOver = true; return; }

    // Move Snake
    snake.insert(snake.begin(), newHead);
    if (newHead == food) {
        score += 10;
        food = { rand() % width, rand() % height }; // Generate new food
    }
    else {
        snake.pop_back(); // Remove last segment if no food eaten
    }
}

// Main Game Function
int main() {
    srand(time(0));

    // Create Window
    RenderWindow window(VideoMode(winWidth, winHeight), "🐍 Snake Game - SFML", Style::Titlebar | Style::Close);
    window.setFramerateLimit(10); // Control Speed

    // Colors
    Color bgColor = Color(50, 50, 50);
    Color snakeColor = Color::Green;
    Color foodColor = Color::Red;

    while (window.isOpen()) {
        Event e;
        while (window.pollEvent(e)) {
            if (e.type == Event::Closed) window.close();
            if (e.type == Event::KeyPressed) {
                if (e.key.code == Keyboard::W && dir != DOWN) dir = UP;
                if (e.key.code == Keyboard::S && dir != UP) dir = DOWN;
                if (e.key.code == Keyboard::A && dir != RIGHT) dir = LEFT;
                if (e.key.code == Keyboard::D && dir != LEFT) dir = RIGHT;
            }
        }

        moveSnake();

        // Drawing
        window.clear(bgColor);

        // Draw Food
        RectangleShape foodShape(Vector2f(cellSize, cellSize));
        foodShape.setFillColor(foodColor);
        foodShape.setPosition(food.x * cellSize, food.y * cellSize);
        window.draw(foodShape);

        // Draw Snake
        for (size_t i = 0; i < snake.size(); i++) {
            RectangleShape snakeSegment(Vector2f(cellSize, cellSize));
            snakeSegment.setFillColor(snakeColor);
            snakeSegment.setPosition(snake[i].x * cellSize, snake[i].y * cellSize);
            window.draw(snakeSegment);
        }

        // Game Over Message
        if (gameOver) {
            Font font;
            if (!font.loadFromFile("arial.ttf")) return -1;
            Text text("GAME OVER", font, 40);
            text.setFillColor(Color::White);
            text.setPosition(winWidth / 3, winHeight / 3);
            window.draw(text);
        }

        window.display();
    }

    return 0;
}
