#include <3ds.h>
#include <citro2d.h>
#include <stdlib.h>

#define SCREEN_WIDTH 400
#define SCREEN_HEIGHT 240

int main(int argc, char* argv[]) {
    gfxInitDefault();
    C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);
    C2D_Init(C2D_DEFAULT_MAX_OBJECTS);
    C2D_Prepare();

    C3D_RenderTarget* top = C2D_CreateScreenTarget(GFX_TOP, GFX_LEFT);

    float playerY = 180.0f;
    float playerVY = 0.0f;
    bool isGrounded = true;
    float obstacleX = 400.0f;
    int score = 0;

    while (aptMainLoop()) {
        hidScanInput();
        u32 kDown = hidKeysDown();

        if (kDown & KEY_START) break;

        // Jump Controls
        if ((kDown & KEY_A) && isGrounded) {
            playerVY = -8.0f;
            isGrounded = false;
        }

        // Physics
        playerY += playerVY;
        playerVY += 0.4f; // Gravity

        if (playerY >= 180.0f) {
            playerY = 180.0f;
            playerVY = 0.0f;
            isGrounded = true;
        }

        // Move Obstacle
        obstacleX -= 3.0f;
        if (obstacleX < -20.0f) {
            obstacleX = 400.0f;
            score += 1;
        }

        // Collision Check
        if (obstacleX < 70.0f && obstacleX > 30.0f && playerY > 160.0f) {
            score = 0; // Reset score on hit
            obstacleX = 400.0f;
        }

        // Render Frame
        C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
        C2D_TargetClear(top, C2D_Color32(20, 20, 30, 255));
        C2D_SceneBegin(top);

        // Ground
        C2D_DrawRectangle(0, 200, 0, SCREEN_WIDTH, 40, C2D_Color32(100, 100, 100, 255), C2D_Color32(100, 100, 100, 255), C2D_Color32(100, 100, 100, 255), C2D_Color32(100, 100, 100, 255));
        
        // Player (Red Square)
        C2D_DrawRectangle(50, playerY, 0, 20, 20, C2D_Color32(230, 40, 40, 255), C2D_Color32(230, 40, 40, 255), C2D_Color32(230, 40, 40, 255), C2D_Color32(230, 40, 40, 255));

        // Obstacle (Blue Box)
        C2D_DrawRectangle(obstacleX, 180, 0, 20, 20, C2D_Color32(40, 140, 230, 255), C2D_Color32(40, 140, 230, 255), C2D_Color32(40, 140, 230, 255), C2D_Color32(40, 140, 230, 255));

        C3D_FrameEnd(0);
    }

    C2D_Fini();
    C3D_Fini();
    gfxExit();
    return 0;
}
