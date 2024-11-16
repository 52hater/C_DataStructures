#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/*
위상 정렬 (Topological Sort):
- 방향 비순환 그래프(DAG)에서 정점들의 선형적 순서를 찾는 알고리즘
- 선행 관계가 있는 작업들을 순서대로 나열
- 예시:
  * 대학 수강 신청 (선수과목 관계)
  * 프로젝트 일정 관리
  * 의존성 있는 패키지 설치

특징:
1. DAG에서만 가능 (사이클이 없어야 함)
2. 결과가 유일하지 않을 수 있음
3. 진입 차수(in-degree)를 사용
4. DFS 또는 진입 차수 기반으로 구현 가능

여기서는 두 가지 방식 모두 구현:
1. DFS + 스택
2. 진입 차수 + 큐
*/

#define MAX_VERTICES 100

// 큐 구조체 및 함수
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

// 그래프 구조체
typedef struct {
    int num_vertices;
    int** adj_matrix;     // 인접 행렬
    int* in_degree;       // 진입 차수 배열
} Graph;

/* 그래프 생성 */
Graph* graph_create(int vertices) {
    Graph* graph = (Graph*)malloc(sizeof(Graph));
    if (!graph) return NULL;

    graph->num_vertices = vertices;

    // 인접 행렬 할당
    graph->adj_matrix = (int**)malloc(vertices * sizeof(int*));
    for (int i = 0; i < vertices; i++) {
        graph->adj_matrix[i] = (int*)calloc(vertices, sizeof(int));
    }

    // 진입 차수 배열 할당
    graph->in_degree = (int*)calloc(vertices, sizeof(int));

    return graph;
}

/* 간선 추가 */
bool graph_add_edge(Graph* graph, int src, int dest) {
    if (!graph || src < 0 || src >= graph->num_vertices ||
        dest < 0 || dest >= graph->num_vertices) {
        return false;
    }

    graph->adj_matrix[src][dest] = 1;
    graph->in_degree[dest]++;  // 진입 차수 증가
    return true;
}

/* DFS 기반 위상 정렬을 위한 재귀 함수 */
void topological_sort_dfs_util(Graph* graph, int vertex, bool visited[],
    int stack[], int* stack_index) {
    visited[vertex] = true;

    // 인접한 모든 정점에 대해
    for (int i = 0; i < graph->num_vertices; i++) {
        if (graph->adj_matrix[vertex][i] && !visited[i]) {
            topological_sort_dfs_util(graph, i, visited, stack, stack_index);
        }
    }

    // 현재 정점을 스택에 추가
    stack[(*stack_index)++] = vertex;
}

/* DFS 기반 위상 정렬 (스택 사용)
 * - 시간복잡도: O(V + E)
 */
void topological_sort_dfs(Graph* graph) {
    bool* visited = (bool*)calloc(graph->num_vertices, sizeof(bool));
    int* stack = (int*)malloc(graph->num_vertices * sizeof(int));
    int stack_index = 0;

    // 모든 정점에 대해 DFS 수행
    for (int i = 0; i < graph->num_vertices; i++) {
        if (!visited[i]) {
            topological_sort_dfs_util(graph, i, visited, stack, &stack_index);
        }
    }

    printf("Topological Sort (DFS): ");
    for (int i = stack_index - 1; i >= 0; i--) {
        printf("%d ", stack[i]);
    }
    printf("\n");

    free(visited);
    free(stack);
}

/* 진입 차수 기반 위상 정렬 (큐 사용)
 * - 시간복잡도: O(V + E)
 */
void topological_sort_indegree(Graph* graph) {
    Queue queue;
    queue_init(&queue);

    // 진입 차수 배열 복사
    int* in_degree = (int*)malloc(graph->num_vertices * sizeof(int));
    for (int i = 0; i < graph->num_vertices; i++) {
        in_degree[i] = graph->in_degree[i];
        if (in_degree[i] == 0) {
            queue_enqueue(&queue, i);
        }
    }

    printf("Topological Sort (In-degree): ");
    int visited_count = 0;

    while (!queue_is_empty(&queue)) {
        int vertex = queue_dequeue(&queue);
        printf("%d ", vertex);
        visited_count++;

        // 인접한 정점들의 진입 차수 감소
        for (int i = 0; i < graph->num_vertices; i++) {
            if (graph->adj_matrix[vertex][i]) {
                in_degree[i]--;
                if (in_degree[i] == 0) {
                    queue_enqueue(&queue, i);
                }
            }
        }
    }
    printf("\n");

    // 사이클 검사
    if (visited_count != graph->num_vertices) {
        printf("Graph contains a cycle!\n");
    }

    free(in_degree);
}

