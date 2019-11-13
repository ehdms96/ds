#include <stdio.h>
#include <stdlib.h>

#define Inf 99999

//priorty Queue -> min heap -> delete를 할때 weight가 가장 작은애가 나올수있도록
//input 첫째줄 : 3자리수, 갯수는 백개까지
//input 둘째줄 : 1에서 2로가는게 2만큼 weight
//input 셋째줄 : 3에서 5로가는 쇼트패스를 구해라 => 경로를 구하고 없다면 노패쓰

typedef struct Node{
    int key;
    int weight;
}Node;

typedef struct Heap{
    int capacity;
    int size;
    Node *element;
}Heap;

void createMinHeap(Heap *H, int heapSize);
void insert(Heap *H, int vertex, int distance);
int findHeap(Heap *H, int vertex);
Node deleteMin(Heap *H);
void printShortestPath(int *pred, int size);

void createAdj(int size);
void insertEdge(Node *N, int size, int node1, int node2, int weight);

//void printAll(Heap *H); //test
int IsFull(Heap *H);
Node MinNode(Node a, Node b);
int Min(int a, int b);

FILE *input;
FILE *output;

Node *dist; //가장 짧은 거리(갸중치)를 누적한 일차원 배열
int *pred; //이전 노드의 값을 저장한 일차원 배열
int **adj; //노드사이의 가중치 값을 저장한 dist 이차원 배열

int main(void) {
    
    input = fopen("input.txt", "r");
    output = fopen("output.txt", "w");
    
    int i=0, index=0, key, node1, node2, weight, start, end;
    char trash;
    
    //첫줄읽기
    dist = malloc(sizeof(Node)*(100));
    //dist배열 초기화
    while(1){
        fscanf(input, "%d", &key);
        fscanf(input, "%c", &trash); //공백
        
        dist[i].key = key;
        dist[i].weight = Inf;
        
        if(trash == ' ')
            trash = 'a';
        if(trash != 'a'){
            index = i+1;
            break;
        }
        i++;
    } //index = 7, dist배열에 key 1~7 에 대해 INF로 초기화 된 상태
    
    //pred배열 초기화
    pred = malloc(sizeof(int)*index);
    
    
    //두번째줄 읽기
    createAdj(index); //->adj 가중치 입력전, inf로 초기화
    while(1){
        fscanf(input, "%d", &node1);
        fscanf(input, "%c", &trash); //-
        fscanf(input, "%d", &node2);
        fscanf(input, "%c", &trash); //-
        fscanf(input, "%d", &weight);
        fscanf(input, "%c", &trash); //-
        if(trash == ' ' || trash == '-')
            trash = 'a';
        insertEdge(dist, index, node1, node2, weight); //->adj 엣지 표시
        if(trash != 'a')
            break;88888
    } //이차원배열에 노드의 경로 가중치가 저장된 상태
    
    //세번째줄 읽기
    fscanf(input, "%d %d", &start, &end);
    //dist의 시작점 초기화
    for(i=0; i<index; i++){
        if(dist[i].key == start)
            dist[i].weight = 0;
    } //dist배열에서 시작점 3의 weight가 0으로 저장된 상태
    
    //dijkstra
    printShortestPath(pred, index);

    int *root = malloc(sizeof(int)*index);
    for(i=0; i<index; i++){
        root[i] = 0;
    }
    
    int j, index2=0;
    for(i=0; i<index; i++){
        if(dist[i].key == end){
            index2 = i;
        }
    }
    root[0] = end;
    
    int fail=0;
    //pred배열을 통해 경로 입력
    for (i=1; i<index; i++) {
        root[i] = pred[index2];
        
        if(dist[index2].weight == Inf){
            fail = 1;
        }
        
        if(pred[index2] == '\0'){
            break;
        }
            
        for(j=0; j<index; j++){
            if(dist[j].key == root[i]){
                index2 = j;
            }
        }
    }
    
    //경로가 없을경우
    if(fail == 1){
        fprintf(output, "no path\n");
    }
    //경로가있을경우
    else{
        //root배열을 따라 경로 출력
        int num=0;
        for(i=0; i<index; i++){
            if(root[i] == '\0'){
                num = i;
                break;
            }
        }
        for(i=num-1; i>=0; i--){
            fprintf(output, "%d ", root[i]);
        }
        fprintf(output, "\n");
    }
    
    //free
    free(dist);
    for(i=0; i<index; i++){
        free(adj[i]);
    }
    free(adj);
    free(pred);
    free(root);
    
    fclose(input);
    fclose(output);
    
    return 0;
    
}

void createMinHeap(Heap *H, int heapSize){
    int i;
    H->capacity = heapSize;
    H->size = 0;
    H->element = malloc(sizeof(Node)*(H->capacity));
    for(i=0; i<H->capacity; i++){
        H->element[i].key = '\0';
        H->element[i].weight = '\0';
    }
    H->element[0].key = -9999;
    H->element[0].weight = -9999;
}

