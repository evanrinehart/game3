#include <stdio.h>
#include <raylib.h>

#define WIN_W 800
#define WIN_H 600

Vector2 camera = {0,0};
float zoom = 1.0;

Vector2 worldToScreen(Vector2 w) {
    Vector2 p;
    p.x = zoom*(w.x - camera.x) + WIN_W/2;
    p.y = zoom*(w.y - camera.y) + WIN_H/2;
    return p;
}

Vector2 screenToWorld(Vector2 p) {

    Vector2 w;
    w.x = (p.x - WIN_W/2) / zoom + camera.x;
    w.y = (p.y - WIN_H/2) / zoom + camera.y;
    return w;
    
}

void draw_dot(Vector2 w) {
    Vector2 p = worldToScreen(w);
    DrawCircle(p.x, p.y, 10 * zoom, RED);
}

void draw_block(Vector2 w, float wsize) {
    Vector2 p = worldToScreen(w);
    float psize = zoom * wsize;
    DrawRectangle(p.x, p.y, psize, psize, BLUE);
}

void draw_grid() {
    Color gray = {30, 30, 30, 255};
    for(int row = -10; row <= 10; row++) {
        Vector2 w1 = {-500, 50*row};
        Vector2 w2 = {500, 50*row};
        Vector2 p1 = worldToScreen(w1);
        Vector2 p2 = worldToScreen(w2);
        DrawLineV(p1, p2, gray);
    }
    for(int col = -10; col <= 10; col++) {
        Vector2 w1 = {50*col, -500};
        Vector2 w2 = {50*col, 500};
        Vector2 p1 = worldToScreen(w1);
        Vector2 p2 = worldToScreen(w2);
        DrawLineV(p1, p2, gray);
    }
}

// game will consist of several concurrent processes
// processes continue in time until they read from a channel
// processes continue independently unless they interact

int main(int argc, char *argv[]) {

    InitWindow(WIN_W, WIN_H, "GAME3");
    SetTargetFPS(60);

    Vector2 click = {0,0};

    for (;;) {
        if (WindowShouldClose()) break;

        if (IsMouseButtonPressed(0)) {
            click = screenToWorld(GetMousePosition());
        }

        if (IsMouseButtonDown(1)) {
            Vector2 motion = GetMouseDelta();
            camera.x -= motion.x / zoom;
            camera.y -= motion.y / zoom;
        }

        float wheel = GetMouseWheelMove();
        if (wheel < 0) zoom /= 2;
        if (wheel > 0) zoom *= 2;
        if (IsKeyDown(KEY_A)) camera.x -= 5/zoom;
        if (IsKeyDown(KEY_W)) camera.y -= 5/zoom;
        if (IsKeyDown(KEY_D)) camera.x += 5/zoom;
        if (IsKeyDown(KEY_S)) camera.y += 5/zoom;

        BeginDrawing();
        ClearBackground(BLACK);

        draw_grid();
        draw_block((Vector2){-50, -50}, 50);
        draw_dot((Vector2){0,0});
        draw_dot((Vector2){50,0});
        draw_dot((Vector2){0,50});
        draw_dot((Vector2){100,0});
        draw_dot((Vector2){0,100});

        DrawText(TextFormat("click = {%f, %f}", click.x, click.y), 1, WIN_H - 60, 20, WHITE);
        DrawText(TextFormat("camera = {%f,%f}", camera.x, camera.y), 1, WIN_H - 40, 20, WHITE);
        DrawText(TextFormat("zoom = %f", zoom), 1, WIN_H - 20, 20, WHITE);

        EndDrawing();
    }

    CloseWindow();

    return 0;

}
