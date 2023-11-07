#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

struct Patient {
    int id;
    char name[100];
    int age;
    char gender[10];
    char address[100];
    char phone[15];
    struct Patient* next;
};

struct Patient* createPatient(char name[], int age, char gender[], char address[], char phone[]) {
    struct Patient* newPatient = (struct Patient*)malloc(sizeof(struct Patient));
    newPatient->id = rand() % 900 + 100; // Generate a random 3-digit ID
    strcpy(newPatient->name, name);
    newPatient->age = age;
    strcpy(newPatient->gender, gender);
    strcpy(newPatient->address, address);
    strcpy(newPatient->phone, phone);
    newPatient->next = NULL;
    return newPatient;
}

void addPatient(struct Patient** head, struct Patient* newPatient) {
    if (*head == NULL) {
        *head = newPatient;
    } else {
        struct Patient* current = *head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = newPatient;
    }
    printf("Patient added successfully. ID: %03d\n", newPatient->id);
}

void displayPatients(struct Patient* head) {
    if (head == NULL) {
        printf("No patients in the database.\n");
    } else {
        struct Patient* current = head;
        printf("Patient Database:\n");
        while (current != NULL) {
            printf("ID: %03d\n", current->id);
            printf("Name: %s\n", current->name);
            printf("Age: %d\n", current->age);
            printf("Gender: %s\n", current->gender);
            printf("Address: %s\n", current->address);
            printf("Phone: %s\n", current->phone);
            printf("------------------------\n");
            current = current->next;
        }
    }
}

void writePatientsToFile(struct Patient* head) {
    FILE* file = fopen("patients.txt", "w");
    if (file == NULL) {
        printf("Error opening file.\n");
        return;
    }

    struct Patient* current = head;
    while (current != NULL) {
        fprintf(file, "%03d|%s|%d|%s|%s|%s\n", current->id, current->name, current->age, current->gender, current->address, current->phone);
        current = current->next;
    }
    
    fclose(file);
    printf("Patients data written to file.\n");
}

void readPatientsFromFile(struct Patient** head) {
    FILE* file = fopen("patients.txt", "r");
    if (file == NULL) {
        printf("No existing patient data found.\n");
        return;
    }

    char line[256];
    while (fgets(line, sizeof(line), file) != NULL) {
        struct Patient* newPatient = (struct Patient*)malloc(sizeof(struct Patient));
        sscanf(line, "%d|%[^|]|%d|%[^|]|%[^|]|%s\n", &newPatient->id, newPatient->name, &newPatient->age, newPatient->gender, newPatient->address, newPatient->phone);
        newPatient->next = NULL;

        if (*head == NULL) {
            *head = newPatient;
        } else {
            struct Patient* current = *head;
            while (current->next != NULL) {
                current = current->next;
            }
            current->next = newPatient;
        }
    }
    
    fclose(file);
    printf("Patients data read from file.\n");
}

void searchPatient(struct Patient* head, int id) {
    if (head == NULL) {
        printf("No patients in the database.\n");
    } else {
        struct Patient* current = head;
        int found = 0;
        while (current != NULL) {
            if (current->id == id) {
                printf("Patient found:\n");
                printf("ID: %03d\n", current->id);
                printf("Name: %s\n", current->name);
                printf("Age: %d\n", current->age);
                printf("Gender: %s\n", current->gender);
                printf("Address: %s\n", current->address);
                printf("Phone: %s\n", current->phone);
                found = 1;
                break;
            }
            current = current->next;
        }
        if (!found) {
            printf("Patient not found.\n");
        }
    }
}

void deletePatient(struct Patient** head, int id) {
    if (*head == NULL) {
        printf("No patients in the database.\n");
    } else {
        struct Patient* current = *head;
        struct Patient* prev = NULL;
        int found = 0;
        while (current != NULL) {
            if (current->id == id) {
                if (prev == NULL) {
                    *head = current->next;
                } else {
                    prev->next = current->next;
                }
                free(current);
                printf("Patient deleted successfully.\n");
                found = 1;
                break;
            }
            prev = current;
            current = current->next;
        }
        if (!found) {
            printf("Patient not found.\n");
        }
    }
}

int main() {
    struct Patient* head = NULL;
    int choice;
    
    srand(time(NULL)); // Initialize random number generator
    
    readPatientsFromFile(&head); // Read existing patient data from file
    
    do {
        printf("Patient Database Management\n");
        printf("1. Add Patient\n");
        printf("2. Display Patients\n");
        printf("3. Search Patient\n");
        printf("4. Delete Patient\n");
        printf("5. Save Patients\n");
        printf("6. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        
        switch (choice) {
            case 1: {
                char name[100];
                int age;
                char gender[10];
                char address[100];
                char phone[15];
                
                printf("Enter patient details:\n");
                printf("Name: ");
                scanf("%s", name);
                printf("Age: ");
                scanf("%d", &age);
                printf("Gender: ");
                scanf("%s", gender);
                printf("Address: ");
                scanf("%s", address);
                printf("Phone: ");
                scanf("%s", phone);
                
                struct Patient* newPatient = createPatient(name, age, gender, address, phone);
                addPatient(&head, newPatient);
                break;
            }
            case 2: {
                displayPatients(head);
                break;
            }
            case 3: {
                int id;
                printf("Enter the ID of the patient to search: ");
                scanf("%d", &id);
                searchPatient(head, id);
                break;
            }
            case 4: {
                int id;
                printf("Enter the ID of the patient to delete: ");
                scanf("%d", &id);
                deletePatient(&head, id);
                break;
            }
            case 5: {
                writePatientsToFile(head);
                break;
            }
            case 6: {
                printf("Exiting...\n");
                break;
            }
            default: {
                printf("Invalid choice. Please try again.\n");
                break;
            }
        }
    } while (choice != 6);
    
    return 0;
}