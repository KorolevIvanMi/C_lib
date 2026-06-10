#include <stdio.h>
#include <stdlib.h>
#include "headers/list.h"

//вывод массива
void showArray(Node* head){
    if(head == NULL){
        return;
    }
    Node* current = head;
    while(current != NULL){
        printf("%d", current->value);
        printf(" ");
        current=current->next;
    }
    printf("\n");
}

//добавление в конец
void append(Node** head, int value){
    Node* new_element = (Node*)malloc(sizeof(Node));
    new_element->value = value;
    new_element->next = NULL;
    if(*head == NULL){
        *head = new_element;
    }
    else{
        Node* current = *head;
        while(current->next != NULL){
            current = current->next;
        }
        current->next = new_element;
    }
}


int main(){
    Node* mylist = NULL;
    append(&mylist, 12);
    append(&mylist, 10);
    append(&mylist, 1);
    showArray(mylist);
    free(mylist);
    return 1;
}