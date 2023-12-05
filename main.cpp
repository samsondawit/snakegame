#include <iostream>
#include <raylib.h>
#include <deque>
#include <thread>
#include <raymath.h>

using namespace std;

static bool allowMove = false;
Color white = {255, 255, 255, 255};
Color black = {0, 0, 0, 255};
Color red = {255, 0, 0, 255};


int cellSize = 20;
int cellCount = 40;
int offset = 90;

double lastUpdateTime = 0;

bool ElementInDeque(Vector2 element, deque<Vector2> deque)
{
    for (unsigned int i = 0; i < deque.size(); i++)
    {
        if (Vector2Equals(deque[i], element))
        {
            return true;
        }
    }
    return false;
}

bool EventTriggered(double interval)
{
    double currentTime = GetTime();
    if (currentTime - lastUpdateTime >= interval)
    {
        lastUpdateTime = currentTime;
        return true;
    }
    return false;
}

class Snake
{
public:
    deque<Vector2> body;
    Vector2 direction;
    bool addSegment = false;
    Color color;

    Snake() : color(black), direction({1, 0}), body({{6, 9}, {5, 9}, {4, 9}}) {}
    Snake(Color snakeColor) : color(snakeColor), direction({1, 0}), body({{6, 9}, {5, 9}, {4, 9}}) {}

    virtual ~Snake() {}

    virtual void Draw()
    {
        for (unsigned int i = 0; i < body.size(); i++)
        {
            float x = body[i].x;
            float y = body[i].y;
            Rectangle segment = Rectangle{offset + x * cellSize, offset + y * cellSize, (float)cellSize, (float)cellSize};
            DrawRectangleRounded(segment, 0.5, 6, color);
        }
    }

    virtual void Update()
    {
        body.push_front(Vector2Add(body[0], direction));
        if (addSegment == true)
        {
            addSegment = false;
        }
        else
        {
            body.pop_back();
        }
    }


    void Reset()
    {
        body = {Vector2{6, 9}, Vector2{5, 9}, Vector2{4, 9}};
        direction = {1, 0};
    }
};

class RandomSnake : public Snake {
public:
    RandomSnake() : Snake(red) {
    body = {Vector2{10, 10}, Vector2{11, 10}, Vector2{12, 10}};
    direction = {1, 0};
    }

    void Update() override {
        
            int randDir = GetRandomValue(0, 3);
            switch (randDir) {
        case 0: 
            if (direction.y != 1) direction = {0, -1}; // Up
            break;
        case 1: 
            if (direction.y != -1) direction = {0, 1};  // Down
            break;
        case 2: 
            if (direction.x != 1) direction = {-1, 0}; // Left
            break;
        case 3: 
            if (direction.x != -1) direction = {1, 0};  // Right
            break;
    }    
        
        Vector2 newHead = Vector2Add(body.front(), direction);
        
        newHead.x = max(0, min(cellCount - 1, (int)newHead.x));
        newHead.y = max(0, min(cellCount - 1, (int)newHead.y));
        // if (newHead.x < 0) newHead.x = 0;
        // if (newHead.y < 0) newHead.y = 0;
        // if (newHead.x >= cellCount) newHead.x = cellCount - 1;
        // if (newHead.y >= cellCount) newHead.y = cellCount - 1;
        body.push_front(newHead);
        body.pop_back(); 
    }
};

class Food
{

public:
    Vector2 position;
    Texture2D texture;

    Food(deque<Vector2> snakeBody)
    {
        Image image = LoadImage("Graphics/food.png");
        texture = LoadTextureFromImage(image);
        UnloadImage(image);
        position = GenerateRandomPos(snakeBody);
    }

    ~Food()
    {
        UnloadTexture(texture);
    }

    void Draw()
    {
        DrawTexture(texture, offset + position.x * cellSize, offset + position.y * cellSize, BLACK);
    }
    
    Vector2 GenerateRandomCell()
    {
        float x = GetRandomValue(0, cellCount - 1);
        float y = GetRandomValue(0, cellCount - 1);
        return Vector2{x, y};
    }

    Vector2 GenerateRandomPos(deque<Vector2> snakeBody)
    {
        Vector2 position = GenerateRandomCell();
        while (ElementInDeque(position, snakeBody))
        {
            position = GenerateRandomCell();
        }
        return position;
    }
};
class SpecialFood : public Food {
public:
    SpecialFood(deque<Vector2> snakeBody) : Food(snakeBody) {
        // Load a different texture for special food
        UnloadTexture(texture); // Unload the base class texture
        Image image = LoadImage("Graphics/special_food.png");
        texture = LoadTextureFromImage(image);
        UnloadImage(image);
        position = GenerateRandomPos(snakeBody);
    }

};

