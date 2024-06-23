#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>
#include <locale.h>
#include <wchar.h>
#include <string.h>
#include "list.h"
#include "main.h"

List* getUnvisitedNeighbours(Cell* cell, MazeMaker* M, int cellrow, int cellcol);
void removewall(Cell* current, Cell* next, int currentrow, int currentcol, int nextrow, int nextcol);
void GenerateMazeDFS(MazeMaker* M);
void GenerateMazePrim(MazeMaker* M);
void GenerateMazeLRU(MazeMaker* M);
void GenerateMazeCombined(MazeMaker* M);


int main(int argc, char** argv) {
    MazeMaker* M = Setup(argc, argv);
    GenerateMaze(M);
    CheckMazeIntegrity(M);
    ClearScreenAndPrintMaze(M);
}

MazeMaker* Setup(int argc, char** argv) {
    setlocale(LC_ALL, "");
    srand(time(NULL));

    if (argc < 3 || argc > 4) {
        printf("Usage: %s <width> <height> [mode]\n", argv[0]);
        printf(" width: The width of the desired maze\n");
        printf(" height: The height of the desired maze\n");
        printf(" mode: Different modes to generate the maze\n");
        printf("       (defaults to depth first search)\n");
        printf("    - depth: uses the depth-first search algorithm");
        printf("    - prim: uses a pseudo-prim's algorithm");
        printf("    - lru: uses the least recently used algorithm");
        printf("    - all: combination of depth, prim, and lru\n");
        exit(0);
    }

    int width = atoi(argv[1]);
    int height = atoi(argv[2]);
    Mode mode = DEPTH;

    if (argc == 4){
        if (strcmp(argv[3], "depth") == 0) mode = DEPTH;
        else if (strcmp (argv[3], "prim") == 0) mode = PRIM;
        else if (strcmp (argv[3], "lru") == 0) mode = LRU;
        else if (strcmp(argv[3], "all") == 0) mode = ALL;
        else {
            printf("Invalid mode specified\n");
        }
    }

    MazeMaker* M = (MazeMaker*)malloc(sizeof(MazeMaker));
    if (M == NULL){
        printf("Failed to allocate memory for Mazemaker. \n");
        exit;
    }

    M->width = width;
    M->height = height;
    M->mode = mode;

    M->maze = (Cell**)malloc(M->height * sizeof(Cell*));
    if (M->maze == NULL){
        printf("Failed to allocate memory for maze rows. \n");
        free(M);
        exit;
    }

    for (int i = 0; i < M->height; i++) {
        M->maze[i] = (Cell*)malloc(M->width * sizeof(Cell));
        if (M->maze[i] == NULL){
            printf("Failed to allocate memory for maze column. \n");
            while (--i >= 0){
                free(M->maze[i]);
            }
            free(M->maze);
            free(M);
            exit;
        }

        for(int j = 0; j < M->width; j++){
            for(int k = 0; k < 4; k++){
                M->maze[i][j].walls[k] = BLOCKED;
            }
            M->maze[i][j].visited = 0;
        }

    }

    M->cells = MakeList();
    if (M->cells == NULL){
        printf("Failed to initializs. \n");
        for (int i = 0; i < M->height; i++){
            free(M->maze[i]);
        }
        free(M->maze);
        free(M);
        exit;
    }

    return M;
}



void GenerateMaze(MazeMaker* M) {
    switch (M->mode) {
        case DEPTH:
        GenerateMazeDFS(M);
        break;

        case PRIM:
        GenerateMazePrim(M);
        break;

        case LRU:
        GenerateMazeLRU(M);
        break;

        case ALL:
        GenerateMazeCombined(M);
        break;

        default:
        printf("Unknown Mode\n");
        exit;
    }
}

void GenerateMazeDFS(MazeMaker* M){

    List* stack = MakeList();
    int startrow = rand() % M->height;
    int startcol = rand() % M->width;
    Cell* startCell = &M->maze[startrow][startcol];
    startCell->visited = 1;
    EnList(stack, startrow, startcol);

    while (stack->count > 0){

        Node* node = Pop(stack);
        int row = node->row;
        int col = node->col;
        free(node);

        List* unvisitedNeighbours = getUnvisitedNeighbours(&M->maze[row][col], M, row, col);
        if (unvisitedNeighbours->count > 0){
            Node* randomNeighbornode = RemoveRandom(unvisitedNeighbours);
            Cell* currentcell = &M->maze[row][col];
            Cell* nextCell = &M->maze[randomNeighbornode->row][randomNeighbornode->col];
            removewall(currentcell, nextCell, row, col, randomNeighbornode->row, randomNeighbornode->col);
            nextCell->visited = 1;
            EnList(stack, row, col);
            EnList(stack, randomNeighbornode->row, randomNeighbornode->col);
            free(randomNeighbornode);
        }

        while (unvisitedNeighbours->count > 0){
            Node* temp = DeList(unvisitedNeighbours);
            free(temp);
        }
        free(unvisitedNeighbours);
    }
    free(stack);

}




