#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/*
DFS (Depth First Search):
- 깊이 우선 탐색
- 재귀 또는 스택으로 구현
- 한 경로를 끝까지 탐색
- 백트래킹의 기본이 되는 알고리즘
*/

#define MAX_VERTICES 100

// 앞서 구현한 인접 리스트 기반 그래프 구조 재사용
typedef struct Node {
    int vertex;
    struct Node* next;
} Node;

typedef struct {
    Node* adj_list[MAX_VERTICES];
    bool visited[MAX_VERTICES];  // 방문 여부 배열 추가
    int num_vertices;
} Graph;

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

    // 인접 리스트와 방문 배열 초기화
    for (int i = 0; i < vertices; i++) {
        graph->adj_list[i] = NULL;
        graph->visited[i] = false;
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

/* 방문 배열 초기화 */
void reset_visited(Graph* graph) {
    for (int i = 0; i < graph->num_vertices; i++) {
        graph->visited[i] = false;
    }
}

/* DFS 재귀 구현
 * - 시간복잡도: O(V + E)
 * - 공간복잡도: O(V) (재귀 스택)
 */
void dfs_recursive(Graph* graph, int vertex) {
    printf("%d ", vertex);  // 현재 정점 방문
    graph->visited[vertex] = true;

    // 인접한 모든 정점에 대해 재귀적으로 DFS 수행
    for (Node* temp = graph->adj_list[vertex]; temp != NULL; temp = temp->next) {
        if (!graph->visited[temp->vertex]) {
            dfs_recursive(graph, temp->vertex);
        }
    }
}

/* 스택 구현 (반복적 DFS용) */
typedef struct {
    int items[MAX_VERTICES];
    int top;
} Stack;

/* 스택 초기화 */
void stack_init(Stack* s) {
    s->top = -1;
}

/* 스택이 비어있는지 확인 */
bool stack_is_empty(Stack* s) {
    return s->top == -1;
}

/* 스택에 원소 추가 */
void stack_push(Stack* s, int value) {
    s->items[++s->top] = value;
}

/* 스택에서 원소 제거 */
int stack_pop(Stack* s) {
    return s->items[s->top--];
}

/* DFS 반복 구현 (스택 사용)
 * - 시간복잡도: O(V + E)
 * - 공간복잡도: O(V)
 */
void dfs_iterative(Graph* graph, int start) {
    Stack stack;
    stack_init(&stack);
    reset_visited(graph);

    // 시작 정점 처리
    stack_push(&stack, start);

    while (!stack_is_empty(&stack)) {
        int vertex = stack_pop(&stack);

        if (!graph->visited[vertex]) {
            printf("%d ", vertex);
            graph->visited[vertex] = true;

            // 인접한 정점들을 스택에 추가 (역순으로)
            for (Node* temp = graph->adj_list[vertex]; temp != NULL; temp = temp->next) {
                if (!graph->visited[temp->vertex]) {
                    stack_push(&stack, temp->vertex);
                }
            }
        }
    }
    printf("\n");
}

/* 연결 요소 찾기
 * - DFS를 사용하여 그래프의 연결 요소 개수 계산
 */
int find_connected_components(Graph* graph) {
    reset_visited(graph);
    int components = 0;

    for (int v = 0; v < graph->num_vertices; v++) {
        if (!graph->visited[v]) {
            printf("Component %d: ", components + 1);
            dfs_recursive(graph, v);
            printf("\n");
            components++;
        }
    }

    return components;
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
    printf("\n=== Graph DFS Menu ===\n");
    printf("1. Add edge\n");
    printf("2. Print graph\n");
    printf("3. DFS (recursive)\n");
    printf("4. DFS (iterative)\n");
    printf("5. Find connected components\n");
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

        case 3: {  // DFS recursive
            int start;
            printf("Enter starting vertex: ");
            scanf("%d", &start);

            printf("DFS traversal (recursive): ");
            reset_visited(graph);
            dfs_recursive(graph, start);
            printf("\n");
            break;
        }

        case 4: {  // DFS iterative
            int start;
            printf("Enter starting vertex: ");
            scanf("%d", &start);

            printf("DFS traversal (iterative): ");
            dfs_iterative(graph, start);
            break;
        }

        case 5:  // Connected components
            printf("\nFinding connected components...\n");
            int components = find_connected_components(graph);
            printf("Total number of connected components: %d\n", components);
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

1. DFS의 기본 원리
--------------
- 가능한 깊이 먼저 탐색
- 막다른 길에서 백트래킹
- 모든 경로 탐색 보장
- 스택/재귀 활용

2. 구현 방식
----------
재귀적 구현:
- 시스템 스택 활용
- 구현이 단순
- 직관적인 이해
- 깊은 재귀 주의

반복적 구현:
- 명시적 스택 사용
- 메모리 제어 용이
- 스택 오버플로우 방지
- 약간 복잡한 구현

3. 시간 복잡도
-----------
- 전체: O(V + E)
- 정점 방문: O(V)
- 간선 탐색: O(E)
- 연결 요소: O(V + E)

4. 공간 복잡도
-----------
- 방문 배열: O(V)
- 스택 공간: O(V)
- 인접 리스트: O(E)
- 전체: O(V + E)

5. 활용 분야
----------
- 위상 정렬
- 연결 요소 찾기
- 사이클 탐지
- 경로 탐색

6. 주요 특징
----------
- 완전 탐색
- 백트래킹 기반
- 메모리 효율적
- 분할 정복 성질

이 구현은 DFS의 두 가지 방식을
모두 제공하며, 실제 응용에서
자주 사용되는 연결 요소 찾기
기능도 포함합니다.
*/