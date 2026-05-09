#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <raylib.h>

#define FPS 30
#define DT 1/FPS
#define HEIGHT 900
#define WIDTH 1600
#define SQUARE_SIZE (WIDTH/100)
#define BOARD_WIDTH  (WIDTH / SQUARE_SIZE + 1)
#define BOARD_HEIGHT  (HEIGHT / SQUARE_SIZE + 1)
#define NEIGHBORS_SIZE 8

typedef enum CellFate {
    STAYS_AS_IS,
    DIES,
    GROWS,
} CellFate;

int get_index(int x, int y){
    return BOARD_HEIGHT * x + y ;    
}

void initialize_board(bool board[]){
    for (int i=0; i<BOARD_WIDTH; i++){
        for (int j=0; j<BOARD_HEIGHT; j++) {
            //if (i < 2 * BOARD_HEIGHT / 3 && i > BOARD_HEIGHT / 3 && j < 2 * BOARD_WIDTH / 3 && j > BOARD_WIDTH / 3) {
            //if (abs(i - WIDTH/2) < 3 || abs(j - HEIGHT) < 3){
            if (false){
                board[get_index(i, j)] = true;
            }
            else {
                board[get_index(i, j)] = false;
            }
        }
    }
}

void print_board(bool board[]) {
    for (int i=0; i<BOARD_WIDTH; i++){
        for (int j=0; j<BOARD_HEIGHT; j++) {
            printf("%d ", board[get_index(i, j)]);
        }
        printf("\n");
    }
}

void draw_board(bool board[]){
    for (int i=0; i<BOARD_WIDTH; i++){
        for (int j=0; j<BOARD_HEIGHT; j++) {
            int x = i * SQUARE_SIZE;
            int y = j * SQUARE_SIZE;
            //printf("(%i, %i):(%i, %i) board=%i, underx=%i, undery=%i\n", j, i, x, y, board[get_index(i, j)] , x < WIDTH , y < HEIGHT);
            if (board[get_index(i, j)] && x < WIDTH && y < HEIGHT){
                DrawRectangle(x, y, SQUARE_SIZE - 1, SQUARE_SIZE - 1, WHITE);
            }
            else {
                //printf("failed for (%d, %d), %d %d %d\n", i, j, board[get_index(i, j)] , x < WIDTH , y < HEIGHT);
            }
        }
    }
}

void get_neighbors(int neighbors[], int size, int x, int y) {
    int coords[8][2] = {
                    {x - 1, y - 1},
                    {x, y - 1 },
                    {x + 1, y - 1},
                    {x - 1, y},
                    {x + 1, y},
                    {x - 1, y + 1},
                    {x, y + 1},
                    {x + 1, y + 1},
                        };
    for (int idx=0; idx < size; idx++){
        if (coords[idx][0] >= 0 && coords[idx][1] >= 0 && coords[idx][0] < WIDTH && coords[idx][1] < HEIGHT) {
            neighbors[idx] = get_index(coords[idx][0], coords[idx][1]);
        }
        else {
            neighbors[idx] = -1;
        }
    } 
}

int count_live_neighbors(int neighbors[], int size, bool board[]){
    int count = 0;
    for (int i=0; i < size; i++) {
        if (neighbors[i] >= 0){
            count += board[neighbors[i]];
        }
    }
    return count;
}



void print_array(int array[], int size){
    printf("HOLA\n");
    for (int i=0; i < size; i++){
        printf("%d ", array[i]);
    }
    printf("\n");
}

CellFate process_cell_fate(bool cell_is_alive, int neighbors_alive){
    CellFate fate;
    if (cell_is_alive) {
        if (neighbors_alive == 2 || neighbors_alive == 3) {
            fate = STAYS_AS_IS;
        }
        else {
            fate = DIES;
        }
    }
    else {
        if (neighbors_alive == 3) {
            fate = GROWS;
        }
        else {
            fate = STAYS_AS_IS;
        }
    }
    return fate;
}