void GenerateMazePrim(MazeMaker* M){

    List* list = MakeList();
    int startrow = rand() % M->height;
    int startcol = rand() % M->width;
    Cell* startCell = &M->maze[startrow][startcol];
    startCell->visited = 1;
    EnList(list, startrow, startcol);

    while (list->count > 0){

        Node* randomnode = RemoveRandom(list);
        int row = randomnode->row;
        int col = randomnode->col;
        

        List* unvisitedNeighbours = getUnvisitedNeighbours(&M->maze[row][col], M, row, col);


        if (unvisitedNeighbours->count > 0){
            Node* Neighbornode = RemoveRandom(unvisitedNeighbours);
            Cell* currentcell = &M->maze[row][col];
            Cell* nextCell = &M->maze[Neighbornode->row][Neighbornode->col];
            removewall(currentcell, nextCell, row, col, Neighbornode->row, Neighbornode->col);
            nextCell->visited = 1;
            EnList(list, Neighbornode->row, Neighbornode->col);

            free(Neighbornode);
        }else{

            free(randomnode);
        }

        List* neighborsofnext = getUnvisitedNeighbours(&M->maze[row][col], M, row, col);
        Node* neighbornode;
        while ((neighbornode = DeList(neighborsofnext))){
            List* furtherneighbours = getUnvisitedNeighbours(&M->maze[neighbornode->row][neighbornode->col], M, neighbornode->row, neighbornode->col);
            if (furtherneighbours->count > 0){
                EnList(list, neighbornode->row, neighbornode->col);
            }
        }


        int unvisitedcount = 0;
        for (int i = 0; i < M->height; i++){
            for (int j = 0; j < M->width; j++){
                if (!M->maze[i][j].visited){
                    unvisitedcount++;
                }
            }
        }


        while (unvisitedNeighbours->count > 0){
            Node* temp = DeList(unvisitedNeighbours);
            free(temp);
        }
        free(unvisitedNeighbours);
    }
    free(list);

}


void GenerateMazeLRU(MazeMaker* M){

    List* list = MakeList();
    int startrow = rand() % M->height;
    int startcol = rand() % M->width;
    Cell* startCell = &M->maze[startrow][startcol];
    startCell->visited = 1;
    EnList(list, startrow, startcol);

    while (list->count > 0){

        Node* lrunode = DeList(list);
        int row = lrunode->row;
        int col = lrunode->col;
        free(lrunode);

        List* unvisitedNeighbours = getUnvisitedNeighbours(&M->maze[row][col], M, row, col);

        if(unvisitedNeighbours->count > 0){
            Node* neighbourNode = RemoveRandom(unvisitedNeighbours);
            Cell* currentcell = &M->maze[row][col];
            Cell* nextCell = &M->maze[neighbourNode->row][neighbourNode->col];
            removewall(currentcell, nextCell, row, col, neighbourNode->row, neighbourNode->col);
            nextCell->visited = 1;
            EnList(list, row, col);
            EnList(list, neighbourNode->row, neighbourNode->col);
            free(neighbourNode);
        }

        while (unvisitedNeighbours->count > 0){
            Node* temp = DeList(unvisitedNeighbours);
            free(temp);
        }
        free(unvisitedNeighbours);
    }
    
    free(list);

}

void GenerateMazeCombined(MazeMaker* M){
    List* list = MakeList();
    int startrow = rand() % M->height;
    int startcol = rand() % M->width;
    Cell* startCell = &M->maze[startrow][startcol];
    startCell->visited = 1;
    EnList(list, startrow, startcol);

    while (list->count > 0){
        Node* chosenNode;
        int randomChance = rand() % 100;

        if (randomChance < 60){
            chosenNode = Pop(list);
        } else if (randomChance < 80){
            chosenNode = RemoveRandom(list);
        }else {
            chosenNode = DeList(list);
        }

        int row = chosenNode->row;
        int col = chosenNode->col;

        List* unvisitedNeighbours = getUnvisitedNeighbours(&M->maze[row][col], M, row, col);
        if (unvisitedNeighbours->count > 0){
            Node* neighbourNode = RemoveRandom(unvisitedNeighbours);
            Cell* currentcell = &M->maze[row][col];
            Cell* nextcell = &M->maze[neighbourNode->row][neighbourNode->col];
            removewall(currentcell, nextcell, row, col, neighbourNode->row, neighbourNode->col);
            nextcell->visited = 1;
            EnList(list, neighbourNode->row, neighbourNode->col);
            EnList(list, row, col);
            free(neighbourNode);
        }

        free(chosenNode);

        while (unvisitedNeighbours->count > 0) {
            Node* temp = DeList(unvisitedNeighbours);
            free(temp);

        }

        free(unvisitedNeighbours);
    }

    free(list);
}


