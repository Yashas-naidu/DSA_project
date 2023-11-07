#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define TABLE_SIZE 1000

struct Patient {
    int id;
    char name[100];
    int age;
    char gender[10];
    char address[100];
    char phone[15];
    struct Patient* next;
};

struct HashTable {
    struct Patient* table[TABLE_SIZE];
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

void savePatientsToFile(struct HashTable* hashTable);

int hashFunction(int id) {
    return id % TABLE_SIZE;
}

void addPatient(struct HashTable* hashTable, struct Patient* newPatient) {
    int index = hashFunction(newPatient->id);
    if (hashTable->table[index] == NULL) {
        hashTable->table[index] = newPatient;
    } else {
        struct Patient* current = hashTable->table[index];
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = newPatient;
    }
    printf("Patient added successfully. ID: %03d\n", newPatient->id);
    savePatientsToFile(hashTable);
}

void savePatientsToFile(struct HashTable* hashTable) {
    FILE* file = fopen("patients.txt", "w");
    if (file == NULL) {
        printf("Error opening file.\n");
        return;
    }

    int i;
    for (i = 0; i < TABLE_SIZE; i++) {
        struct Patient* current = hashTable->table[i];
        while (current != NULL) {
            fprintf(file, "%03d|%s|%d|%s|%s|%s\n", current->id, current->name, current->age, current->gender, current->address, current->phone);
            current = current->next;
        }
    }
    
    fclose(file);
    printf("Patients data written to file.\n");
}

void readPatientsFromFile(struct HashTable* hashTable) {
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

        int index = hashFunction(newPatient->id);
        if (hashTable->table[index] == NULL) {
            hashTable->table[index] = newPatient;
        } else {
            struct Patient* current = hashTable->table[index];
            while (current->next != NULL) {
                current = current->next;
            }
            current->next = newPatient;
        }
    }
    
    fclose(file);
    printf("Patients data read from file.\n");
}

void searchPatient(struct HashTable* hashTable, int id) {
    int index = hashFunction(id);
    struct Patient* current = hashTable->table[index];
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

void deletePatient(struct HashTable* hashTable, int id) {
    int index = hashFunction(id);
    struct Patient* current = hashTable->table[index];
    struct Patient* prev = NULL;
    int found = 0;
    while (current != NULL) {
        if (current->id == id) {
            if (prev == NULL) {
                hashTable->table[index] = current->next;
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
    } else {
        savePatientsToFile(hashTable);
    }
}

int main() {
    struct HashTable hashTable;
    int choice;
    
    srand(time(NULL)); // Initialize random number generator
    
    memset(hashTable.table, 0, sizeof(hashTable.table)); // Initialize hash table
    
    readPatientsFromFile(&hashTable); // Read existing patient data from file
    
    do {
        printf("Patient Database Management\n");
        printf("1. Add Patient\n");
        printf("2. Search Patient\n");
        printf("3. Delete Patient\n");
        printf("4. Exit\n");
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
                addPatient(&hashTable, newPatient);
                break;
            }
            case 2: {
                int id;
                printf("Enter the ID of the patient to search: ");
                scanf("%d", &id);
                searchPatient(&hashTable, id);
                break;
            }
            case 3: {
                int id;
                printf("Enter the ID of the patient to delete: ");
                scanf("%d", &id);
                deletePatient(&hashTable, id);
                break;
            }
            case 4: {
                printf("Exiting...\n");
                break;
            }
            default: {
                printf("Invalid choice. Please try again.\n");
                break;
            }
        }
    } while (choice != 4);
    
    return 0;
}