#include <raylib.h>
#include <stdint.h>

#define screenWidth 1440
#define screenHeight 900
#define cellSize 20
#define numCellsX (screenWidth/cellSize)
#define numCellsY (screenHeight/cellSize)

typedef struct {
    Color color;
    uint8_t isAlive;
} Cell;

static Cell globalMainBuffer[numCellsY][numCellsX];
static Cell globalBackBuffer[numCellsY][numCellsX];
static uint8_t isGamePaying = 0;
static uint8_t targetFPS = 10;

void init() {
    SetTargetFPS(targetFPS);               

    // Fill the grid with cells
    for(uint32_t i = 0; i < (numCellsY); i++) {
        for(uint32_t j = 0; j < (numCellsX); j++) {    
            globalBackBuffer[i][j].color = BLACK;
            globalBackBuffer[i][j].isAlive = 0;
        }
    }

    InitWindow(screenWidth, screenHeight, "Game of Life");
    SetTextLineSpacing(40);

}

void DrawGameOfLife() {
    BeginDrawing();        
        //Draw the grid of cells
        for(uint32_t i = 0; i < (numCellsY); i++) {
            for(uint32_t j = 0; j < (numCellsX); j++) {
                Color color = globalMainBuffer[i][j].isAlive ? WHITE : BLACK;
                DrawRectangle(j * cellSize, i * cellSize, cellSize, cellSize, color);
            }
        }
        
        DrawText("Click on the cells to make them alive! \n Press SPACE to start/stop the game \n 'up' or 'down' to adjust the speed", 
            screenWidth/2 -200, screenHeight -200, 20, LIGHTGRAY);

    EndDrawing();
}

void HandleMouseEvents() {
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        Vector2 mousePos = GetMousePosition();
        uint32_t cellX = mousePos.x / cellSize;
        uint32_t cellY = mousePos.y / cellSize;
        globalMainBuffer[cellY][cellX].isAlive = !globalMainBuffer[cellY][cellX].isAlive;
    }
}

void HandleKeyboardEvents() {
    if (IsKeyPressed(KEY_SPACE)) {
        isGamePaying = !isGamePaying;
    }

    //Increase the speed of the game
    if (IsKeyPressed(KEY_UP)) {

        if(targetFPS >= 60) {
            return;
        }

        targetFPS += 5;
        SetTargetFPS(targetFPS);
    }

    //Decrease the speed of the game
    if (IsKeyPressed(KEY_DOWN)) {

        if(targetFPS <= 5) {
            return;
        }

        targetFPS -= 5;
        SetTargetFPS(targetFPS);
    }
}

uint8_t CheckAliveNeighbours(uint32_t x, uint32_t y) {
    uint8_t numAlive = 0;

    for(int i = -1; i < 2; i++) {
        for(int j = -1; j < 2; j++) {

            if(i == 0 && j == 0) {
                continue;
            }

            if(x + j < 0 || x + j >= numCellsX || y + i < 0 || y + i >= numCellsY) {
                continue;
            }

            if(globalMainBuffer[y + i][x + j].isAlive) {
                numAlive++;
            }
        }
    }

    return numAlive;
}

void UpdateGameOfLife() {
    for(uint32_t i = 0; i < numCellsY; i++) {
        for(uint32_t j = 0; j < numCellsX; j++) {
            uint8_t numAliveNeighbours = CheckAliveNeighbours(j, i);

            //1 Any live cell with fewer than two live neighbours dies, as if by underpopulation.
            if(globalMainBuffer[i][j].isAlive && numAliveNeighbours < 2) {
                globalBackBuffer[i][j].isAlive = 0;
            }

            //2 Any live cell with two or three live neighbours lives on to the next generation.
            if(globalMainBuffer[i][j].isAlive && (numAliveNeighbours == 2 || numAliveNeighbours == 3)) {
                globalBackBuffer[i][j].isAlive = 1;
            }

            //3 Any live cell with more than three live neighbours dies, as if by overpopulation.
            if(globalMainBuffer[i][j].isAlive && numAliveNeighbours > 3) {
                globalBackBuffer[i][j].isAlive = 0;
            }

            //4 Any dead cell with exactly three live neighbours becomes a live cell, as if by reproduction.
            if(!globalMainBuffer[i][j].isAlive && numAliveNeighbours == 3) {
                globalBackBuffer[i][j].isAlive = 1;
            }
        }
    }

    //Copy the back buffer to the main buffer
    for(uint32_t i = 0; i < numCellsY; i++) {
        for(uint32_t j = 0; j < numCellsX; j++) {
            globalMainBuffer[i][j].isAlive = globalBackBuffer[i][j].isAlive;
        }
    }
}

int main (int argc, char* argvp[]) {
    init();
    
    while (!WindowShouldClose())   {
        HandleMouseEvents();
        HandleKeyboardEvents();
        
        if(isGamePaying) {
            UpdateGameOfLife();
        }

        DrawGameOfLife();
    }

    CloseWindow();        // Close window and OpenGL context

    return 0;
}