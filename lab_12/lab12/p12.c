#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//solution number
#define Linear 1
#define Quadratic 2
#define Double 3
#define del 0

typedef struct HashTbl *HashTable;
struct HashTbl{
    int count;
    int TableSize;
    int *TheLists;
};

void Init(HashTable H, int size);
void Insert(HashTable H, int value, int solution);
void Delete(HashTable H, int value, int solution);
int Find(HashTable H, int value, int solution); //인덱스 반환
void print(HashTable H);
int Hash(int value, int size, int i, int solution); //인덱스 반환

FILE *input;
FILE *output;

int main(void) {
    
    input = fopen("input.txt", "r");
    output = fopen("output.txt", "w");
    
    int testnum;
    fscanf(input, "%d", &testnum); //test 횟수
    
    char probing[10];
    int solution=0;
    int hashSize;
    char idfpq[1];
    int value;
    
    int i=0, j=0, t=testnum;
    for(i; i<t; i++){
        struct HashTbl H;
        
        fscanf(input, "%s", probing); //Linear, Quadratic, Double
        fprintf(output, "%s\n", probing);
        fscanf(input, "%d", &hashSize); //prime number
        
        if(strcmp(probing, "Linear") == 0){
            solution = Linear;
        }
        else if(strcmp(probing, "Quadratic") == 0){
            solution = Quadratic;
        }
        else if(strcmp(probing, "Double") == 0){
            solution = Double;
        }
        
        Init(&H, hashSize);
        
        for(;;){ //insert, find, delete, print, quiet
            fscanf(input, "%s", idfpq);
            if(strcmp(idfpq, "i") == 0){
                fscanf(input, "%d", &value);
                Insert(&H, value, solution);
            }
            else if(strcmp(idfpq, "f") == 0){
                fscanf(input, "%d", &value);
                j = Find(&H, value, solution);
                if( j == -1 ){
                    fprintf(output, "Not found\n");
                }
                else{
                    fprintf(output, "hash table index : %d\n", j);
                }
            }
            else if(strcmp(idfpq, "d") == 0){
                fscanf(input, "%d", &value);
                Delete(&H, value, solution);
            }
            else if(strcmp(idfpq, "p") == 0){
                print(&H);
            }
            else if(strcmp(idfpq, "q") == 0){
                free(H.TheLists);
                fprintf(output, "\n");
                break;
            }
        }
    }
    
    fclose(input);
    fclose(output);
    
    return 0;
}

void Init(HashTable H, int size){
    H->count = 0;
    H->TableSize = size;
    H->TheLists = malloc(sizeof(int)*H->TableSize);
    int i;
    for(i=0; i<H->TableSize; i++){
        H->TheLists[i] = '\0';
    }
}

void Insert(HashTable H, int value, int solution){
    int j;
    if(H->count != 0 && Find(H, value, solution) != -1){ //기존에 이미 존재하는 값이면
        fprintf(output, "Already exists\n");
    }
    else{ //그렇지 않으면 맞는 인덱스를 찾아서 삽입
        for(j=0; ; ){
            if(H->TheLists[ Hash(value, H->TableSize, j, solution) ] == '\0' || //넣으려는 값이 비었거나
               H->TheLists[ Hash(value, H->TableSize, j, solution) ] == del){ //지워진상태라면 빈 공간으로 간주하여
                H->TheLists[ Hash(value, H->TableSize, j, solution) ] = value; //값을 insert
                H->count++; //배열안의 data 개수
                fprintf(output, "Inserted %d\n", value);
                break;
            }
            else{ //그렇지않으면 그 다음 Hash값으로 인덱스반환하여 삽입
                j++;
            }
        }
    }
}

void Delete(HashTable H, int value, int solution){
    if(Find(H, value, solution) == -1){
        fprintf(output, "%d not exists\n", value);
    }
    else{
        H->TheLists[Find(H, value, solution)] = del;
        H->count--;
        fprintf(output, "Deleted %d\n", value);
    }
}

int Find(HashTable H, int value, int solution){
    int i;
    for(i=0;;){
        if(H->count != 0 && H->TheLists[ Hash(value, H->TableSize, i, solution) ] == '\0'){ //null을 만날때까지 find, 이때까지 못찾으면 -1 반환
            return -1;
        }
        else if(H->TheLists[ Hash(value, H->TableSize, i, solution) ] == del){ //삭제된값은 계속해서 찾고
            i++;
            continue;
        }
        else if(H->TheLists[ Hash(value, H->TableSize, i, solution) ] == value){ //찾고자하는 값을 찾으면
            return Hash(value, H->TableSize, i, solution); //index 반환
        }
        else
            i++;
    }
}

void print(HashTable H){
    int i;
    for(i=0; i<H->TableSize; i++){
        if(H->TheLists[i] == '\0'){
            fprintf(output, "%d ", 0);
        }
        else{
            fprintf(output, "%d ", H->TheLists[i]);
        }
    }
    fprintf(output, "\n");
}

int Hash(int value, int size, int i, int solution){ ////Return the hash table index
    switch (solution) {
        case Linear:
            return (value+i)%size;
        case Quadratic:
            return (value+i*i)%size;
        case Double :
            return (value+i*(7-(value%7)))%size;
        default:
            return -1;
    }
}
