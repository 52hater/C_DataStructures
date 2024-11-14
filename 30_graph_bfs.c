#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/*
BFS (Breadth First Search):
- 너비 우선 탐색
- 큐를 사용하여 구현
- 같은 레벨의 정점들을 먼저 탐색
- 최단 경로 찾기의 기본이 되는 알고리즘
*/

#define MAX_VERTICES 100
#define INFINITY 999999

// 인접 리스트 그래프 구조 유지
typedef struct Node {
    int vertex;
    struct Node* next;
} Node;

typedef struct {
    Node* adj_list[MAX_VERTICES];
    bool visited[MAX_VERTICES];   // 방문 배열
    int level[MAX_VERTICES];      // 각 정점의 레벨(시작점으로부터의 거리)
    int parent[MAX_VERTICES];     // 각 정점의 부모 정점
    int num_vertices;
} Graph;

/* 큐 구현 */
typedef struct {
    int items[MAX_VERTICES];
    int front;
    int rear;
} Queue;

/* 큐 초기화 */
void queue_init(Queue* q) {
    q->front = -1;
    q->rear = -1;
}

/* 큐가 비어있는지 확인 */
bool queue_is_empty(const Queue* q) {
    return q->front == -1;
}

/* 큐에 원소 추가 */
void queue_enqueue(Queue* q, int value) {
    if (q->front == -1)
        q->front = 0;
    q->items[++q->rear] = value;
}

/* 큐에서 원소 제거 */
int queue_dequeue(Queue* q) {
    int item = q->items[q->front];
    if (q->front == q->rear) {
        q->front = -1;
        q->rear = -1;
    }
    else {
        q->front++;
    }
    return item;
}

/* 새로운 노드 생성 */
Node* create_node(int v) {
    Node* new_node = (Node*)malloc(sizeof(Node));
    if (new_node) {
        new_node->vertex = v;
        new_node->next = NULL;
    }
    return new_node;
}

/* 그래프 생성 */
Graph* graph_create(int vertices) {
    Graph* graph = (Graph*)malloc(sizeof(Graph));
    if (!graph) return NULL;

    graph->num_vertices = vertices;

    for (int i = 0; i < vertices; i++) {
        graph->adj_list[i] = NULL;
        graph->visited[i] = false;
        graph->level[i] = INFINITY;
        graph->parent[i] = -1;
    }

    return graph;
}

/* 간선 추가 */
bool graph_add_edge(Graph* graph, int src, int dest) {
    // src에서 dest로의 간선 추가
    Node* new_node = create_node(dest);
    if (!new_node) return false;

    new_node->next = graph->adj_list[src];
    graph->adj_list[src] = new_node;

    // 무방향 그래프를 위한 반대 방향 간선 추가
    new_node = create_node(src);
    if (!new_node) return false;

    new_node->next = graph->adj_list[dest];
    graph->adj_list[dest] = new_node;

    return true;
}

/* 방문 배열과 레벨 배열 초기화 */
void reset_arrays(Graph* graph) {
    for (int i = 0; i < graph->num_vertices; i++) {
        graph->visited[i] = false;
        graph->level[i] = INFINITY;
        graph->parent[i] = -1;
    }
}

/* BFS 구현
 * - 시간복잡도: O(V + E)
 * - 공간복잡도: O(V)
 */
void bfs(Graph* graph, int start) {
    reset_arrays(graph);
    Queue queue;
    queue_init(&queue);

    // 시작 정점 처리
    graph->visited[start] = true;
    graph->level[start] = 0;
    queue_enqueue(&queue, start);

    printf("BFS traversal: ");

    while (!queue_is_empty(&queue)) {
        int current = queue_dequeue(&queue);
        printf("%d ", current);

        // 인접한 모든 정점에 대해
        for (Node* temp = graph->adj_list[current]; temp != NULL; temp = temp->next) {
            if (!graph->visited[temp->vertex]) {
                graph->visited[temp->vertex] = true;
                graph->level[temp->vertex] = graph->level[current] + 1;
                graph->parent[temp->vertex] = current;
                queue_enqueue(&queue, temp->vertex);
            }
        }
    }
    printf("\n");
}

/* 특정 정점까지의 최단 경로 출력 */
void print_shortest_path(const Graph* graph, int start, int end) {
    if (start == end) {
        printf("%d", start);
        return;
    }

    if (graph->parent[end] == -1) {
        printf("No path exists from %d to %d", start, end);
        return;
    }

    print_shortest_path(graph, start, graph->parent[end]);
    printf(" -> %d", end);
}

