#include <stdio.h>
#include <stdlib.h>
#define _CRT_SECURE_NO_WARNINGS

typedef struct Graph{
    int size;
    int *node;
    int **G_matrix;
}Graph;

typedef struct Queue{
    int *key;
    int first;
    int rear;
    int qsize;
    int max_queue_size;
}Queue;

typedef struct Vertex{
    int *node; //==Graph.node
    int *indegree; //==count(Indegree(G_matrix))
    int **V_matrix; //==Graph.G_matrix
}Vertex;

Graph CreateGraph(int nodes[]);
void DeleteGraph(Graph G);
void InsertEdge(Graph G, int a, int b);
int CheckIndegree(Vertex V, int node, Graph G);
void Topsort(Graph G);

int CheckEmptyVertex(Vertex V, Graph G);
Queue MakeNewQueue(int max_Q_size);
void DeleteQueue(Queue *Q);
int isEmpty(Queue *Q); //큐가 비어있는지 확인
int isFull(Queue *Q); //큐가 꽉찼는지 확인 -> 원형큐
void Enqueue(Queue *Q, int x);
int Dequeue(Queue *Q);

FILE *input;
FILE *output;

int main(void) {

    input = fopen("input.txt", "r");
    output = fopen("output.txt", "w");

    int node[6], i, j, edge1, edge2;
    char trash;

    for(i=0; i<6; i++){
        fscanf(input, "%d", &node[i]);
    }
    
    int nodesize = sizeof(node)/sizeof(node[0]);
    printf("sizeof node : %d\n", nodesize);

    //file 첫줄을 통해 node를 받아 그래프 만들기
    Graph G = CreateGraph(node);

    //file 두번째줄을 통해 degree연결
    while(EOF!=fscanf(input, "%d%c%d ", &edge1, &trash, &edge2)){
        InsertEdge(G, edge1, edge2);
    }

    fprintf(output, "Adjacency matrix\n");
    fprintf(output, "  ");
    for(i=0; i<G.size; i++){
      fprintf(output, "%d ", G.node[i]); //가로 1 2 3 6 5 7 출력
    }
    fprintf(output, "\n");
    for(i=0; i<G.size; i++){
      fprintf(output, "%d ", G.node[i]);  //세로 1 2 3 6 5 7 출력
      for(j=0; j<G.size; j++){
        fprintf(output, "%d ", G.G_matrix[i][j]); //위의 InsertEdge(G, edge1, edge2)의 결과
      }
      fprintf(output, "\n");
    }
    fprintf(output, "\n\n");

    //Topological sort
    Topsort(G);

    fclose(input);
    fclose(output);

    //동적할당 해제
    DeleteGraph(G);

    return 0;
}

Graph CreateGraph(int nodes[]){
    Graph G; int i, j;

    G.size = sizeof(nodes)/sizeof(nodes[0]);
    //printf("%d",sizeof(nodes));
    //printf("%d",sizeof(nodes[0]));
    //printf("G.nodes sizeof is : %d\n", G.size);
    G.node = malloc(sizeof(int)*(G.size));
    G.G_matrix = malloc(sizeof(int*)*G.size);

    for(i = 0; i<G.size; i++){
        G.G_matrix[i] = malloc(sizeof(int)*G.size);
    }

    for(i=0; i<G.size; i++){
        G.node[i] = nodes[i];
    }

    for(i=0; i<G.size; i++){
        for(j=0; j<G.size; j++){
            G.G_matrix[i][j] = 0;
        }
    }

    return G;
}

void DeleteGraph(Graph G){
    int i;

    free(G.node);
    for(i=0; i<G.size; i++){
        free(G.G_matrix[i]);
    }
    free(G.G_matrix);

}

void InsertEdge(Graph G, int a, int b){
    int i, index1=0, index2=0;

    //vertex가 들어있는 해당 인덱스를 찾는과정
    for(i=0; i<G.size; i++){
        if(G.node[i] == a){
            index1 = i;
        }
        if(G.node[i] == b){
            index2 = i;
        }
    }

    //찾은 인덱스를 메트릭스로 표현
    G.G_matrix[index1][index2] = 1;
}

int CheckIndegree(Vertex V, int node, Graph G){
    int i, count=0, index=0;
    int V_size = G.size; //해당 그래프의 vertex를 연결(사이즈를 가져오기 위함)

    //해당 노드(vertex)가 들어있는 인덱스를 찾는 과정
    for(i=0; i<V_size; i++){
        if(V.node[i] == node)
            index = i;
    }

    //메트릭스 세로줄 1 체크하여 count
    for(i=0; i<V_size; i++){
        if(V.V_matrix[i][index] == 1)
            count++;
    }

    return count;
}

