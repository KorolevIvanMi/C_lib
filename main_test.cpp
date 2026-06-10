// Простой список с CRUD
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

//взятие элемента по индексу
int get_on_pos(Node* head, int pos){
    int i = 0;
    if(head == NULL){
        return -1;
    }
    Node* current  = head;
    while (i != pos){
        if (current == NULL){
            return -1;
        }
        else{
            current = current->next;
        }
        i = i+1;
    }
    if (current == NULL){
            return -1;
    }
    return current->value;

}

//замена на позиции
void change_on_pos(Node** head, int pos , int value ){
    if(*head == NULL){
        return;
    }
    if(pos < 0){
        return;
    }
    int i = 0;
    Node* current = *head;
    while(pos != i){
        if (current == NULL){
            return;
        }
        else{
            current = current->next;
        }
        i = i + 1;
    }
    current->value = value;
}

//удаление с конца
void pop(Node** head){
    if(*head == NULL){
        return;
    }
    if ((*head)->next == NULL){
        free(*head);
        *head = NULL;
        return;
    }

    Node* current = *head;
    while(current->next->next != NULL){
        current=current->next;
    }
    free(current->next);
    current->next = NULL;
}


int main(){
    Node* mylist = NULL;
    append(&mylist, 12);
    append(&mylist, 10);
    append(&mylist, 1);
    showArray(mylist);

    printf("%d",  get_on_pos(mylist, 3));
    printf("\n");

    change_on_pos(&mylist, 2, -23 );
    showArray(mylist);

    pop(&mylist);
    showArray(mylist);
    pop(&mylist);
    showArray(mylist);
    pop(&mylist);
    showArray(mylist);


    free(mylist);
    return 0;
}