/* 레벨 별 정점 출력 */
void print_levels(const Graph* graph) {
    printf("\nLevels from source:\n");
    for (int i = 0; i < graph->num_vertices; i++) {
        if (graph->level[i] != INFINITY) {
            printf("Vertex %d: Level %d\n", i, graph->level[i]);
        }
    }
}

/* 그래프 출력 */
void graph_print(const Graph* graph) {
    printf("\nGraph Adjacency List:\n");
    for (int i = 0; i < graph->num_vertices; i++) {
        printf("[%d]", i);
        for (Node* temp = graph->adj_list[i]; temp != NULL; temp = temp->next) {
            printf(" -> %d", temp->vertex);
        }
        printf("\n");
    }
}

/* 그래프 메모리 해제 */
void graph_destroy(Graph* graph) {
    if (!graph) return;

    for (int i = 0; i < graph->num_vertices; i++) {
        Node* current = graph->adj_list[i];
        while (current) {
            Node* temp = current;
            current = current->next;
            free(temp);
        }
    }

    free(graph);
}

/* 메뉴 출력 */
void print_menu(void) {
    printf("\n=== Graph BFS Menu ===\n");
    printf("1. Add edge\n");
    printf("2. Print graph\n");
    printf("3. BFS traversal\n");
    printf("4. Print shortest path\n");
    printf("5. Print vertex levels\n");
    printf("0. Exit\n");
    printf("Choice: ");
}

int main(void) {
    int num_vertices;
    printf("Enter number of vertices: ");
    scanf("%d", &num_vertices);

    Graph* graph = graph_create(num_vertices);
    if (!graph) {
        printf("Failed to create graph\n");
        return 1;
    }

    int choice;
    do {
        print_menu();
        scanf("%d", &choice);

        switch (choice) {
        case 1: {  // Add edge
            int src, dest;
            printf("Enter source vertex: ");
            scanf("%d", &src);
            printf("Enter destination vertex: ");
            scanf("%d", &dest);

            if (graph_add_edge(graph, src, dest)) {
                printf("Edge added successfully\n");
            }
            else {
                printf("Failed to add edge\n");
            }
            break;
        }

        case 2:  // Print graph
            graph_print(graph);
            break;

        case 3: {  // BFS traversal
            int start;
            printf("Enter starting vertex: ");
            scanf("%d", &start);

            if (start >= 0 && start < graph->num_vertices) {
                bfs(graph, start);
            }
            else {
                printf("Invalid vertex\n");
            }
            break;
        }

        case 4: {  // Print shortest path
            int start, end;
            printf("Enter start vertex: ");
            scanf("%d", &start);
            printf("Enter end vertex: ");
            scanf("%d", &end);

            if (start >= 0 && start < graph->num_vertices &&
                end >= 0 && end < graph->num_vertices) {
                bfs(graph, start);  // BFS 실행하여 최단 경로 정보 갱신
                printf("Shortest path: ");
                print_shortest_path(graph, start, end);
                printf("\nDistance: %d\n", graph->level[end]);
            }
            else {
                printf("Invalid vertices\n");
            }
            break;
        }

        case 5:  // Print levels
            print_levels(graph);
            break;

        case 0:  // Exit
            printf("Exiting program\n");
            break;

        default:
            printf("Invalid choice\n");
        }
    } while (choice != 0);

    graph_destroy(graph);
    return 0;
}

/*
==========================================
상세 설명 및 주요 개념
==========================================

1. BFS의 기본 원리
--------------
- 레벨 단위 탐색
- 큐 자료구조 사용
- 최단 경로 보장
- 모든 정점 방문

2. 구현 특징
----------
- 큐 기반 구현
- 레벨 정보 유지
- 부모 정보 저장
- 경로 추적 가능

3. 시간 복잡도
-----------
- 전체: O(V + E)
- 정점 방문: O(V)
- 간선 탐색: O(E)
- 경로 출력: O(V)

4. 공간 복잡도
-----------
- 큐 공간: O(V)
- 방문 배열: O(V)
- 레벨/부모 배열: O(V)
- 전체: O(V)

5. 활용 분야
----------
- 최단 경로 찾기
- 네트워크 홉 계산
- 웹 크롤링
- 최소 이동 거리

6. 장단점
-------
장점:
- 최단 경로 보장
- 레벨 단위 처리
- 완전 탐색
- 구현 단순

단점:
- 메모리 사용량
- 깊은 경로 비효율
- 가중치 고려 못함
- 공간 복잡도

이 구현은 BFS의 기본 기능과 함께
최단 경로 찾기, 레벨 정보 확인 등
교육적으로 중요한 기능들을
포함하고 있습니다.
*/