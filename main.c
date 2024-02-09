#include <raylib.h>
#include <stdint.h>

#define screenWidth 1440
#define screenHeight 900
#define cellSize 20
#define numCellsX (screenWidth/cellSize)
#define numCellsY (screenHeight/cellSize)

typedef struct {
    uint32_t x;
    uint32_t y;
} Position;

typedef struct {
    Position position;
    Color color;
} Cell;

static Cell cells[numCellsY][numCellsX];

void init() {
    SetTargetFPS(61);               

    // Fill the grid with cells
    uint8_t colorToggle = 0;
    for(uint32_t i = 0; i < (numCellsY); i++) {
        for(uint32_t j = 0; j < (numCellsX); j++) {    
            cells[i][j].color = colorToggle ? BLACK : RAYWHITE;
            colorToggle = !colorToggle;
        }
        colorToggle = !colorToggle;
    }

    InitWindow(screenWidth, screenHeight, "Game of Life");
}

int main (int argc, char* argvp[]) {
    init();

    int shouldWindowClose = WindowShouldClose();
    while (!WindowShouldClose())   {

        BeginDrawing();
        
            //find the length of cells array
            uint32_t cellsArrayLength = sizeof(cells) / sizeof(cells[0]);
            uint32_t numRows = sizeof(cells);
    
            //Draw the grid of cells
            for(uint32_t i = 0; i < (numCellsY); i++) {
                for(uint32_t j = 0; j < (numCellsX); j++) {
                    DrawRectangle(j * cellSize, i * cellSize, cellSize, cellSize, cells[i][j].color);
                }
            }

        EndDrawing();
    }

    CloseWindow();        // Close window and OpenGL context

    return 0;
}