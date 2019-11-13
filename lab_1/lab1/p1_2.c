#include <stdio.h>
#include <stdlib.h>

typedef struct {
    char name[30];
    int studentID;
    char major[30];
}studentT;

int main(int argc, char* argv[]){

    int i, j, k, Stu_Num;
    
    FILE *stu_info = fopen(argv[1], "rw");
    FILE *stu_output = fopen(argv[2], "w");

    fscanf(stu_info, "%d", &Stu_Num);
    studentT *stuT = malloc(sizeof(studentT)*Stu_Num);

    for(i=0; i<Stu_Num; i++){
        fscanf(stu_info, "%s %d %s", stuT[i].name, &stuT[i].studentID, stuT[i].major);
    }
    fclose(stu_info);

    for(j=0; j<Stu_Num; j++){
        fprintf(stu_output, "%s %d %s\n", stuT[j].name, stuT[j].studentID, stuT[j].major);
    }
    fclose(stu_output);

    for(k=0; k<Stu_Num; k++){
        printf("%s %d %s\n", stuT[k].name, stuT[k].studentID, stuT[k].major);
    }
    printf("\n");

    return 0;
}

