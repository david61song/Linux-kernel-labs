#include <stdio.h>
#include <stdlib.h>

typedef struct _node{
    int data;
    struct _node *next;
}node;

node* make_node(int data){
    node *new_node = malloc(sizeof(node));
    new_node -> data = data;
    return new_node;
}

void remove_node(node **head, int data){
    node *curr = *head;
    node *prev = NULL;

    while (curr->data != data){
	prev = curr;
	curr = curr->next;
    }

    /* removing first entry */

    if (!prev){
	prev = *head;
	*head = (*head)->next;
	free(prev);
    }

    else{
	prev->next = curr->next;
	free(curr);
    }

}

int main(){
    node *head = make_node(3);
    node *a = make_node(4);
    node *b = make_node(5);

    head->next = a;
    a->next = b;

    remove_node(&head, 4);
    printf("%d", head->data);

}