void insert(Heap *H, int vertex, int distance){
    int i;
    int f_num = findHeap(H, vertex); //vertex가 이미존재하는지 안하는지 판단하고, 있다면 그 index를 반환
    
    if(IsFull(H)==1){
        fprintf(output, "Priority Queue is Full\n");
        return;
    }
    
    //find한 vertex값이 이미 존재하면
    if(f_num>=0 && H->element[f_num].key == vertex){
        //distance(각 노드의 weight를 합한 값)을 비교하여 작으면 데이터를 업데이트한다
        if(H->element[f_num].weight >= distance){
            H->element[f_num].weight = distance;
            return;
        }
        //비교한 값이 작지않으면 무시한다
        else
            return;
    }
    
    //기존에 vertex가 없으면, 해당 vertex의 distance값을 삽입한다
    for(i=++H->size; H->element[i/2].weight > distance; i/=2){
        H->element[i] = H->element[i/2];
    }
    H->element[i].weight = distance;
    H->element[i].key = vertex;
}

int findHeap(Heap *H, int vertex){
    int i;
    for(i=1; i<=H->capacity; i++){
        if(H->element[i].key == vertex)
            return i;
    }
    return -1;
}

Node deleteMin(Heap *H){
    int i, child;
    Node min = H->element[1];
    Node last = H->element[H->size--];
    
    //PercDown
    for(i=1; i*2<=H->size; i=child){
        child = i*2;
        if(child != H->size && H->element[child+1].weight < H->element[child].weight)
            child++;
        if(last.weight > H->element[child].weight){
            H->element[i].key = H->element[child].key;
            H->element[i].weight = H->element[child].weight;
        }
        else
            break;
    }
    H->element[i] = last;
    H->element[H->size+1].key = '\0';
    H->element[H->size+1].weight = '\0';
    return min;
}

void printShortestPath(int *pred, int size){
    Heap minH;
    Node min;
    createMinHeap(&minH, 10);
    int index=0;
    int i;
    
    for(i=0; i<size; i++){
        insert(&minH, dist[i].key, dist[i].weight);
    }
    //priority Queue (min heap)이 빌때까지
    do{
        //힙에서 가장 위에 있는값을 꺼낸다
        min = deleteMin(&minH);
        
        //기존에 있던 dist의 weight값과 min의 distance를 비교하여 더 작은값으로 저장한다.
        for(i=0; i<size; i++){
            if(dist[i].key == min.key)
                index = i;
        }
        //같거나 크면 연산을 한 후 더 작아지는 정점들을 큐에 넣는다
        if(dist[index].weight >= min.weight){
            dist[index] = MinNode(dist[index], min);
            
            //인접 정점을 계산하고 기존의 dist보다 더 작아지는 정점을 큐에 넣는다
            for(i=0; i<size; i++){
                if(adj[index][i] != Inf){
                    if(dist[i].weight >= dist[index].weight+adj[index][i]){
                        dist[i].weight = Min(dist[i].weight, dist[index].weight+adj[index][i]);
                        pred[i] = dist[index].key;
                        insert(&minH, dist[i].key, dist[i].weight);
                    }
                }
                else
                    continue;
            }
        }
        else
            continue;
    }while(minH.size>0);
    
    for(i=0; i<size; i++){
        if(dist[i].weight == 0){
            pred[i] = '\0';
        }
    }
    
    /*test
    for(i=0; i<size; i++){
        printf("%5d ", i);
    }printf("\n");
    for(i=0; i<size; i++){
    printf("%5d ", dist[i].key);
    }printf("\n");
    for(i=0; i<size; i++){
        printf("%5d ", dist[i].weight);
     }printf("\n");
    for(i=0; i<size; i++){
        printf("%5d ", pred[i]);
    }printf("\n");*/
    
    free(minH.element);
}

void createAdj(int size){
    int i, j;
    adj = malloc(sizeof(int*)*size);
    for(i=0; i<size; i++){
        adj[i] = malloc(sizeof(int)*size);
    }
    
    for(i=0; i<size; i++){
        for(j=0; j<size; j++){
            adj[i][j] = Inf;
        }
    }
}

void insertEdge(Node *N, int size, int node1, int node2, int weight){
    int i, index1=0, index2=0;
    for(i=0; i<size; i++){
        if(N[i].key == node1)
            index1 = i;
        if(N[i].key == node2)
            index2 = i;
    }
    adj[index1][index2] = weight;
}
/*test
void printAll(Heap *H){
    int i;
    printf("H's key : ");
    for(i=0; i<H->capacity; i++){
        printf("%d ", H->element[i].key);
    }
    printf("\n");
}
*/
int IsFull(Heap *H){
    if(H->size == (H->capacity-1))
        return 1;
    else return 0;
}

Node MinNode(Node a, Node b){
    if(a.weight<=b.weight)
        return a;
    else
        return b;
}

int Min(int a, int b){
    if(a<=b)
        return a;
    else
        return b;
}
