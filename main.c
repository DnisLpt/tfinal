#include "file.h"

#define FILE_NAME "country_Matrix.bin"

/* Toda a info de um país */
typedef struct country
{
    long active_students;                 // nº de estudantes que estão activos
    long end_students;                    // nº de estudantes que completaram o curso
    long quit_students;                   // nº de estudantes que abandoraram o curso

}Country;

Country *new_Country()
{
    Country *country = malloc(sizeof(Country));
    return country;
}

Country *matrix[ALPHABET_SIZE][ALPHABET_SIZE];   //matriz que vai guardar a informacao de cada pais
bool exist = true;

/* Funcao para adicionar novo estudante <ID, codigo pais> */
void addStudent(FILE *file, FILE *file2, char i[],char j[])
{
    Student *temporary = new_student();

    strcpy(temporary->identifier,i);
    strcpy(temporary->country,j);
    temporary->state = 'I';
    
    if(saveInFile(file,temporary) == 1)
    {
        int a = POS(j[0]);
        int b = POS(j[1]);

        matrix[a][b]->active_students++;
    }
    else
    {
        printf("+ estudante %s existe\n",i);
    }
}

/* Funcao para remover um estudante <ID> */
void removeStudent(FILE *file,FILE *file2,char id[])
{
    Student *temporary = new_student();

    strcpy(temporary->identifier,id);
    int temp = changeInFile(file,temporary, 'R');
    
    if(temp==1)
    {
        int p = POS(current_country[0]);
        int c = POS(current_country[1]);

        matrix[p][c]->active_students--;
    }
}

/* Funcao para assinalar que o estudante terminou o curso <ID> */
void endStudent(FILE *file,FILE *file2,char id[])
{
    Student *temporary = new_student();

    strcpy(temporary->identifier,id);
    int temp = changeInFile(file,temporary, 'T');
    if(temp==1)
    {
        int p = POS(current_country[0]);
        int c = POS(current_country[1]);

        matrix[p][c]->end_students++;
        matrix[p][c]->active_students--;
    }
}

/* Funcao para assinalar que o estudante abandonou o curso <ID> */
void quitStudent(FILE *file,FILE *file2,char id[])
{
    Student *temporary = new_student();

    strcpy(temporary->identifier,id);
    int temp = changeInFile(file,temporary, 'A');
    
    if(temp==1)
    {
        int p = POS(current_country[0]);
        int c = POS(current_country[1]);

        matrix[p][c]->quit_students++;
        matrix[p][c]->active_students--;
    }
}

