
// The Node struct. It stores two int values "row" and "col". "row" and "col"
// are used to store location information of the cell we add to the list
typedef struct _node {
    int row;
    int col;
    struct _node* prev;
    struct _node* next;
} Node;

// The List struct keeps track of the head and tail pointer of our doubly
// linked list. It also keeps track of the number of elements in the list
// The "count" will be used when selecting a random node in the list.
typedef struct _List {
    int count;
    Node* head;
    Node* tail;
} List;

// Create an empty List data structure
List* MakeList();
Node* CreateNode(int, int, Node*, Node*);
// Enqueue a node to the list (Add the node at the end of the list)
void EnList(List*, int, int);
// Dequeue a node to the list (Remove the last node of the list)
Node* DeList(List*);
// Pop from the list (Remove the first node of the list)
Node* Pop(List*);
// Select a random node and remove it from the list
Node* RemoveRandom(List*);
// Print the content of the list. This is primarily used for debugging purposes
void Print(List*);
