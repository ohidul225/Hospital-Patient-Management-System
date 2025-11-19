#include<stdio.h>
#include<stdlib.h>
#include<string.h>

struct Patient{
    int id;
    char name[20];
    int age;
    int seriousness;
    char condition[50];
    struct Patient* next;
};

//All patient records
struct Patient* head = NULL;

//Normal queue
struct Patient* front = NULL;
struct Patient* rare = NULL;

//Urgent queue
struct Patient* urgentHead = NULL;

//Function prototypes
void enqueue(struct Patient* NewNode);
struct Patient* dequeue();
void insertUrgent(struct Patient* p);
struct Patient* removeUrgent();
void addPatient(int Id, char Name[20], int Age, int Seriousness, char Condition[50]);
void searchPatient(int Id , char Name[20]);
void treatPatient();
void showQueues();
void PrintList(struct Patient* temp);

// ---------------- ADD PATIENT (Corrected) ----------------
void addPatient(int Id, char Name[20], int Age, int Seriousness, char Condition[50]){
    
    // --- 1. Create a node for the Master List (head) ---
    struct Patient* headNode = (struct Patient*)malloc(sizeof(struct Patient));
    headNode->id = Id;
    strcpy(headNode->name, Name);
    headNode->age = Age;
    headNode->seriousness = Seriousness;
    strcpy(headNode->condition, Condition);
    headNode->next = NULL;

    
    if(head == NULL){
        head = headNode;
    } else {
        struct Patient* curr = head;
        while(curr->next != NULL){
            curr = curr->next;
        }
        curr->next = headNode;
    }

    // --- 2. Create a SEPARATE node for the Treatment Queue ---
    struct Patient* queueNode = (struct Patient*)malloc(sizeof(struct Patient));
    queueNode->id = Id;
    strcpy(queueNode->name, Name);
    queueNode->age = Age;
    queueNode->seriousness = Seriousness;
    strcpy(queueNode->condition, Condition);
    queueNode->next = NULL; 

    
    if(Seriousness > 5){
        insertUrgent(queueNode); 
    } else {
        enqueue(queueNode); 
    }
}
// ---------------- SEARCH PATIENT ----------------
void searchPatient(int Id , char Name[20]){
    if(head == NULL){
        printf("The List is empty\n");
        return;
    }

    struct Patient* curr = head;
    while(curr != NULL){
        if(curr->id == Id){
            printf("\nPatient found by ID:\n");
            printf("ID: %d\nName: %s\nAge: %d\nSeriousness: %d\nCondition: %s\n", 
                   curr->id, curr->name, curr->age, curr->seriousness, curr->condition);
            return;
        }
        if(strcmp(curr->name, Name) == 0){
            printf("\nPatient found by Name:\n");
            printf("ID: %d\nName: %s\nAge: %d\nSeriousness: %d\nCondition: %s\n", 
                   curr->id, curr->name, curr->age, curr->seriousness, curr->condition);
            return;
        }
        curr = curr->next;
    }
    printf("Patient NOT found.\n");
}

// ---------------- NORMAL QUEUE ----------------
void enqueue(struct Patient* NewNode){
    NewNode->next = NULL;
    if(rare == NULL){
        front = rare = NewNode;
    } else {
        rare->next = NewNode;
        rare = NewNode;
    }
}

struct Patient* dequeue(){
    if(front == NULL){
        printf("No normal patients waiting.\n");
        return NULL;
    }
    struct Patient* temp = front;
    front = front->next;
    if(front == NULL) rare = NULL;
    temp->next = NULL;
    return temp;
}

// ---------------- URGENT QUEUE ----------------
void insertUrgent(struct Patient* p){
    p->next = NULL;
    if(urgentHead == NULL){
        urgentHead = p;
    } else if(p->seriousness > urgentHead->seriousness){
        p->next = urgentHead;
        urgentHead = p;
    } else {
        struct Patient* temp = urgentHead;
        while(temp->next != NULL && temp->next->seriousness >= p->seriousness){
            temp = temp->next;
        }
        p->next = temp->next;
        temp->next = p;
    }
}

