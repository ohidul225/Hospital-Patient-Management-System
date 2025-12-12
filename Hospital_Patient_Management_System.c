#include<stdio.h>
#include<stdlib.h>
#include<string.h>

// ---------------- PATIENT STRUCT ----------------
struct Patient {
    int id;
    char name[20];
    int age;
    int seriousness;
    char condition[50];
    struct Patient* next;
};

// ---------------- APPOINTMENT STRUCT ----------------
struct Appointment {
    int id;
    char name[20];
    char timeSlot[20];
    struct Appointment* next;
};

// All Lists
struct Patient* head = NULL;        // All patient records (Master List)
struct Patient* front = NULL;       // Normal queue
struct Patient* rare = NULL;
struct Patient* urgentHead = NULL;  // Urgent queue (Priority Queue)
struct Appointment* appHead = NULL; // Appointment list

// FUNCTION DECLARATIONS
void enqueue(struct Patient* NewNode);
struct Patient* dequeue();
void insertUrgent(struct Patient* p);
struct Patient* removeUrgent();
void addPatient(int Id, char Name[20], int Age, int Seriousness, char Condition[50]);
void searchPatient(int Id , char Name[20]); // This was missing implementation
void treatPatient();
void showQueues();
void PrintList(struct Patient* temp);

// NEW FEATURE FUNCTIONS
void bookAppointment(int Id, char name[20], char timeSlot[20]);
void showAppointments();
char* getDoctor(int count);

// ---------------- ADD PATIENT ----------------
void addPatient(int Id, char Name[20], int Age, int Seriousness, char Condition[50]){
    
    // 1. Add to Master List (History)
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

    // 2. Create a copy for the Queue (Triage)
    struct Patient* queueNode = (struct Patient*)malloc(sizeof(struct Patient));
    // Copy data manually to be safe
    queueNode->id = Id;
    strcpy(queueNode->name, Name);
    queueNode->age = Age;
    queueNode->seriousness = Seriousness;
    strcpy(queueNode->condition, Condition);
    queueNode->next = NULL;

    // 3. Add to proper queue based on Seriousness
    if(Seriousness > 5){
        insertUrgent(queueNode);
    } else {
        enqueue(queueNode);
    }
}

// ---------------- SEARCH PATIENT (FIXED) ----------------
void searchPatient(int Id, char Name[20]){
    struct Patient* temp = head;
    int found = 0;
    
    printf("\n----- SEARCH RESULTS -----\n");
    
    if(head == NULL) {
        printf("No records found.\n");
        return;
    }

    while(temp != NULL){
        // Search by ID
        if(Id != -1){
            if(temp->id == Id){
                printf("FOUND -> Name: %s | Age: %d | Condition: %s\n", temp->name, temp->age, temp->condition);
                found = 1;
            }
        }
        // Search by Name
        else {
            if(strcmp(temp->name, Name) == 0){
                printf("FOUND -> ID: %d | Age: %d | Condition: %s\n", temp->id, temp->age, temp->condition);
                found = 1;
            }
        }
        temp = temp->next;
    }

    if(found == 0){
        printf("Patient not found in records.\n");
    }
}

// ---------------- BOOK APPOINTMENT ----------------
void bookAppointment(int Id, char name[20], char timeSlot[20]){
    struct Appointment* app = (struct Appointment*)malloc(sizeof(struct Appointment));
    app->id = Id;
    strcpy(app->name, name);
    strcpy(app->timeSlot, timeSlot);
    app->next = NULL;

    if(appHead == NULL){
        appHead = app;
    } else {
        struct Appointment* temp = appHead;
        while(temp->next != NULL) temp = temp->next;
        temp->next = app;
    }

    printf("\nAppointment Booked Successfully for %s at %s!\n", name, timeSlot);
}

// ---------------- SHOW APPOINTMENTS ----------------
void showAppointments(){
    if(appHead == NULL){
        printf("\nNo Appointments Booked.\n");
        return;
    }

    printf("\n----- APPOINTMENT LIST -----\n");
    struct Appointment* temp = appHead;
    while(temp != NULL){
        printf("ID: %d | Name: %s | Time: %s\n", temp->id, temp->name, temp->timeSlot);
        temp = temp->next;
    }
    printf("\n");
}

// ---------------- NORMAL QUEUE (FIFO) ----------------
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
        return NULL;
    }
    struct Patient* temp = front;
    front = front->next;
    if(front == NULL) rare = NULL;
    temp->next = NULL;
    return temp;
}

// ---------------- URGENT QUEUE (PRIORITY) ----------------
// High seriousness goes to the front

void insertUrgent(struct Patient* p){
    p->next = NULL;
    
    // Case 1: List is empty
    if(urgentHead == NULL){
        urgentHead = p;
        return;
    }

    // Case 2: New patient is MORE serious than the head
    if(p->seriousness > urgentHead->seriousness){
        p->next = urgentHead;
        urgentHead = p;
        return;
    }

    // Case 3: Insert somewhere in the middle or end
    struct Patient* temp = urgentHead;
    while(temp->next != NULL && temp->next->seriousness >= p->seriousness){
        temp = temp->next;
    }
    p->next = temp->next;
    temp->next = p;
}

