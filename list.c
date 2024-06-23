#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "list.h"

List* MakeList() {

    List* newlist = (List*)malloc(sizeof(List));
    if(newlist == NULL) {
        return NULL;
    }

    newlist ->count = 0;
    newlist ->head =NULL;
    newlist ->tail = NULL;

    return newlist;
}

Node* CreateNode(int inRow, int inCol, Node* inPrev, Node* inNext) {

    Node* newNode = (Node*)malloc(sizeof(Node));
    if (newNode == NULL){
        return NULL;
    }

    newNode ->col = inCol;
    newNode ->next = NULL;
    newNode ->prev = NULL;
    newNode ->row = inRow;

    return newNode;
}

void EnList(List* que, int inRow, int inCol) {

    if (que == NULL){
        return;
    }

    Node* newNode = CreateNode(inRow, inCol, NULL, NULL);

    if (newNode == NULL){
        return;
    }

    if (que -> head == NULL){
        que ->head = newNode;
        que ->tail = newNode;

    } else {

        que ->tail ->next = newNode;
        newNode -> prev = que -> tail;
        que ->tail = newNode;
    }

    que ->count++;
}

Node* Pop(List* que) {

    if (que == NULL || que -> tail == NULL){
        return NULL;
    }

    Node* removeNode = que -> tail;

    if (que ->head == que->tail){
        que ->head = NULL;
        que->tail = NULL;
    } else {
        que -> tail = removeNode->prev;
        que->tail->next = NULL;
    }

    que->count--;

    removeNode -> prev = NULL;
    removeNode->next = NULL;

    return removeNode;

}

Node* DeList(List* que) {

    if (que == NULL || que -> head == NULL){
        return NULL;
    }

    Node* removeNode = que -> head;
    que -> head = que ->head ->next;

    if (que ->head == NULL){
        que ->tail = NULL;
    } else {
        que ->head ->prev = NULL;
    }


    que ->count--;

    removeNode -> next = NULL;

    return removeNode;
}

Node* RemoveRandom(List* que) {

    if (que == NULL || que -> head == NULL || que -> count == 0){
        return NULL;
    }

    int index = rand() % que ->count;
    int i = 0;

    Node* current = que -> head;
    while (i < index && current != NULL){
        current = current ->next;
        i++;
    }
    if (current == NULL){
        return NULL;
    }


    if (current == que ->head){
        que->head = current->next;
        if (que->head == NULL){
            que->tail = NULL;
        } else {
            que->head->prev = NULL;
        }
    }
    else if (current == que ->tail){
        que->tail = current->prev;
        if (que->tail == NULL){
            que->head = NULL;   
        } else { 
            que->tail->next = NULL;
        }
       
    } else {
        current->prev->next = current->next;
        current->next->prev = current->prev;
    }

    que -> count--;
    current ->next = NULL;
    current ->prev = NULL;

    if (que->count == 0){
        que->head = NULL;
        que->tail = NULL;
    }

    return current;
    
}


void Print(List* que) {
    for (Node* t = que->head; t != NULL; t = t->next) {
        printf("(%d, %d) ", t->row, t->col);
    }
    printf("\n");
}
