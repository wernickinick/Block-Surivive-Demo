#include "raylib.h"
#include <iostream>
#include <vector>
using namespace std;
int main() {

    ChangeDirectory(GetApplicationDirectory());
    const int screenWidth = 900;
    const int screenHeight = 600;
    const int fps = 60;
    const float playerSize = 50.0f;
    float gravity = 0.6f;
    bool useGravity = true;
    int framesCounter = 0;
    float velocityY = 0.0f;
    bool grounded = true;
    float groundY = 495.0f;
    float rotation = 0.0f;
    int jumpDuration = 0;
    int jumpFrame = 0;
    bool levelComplete = false;
    bool startScreen = true;
    vector<float> spikeX = {900.0f, 1300.0f, 1700.0f, 
                            2200.0f, 2450.0f, 2800.0f, 
                            2850.0f, 3050.0f,3400.0f,3450.0f, 
                            3800.0f, 4100.0f, 4400.0f, 4600.0f,
                            4900.0f, 5150.0f, 5500.0f, 5550.0f,
                            5800.0f, 5850.0f, 6100.0f, 6500.0f,
                            7050.0f,7100.0f};
    vector<float> boxX = {950.0f,3350.0f, 6700.0f,6750.0f,6800.0f,6850.0f,6900.0f,
                          6950.0f,7000.0f};
    vector<float> endX = {7300.0f};
    float spikeScale = 0.07f;
    float boxScale = 0.17f;
    float endpointScale = 1.0f;
    float floorTopY = 470.0f;
    bool gameover = false;
    float hitboxPadding = 15.0f;
    SetExitKey(KEY_NULL);
    bool exitWindowRequested = false;
    bool exitWindow = false;
    InitWindow(screenWidth, screenHeight, "Block Survive");

    Vector2 playerPosition = {300.0f, 400.0f };

    SetTargetFPS(fps);

    Texture2D floor = LoadTexture("assets/floor.png");
    Texture2D player = LoadTexture("assets/PlayerSkin.png");
    Texture2D background = LoadTexture("assets/background/background.png");
    Texture2D midground = LoadTexture("assets/background/midground.png");
    Texture2D foreground = LoadTexture("assets/background/foreground.png");
    Texture2D spike = LoadTexture("assets/spike.png");
    Texture2D box = LoadTexture("assets/box.png");
    Texture2D endpoint = LoadTexture("assets/endpoint.png");
        
    float scrollingBack = 0.0f;
    float scrollingMid = 0.0f;
    float scrollingFore = 0.0f;
    float scrollingSpike = 0.0f;
        while (!exitWindow) {
            if(WindowShouldClose()) exitWindowRequested = true;
            if(IsKeyPressed(KEY_P)) exitWindowRequested = !exitWindowRequested;

            if(exitWindowRequested){
            if(IsKeyPressed(KEY_Y)) exitWindow = true;
        }
        if(!exitWindowRequested){
        scrollingBack -= 2.1f;
        scrollingMid -= 3.0f;
        scrollingFore -= 5.0f;
        for (auto &x : spikeX)
        {
            x-=6.0f;
        }
        for (auto &x : boxX){
            x-=6.0f;
        }
        for (auto &x : endX){
            x-=6.0f;
        }       
        if(scrollingBack <= -background.width*2){
            scrollingBack = 0;
        }
        if(scrollingMid <= -midground.width*2){
            scrollingMid = 0;
        }
        if(scrollingFore <= -foreground.width*2){
            scrollingFore = 0;
        }
        }
        BeginDrawing();
        DrawTextureEx(background, Vector2{ scrollingBack, 0 }, 0.0f, 2.0f, WHITE);
        DrawTextureEx(background, Vector2{ background.width*2 + scrollingBack, 0 }, 0.0f, 2.0f, WHITE);

        // Draw midground image twice
        DrawTextureEx(midground, Vector2{ scrollingMid, 0 }, 0.0f, 2.0f, WHITE);
        DrawTextureEx(midground, Vector2{ midground.width*2 + scrollingMid, 0 }, 0.0f, 2.0f, WHITE);

        // Draw foreground image twice
        DrawTextureEx(foreground, Vector2{ scrollingFore, 0 }, 0.0f, 2.0f, WHITE);
        DrawTextureEx(foreground, Vector2{ foreground.width*2 + scrollingFore, 0 }, 0.0f, 2.0f, WHITE);

        if(exitWindowRequested){
            DrawRectangle(150,100,600,200,RED);
            DrawText("Are you sure you want to quit?",200,150,30,WHITE);
            DrawText("[Y for YES]",200,200,30,WHITE);
            DrawText("[P to RESUME]",200,250,30,WHITE);
        }

        float spikeY = floorTopY - (spike.height *spikeScale);
        float spikeW = spike.width * spikeScale;
        float spikeH = spike.height * spikeScale;
        float boxY = floorTopY - (spike.height *spikeScale);
        float boxW = box.width * boxScale;
        float boxH = spike.height * boxScale;
        float endY = floorTopY - (endpoint.height *endpointScale);
        float endW = endpoint.width * endpointScale;
        float endH = endpoint.height * endpointScale;
        float playerLeft = playerPosition.x - playerSize / 2.0f + hitboxPadding;
        float playerRight = playerPosition.y - playerSize / 2.0f + hitboxPadding;
        float playerHitsize = playerSize - hitboxPadding * 2;
        
        for(float x : spikeX){
            if(playerLeft < x + spikeW && playerLeft 
                    + playerHitsize > x && playerRight < spikeY
                    + spikeH && playerRight + playerHitsize > spikeY)
            {
                gameover = true;
            }
        }

        for (float x : spikeX)
        {
            DrawTextureEx(spike, Vector2{x, spikeY}, 0.0f, spikeScale, WHITE);
        }

                for (float x : boxX){
            DrawTextureEx(box, Vector2{x, boxY}, 0.0f, boxScale, WHITE);
        }

        for(float x: endX){
            if (playerLeft < x + endW && playerLeft + playerHitsize > x && playerRight < endY + endH && playerRight + playerHitsize > endY)
            {
                levelComplete = true; //will need to change this to end screen instead of restart end
            }
        }
        for (float x : endX){
            DrawTextureEx(endpoint, Vector2{x, endY}, 0.0f, endpointScale, WHITE);
        }
        floor.height = 470;
        floor.width = 900;

        if(!exitWindowRequested){
            if(IsKeyDown(KEY_SPACE) && grounded == true) {
                velocityY = -12.0f;
                grounded = false;
                jumpFrame = 0;
                jumpDuration = (int)(2.0f * 12.0f / gravity);
                }
        else if(useGravity || playerPosition.y > 0.2f){
            velocityY += gravity;
        }

        if (!grounded) {
            jumpFrame++;
            rotation = ((float)jumpFrame / (float)jumpDuration) * 180.0f;
        if (rotation > 180.0f) rotation = 180.0f;
        } else {
            rotation = 0.0f;
        }

        playerPosition.y += velocityY / 2;

        float landY = groundY - playerSize;   // where the player stands on the ground

        for (float x : boxX) {
            bool overOrInBox = playerLeft < x + boxW && playerLeft + playerHitsize > x;
                if (!overOrInBox) continue;

            float feet = playerPosition.y + playerSize / 2.0f;

            if (feet <= boxY + 20.0f) {
            landY = boxY - playerSize / 2.0f;   // above the box, so its top is the floor
            } else {
                 gameover = true;                    // hit the side
             }
        }

    if (playerPosition.y >= landY) {
        playerPosition.y = landY;
        velocityY = 0.0f;
        grounded = true;
    } else {
        grounded = false;                       // in the air or walked off the box
    }

    }

    Rectangle sourceRec{ 0.0f, 0.0f, (float)player.width, (float)player.height };
    Rectangle destRec{ playerPosition.x, playerPosition.y, playerSize, playerSize };
    Vector2 origin{ playerSize/2.0f, playerSize/2.0f };
    DrawTexturePro(player, sourceRec, destRec, origin, (float)rotation, WHITE);
    DrawTexture(floor, 0, 470, WHITE);      
        if (playerPosition.y + playerSize > groundY){
            playerPosition.y = groundY - playerSize;
            velocityY = 0.0f;
            grounded = true;

        }

        if(levelComplete){
            DrawRectangle (0,0,900,600,GREEN);
            DrawText("You Beat The First Level!", 250,150,30,WHITE);
            DrawText("Press [SPACE] To Play Again!", 220, 200, 30, WHITE);
            DrawText("Press [Q] To Quit!", 280,250,30,WHITE);

            if(IsKeyPressed(KEY_SPACE)){
                levelComplete = false;
                gameover = true;
            }
            if(IsKeyPressed(KEY_Q)){
                exitWindow = true;
            }

        }
        if(startScreen){
            spikeX = {};
            boxX = {};
            endX = {};
            DrawText("Press [SPACE] TO PLAY!", 150,150,50,WHITE);
            DrawText("Press [ESC] TO PAUSE!", 180,220,50,WHITE);
            if(IsKeyPressed(KEY_SPACE)){
                startScreen = false;
                gameover = true;
                levelComplete = false;
            }
        }
            
       

 
//RESTART GAME FUNCTION
        if (gameover)
        {
            playerPosition.x = 300.0f;
            playerPosition.y = 400.0f;
            velocityY = 0.0f;
            grounded = true;
            rotation = 0.0f;

            spikeX = {900.0f, 1300.0f, 1700.0f, 
                      2200.0f, 2450.0f, 2800.0f, 
                      2850.0f, 3050.0f,3400.0f,3450.0f, 
                      3800.0f, 4100.0f, 4400.0f, 4600.0f,
                      4900.0f, 5150.0f, 5500.0f, 5550.0f,
                      5800.0f, 5850.0f, 6100.0f, 6500.0f,
                      7050.0f, 7100.0f};
            boxX = {950.0f,3350.0f, 6700.0f,6750.0f,6800.0f,6850.0f,6900.0f,
                    6950.0f,7000.0f};
            endX = {7300.0f};


            scrollingBack = 0.0f;
            scrollingMid = 0.0f;
            scrollingFore = 0.0f;
            DrawTexture(floor, 0,470,WHITE); 
            gameover = false;
        }


        framesCounter++;
        EndDrawing();
       
    }
//RESTART GAME FUNCTION END

    UnloadTexture(player);
    UnloadTexture(floor);
    UnloadTexture(background);
    UnloadTexture(midground);
    UnloadTexture(foreground);
    UnloadTexture(box);
    UnloadTexture(spike);
    UnloadTexture(endpoint);

    CloseWindow();
    return 0;
}

