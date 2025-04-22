#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 512

struct node{
    char product_id[50];
    char customer_id[50];
    int rating;
    char review[200];

    struct node*link;
};

// Create first node (head)
struct node* createNode(char product_id[], char customer_id[], int rating, char review[]){
    struct node *new_node = (struct node*)malloc(sizeof(struct node));
    strcpy(new_node->product_id, product_id);
    strcpy(new_node->customer_id, customer_id);
    new_node->rating = rating;
    strcpy(new_node->review, review);
    new_node->link = NULL;
    return new_node;
}
void appendNode(struct node**head_ref, struct node*new_node){
    if(*head_ref == NULL){
        *head_ref = new_node;
        return;
    }
    struct node*current = *head_ref;
    while (current->link != NULL){
        current = current->link;
    }
    current->link = new_node;
}
void displayList(struct node*head){
    struct node*current = head;
    while (current != NULL){
        printf("Product ID: %s\n", current->product_id);
        printf("Customer ID: %s\n", current->customer_id);
        printf("Rating: %d\n", current->rating);
        printf("Review: %s\n", current->review);
        printf("\n");
        current = current->link;
    }
}
int countReviews(struct node*head){
    int count = 0;
    struct node*current = head;
    while (current != NULL){
        count++;
        current = current->link;
    }
    return count;
}

int main(){
    FILE *file = fopen("reviews_cleaned.csv", "r");
    if (file == NULL){
        printf("Error opening file.\n");
        return 1;
    }

    struct node*head = NULL;
    char line[MAX_LINE_LENGTH];

    fgets(line, sizeof(line), file); // Skip header line

    while(fgets(line,sizeof(line), file)){
        char*product_id = strtok(line, ",");
        char*customer_id = strtok(NULL, ",");
        char*rating_str = strtok(NULL, ",");
        char*review = strtok(NULL, "\n");

        if(product_id && customer_id && rating_str && review){
            int rating = atof(rating_str);
            struct node*new_node = createNode(product_id, customer_id, rating, review);
            appendNode(&head, new_node);
        }
    }
    fclose(file);

    displayList(head);
    printf("Total number of reviews: %d\n", countReviews(head));

    return 0;
}

