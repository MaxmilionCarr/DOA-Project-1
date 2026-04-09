/*
 * Starter file for Project 1 Part 2 (Link https://edstem.org/au/courses/31090/lessons/104103/slides/714974)
 *
 * Created for COMP20007 Design of Algorithms 2025
 * Template written by Thomas Minuzzo <thomas.minuzzo@unimelb.edu.au>
 * Implementation by YOUR NAME HERE.
 * Compile with gcc -Wall -g -o toposort toposort.c -lm
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define CODE_LENGTH 9
#define DEBUG 0
#define PRINTED -1

typedef struct{
    char code[CODE_LENGTH+1];//+1 for null byte
    char** pre_req;//Malloc each to CODE_LENGTH + !
    int num_prereqs;
} subject_t;

/* Reads a list of subjects and their prerequisites into a dynamically allocated adjacency list*/
subject_t* read_major(int* num_subjects);
/* Prints the associated memory of a major, for debugging*/
void print_major(subject_t* major, int num_subjects);
/* Implements Kahn's algorithm to topologically sort a major's subjects */
void kahns_toposort(subject_t* major, int num_subjects);
/* Frees a dynamically allocated adjacency list*/
void free_major(subject_t* major, int num_subjects);


/* Helper to remove pre requisites in a memory friendly way
return 1 means found and removed
return 0 means not found so not removed */
int remove_prerequisites(subject_t* target, subject_t* prereq);

int main(int argc, char** argv){

    int num_subjects;
    subject_t* major = read_major(&num_subjects);

    if(DEBUG) print_major(major,num_subjects);

    kahns_toposort(major,num_subjects);

    free_major(major,num_subjects);

    return 0;
}

subject_t* read_major(int* num_subjects){

    scanf("Number of subjects: %d\n", num_subjects);

    subject_t* major = (subject_t*)malloc(sizeof(subject_t)*(*num_subjects));

    for(int i = 0 ; i < (*num_subjects) ; i++){
        for(int c = 0 ; c < CODE_LENGTH ; c++){
            scanf("%c", &major[i].code[c]);
        }
        major[i].code[CODE_LENGTH]='\0';

        if(DEBUG){printf("%s\n",major[i].code);}
        
        scanf("(%d)",&major[i].num_prereqs);

        major[i].pre_req = (char**)malloc(sizeof(char*)*major[i].num_prereqs);
        for(int j = 0 ; j < major[i].num_prereqs ; j++){
             major[i].pre_req[j] = (char*)malloc(sizeof(char)*(CODE_LENGTH+1));
            for(int c = 0 ; c < CODE_LENGTH ; c++){
                scanf("%c", &major[i].pre_req[j][c]);
            } 
            major[i].pre_req[j][CODE_LENGTH]='\0';
            if(DEBUG){printf("%s\n",major[i].pre_req[j]);}

            scanf(",");
                   
        }
        scanf("\n");
        

    }
    return major;

}

void print_major(subject_t* major, int num_subjects){
    for(int i = 0 ; i < num_subjects ; i++){
        printf("%s - ", major[i].code);
        for(int j = 0 ; j < major[i].num_prereqs ; j++){
            printf("%s", major[i].pre_req[j]);
            if(j!=major[i].num_prereqs-1){ //Not on last pre-requisite, needs a comma
                printf(", ");
            }
        }
        printf("\n");

    }

}

/**
    @brief Finds the best drone to eliminate a given lantern
    @param drones An array of drone_t structs representing the given drones
    @param num_drones The number of drones in the array
    @param lantern A pointer to a lantern_t struct representing the current lantern
    @param drone_free_time An array of integers representing the time at which each drone will be free to eliminate a new lantern
    @param drone_cur_pos An array of integers representing the current position of each drone
    @return The index of the best drone to eliminate the lantern, or -1 if no drone can eliminate the lantern
**/ 
int remove_prerequisites(subject_t* target, subject_t* prereq) {
    for (int i = 0; i < target->num_prereqs; i++) {
        char* code = target->pre_req[i];
        if (strcmp(code, prereq->code) == 0) {
            free(code);
            for (int j = i; j < target->num_prereqs - 1; j++) {
                target->pre_req[j] = target->pre_req[j + 1];
            }
            target->num_prereqs--;
            return 1;
        }
    }
    return 0;
}

void kahns_toposort(subject_t* major, int num_subjects){
    //TO DO: Complete Kahn's algorithm

    int sorted_count = 0;

    // Allocate an array to store the sorted order of subject codes (as pointers to strings)
    char **code_prints = (char**)malloc(sizeof(char*) * num_subjects);   

    while (sorted_count < num_subjects) {

        int found_index = -1;
        
        // Iterate through the major to find a subject without prerequisities
        for (int i = 0; i < num_subjects; i++) {
            if (major[i].num_prereqs == 0) {
                found_index = i;
                break;
            }
        }

        // If no subject without prerequisites is found, it means there is a cycle or missing prereqs
        if (found_index == -1) {
            printf("Pre-requisites impossible - check for cycles or missing pre-requisites\n");
            free(code_prints);
            return;
        }

        // Add the subject code to the sorted order and mark it as added by setting num_prereqs to -1
        subject_t* current = &major[found_index];
        code_prints[sorted_count] = current->code;
        sorted_count++;
        current->num_prereqs = -1;
        
        // For every subject in the major, remove the current subject from their prereqs if they exist
        for (int i = 0; i < num_subjects; i++) {

            // For speed skip subjects that have already been added to sorted order / have no prereqs
            if (major[i].num_prereqs > 0) {
                remove_prerequisites(&major[i], current);
            }
        }
    }

    printf("Sorted Order:\n");
    for (int i = 0; i < sorted_count; i++) {
        printf("%s\n", code_prints[i]);
    }
    free(code_prints);
}

void free_major(subject_t* major, int num_subjects){
    for(int i = 0; i < num_subjects ; i++){
        //Note: this set, j, will be empty after Kahn's algorithm
        for(int j = 0; j < major[i].num_prereqs ; j++){
            free(major[i].pre_req[j]);
        }
        free(major[i].pre_req);
    }
    free(major);
}


/* =====================================================================
   This skeleton/program is compiled by the comp20007 teaching team,
   the university of Melbourne.
   Last Updated: 16/02/2026
   ================================================================== */