/* 그래프 출력 */
void graph_print(const Graph* graph) {
    printf("\nAdjacency Matrix:\n");
    for (int i = 0; i < graph->num_vertices; i++) {
        for (int j = 0; j < graph->num_vertices; j++) {
            printf("%d ", graph->adj_matrix[i][j]);
        }
        printf("\n");
    }

    printf("\nIn-degrees: ");
    for (int i = 0; i < graph->num_vertices; i++) {
        printf("%d ", graph->in_degree[i]);
    }
    printf("\n");
}

/* 그래프 메모리 해제 */
void graph_destroy(Graph* graph) {
    if (!graph) return;

    for (int i = 0; i < graph->num_vertices; i++) {
        free(graph->adj_matrix[i]);
    }
    free(graph->adj_matrix);
    free(graph->in_degree);
    free(graph);
}

/* 메뉴 출력 */
void print_menu(void) {
    printf("\n=== Topological Sort Menu ===\n");
    printf("1. Add edge\n");
    printf("2. Topological sort (DFS)\n");
    printf("3. Topological sort (In-degree)\n");
    printf("4. Print graph\n");
    printf("0. Exit\n");
    printf("Choice: ");
}

int main(void) {
    // 예제: 대학 수강 신청 시나리오
    printf("College Course Prerequisites Example\n");
    printf("Courses: \n");
    printf("0: Introduction to Programming\n");
    printf("1: Data Structures\n");
    printf("2: Algorithms\n");
    printf("3: Database\n");
    printf("4: Web Development\n");

    Graph* graph = graph_create(5);
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
            printf("Enter prerequisite course (0-4): ");
            scanf("%d", &src);
            printf("Enter dependent course (0-4): ");
            scanf("%d", &dest);

            if (graph_add_edge(graph, src, dest)) {
                printf("Prerequisite added successfully\n");
            }
            else {
                printf("Failed to add prerequisite\n");
            }
            break;
        }

        case 2:  // Topological sort (DFS)
            topological_sort_dfs(graph);
            break;

        case 3:  // Topological sort (In-degree)
            topological_sort_indegree(graph);
            break;

        case 4:  // Print graph
            graph_print(graph);
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

1. 위상 정렬이란?
-------------
- 방향 그래프의 정점들을 선형으로 정렬
- 모든 간선 (u,v)에 대해 u가 v보다 앞에 위치
- 선행 관계를 만족하는 순서 찾기
- DAG에서만 가능

2. 위상 정렬의 활용
---------------
교육:
- 교과 과정 설계
- 선수과목 체계

프로젝트 관리:
- 작업 순서 결정
- 일정 계획

소프트웨어:
- 빌드 의존성
- 패키지 설치 순서

3. 구현 방식
----------
DFS 기반:
- 스택 사용
- 재귀적 구현
- 완료 순서 역순

진입 차수 기반:
- 큐 사용
- 반복적 구현
- 0차수부터 처리

4. 시간 복잡도
-----------
- 두 방식 모두 O(V + E)
- 인접 행렬: O(V²)
- 인접 리스트: O(V + E)

5. 특징과 제약
-----------
특징:
- 결과 비유일성
- 부분 순서 존재
- 사이클 검출 가능

제약:
- DAG만 가능
- 사이클 불가
- 모호성 존재

6. 응용 예시
----------
- 작업 스케줄링
- 수강 신청 시스템
- 의존성 해석
- 게임 스킬트리

이 구현은 위상 정렬의 두 가지
주요 접근 방식을 모두 보여주며,
실제 응용 사례(수강 신청)를
예제로 제시합니다.
*/