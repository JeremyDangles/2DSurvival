#include <raylib.h>
#include <iostream>
#include <cmath>

float screenWidth = 1280;
float screenHeight = 720;  

Vector2 centreScreen = { screenWidth / 2, screenHeight / 2};

float playerSpeed = 200.0f;

bool isDayTime = false;
Color NIGHTTIME = { 0, 0, 0, 200 };

void movePlayer(Vector2 moveDirection);
Vector2 getMoveDirection();
Vector2 setPlayerPosition(Vector2 playerPosition, Vector2 moveDirection, float playerSpeed, float deltaTime);
int showCurrentTime(double currentTime);
void handleDayNightCycle(double currentTime);

int main()
{    
    Vector2 playerPosition = centreScreen;

    SetConfigFlags(FLAG_MSAA_4X_HINT);
    InitWindow(screenWidth, screenHeight, "2DSurvival");

    while (!WindowShouldClose())
    {
        float deltaTime = GetFrameTime();
        double currentTime = GetTime();

        Vector2 moveDirection = getMoveDirection();
        playerPosition = setPlayerPosition(playerPosition, moveDirection, playerSpeed, deltaTime);

        BeginDrawing();
            ClearBackground(DARKGREEN);
            
            movePlayer(playerPosition); 

            handleDayNightCycle(currentTime);

            showCurrentTime(currentTime);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}

void handleDayNightCycle(double currentTime)
{
    double time = fmod(currentTime, 24);

    if (time >= 20 && time <= 22) // DUSK
    {
        int initialOpacity = 0;
        int finalOpacity = 200;
        
        float t = (time - 20) / 2.0f;

        int currentOpacity = (int)(((1.0f - t) * initialOpacity) + (t * finalOpacity));

        DrawRectangle(0, 0, screenWidth, screenHeight, {0, 0, 0, (unsigned char)currentOpacity});
    }

    if (time < 4 || time > 22) // NIGHT
    {
        DrawRectangle(0, 0, screenWidth, screenHeight, NIGHTTIME);
    }

    if (time >= 4 && time <= 6) // DAWN
    {
        int initialOpacity = 200;
        int finalOpacity = 0;
        
        float t = (time - 4) / 2.0f;

        int currentOpacity = (int)(((1.0f - t) * initialOpacity) + (t * finalOpacity));

        DrawRectangle(0, 0, screenWidth, screenHeight, {0, 0, 0, (unsigned char)currentOpacity});
    }
}

int showCurrentTime(double currentTime)
{
    int dayLength = 24;

    int currentDay = (int)(currentTime / dayLength) + 1;
    int currentTimeInt = (int)currentTime % dayLength;

    std::string day = "DAY " + std::to_string(currentDay);
    DrawText(day.c_str(), 50, 30, 20, WHITE);

    std::string time = std::to_string(currentTimeInt);
    DrawText(time.c_str(), 50, 50, 20, WHITE);

    return currentTimeInt;
}

void movePlayer(Vector2 playerPosition)
{
    DrawCircle(playerPosition.x, playerPosition.y, 20, WHITE);
}

Vector2 setPlayerPosition(Vector2 playerPosition, Vector2 moveDirection, float playerSpeed, float deltaTime)
{
    playerPosition.x += moveDirection.x * playerSpeed * deltaTime;
    playerPosition.y += moveDirection.y * playerSpeed * deltaTime;

    return playerPosition;
}

Vector2 getMoveDirection()
{
    Vector2 moveDirection = { 0.0f, 0.0f };

    if (IsKeyDown(KEY_W))
    {
        moveDirection.y -= 1.0f;
    }
    if (IsKeyDown(KEY_S))
    {
        moveDirection.y += 1.0f;
    }
    if (IsKeyDown(KEY_A))
    {
        moveDirection.x -= 1.0f;
    }
    if (IsKeyDown(KEY_D))
    {
        moveDirection.x += 1.0f;
    }

    float length = sqrtf((moveDirection.x * moveDirection.x) + (moveDirection.y * moveDirection.y));

    //std::cout << length << std::endl;
    if (length > 1.0f)
    {
        moveDirection.x /= length;
        moveDirection.y /= length;
    }

    return moveDirection;
}