int process_board(bool board[], bool board_copy[]) {
    int neighbors[NEIGHBORS_SIZE];
    CellFate cell_fate;
    int index;
    int alive_cells = 0;
    for (int i=0; i < BOARD_WIDTH; i++) {
        for (int j=0; j < BOARD_HEIGHT; j++) {
            index = get_index(i, j);
            get_neighbors(neighbors, NEIGHBORS_SIZE, i, j);
            int neighbors_alive = count_live_neighbors(neighbors, NEIGHBORS_SIZE, board_copy);
            cell_fate = process_cell_fate(board[index], neighbors_alive);
            alive_cells += board[index];
            
            switch (cell_fate)
            {
            case DIES:
                board[index] = false;
                break;
            case GROWS:
                board[index] = true;
            default:
                break;
            }
            
        }
    }
    return alive_cells;
}


int get_index_from_pixels(int x, int y){
    int i = x / SQUARE_SIZE;
    int j = y / SQUARE_SIZE;
    return get_index(i, j);
}


void main(){

	// Tell the window to use vsync and work on high DPI displays
	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);

	// Create the window and OpenGL context
	InitWindow(WIDTH, HEIGHT, "Conway's GOL");
    
    SetTargetFPS(FPS);

    printf("SQUARE_SIZE %d\n", SQUARE_SIZE);
    printf("HEIGHT %d\n", HEIGHT);
    printf("WIDTH %d\n", WIDTH);
    printf("BOARD_WIDTH %d\n", BOARD_WIDTH);
    printf("BOARD_HEIGHT %d\n", BOARD_HEIGHT);
    
    bool board[BOARD_WIDTH * BOARD_HEIGHT];
    bool board_copy[BOARD_WIDTH * BOARD_HEIGHT];
    initialize_board(board);
    memcpy(board_copy, board, sizeof(board));
    // print_board(board);
    
    int neighbors[NEIGHBORS_SIZE] = {0};

    //printf("SIZEOF NEIGHBORS %d\n", NEIGHBORS_SIZE);
    //print_array(neighbors, NEIGHBORS_SIZE);
    //get_neighbors(neighbors, NEIGHBORS_SIZE, 0, 0);
    //print_array(neighbors, NEIGHBORS_SIZE);
    //get_neighbors(neighbors, NEIGHBORS_SIZE, 15, 15);
    //print_array(neighbors, NEIGHBORS_SIZE);
    //get_neighbors(neighbors, NEIGHBORS_SIZE, WIDTH - 1, HEIGHT - 1);
    //print_array(neighbors, NEIGHBORS_SIZE);
    
    bool start_simulation = false;
    bool board_changed = true;

    int generation = 0;
    int live_cells = 0;
    char live_cells_text[100];
    char generation_text[100];
    sprintf(live_cells_text, "CELLS: %d", live_cells);
    sprintf(generation_text, "GENERATION: %d", generation);
	while (!WindowShouldClose())		// run the loop until the user presses ESCAPE or presses the Close button on the window
	{
		// drawing
		BeginDrawing();

		// Setup the back buffer for drawing (clear color and depth buffers)

		// draw some text using the default font
		// DrawText("Hello Raylib", 200,200,20,WHITE);
        if (board_changed) {
		    ClearBackground(BLACK);
            draw_board(board);
            memcpy(board_copy, board, sizeof(board));
            board_changed = false;
        }

        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && !start_simulation){
            int x = GetMouseX();
            int y = GetMouseY();
            int idx = get_index_from_pixels(x, y);
            board[idx] = !IsKeyDown(KEY_LEFT_CONTROL);
            board_changed = true;
        }
        
        sprintf(live_cells_text, "CELLS: %d", live_cells);
        sprintf(generation_text, "GENERATION: %d", generation);

        DrawFPS(10, 10);
        DrawText(live_cells_text, 50, 50, 30, GREEN);
        DrawText(generation_text, 50, 85, 30, GREEN);

		
		// end the frame and get ready for the next one  (display frame, poll input, etc...)
		EndDrawing();
        
        if (start_simulation){
            live_cells = process_board(board, board_copy);
            generation += 1;
            board_changed = true;
        }

        KeyboardKey key_pressed = GetKeyPressed();
        switch (key_pressed)
        {
        case KEY_SPACE:
            printf("space\n");
            start_simulation = true;
            break;
        case KEY_R:
            printf("R\n");
            start_simulation = false;
            initialize_board(board);
            board_changed = true;
        default:
            break;
        }
        
        
        

	}
}