struct Patient* removeUrgent(){
    if(urgentHead == NULL) return NULL;
    struct Patient* temp = urgentHead;
    urgentHead = urgentHead->next;
    temp->next = NULL;
    return temp;
}

// ---------------- TREAT PATIENT ----------------
void treatPatient(){
    struct Patient* p = NULL;
    if(urgentHead != NULL){
        p = removeUrgent();
        printf("\nTreating URGENT patient:\n");
    } else if(front != NULL){
        p = dequeue();
        printf("\nTreating NORMAL patient:\n");
    } else {
        printf("\nNo patients waiting!\n");
        return;
    }
    printf("ID: %d | Name: %s | Seriousness: %d | Condition: %s\n",
           p->id, p->name, p->seriousness, p->condition);
}

// ---------------- SHOW QUEUES ----------------
void showQueues(){
    printf("\n--- URGENT PATIENTS ---\n");
    if(urgentHead == NULL){
        printf("No urgent patients waiting.\n");
    } else {
        struct Patient* t = urgentHead;
        int count = 1;
        while(t != NULL){
            printf("%d. ID: %d | Name: %s | Seriousness: %d | Condition: %s\n",
                   count, t->id, t->name, t->seriousness, t->condition);
            t = t->next;
            count++;
        }
    }

    printf("\n--- NORMAL PATIENTS ---\n");
    if(front == NULL){
        printf("No normal patients waiting.\n");
    } else {
        struct Patient* t = front;
        int count = 1;
        while(t != NULL){
            printf("%d. ID: %d | Name: %s | Seriousness: %d | Condition: %s\n",
                   count, t->id, t->name, t->seriousness, t->condition);
            t = t->next;
            count++;
        }
    }
    printf("\n");
}

// ---------------- ALL RECORDS ----------------
void PrintList(struct Patient* temp){
    while(temp != NULL){
        
        printf("ID: %d\n", temp->id);
        printf("Name: %s\n", temp->name);
        printf("Age: %d\n", temp->age);
        printf("Seriousness: %d\n", temp->seriousness);
        printf("Condition: %s\n\n", temp->condition);
        temp = temp->next;
    }

    printf("No Patient in the records\n");
}

// ---------------- MAIN FUNCTION ----------------
int main(){
    int sz;
    int id, age, seriousness;
    char name[50];
    char condition[100];
    int choice = 0;

    while(choice != 6){
        printf("\n1. Add Patient\n");
        printf("2. Treat Patient\n");
        printf("3. Show Queues\n");
        printf("4. Show All Records\n");
        printf("5. Search Patient\n");
        printf("6. Exit\n");

        printf("\nEnter choice: ");
        scanf("%d", &choice);
        printf("\n");

        switch(choice){
            case 1:
                printf("How many patients do you want to add: ");
                scanf("%d", &sz);
                while(sz > 0){
                    printf("\nEnter Patient ID: ");
                    scanf("%d", &id);
                    printf("Enter Name: ");
                    scanf("%s", name);
                    printf("Enter Age: ");
                    scanf("%d", &age);
                    printf("Enter Seriousness (1-10): ");
                    scanf("%d", &seriousness);
                    printf("Enter Condition: ");
                    scanf("%s", condition);

                    addPatient(id, name, age, seriousness, condition);
                    sz--;
                }
                break;

            case 2:
                treatPatient();
                break;

            case 3:
                showQueues();
                break;

            case 4:
                PrintList(head);
                break;

            case 5:{
                int searchChoice;
                int searchId;
                char searchName[50];

                printf("\nSearch by:\n1. ID\n2. Name\nEnter your choice: ");
                scanf("%d", &searchChoice);

                if(searchChoice == 1){
                    printf("Enter ID: ");
                    scanf("%d", &searchId);
                    searchPatient(searchId, "");
                } else if(searchChoice == 2){
                    printf("Enter Name: ");
                    scanf("%s", searchName);
                    searchPatient(-1, searchName);
                } else {
                    printf("Invalid search option.\n");
                }
                break;
            }

            case 6:
                printf("Exiting program...\n");
                break;

            default:
                printf("Invalid choice! Try again.\n");
        }
    }
    return 0;
}
