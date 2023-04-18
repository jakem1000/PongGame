#include <iostream>
#include <raylib.h>


int player_score = 0;
int cpu_score = 0;


// Class: Represents ball object
class Ball {
public:
    float x, y; // Ball x/y coordinates
    int speed_x, speed_y; // Ball x/y speeds
    int radius; // Ball radius

    // Method: Draw a white circle at x/y coordinates, using radius value
    void Draw() {
        DrawCircle(x, y, radius, WHITE);
    }

    // Method: Updates the ball object
    void Update() {

        // Moves ball based on speed values (effectively functioning as direction inputs)
        x += speed_x;
        y += speed_y;

        // Edge Collision Detection:
        if (y + radius >= GetScreenHeight() || y - radius <= 0) { // If: ball is touching top/bottom edge
            speed_y *= -1; // Reverse Y direction
        }
        if (x + radius >= GetScreenWidth()) { // If ball is touching right edge:
            cpu_score++; // Give CPU 1 point
            ResetBall();
        }


        if (x - radius <= 0) { // If: ball is touching left edge:
            player_score++;
            ResetBall();
        }
    }

    // Resets the ball to center when someone scores
    void ResetBall() {
        x = GetScreenWidth() / 2; // Get x/y coordinates in center of window
        y = GetScreenHeight() / 2;

        int speed_choices[2] = { -1, 1 };
        speed_x *= speed_choices[GetRandomValue(0, 1)]; // assign random direction to ball X/Y
        speed_y *= speed_choices[GetRandomValue(0, 1)];
    }



};


// Class: represents paddle objects
class Paddle {
protected:

    void LimitMovement() {
        if (y <= 0) { // If paddle past top of screen:
            y = 0; // Limit paddle to top of screen
        }

        if (y + height >= GetScreenHeight()) { // If paddle past bottom of screen:
            y = GetScreenHeight() - height; // Limit paddle to bottom of screen
        }
    }

public:

    float x, y; // x/y position of paddle
    float width, height; // width/height of paddle rectangle
    int speed; // speed that paddle moves at

    // Method: draws paddle object
    void Draw() {
        DrawRectangle(x, y, width, height, WHITE);
    }

    // Method: updates paddle object every frame
    void Update() {
        // Move paddle based on user input
        if (IsKeyDown(KEY_UP)) {
            y = y - speed;  // Subtract speed because Up is -1Y direction
        }
        if (IsKeyDown(KEY_DOWN)) {
            y = y + speed;  // Add speed because Down is +1Y direction
        }

        LimitMovement(); // Limit paddle movement between top and bottom edges of window
    }
};

// Class: represents CPU paddle, controlled by AI
class CpuPaddle : public Paddle {
public:

    // Method: Overriding Update method of paddle
    void Update(int ball_y) {
        if (y + height / 2 > ball_y) { // If center of CPU paddle is below ball:
            y = y - speed; // Move CPU up
        }
        if (y + height / 2 <= ball_y) { // If center of CPU paddle is above ball:
            y = y + speed; // Move CPU down
        }

        LimitMovement();
    }


};


// Instantiate Objects

Ball ball; // Declare ball object
Paddle player; // Declare player paddle object
CpuPaddle cpu; // Declare CPU paddle object


int main() {


    std::cout << "Starting the game..." << std::endl; // Print start message
    const int screen_width = 1280;  // Set window width/height
    const int screen_height = 800;
    InitWindow(screen_width, screen_height, "Jake's Pong Game"); // Initialize window
    SetTargetFPS(60); // Set max FPS to 60


    // Initialize ball:
    ball.radius = 20;
    ball.x = screen_width / 2;
    ball.y = screen_height / 2;
    ball.speed_x = 7;
    ball.speed_y = 7;

    // Initialize player paddle:
    player.width = 25; // Paddle dimensions: 25x120
    player.height = 120;
    player.x = screen_width - player.width - 10; // Draw on right side of screen
    player.y = screen_height / 2 - player.height / 2;
    player.speed = 6;

    // Initialize CPU paddle:
    cpu.height = 120; // Paddle dimensions: 25x120
    cpu.width = 25;
    cpu.x = 10;
    cpu.y = screen_height / 2 - cpu.height / 2;
    cpu.speed = 5;




    while (WindowShouldClose() == false) { // If Window is not trying to be closed:

        // Draw window is a single Update frame: Begins with 'BeginDrawing', ends with 'EndDrawing'
        BeginDrawing(); // Starts drawing window


        // Updating
        ball.Update(); // Update ball position
        player.Update(); // Update player paddle position
        cpu.Update(ball.y); // Update CPU paddle based on current ball y coordinate


        // Checking for collisions:
        // 
        // If ball is touching Player paddle:
        if (CheckCollisionCircleRec(Vector2{ ball.x, ball.y }, ball.radius, Rectangle{ player.x, player.y, player.width, player.height })) {
            ball.speed_x *= -1; // Reverse ball X movement
        }
        // If ball is touching CPU paddle:
        if (CheckCollisionCircleRec(Vector2{ ball.x, ball.y }, ball.radius, Rectangle{ cpu.x, cpu.y, cpu.width, cpu.height })) {
            ball.speed_x *= -1; // Reverse ball X movement
        }


        // Drawing
        ClearBackground(BLACK); // Fill entire window with black
        DrawLine(screen_width / 2, 0, screen_width / 2, screen_height, WHITE); // Draw line separating player sides
        ball.Draw(); // Draw ball

        // Draw paddles:
        cpu.Draw();
        player.Draw();
        DrawText(TextFormat("%i", cpu_score), screen_width / 4 - 20, 20, 80, WHITE);
        DrawText(TextFormat("%i", player_score), 3 * screen_width / 4 - 20, 20, 80, WHITE);

        EndDrawing(); // Ends drawing window
    }



    CloseWindow(); // Close game window
    return 0;
}