class Game
{
public:
    Snake snake;
    RandomSnake randomSnake;
    Food* regularFood = new Food(snake.body);
    SpecialFood* specialFood = nullptr;
    bool running = true;
    bool ispaused = false;
    int score = 0;
    int lives = 5;
    Sound eatSound;
    Sound wallSound;
    Sound lifeSound; 
    int lastScoreIncrement = 0;

    Game() : snake(), randomSnake()
    {
        InitAudioDevice();
        eatSound = LoadSound("Sounds/eat.mp3");
        wallSound = LoadSound("Sounds/wall.mp3");
        lifeSound = LoadSound("Sounds/life.mp3");
    }

    ~Game() {
        delete regularFood;
        if (specialFood != nullptr) {
            delete specialFood;
        }
        UnloadSound(eatSound);
        UnloadSound(wallSound);
        UnloadSound(lifeSound);
        CloseAudioDevice();
    }

    void Draw()
    {
        regularFood->Draw();
        if (specialFood != nullptr) {
            specialFood->Draw();
        }
        snake.Draw();
        DrawText(TextFormat("Lives: %i", lives), offset - 5, offset + cellSize * cellCount + 50, 40, black);
        if(score >= 5){
            randomSnake.Draw();
        }

    }
    bool ShouldGenerateSpecialFood() {
    // Random chance when score is over 10 and lives less than 4
        if (score >= 10 && lives < 4) {
            return GetRandomValue(0, 10) < 3; // 30% chance to generate special food
    }
    return false;
}

    void Update()
    {
        if (running)
        {
            snake.Update();
            CheckCollisionWithFood(regularFood);
            if (ShouldGenerateSpecialFood() && specialFood == nullptr) {
                specialFood = new SpecialFood(snake.body);
        }
            if (specialFood != nullptr) {
                CheckCollisionWithFood(specialFood);
            }
            CheckCollisionWithEdges();
            CheckCollisionWithTail();
            if (score >= 5){
                randomSnake.Update();
                CheckCollisionWithRandomSnake();
            }
        }
    }
    
    private:
    void CheckCollisionWithFood(Food* food) {
        if (Vector2Equals(snake.body[0], food->position)) {
            if (food == regularFood) {
                score++;
                food->position = food->GenerateRandomPos(snake.body);
                snake.addSegment = true;
                PlaySound(eatSound);
            } else if (food == specialFood) {
                lives += 2;
                PlaySound(lifeSound);
                delete specialFood;
                specialFood = nullptr;
            }
        }
    }

    private:
    void CheckCollisionWithEdges()
    {
        if (snake.body[0].x == cellCount || snake.body[0].x == -1 ||
            snake.body[0].y == cellCount || snake.body[0].y == -1)
        {
            
                ReduceLife();
                // Reset direction to avoid immediate collision
                snake.direction = {1, 0};
        }
    }
    
    void CheckCollisionWithRandomSnake() {
        // Check if any segment of the player's snake collides with the random snake
        for (const auto& segment : randomSnake.body) {
            if (Vector2Equals(snake.body[0], segment)) {
                ReduceLife();
                break; // Exit the loop after detecting a collision
            }
        }

        // Check if the head of the random snake collides with any part of the player's snake
        for (const auto& segment : snake.body) {
            if (Vector2Equals(segment, randomSnake.body[0])) {
                ReduceLife();
                break; // Exit the loop after detecting a collision
            }
        }
    }
    
    void CheckCollisionWithTail()
    {
        deque<Vector2> headlessBody = snake.body;
        headlessBody.pop_front();

        if (ElementInDeque(snake.body[0], headlessBody))
        {
            PlaySound(wallSound); 
            lives--; 
            if (snake.body.size() > 1) {
                    snake.body.pop_back();
            }

            if (lives <= 0) {
                GameOver();
            }
    }
}
   
