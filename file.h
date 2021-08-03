#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define ALPHABET_SIZE 26
#define POS(c)  ((c) - 'A')

char current_country[3]; 

/* Toda a info de um estudate */
typedef struct student
{
    char state;                         // 1 letra para identificar o estado (I-para se o estudante ativo,
                                        // T-para se terminou, A-se abandonou, e R-para quando o identificador for removido)
    char country[3];                    // 2 letras maiúsculas
    char identifier[7];                 // cada identificador tem 6 letra ou valor numerico de 0 a 9

}Student;

long hashFile(char *str);
int saveInFile(FILE *file, Student *student);
int changeInFile(FILE *file, Student *student, char estado);
bool checkInFile(FILE *file, Student *student);
void file_init();
Student *new_student();
