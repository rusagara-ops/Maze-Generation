typedef enum _wallstatus {
    BLOCKED = 1,
    OPEN = 0
} WallStatus;

// The direction of the wall.
typedef enum _direction {
    NORTH = 0,
    WEST = 1,
    SOUTH = 2,
    EAST = 3
} Direction;

// The mode enum for the four maze generation algorithms we implement
typedef enum _mode {
    DEPTH = 4,
    PRIM = 5,
    LRU = 6,
    ALL = 7
} Mode;

// The cell struct contains information regarding the status of its four walls
// and whether the cell is visited or not
typedef struct _cell {
    WallStatus walls[4];
    int visited;
} Cell;

// The MazeMaker struct stores all information necessary, including the size of
// the maze, the maze array, the chosen generation algorithm, and the list
// containing the visited cells.
typedef struct _mazemaker {
    int width;
    int height;
    Mode mode;
    Cell** maze;
    List* cells;
} MazeMaker;

MazeMaker* Setup(int, char**);
void GenerateMaze(MazeMaker*);
// Given a maze in the MazeMaker, it prints the maze using unicode.
void PrintMaze(MazeMaker*);
void ClearScreenAndPrintMaze(MazeMaker*);
wint_t GetBorderUnicode(int, int, int, int);
// Chech whether the maze is at a correct state. The walls connecting two 
// adjacent cells are either both OPEN or both CLOSED for all adjacent cells.
void CheckMazeIntegrity(MazeMaker*);
