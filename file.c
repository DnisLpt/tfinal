#include "file.h"

#define FILE_NAME "students_file.bin"

/* Valores definidos para usar na funcao hashFile */
#define BASICHASHFICH 3420657955     // hashValue de 000000 que é calculado da forma: 55*36⁰ + 55*36¹ + 55*36² +...+ 55*36⁵
#define DIVHASHFICH 48               // a divisão por 48 é feita para que o ultimo valor de hash possivel (do ID ZZZZZZ)
#define HASHZZZZZZ 498845941         // sizeof(Student)*hashFile("ZZZZZZ") --> que equivale ao final do ficheiro --> tamanho final do ficheiro 

Student *new_student()
{
    Student *student = malloc(sizeof(Student));
    return student;
}

/* 
    Funcao que calcula o código hash de uma string ID a partir dos valores de ASCII para guardar
    no ficheiro binário com uma posição predefinida (o ficheiro vai funcionar como uma hashtable) 
*/
long hashFile(char *str)
{
    long hash = 0;
    for(int index = 0; index < 6; index++ )
    {
        if((int) str[index] < (int) ':')
        {
            hash += ((long) str[index] + 7) * pow(36,6-(index+1));
        }
        else
        {
            hash += ((long) str[index]) * pow(36,6-(index+1));
        }
    }
    return (hash - BASICHASHFICH)/DIVHASHFICH;
}

/*
    Funcao usada para adicionar o estudante student para o ficheiro bin, caso nao exista return 1, se já existir return 0
*/
int saveInFile(FILE *file, Student *student)
{
    Student *testing = new_student();

    long hash = hashFile(student->identifier);

    long position = sizeof(Student)*hash;
    long first_empty_position = -1;
    fseek(file, position, SEEK_SET);
    if(fread(testing, sizeof(Student), 1, file) == 0)
    {
        strcpy(testing->identifier,"");
    }

    if(strcmp(testing->identifier, "") == 0 || (strcmp(testing->identifier, student->identifier) == 0 && testing->state == 'R'))
    {
        fseek(file, position, SEEK_SET);
        fwrite(student,sizeof(Student),1,file);
        return 1;
    }
    if(strcmp(testing->identifier, student->identifier) == 0)
    {
        return 0;
    }
    else
    {
        if(testing->state == 'R')
        {
            first_empty_position = position;
        }
        while(strcmp(testing->identifier, student->identifier) != 0 && strcmp(testing->identifier, "") != 0) //entra em ciclo linear por causa de colisão em ficheiro
        {
            position = position + sizeof(Student);

            if(position > HASHZZZZZZ)
            {
                position = 0;
            }
            fseek(file, position, SEEK_SET);
            if(fread(testing, sizeof(Student), 1, file) == 0)
            {
                strcpy(testing->identifier,"");
            }

            if(strcmp(testing->identifier, student->identifier) == 0)
            {
                if(testing->state != 'R')
                {
                    return 0;
                }
                else
                {
                    fseek(file, position, SEEK_SET);
                    fwrite(student,sizeof(Student),1,file);
                    return 1;
                }
            }

            if(first_empty_position == -1 && testing->state == 'R')
            {
                first_empty_position = position;
            }
        }
    }
    if(first_empty_position == -1)
    {
        fseek(file, position, SEEK_SET);
        fwrite(student,sizeof(Student),1,file);
        return 1;
    }
    else
    {
        fseek(file, first_empty_position, SEEK_SET);
        fwrite(student,sizeof(Student),1,file);
        return 1;
    }
    return 0;
}

/*
    Funcao usada para alterar a informação do estudante student no ficheiro
*/
int changeInFile(FILE *file, Student *student, char estado)
{
    Student *testing = new_student();

    long hash = hashFile(student->identifier);

    long position = sizeof(Student)*hash;
    fseek(file, position, SEEK_SET);
    
    if(fread(testing, sizeof(Student), 1, file) == 0)
    {
        printf("+ estudante %s inexistente\n", student->identifier);
        return 0;
    }

    current_country[0] = testing->country[0];
    current_country[1] = testing->country[1];

    if(strcmp(testing->identifier, "") == 0)
    {
        printf("+ estudante %s inexistente\n", student->identifier);
        return 0;
    }
    if(strcmp(testing->identifier, student->identifier) == 0)
    {
        if(testing->state == 'I')
        {
            fseek(file, position, SEEK_SET);
            fwrite(&estado,sizeof(char),1, file);
            return 1;
        }
        else if(testing->state == 'A')
        {
            printf("+ estudante %s abandonou\n", student->identifier);
            return 0;
        }
        else if(testing->state == 'T')
        {
            printf("+ estudante %s terminou\n", student->identifier);
            return 0;
        }
        else if(testing->state == 'R')
        {
            printf("+ estudante %s inexistente\n", student->identifier);
            return 0;
        }
        
    }
    else
    {
        while(strcmp(testing->identifier, student->identifier) != 0 && strcmp(testing->identifier, "") != 0)
        {
            position = position + sizeof(Student);
            if(position > HASHZZZZZZ)
            {
                position = 0;
            }
            fseek(file, position, SEEK_SET);
            if(fread(testing, sizeof(Student), 1, file) == 0)
            {
                printf("+ estudante %s inexistente\n", student->identifier);
                return 0;
            }

            current_country[0] = testing->country[0];
            current_country[1] = testing->country[1];

            if(strcmp(testing->identifier, student->identifier) == 0)
            {
                if(testing->state == 'I')
                {
                    fseek(file, position, SEEK_SET);
                    fwrite(&estado,sizeof(char),1, file);
                    return 1;
                }
                else if(testing->state == 'A')
                {
                    printf("+ estudante %s abandonou\n", student->identifier);
                    return 0;
                }
                else if(testing->state == 'T')
                {
                    printf("+ estudante %s terminou\n", student->identifier);
                    return 0;
                }
                else if(testing->state == 'R')
                {
                    printf("+ estudante %s inexistente\n", student->identifier);
                    return 0;
                }
                
            }
        }
    }
    printf("+ estudante %s inexistente\n", student->identifier);
    return 0;
}