wint_t GetBorderUnicode(int right, int left, int up, int down) {
    wint_t boxes[16] = {0x20, 0x2575, 0x2577, 0x2502, 
                        0x2574, 0x2518, 0x2510, 0x2524, 
                        0x2576, 0x2514, 0x250C, 0x251c,
                        0x2500, 0x2534, 0x252c, 0x253c};
    return boxes[right * 8 + left * 4 + down * 2 + up * 1];
}

void PrintMaze(MazeMaker* M) {
    for (int i = 0; i < M->height; i++) {
        for (int j = 0; j < M->width; j++) {
            int upwall = (i - 1 >= 0) ? M->maze[i-1][j].walls[WEST] : 0;
            int leftwall = (j - 1 >= 0) ? M->maze[i][j-1].walls[NORTH] : 0;
            int downwall = M->maze[i][j].walls[WEST];
            int rightwall = M->maze[i][j].walls[NORTH];

            printf("%lc", GetBorderUnicode(rightwall, leftwall, upwall, downwall));
            printf("%lc", GetBorderUnicode(rightwall, rightwall, 0, 0));
        }

        int upwall = (i - 1 >= 0) ? M->maze[i - 1][M->width - 1].walls[EAST] : 0;
        int downwall = M->maze[i][M->width - 1].walls[EAST];
        int leftwall = M->maze[i][M->width - 1].walls[NORTH];
        int rightwall = 0;
        printf("%lc\n", GetBorderUnicode(rightwall, leftwall, upwall, downwall));
    }

    printf("%lc", GetBorderUnicode(1, 0, 1, 0));
    printf("%lc", GetBorderUnicode(1, 1, 0, 0));
    for (int j = 1; j < M->width; j++) {
        int upwall = M->maze[M->height - 1][j].walls[WEST];
        int downwall = 0;
        int leftwall = 1;
        int rightwall = 1;
        printf("%lc", GetBorderUnicode(rightwall, leftwall, upwall, downwall));
        printf("%lc", GetBorderUnicode(rightwall, rightwall, 0, 0));
    }
    printf("%lc\n", GetBorderUnicode(0, 1, 1, 0));
}

void ClearScreenAndPrintMaze(MazeMaker* M) {
    printf("\033[2J");
    printf("\033[H");
    PrintMaze(M);
}

int CheckMazeIntegrityHelper(MazeMaker* M) {
    for (int j = 0; j < M->width; j++) {
        if (M->maze[0][j].walls[NORTH] == OPEN) return 0;
        if (M->maze[M->height - 1][j].walls[SOUTH] == OPEN) return 0;
    }

    for (int i = 0; i < M->height; i++) {
        if (M->maze[i][0].walls[WEST] == OPEN) return 0;
        if (M->maze[i][M->width - 1].walls[EAST] == OPEN) return 0;
    }

    for (int i = 0; i < M->height - 1; i++) {
        for (int j = 0; j < M->width - 1; j++) {
            if (M->maze[i][j].walls[EAST] != M->maze[i][j + 1].walls[WEST]) return 0;
            if (M->maze[i][j].walls[SOUTH] != M->maze[i + 1][j].walls[NORTH]) return 0;
        }
    }

    if (M->maze[M->height - 1][M->width - 1].walls[NORTH] != 
        M->maze[M->height - 2][M->width - 1].walls[SOUTH]) return 0;
    if (M->maze[M->height - 1][M->width - 1].walls[WEST] != 
        M->maze[M->height - 1][M->width - 2].walls[EAST]) return 0;
    return 1;
}

void CheckMazeIntegrity(MazeMaker* M) {
    if (!CheckMazeIntegrityHelper(M)) {
        printf("Uh oh. Maze is screwed up\n");
        exit(0);
    }
}

List* getUnvisitedNeighbours(Cell* cell, MazeMaker* M, int cellrow, int cellcol){
    List* neighbors = MakeList();

    if (cellrow > 0 && !M->maze[cellrow - 1][cellcol].visited){
        EnList(neighbors, cellrow - 1, cellcol);
    }

    if (cellrow < M->height -1 && !M->maze[cellrow + 1][cellcol].visited){
        EnList(neighbors, cellrow + 1, cellcol);
    }


    if (cellcol > 0 && !M->maze[cellrow][cellcol -1].visited){
        EnList(neighbors, cellrow, cellcol - 1);
    }


    if (cellcol < M->width - 1 && !M->maze[cellrow][cellcol + 1].visited){
        EnList (neighbors, cellrow, cellcol + 1);
    }


    return neighbors;
}

void removewall(Cell* current, Cell* next, int currentrow, int currentcol, int nextrow, int nextcol){
    

    if(currentrow == nextrow){
        if(currentcol < nextcol){
            current->walls[EAST] = OPEN;
            next->walls[WEST] = OPEN;
        } else {
            current->walls[WEST] = OPEN;
            next->walls[EAST] = OPEN;
        }


    } else if (currentcol == nextcol) {
        if (currentrow < nextrow){
            current->walls[SOUTH] = OPEN;
            next->walls[NORTH] = OPEN;
        } else {
            current->walls[NORTH] = OPEN;
            next->walls[SOUTH] = OPEN;
        }
    }
}