struct Patient* removeUrgent(){
    if(urgentHead == NULL) return NULL;
    struct Patient* temp = urgentHead;
    urgentHead = urgentHead->next;
    temp->next = NULL;
    return temp;
}

// ------------- DOCTOR ASSIGNMENT -------------
char* getDoctor(int count){
    if(count % 2 == 0)
        return "Dr. Karim (9 AM - 1 PM)";
    else
        return "Dr. Sara (2 PM - 6 PM)";
}

// ---------------- TREAT PATIENT ----------------
void treatPatient(){
    static int treatCount = 0;
    struct Patient* p = NULL;

    // Priority Check: Urgent queue first!
    if(urgentHead != NULL){
        p = removeUrgent();
        printf("\n>>> Treating URGENT Patient <<<\n");
    }
    else if(front != NULL){
        p = dequeue();
        printf("\n>>> Treating NORMAL Patient <<<\n");
    }
    else {
        printf("\nNo patients waiting in queue.\n");
        return;
    }

    treatCount++;

    printf("ID: %d | Name: %s | Seriousness: %d | Condition: %s\n",
           p->id, p->name, p->seriousness, p->condition);

    printf("Assigned Doctor: %s\n", getDoctor(treatCount));
    
    // Free the queue memory (but Master List record remains)
    free(p);
}

// ---------------- SHOW QUEUES ----------------
void showQueues(){
    printf("\n----- URGENT QUEUE (High Priority) -----\n");
    if(urgentHead == NULL) printf("No urgent patients.\n");
    else{
        struct Patient* t = urgentHead; int c = 1;
        while(t != NULL){
            printf("%d. ID: %d | Name: %s | Seriousness: %d\n",
                   c++, t->id, t->name, t->seriousness);
            t = t->next;
        }
    }

    printf("\n----- NORMAL QUEUE (Standard) -----\n");
    if(front == NULL) printf("No normal patients.\n");
    else{
        struct Patient* t = front; int c = 1;
        while(t != NULL){
            printf("%d. ID: %d | Name: %s | Seriousness: %d\n",
                   c++, t->id, t->name, t->seriousness);
            t = t->next;
        }
    }
}

// ---------------- PRINT ALL RECORDS ----------------
void PrintList(struct Patient* temp){
    if(temp == NULL){
        printf("No patients recorded in history.\n");
        return;
    }

    printf("\n----- ALL PATIENT RECORDS (HISTORY) -----\n");
    while(temp != NULL){
        printf("ID: %d | Name: %s | Age: %d | Seriousness: %d | Condition: %s\n",
               temp->id, temp->name, temp->age, temp->seriousness, temp->condition);
        temp = temp->next;
    }
}

// ---------------- MAIN FUNCTION ----------------
int main(){
    int choice, id, age, seriousness, sz;
    char name[50], condition[100], timeSlot[30];

    // Disable buffering to ensure prints appear immediately
    setbuf(stdout, NULL);

    while(1){
        printf("\n============ HOSPITAL MENU ============\n");
        printf("1. Add Patient (Check-in)\n");
        printf("2. Treat Patient (Doctor Visit)\n");
        printf("3. Show Waiting Queues\n");
        printf("4. Show All Patient History\n");
        printf("5. Search Patient\n");
        printf("6. Book Appointment\n");
        printf("7. Show Appointments\n");
        printf("8. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch(choice){

            case 1:
                printf("How many patients to add: ");
                scanf("%d", &sz);
                while(sz--){
                    printf("\n--- Patient Details ---\n");
                    printf("ID: "); scanf("%d", &id);
                    printf("Name: "); scanf("%s", name);
                    printf("Age: "); scanf("%d", &age);
                    printf("Seriousness (1-10): "); scanf("%d", &seriousness);
                    printf("Condition: "); scanf("%s", condition);
                    addPatient(id, name, age, seriousness, condition);
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

            case 5: {
                int findOpt;
                printf("\n1. Search by ID\n2. Search by Name\nChoice: ");
                scanf("%d", &findOpt);
                if(findOpt == 1){
                    printf("Enter ID: "); scanf("%d", &id);
                    searchPatient(id, "");
                } else {
                    printf("Enter Name: "); scanf("%s", name);
                    searchPatient(-1, name);
                }
            } break;

            case 6:
                printf("\n--- Book Appointment ---\n");
                printf("Enter Patient ID: ");
                scanf("%d", &id);
                printf("Enter Name: ");
                scanf("%s", name);
                printf("Enter Time Slot (e.g., 10AM): ");
                scanf("%s", timeSlot);
                bookAppointment(id, name, timeSlot);
                break;

            case 7:
                showAppointments();
                break;

            case 8:
                printf("\nExiting program...\n");
                exit(0);

            default:
                printf("Invalid choice. Please try again.\n");
        }
    }
    return 0;
}