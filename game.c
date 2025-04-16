#include <stdio.h>
#include <math.h>
#include <raylib.h>

#define WIN_W 800
#define WIN_H 600

#define MIN(X,Y) ((X) < (Y) ? (X) : (Y))
#define MAX(X,Y) ((X) < (Y) ? (Y) : (X))

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

void draw_tri(Vector2 w1, Vector2 w2, Vector2 w3) {
    Vector2 p1 = worldToScreen(w1);
    Vector2 p2 = worldToScreen(w2);
    Vector2 p3 = worldToScreen(w3);
    DrawTriangle(p1, p2, p3, GREEN);
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


Vector2 rot(Vector2 v, float angle) {
    Vector2 u;
    u.x = v.x * cos(angle) - v.y * sin(angle);
    u.y = v.x * sin(angle) + v.y * cos(angle);
    return u;
}

Vector2 add(Vector2 a, Vector2 b) {
    Vector2 c = {a.x + b.x, a.y + b.y};
    return c;
}


float tri_angle = 0;
float tri_spin = 0.005;
Vector2 tri_base = {150,150};

void triangulate(Vector2 *p1, Vector2 *p2, Vector2 *p3) {
    float a = 10 * tan(M_PI / 6);
    float c = 10 * tan(M_PI / 3) - a;
    Vector2 base = tri_base;
    Vector2 t1 = {-10,-a};
    Vector2 t2 = {0,c};
    Vector2 t3 = {10,-a};
    float angle = tri_angle;
    *p1 = add(base, rot(t1, angle));
    *p2 = add(base, rot(t2, angle));
    *p3 = add(base, rot(t3, angle));
}

void tri_render() {
    Vector2 p1, p2, p3;
    triangulate(&p1, &p2, &p3);
    draw_tri(p1, p2, p3);
}

void tri_advance() {
    tri_angle += tri_spin;
}

int tri_hit(Vector2 w) {
    Vector2 p1, p2, p3;
    triangulate(&p1, &p2, &p3);
    float xmin = MIN(p1.x, MIN(p2.x, p3.x));
    float xmax = MAX(p1.x, MAX(p2.x, p3.x));
    float ymin = MIN(p1.y, MIN(p2.y, p3.y));
    float ymax = MAX(p1.y, MAX(p2.y, p3.y));
    if(w.x < xmin || xmax < w.x) return 0;
    if(w.y < ymin || ymax < w.y) return 0;
    return 1;
}



int main(int argc, char *argv[]) {

    InitWindow(WIN_W, WIN_H, "GAME3");
    SetTargetFPS(60);

    InitAudioDevice();

    Vector2 click = {0,0};
    Sound sound = LoadSound("sound.wav");

    for (;;) {
        if (WindowShouldClose()) break;

        if (IsMouseButtonPressed(0)) {
            click = screenToWorld(GetMousePosition());
            if(tri_hit(click)){
                PlaySound(sound);
            }
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

        // render all renderable processes
        tri_render();

        DrawText(TextFormat("click = {%f,%f}", click.x, click.y), 1, WIN_H - 60, 20, WHITE);
        DrawText(TextFormat("camera = {%f,%f}", camera.x, camera.y), 1, WIN_H - 40, 20, WHITE);
        DrawText(TextFormat("zoom = %f", zoom), 1, WIN_H - 20, 20, WHITE);

        EndDrawing();

        // advance processes in time
        tri_advance();
    }

    CloseWindow();

    return 0;

}