/* Funcao para obter dados de um pais <codigo pais> */
void singleCountryInfo(char country[],FILE *file2)
{
    int a = POS(country[0]);
    int b = POS(country[1]);

    long total = matrix[a][b]->active_students + matrix[a][b]->end_students + matrix[a][b]->quit_students;
    
    if(total== 0)
    {
        printf("+ sem dados sobre %c%c\n",country[0], country[1]);
    }
    else
    {
        printf("+ %c%c - ", country[0], country[1]);
        printf("correntes: %ld, ", matrix[a][b]->active_students);
        printf("diplomados: %ld, ", matrix[a][b]->end_students);
        printf("abandonaram: %ld, ", matrix[a][b]->quit_students);
        printf("total: %ld\n", total);
    }
    
}
// funcao usada para ler o input dado
int ReadInput(FILE *file,FILE *file2)
{
    char str2[12];  //input lido
    char i[7];  // guardar ID lido
    char j[3];  // guardar codigo pais lido
    char str;   // guarda a instrucao

    while(scanf("%[^\n]", str2) != EOF)
    {
        str = str2[0];  //str passa a ser a letra inicial do input(tipo de operacao a ser feita)
        
        if (str == 'I')  // Introduzir um novo estudante
        {  //a string vai ser cortada para obtermos o ID do estudante e o codigo do pais
            i[0]=str2[2];
            i[1]=str2[3];
            i[2]=str2[4];
            i[3]=str2[5];
            i[4]=str2[6];
            i[5]=str2[7];
            i[6]='\0';
            j[0]=str2[9];
            j[1]=str2[10];
            j[2]='\0';

            addStudent(file,file2,i,j);
        }
        else if (str == 'R')  // Remover um estudante
        {  //a string vai ser cortada para obtermos o ID do estudante
            i[0]=str2[2];
            i[1]=str2[3];
            i[2]=str2[4];
            i[3]=str2[5];
            i[4]=str2[6];
            i[5]=str2[7];
            i[6]='\0';
           
            removeStudent(file,file2,i);
        }
        else if (str == 'T')  // Assinalar que um estudante terminou o curso
        {  //a string vai ser cortada para obtermos o ID do estudante
            i[0]=str2[2];
            i[1]=str2[3];
            i[2]=str2[4];
            i[3]=str2[5];
            i[4]=str2[6];
            i[5]=str2[7];
            i[6]='\0';
            

            endStudent(file,file2,i);
        }
        else if (str == 'A')  // Assinalar que um estudante termonou o curso
        {  //a string vai ser cortada para obtermos o ID do estudante
            i[0]=str2[2];
            i[1]=str2[3];
            i[2]=str2[4];
            i[3]=str2[5];
            i[4]=str2[6];
            i[5]=str2[7];
            i[6]='\0';

            quitStudent(file,file2,i);
        }
        else if (str == 'P')  // Obter dados de um pais
        {  //a string vai ser cortada para obtermos o codigo do pais
            j[0]=str2[2];
            j[1]=str2[3];
            j[2]='\0';
            
            singleCountryInfo(j,file2);
        }
        else if (str == 'X')  // termina a execução do programa
        {
            return -1;
        }
        scanf("%*c");
    }
return 0;
}

/* Funcao para guardar a matrix com a informação dos países no ficheiro respetivo */
void saveMatrix(FILE* file)
{
    fseek(file, 0, SEEK_SET);

    for(int i = 0; i < ALPHABET_SIZE; i++)
    {
        for(int j = 0; j < ALPHABET_SIZE; j++)
        {
            fwrite(matrix[i][j],sizeof(Country),1,file);
        }
    }
}
// Esta funcao carrega o conteudo do file com informacao sobre os paises na matriz
void getMatrixFromFile(FILE * file)
{
    for(int i = 0; i < ALPHABET_SIZE; i++)
    {
        for(int j = 0; j < ALPHABET_SIZE; j++)
        {
            matrix[i][j] = new_Country();
            fread(matrix[i][j],sizeof(Country),1,file);
        }
    }
}

int main()
{
    FILE *file2;
    FILE *fileMatrix = fopen(FILE_NAME, "r+");

    if(fileMatrix == NULL)      //se nao existir ficheiro com uma matriz, entao iremos criar uma em memoria
    {
        exist = false;
        fileMatrix = fopen(FILE_NAME,"w+");
        for (int i = 0; i<ALPHABET_SIZE;i++)    //inicializacao de matriz em memoria a 0's
        {
            for (int j = 0; j<ALPHABET_SIZE;j++)
            {
                matrix[i][j] = new_Country();   
                matrix[i][j]->active_students = 0;
                matrix[i][j]->end_students = 0;
                matrix[i][j]->quit_students = 0;
            }
        }
    }
    else
    {
        exist = true;
        getMatrixFromFile(fileMatrix);  //carregar a informacao dos paises em memoria
    }
        
    file2 = fopen("students_file.bin", "r+");
    if(file2 == NULL)
    {
        file2 = fopen("students_file.bin","w+");
    }

    ReadInput(file2,fileMatrix);

    fileMatrix = fopen(FILE_NAME, "r+");

    saveMatrix(fileMatrix);     //grava a matriz no ficheiro
    fclose(fileMatrix);
    
    return 0;
}
