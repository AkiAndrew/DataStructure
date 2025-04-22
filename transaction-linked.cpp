#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 512

struct node{
    char customer_id[50];
    char product[100];
    char category[50];
    float price;
    char date[20];
    char payment_method[50];
    
    struct node *link;
};

    //Create fiest node (head)
    struct node* createNode(char customer_id[], char product[], char category[], float price, char date[], char payment_method[]){
        struct node *new_node = (struct node*)malloc(sizeof(struct node));
        strcpy(new_node->customer_id, customer_id);
        strcpy(new_node->product, product);
        strcpy(new_node->category, category);
        new_node->price = price;
        strcpy(new_node->date, date);
        strcpy(new_node->payment_method, payment_method);
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
            printf("Customer ID: %s\n", current->customer_id);
            printf("Product: %s\n", current->product);
            printf("Category: %s\n", current->category);
            printf("Price: %.2f\n", current->price);
            printf("Date: %s\n", current->date);
            printf("Payment Method: %s\n", current->payment_method);
            printf("\n");
            current = current->link;
        }
    }

    int countTransactions(struct node*head){
        int count = 0;
        struct node*current = head;
        while (current != NULL){
            count++;
            current = current->link;
        }
        return count;
    }

    int main(){
        FILE *file = fopen("transactions_cleaned.csv", "r");
        if(!file){
            printf("Error opening file.\n");
            return 1;
        }
        struct node*head = NULL;
        char line[MAX_LINE_LENGTH];

        fgets(line,sizeof(line),file); // Skip header line

        while(fgets(line,sizeof(line), file)){
            char*customer_id = strtok(line, ",");
            char*product = strtok(NULL, ",");
            char*category = strtok(NULL, ",");
            char*price_str = strtok(NULL, ",");
            char*date = strtok(NULL, ",");
            char*payment_method = strtok(NULL, "\n");

            if(customer_id && product && category && price_str && date && payment_method){
                float price = atof(price_str);
                struct node*new_node = createNode(customer_id, product, category, price, date, payment_method);
                appendNode(&head, new_node);
            }
        }

        fclose(file);

        displayList(head);
        printf("Total Transactions: %d\n", countTransactions(head));

        return 0;
    }





