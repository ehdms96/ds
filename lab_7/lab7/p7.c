#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct edge{
    int x1;
    int x2;
}edge; //모서리

void init(int num, int *S); //배열 초기화하는 함수
int Find(int x, int *S); //루트값을 반환하는 함수
void Union(int *S, int r1, int r2); //서로 루트가다르다면 하나로 합치는 함수

int **out_matrix; //union에 의해 edge의 벽이 있는지 없는지 체크하는 배열
char **print_matrix; //out_matrix에 의해 미로를 출력

int main(void){
    int array_size, i, j;
    int cnt=0;
    srand((unsigned int)time(NULL));
    
    FILE *input = fopen("input.txt", "r");
    FILE *output = fopen("output.txt", "w");

    fscanf(input,"%d", &array_size);
    int set_size = array_size*array_size+1;
    int edge_size = 2*array_size*(array_size-1);
    int matrix_size = 2*array_size+1;
    int check_union = array_size*array_size;

    //동적할당
    int *S = malloc(sizeof(int)*set_size); //입력받은 수의 정사각형 이중배열을 만들기 위한 배열
    edge *E = malloc(sizeof(edge)*(edge_size+1)); //모서리의 모든값을 저장하여 이중에서 랜덤하게 뽑는 구조체배열
    
    out_matrix = (int**)malloc(sizeof(int*)*(matrix_size+1));
    for(i = 0; i<matrix_size+1; i++){
        out_matrix[i] = (int*)malloc(sizeof(int)*(matrix_size+1));
    }
    print_matrix = (char**)malloc(sizeof(char*)*(matrix_size+1));
    for(i = 0; i<matrix_size+1; i++){
        print_matrix[i] = (char*)malloc(sizeof(char)*(matrix_size+1));
    }
    
    init(set_size, S);

    //입력받은 수의 이차원배열의 존재하는 모든 모서리값을 구조체 배열 E 에 저장
    for(i = 1; i<set_size; i++){
        edge edge_right, edge_bottom;
        edge_right.x1 = i;
        edge_bottom.x1 = i;
        if(i%array_size != 0){
            edge_right.x2 = i+1;
            E[++cnt] = edge_right;
        }
        if((i+array_size) <= (array_size*array_size)){
            edge_bottom.x2 = i+array_size;
            E[++cnt] = edge_bottom;
        }
    }

    //printf("edge input ok\n");

    //루트값이 같지않다면 미로벽 허물기
    while(1){
        int radom_idx = rand()%edge_size+1; //edge개수 중에서 랜덤하게 선택
        edge now = E[radom_idx]; //랜덤하게 선택된 현재 모서리 값
        if(Find(now.x1, S) == Find(now.x2, S)){
            continue;
        }else{
            Union(S, now.x1, now.x2);
            check_union--;

            int k1, k2;
            k1 = now.x1-1; k2 = now.x2-1;

            if(k2-k1==1){
                int n = ((int)(k1/array_size))*2 + 1;
                int m = ((int)(k2%array_size))*2;
                out_matrix[n][m] = 1;
            }else{
                int v = ((int)(k2/array_size))*2;
                int w = ((int)(k1%array_size))*2 + 1;
                out_matrix[v][w] = 1;
            }
        }
        if(check_union == 1)
            break;
    }

    //printf("root equal maze wall delete ok\n");

    //matrix 테두리 : 모든벽이 있는상태
    for(i=0; i<=2*array_size; i++){
        if(i%2 == 0){
            print_matrix[0][i] = '+';
            print_matrix[2*array_size][i] = '+';
            print_matrix[i][0] = '+';
            print_matrix[i][2*array_size] = '+';
        }else{
            print_matrix[0][i] = '-';
            print_matrix[2*array_size][i] = '-';
            print_matrix[i][0] = '|';
            print_matrix[i][2*array_size] = '|';
        }
    }

    //printf("matrix all wall ok\n");

    //out_matrix를 기준으로 뚫려있는 벽 구분해서 미로 안의 벽 세우기
    for(i=1; i<2*array_size; i++){
        if(i%2 == 1){
            for(j=1; j<2*array_size; j++){
                if(j%2==0 && out_matrix[i][j] == 0)
                    print_matrix[i][j] = '|';
                else
                    print_matrix[i][j] = ' ';
            }
        } 
        else{
            for(j=1; j<=2*array_size; j++){
                if(j%2==0)
                    print_matrix[i][j] = '+';
                else{
                    if(out_matrix[i][j] == 0)
                        print_matrix[i][j] = '-';
                    else
                        print_matrix[i][j] = ' ';
                }
            }
        }
    }

    //printf("maze inner wall ok\n");

    //미로의 입구와 출구
    print_matrix[1][0] = ' ';
    print_matrix[2*array_size-1][2*array_size] = ' ';

    //printf("maze's enter exit ok\n");

    //미로 출력
    for(i=0; i<=2*array_size; i++){
        for(j=0; j<=2*array_size; j++){
            fprintf(output, "%c", print_matrix[i][j]);
        }
        fprintf(output, "\n");
    }

    //printf("maze!!!!\n");

    //동적할당 해제
    free(S);
    free(E);    
    for(i = 0; i<=matrix_size; i++){
        free(out_matrix[i]);
    }
    free(out_matrix);
    for(i = 0; i<=matrix_size; i++){
        free(print_matrix[i]);
    }
    free(print_matrix);
    
    //printf("free ok\n");

    //파일닫기
    fclose(input);
    fclose(output);

    //printf("file close ok\n");
    
    return 0;
}

void init(int num, int *S){
    int i=0;
    S[i] = -99;
    for(i=1; i<num; i++)
        S[i] = 0;

}

int Find(int x, int *S){
    if(S[x] <= 0)
        return x;
    else
        return (S[x] = Find(S[x], S));
}

void Union(int *S, int r1, int r2){
    int root1 = Find(r1, S); //r1이 가리키고있는 루트값
    int root2 = Find(r2, S); //r2이 가리키고있는 루트값
    
    if(root1 != 0 && root2 !=0 && root1 == root2) // 같은 루트를 가리키고 있으므로 한 집합안에 있음
        return;
    else{
        // 서로 다른 루트를 가리키고 있고
        if(S[root1] < S[root2]){ // 높이가 다르면 작은것을 큰쪽으로 연결
            S[root2] = root1;
        }
        else if(S[root1] > S[root2]){
            S[root1] = root2;
        }
        else if(S[root1] == S[root2]){ // 높이가 같으면 연결방향은 상관없지만
            S[root1]--; // 높이를 하나 늘려줌(높이를 음수로 표현하므로 --)
            S[root2] = root1;
        }
    }
}
