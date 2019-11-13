#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define _CRT_SECURE_NO_WARNINGS
FILE *input;
FILE *output;
typedef struct Node *PtrToNode;
typedef PtrToNode List;
typedef PtrToNode Position;
typedef struct{
    int studentID;
    char* studentName;
} ElementType;
struct Node{
    ElementType element;
    PtrToNode next;
};
//함수선언
void PrintAllNode(List L); //전체노드출력
void PrintCurrent(List L); //현재노드출력
void AddNode(int id, char* name, List L); //노드 삽입
void DelNode(int id, List L); //노드 삭제
Position FindNode(int id, List L); //머리노드부터 차례로  검색
int main(void){
    char idf;
    input = fopen("input.txt", "rw");
    output = fopen("output.txt", "w");
    //List *head = (List *)malloc(sizeof(List));
    //(*head)->next = NULL; //head에 next 인자가 없다고 오류가 뜸
    
    List head = (List)malloc(sizeof(List));
    struct Node header;
    head = &header;
    head->next = NULL;
    /*
     AddNode(2016, "a aa", head);
     AddNode(2017, "b bb", head);
     AddNode(2015, "c cc", head);
     AddNode(2019, "d dd", head);
     AddNode(2017, "AA A", head);
     AddNode(2018, "ee e", head);
     AddNode(2020, "ff f", head);
     
     FindNode(2015, head);
     FindNode(2017, head);
     FindNode(2010, head);
     DelNode(2015, head);
     DelNode(2017, head);
     DelNode(2010, head);
     DelNode(2020, head);
     PrintAllNode(head);
     */
    
    /*    (node->element).studentID = 2016025187;
     (node->element).studentName = "a";
     head->next = node;
     printf("%d %s", (node->element).studentID, (node->element).studentName); //선택노드 출력
     PrintAllNode(head); //전체노드 출력
     PrintCurrent(head); //현재노드 출력
     
     */
    
    int i;
    char name1[30];
    char name2[30];
    while(!feof(input)){
        fscanf(input, "%c", &idf);
        
        switch(idf){
            case 'i' : {
                int id1;
                fscanf(input, "%d %s %s\n", &id1, name1, name2);
                strcat(name1, " ");
                strcat(name1, name2);
                AddNode(id1, name1, head);
                break;
            }
            case 'f' : {
                int id2;
                char name[61] = "";
                fscanf(input, "%d", &id2);
                fscanf(input, "%[^\n]", name);
                FindNode(id2, head);
                break;
            }
            case 'd' : {
                int id3;
                char name[61] =  "";
                fscanf(input, "%d", &id3);
                fscanf(input, "%[^\n]", name);
                DelNode(id3, head);
                break;
            }
            case 'p' :
                PrintAllNode(head);
                return;
        }
    }
    fclose(input);
    fclose(output);
    free(head);
    return 0;
}
void PrintAllNode(List L){
    fprintf(output,"-----LIST-----\n");
    L = L->next;
    while(L != NULL){
        fprintf(output,"%d %s\n", (L->element).studentID, (L->element).studentName);
        L = L->next;
    }
    fprintf(output,"--------------\n");
}
void PrintCurrent(List L){
    L = L->next;
    fprintf(output,"Current List > ");
    while(1){
        fprintf(output,"%d %s", (L->element).studentID, (L->element).studentName);
        if(L->next != NULL)
            fprintf(output,"-");
        else if(L->next == NULL){
            fprintf(output,"\n");
            break;
        }
        L = L->next;
    }
}
void AddNode(int id, char* name, List L){
    Position tmp = malloc(sizeof(Position));
    Position P = L;
    //처음 노드는 무조건 헤더노드 뒤에 삽입
    if(L->next == NULL){
        L->next = tmp;
        (tmp->element).studentID = id;
        
        //(tmp->element).studentName = name;
        //네임이 포인터 이름을가리키고있는
        //=을 하면 스튜던트 네임이 네임을 가리키게된단말이야.
        //이게 반복되면 뒤에 들어온 이름만 계속 가리키게되는거지
        //따로 방을 안만들어주면
        (tmp->element).studentName = (char*)malloc(sizeof(char) * 31);
        strcpy((tmp->element).studentName, name);
        
        tmp->next = NULL;
        fprintf(output,"Insertion Success : %d\n", (tmp->element).studentID);
        PrintCurrent(L);
        
        return;
    }
    while(P != NULL){
        if((P->element).studentID < id){
            if( P->next == NULL ){
                tmp->next = NULL;
                (tmp->element).studentID = id;
                (tmp->element).studentName = (char*)malloc(sizeof(char) * 31);
                strcpy((tmp->element).studentName, name);
                P->next = tmp;
                fprintf(output,"Insertion Success : %d\n", (tmp->element).studentID);
                PrintCurrent(L);
                
                break;
            }
            else if( ((P->next)->element).studentID > id ){
                tmp->next = P->next;
                (tmp->element).studentID = id;
                (tmp->element).studentName = (char*)malloc(sizeof(char) * 31);
                strcpy((tmp->element).studentName, name);
                P->next = tmp;
                fprintf(output,"Insertion Success : %d\n", (tmp->element).studentID);
                PrintCurrent(L);
                
                break;
            }
        } else if((P->element).studentID == id){
            fprintf(output,"Insertion Failed. ID %d already exists.\n", id);
            break;
        } else{
            tmp->next = P->next;
            (tmp->element).studentID = id;
            (tmp->element).studentName = (char*)malloc(sizeof(char) * 31);
            strcpy((tmp->element).studentName, name);
            fprintf(output,"Insertion Success : %d\n", (tmp->element).studentID);
            PrintCurrent(L);
            
            break;
        }
        P = P->next;
    }
}
void DelNode(int id, List L){
    Position P, tmp;
    tmp = P = L;
    if( L == NULL ){
        PrintCurrent(L);
        return;
    }
    if((P->element).studentID == id){
        P->next = L;
        fprintf(output, "Deletion Success : %d\n", id);
        PrintCurrent(L);
    } else {
        while((P->element).studentID != id && P->next != NULL){
            tmp = P;
            P = P->next;
        }
        if((P->element).studentID != id && P->next == NULL){
            fprintf(output, "Deletion Failed. Student ID %d is not in the list.\n", id);
        } else{
            tmp->next = P->next;
            fprintf(output, "Deletion Success : %d\n", id);
            PrintCurrent(L);
        }
    }
    return;
    /*
     Position P;
     if(L == NULL)
     return;
     L->next = P;
     if((P->element).studentID == id){
     printf("Deletion Success : %d\n", id);
     L = P->next;
     P->next = NULL;
     free(P);
     return;
     }
     while(P->next != NULL){
     if(((P->next)->element).studentID == id){
     Position tmp = P->next;
     printf("Deletion Success : %d\n", id);
     if (tmp->next != NULL)
     P->next = tmp->next;
     else
     P->next = NULL;
     
     tmp->next = NULL;
     free(tmp);
     return;
     } else if(((P->next)->element).studentID != id){
     printf("Deletion Failde. Student ID %d is not in the list.\n", id);
     return;
     } else
     P = P->next;
     }
     */
}
Position FindNode(int id, List L){
    Position P = L;
    while(P->next != NULL){
        if( (P->element).studentID == id){
            break;
        }
        P = P->next;
    }
    
    if ( P->next == NULL ){
        fprintf(output,"Find %d Failed. There is no student ID\n", id);
    } else{
        fprintf(output,"Find Success : %d %s\n", (P->element).studentID, (P->element).studentName);
    }
}