    void ReduceLife() {
        PlaySound(wallSound);
        lives--;

        if (snake.body.size() > 1) {
                    snake.body.pop_back();
                }
                
        if (lives <= 0) {
            GameOver();
        } else {
            // Optionally, you can reset the snake's position or length here
            snake.body[0] = Vector2{max(1, min(cellCount - 2, (int)snake.body[0].x)),
                                    max(1, min(cellCount - 2, (int)snake.body[0].y))};
        }
}   
    public:
    void TogglePause(){
            ispaused = !ispaused;
        }

    void HandleInput() {
                if (IsKeyPressed(KEY_P)) {
                    TogglePause();
                }
                if (!ispaused){
                    if (IsKeyPressed(KEY_UP) && snake.direction.y != 1 && allowMove) {
                        snake.direction = {0, -1};
                        allowMove = false;
                    } else if (IsKeyPressed(KEY_DOWN) && snake.direction.y != -1 && allowMove) {
                        snake.direction = {0, 1};
                        allowMove = false;
                    } else if (IsKeyPressed(KEY_LEFT) && snake.direction.x != 1 && allowMove) {
                        snake.direction = {-1, 0};
                        allowMove = false;
                    } else if (IsKeyPressed(KEY_RIGHT) && snake.direction.x != -1 && allowMove) {
                        snake.direction = {1, 0};
                        allowMove = false;
                    } 
                }
    }

    // New method for resetting the game
    void ResetGame() {
        snake.Reset();
        delete regularFood; // Delete the existing instance
        regularFood = new Food(snake.body); // Create a new instance

    // Reset special food
    if (specialFood != nullptr) {
        delete specialFood;
        specialFood = nullptr;
    }
        score = 0;
        lives = 5;
        running = true;
    }


    void GameOver()
    {
        if (lives <= 0)
        {
            PlaySound(wallSound); 
            running = false; // Stop the game
        }
}


    // private:
    // bool shouldTerminate = false;  

    // public:
    // void terminateThreads() {
    //     shouldTerminate = true;
    // }

    // bool isRunning() {
    //     return !shouldTerminate && running;
    // }

};

    // void UpdateRandomSnake(RandomSnake* randomSnake, Game* game) {
    //             while (game->isRunning()) {
    //                 randomSnake->Update();
    //                 std::this_thread::sleep_for(std::chrono::milliseconds(200));
    //         }
    //     };

    // void UpdatePlayerSnake(Snake* snake, Game* game) {
    //             while (game->isRunning()) {
    //                 snake->Update();
    //                 std::this_thread::sleep_for(std::chrono::milliseconds(200));
    //         }
    //     };

int main()
{
    cout << "Starting the game..." << endl;
    InitWindow(2 * offset + cellSize * cellCount, 2 * offset + cellSize * cellCount, "Samson's Snake Game");
    int basefps = 30;
    SetTargetFPS(basefps);

    Game game = Game();
    // std::thread playerSnakeThread(UpdatePlayerSnake, &game.snake, &game);
    // std::thread randomSnakeThread(UpdateRandomSnake, &game.randomSnake, &game);


    while (WindowShouldClose() == false)
    {
        BeginDrawing();
        game.HandleInput();
        if (game.lives > 0 && !game.ispaused) {
            
            if (EventTriggered(0.2)) {
                    allowMove = true;
                    game.Update();
        }
    }   else if (game.ispaused) {
        // Render pause screen or overlay
            DrawText("Game Paused. Press P to Resume", 100, 200, 20, black);
    }


        if (game.score >= game.lastScoreIncrement + 5)
        {
            basefps += 5;
            SetTargetFPS(basefps);
            game.lastScoreIncrement = game.score; 

        }
       
        if (game.lives <=0 ) {
        DrawText("Game Over! Press R to Restart or Any Other Key to Quit", 100, 200, 20, red);

        if (IsKeyPressed(KEY_R)) {
            game.ResetGame();
        }

        else if (GetKeyPressed()) {
        CloseWindow();
        return 0;
    }
}

        // Drawing
        ClearBackground(white);
        DrawRectangleLinesEx(Rectangle{(float)offset - 5, (float)offset - 5, (float)cellSize * cellCount + 10, (float)cellSize * cellCount + 10}, 5, black);
        DrawText("Samson's snake game", offset - 5, 40, 40, black);
        DrawText(TextFormat("%i", game.score), offset - 5, offset + cellSize * cellCount + 10, 40, black);
        game.Draw();

        EndDrawing();
    }
    // game.terminateThreads();
    // playerSnakeThread.join();
    // randomSnakeThread.join();
    CloseWindow();
    return 0;
}