void Topsort(Graph G){
    Queue Q; Vertex V;
    int i, j;
    int check_node=0, index=0, adjacent=0;

    Q = MakeNewQueue(10);

    //V 동적할당
    V.indegree = malloc(sizeof(int)*(G.size));
    V.node = malloc(sizeof(int)*(G.size));
    V.V_matrix = (int**)malloc(sizeof(int*)*(G.size));
    for(i = 0; i<G.size; i++)
        V.V_matrix[i] = (int*)malloc(sizeof(int)*(G.size));

    //V 초기화
    for(i=0; i<G.size; i++){
        V.node[i] = G.node[i];}
    for(i=0; i<G.size; i++){
        for(j=0; j<G.size; j++){
            V.V_matrix[i][j] = G.G_matrix[i][j];}}
    for(i=0; i<G.size; i++){
        V.indegree[i] = CheckIndegree(V, V.node[i], G);}

    //각각의 꼭짓점에 대해 indegree가 0이면 enQ
    for(i=0; i<G.size; i++){
        if(V.indegree[i] == 0)
            Enqueue(&Q, V.node[i]);
    }

    fprintf(output, "TopSort Result : ");
    
    //Topological sort
    while (CheckEmptyVertex(V, G)!=0){
        check_node = Dequeue(&Q);
        fprintf(output, "%d ", check_node);

        //for문 1 : deQ된 꼭짓점의 인덱스를 찾아서,
        for(i=0; i<G.size; i++){
            if(V.node[i] == check_node)
                index = i;
        }
        
        //for문 2 : deQ한 꼭짓점을 제외하고 다시 of in-degree계산
        for(i=0; i<G.size; i++){
            if(V.V_matrix[index][i] == 1){
                adjacent = V.node[i];

                //deQ된 꼭짓점을 제외하고 indegree를 다시 계산
                V.V_matrix[index][i] = 0;
                V.indegree[i] = CheckIndegree(V, adjacent, G);

                //제외한 후 인접한 꼭짓점의 indegree가 0이 되면 enQ
                if(V.indegree[i] == 0)
                    Enqueue(&Q, V.node[i]);
            }
            //V.V_matrix[index][i] != 1인 경우에는 for2 다시 수행
            else{
              continue;
            }
        }
        
        //CheckEmptyVertex(V, G)==0 일때까지 수행
    }
    //만약 큐에 vertex가 남아있다면 마저 다 출력
    if(!isEmpty(&Q)){
      check_node = Dequeue(&Q);
      fprintf(output, "%d ", check_node);
    }
    fprintf(output, "\n");

    //V 동적할당 해제
    free(V.indegree);
    free(V.node);
    for(i=0; i<G.size; i++){
        free(V.V_matrix[i]);
    }
    free(V.V_matrix);

    //큐 동적할당 해제
    DeleteQueue(&Q);
}

int CheckEmptyVertex(Vertex V, Graph G){
    int i;
    int V_size = G.size;
    int isEmpty=0;

    for(i=0; i<V_size; i++){
        if(V.indegree[i] != 0)
            isEmpty++; //vertex의 indegree가 한개라도 0이 아니면 empty가 아님
    }

    if(isEmpty == 0)
        return 0; //(전체다 0이어야 topsort종료)
    else
        return isEmpty; //false
}

Queue MakeNewQueue(int max_Q_size){
    Queue Q;
    Q.key = malloc(sizeof(int)*max_Q_size);
    Q.first = -1;
    Q.rear = -1;
    Q.qsize = 0;
    Q.max_queue_size = max_Q_size;

    return Q;
}

void DeleteQueue(Queue *Q){
    free(Q->key);
}

//큐가 꽉차있는지 확인
int isFull(Queue *Q){
    int tmp = (Q->rear+1)%Q->max_queue_size; //원형 큐에서 rear+1을 MAX로 나눈 값이
    if(tmp == Q->first) //front와 같으면 큐는 가득 차있는 상태 (Q->qsize == Q->max_queue_size)
        return 1; //true
    else
        return 0; //false
}

//큐가 비어있는지 확인
int isEmpty(Queue *Q){
    if(Q->first == Q->rear) //값이 같으면 큐는 비어있는 상태 (Q->qsize == 0)
        return 1; //true
    else
        return 0; //false
}

void Enqueue(Queue *Q, int x){
    if(isFull(Q))
        fprintf(output, "Full\n");
    else{
        Q->qsize++;
        Q->key[++Q->rear] = x;
    }
}

int Dequeue(Queue *Q){
    if(isEmpty(Q)){
        fprintf(output, "Empty\n");
        return -1;
    }
    else{
        Q->qsize--;
        int dq = Q->key[Q->first+1];
        Q->first = (Q->first+1)%Q->max_queue_size;
        return dq;
    }
}
