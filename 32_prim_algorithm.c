#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>

/*
최소 신장 트리 (Minimum Spanning Tree, MST):
- 그래프의 모든 정점을 연결하는 부분 그래프
- 사이클이 없는 트리 형태
- 간선들의 가중치 합이 최소
- 네트워크 설계, 배관/도로 설계 등에 활용

Prim vs Kruskal 알고리즘:
1. 동작 방식
   - Prim: 정점 중심, 하나의 트리를 확장
   - Kruskal: 간선 중심, 여러 트리를 합병

2. 적합한 경우
   - Prim: 밀집 그래프(간선이 많은 경우)
   - Kruskal: 희소 그래프(간선이 적은 경우)

3. 시간 복잡도
   - Prim: O(V²) 기본 구현, O(E log V) 힙 사용시
   - Kruskal: O(E log E) 간선 정렬이 지배적

4. 구현 복잡도
   - Prim: 우선순위 큐 사용
   - Kruskal: Union-Find 자료구조 사용
*/

#define MAX_VERTICES 100
#define INF INT_MAX

typedef struct {
    int num_vertices;
    int graph[MAX_VERTICES][MAX_VERTICES];  // 인접 행렬
} Graph;

/* 그래프 생성 */
Graph* graph_create(int vertices) {
    Graph* graph = (Graph*)malloc(sizeof(Graph));
    if (!graph) return NULL;

    graph->num_vertices = vertices;

    // 인접 행렬 초기화
    for (int i = 0; i < vertices; i++) {
        for (int j = 0; j < vertices; j++) {
            graph->graph[i][j] = i == j ? 0 : INF;
        }
    }

    return graph;
}

/* 간선 추가 */
bool graph_add_edge(Graph* graph, int src, int dest, int weight) {
    if (!graph || src < 0 || src >= graph->num_vertices ||
        dest < 0 || dest >= graph->num_vertices) {
        return false;
    }

    // 무방향 그래프이므로 양쪽에 추가
    graph->graph[src][dest] = weight;
    graph->graph[dest][src] = weight;
    return true;
}

/* 최소 키 값을 가진 정점 찾기
 * - 아직 MST에 포함되지 않은 정점 중에서 찾음
 */
int find_min_key(int key[], bool included[], int vertices) {
    int min = INF, min_index;

    for (int v = 0; v < vertices; v++) {
        if (!included[v] && key[v] < min) {
            min = key[v];
            min_index = v;
        }
    }

    return min_index;
}

/* Prim 알고리즘
 * - 시간복잡도: O(V²)
 * - 공간복잡도: O(V)
 */
void prim_mst(Graph* graph, int start) {
    int vertices = graph->num_vertices;
    int* parent = (int*)malloc(vertices * sizeof(int));  // MST를 저장할 배열
    int* key = (int*)malloc(vertices * sizeof(int));     // 최소 가중치
    bool* included = (bool*)malloc(vertices * sizeof(bool));  // MST 포함 여부

    // 초기화
    for (int i = 0; i < vertices; i++) {
        key[i] = INF;
        included[i] = false;
    }

    // 시작 정점 설정
    key[start] = 0;
    parent[start] = -1;  // 시작 정점은 부모가 없음

    // MST 구성 (정점 개수 - 1개의 간선 선택)
    for (int count = 0; count < vertices - 1; count++) {
        // 최소 키 값을 가진 정점 선택
        int u = find_min_key(key, included, vertices);
        included[u] = true;

        // 선택된 정점과 연결된 정점들의 키 값 갱신
        for (int v = 0; v < vertices; v++) {
            if (graph->graph[u][v] && !included[v] &&
                graph->graph[u][v] < key[v]) {
                parent[v] = u;
                key[v] = graph->graph[u][v];
            }
        }
    }

    // MST 출력
    int total_weight = 0;
    printf("\nMinimum Spanning Tree edges:\n");
    for (int i = 1; i < vertices; i++) {
        printf("%d -- %d (weight: %d)\n",
            parent[i], i, graph->graph[i][parent[i]]);
        total_weight += graph->graph[i][parent[i]];
    }
    printf("Total MST weight: %d\n", total_weight);

    // MST의 레벨 구조 출력
    printf("\nMST Level Structure (from vertex %d):\n", start);
    int level[MAX_VERTICES] = { 0 };
    bool visited[MAX_VERTICES] = { false };

    // BFS를 사용하여 레벨 계산
    int queue[MAX_VERTICES];
    int front = 0, rear = 0;

    queue[rear++] = start;
    visited[start] = true;
    level[start] = 0;

    while (front < rear) {
        int current = queue[front++];
        printf("Level %d: Vertex %d\n", level[current], current);

        for (int i = 0; i < vertices; i++) {
            if (parent[i] == current && !visited[i]) {
                queue[rear++] = i;
                visited[i] = true;
                level[i] = level[current] + 1;
            }
        }
    }

    free(parent);
    free(key);
    free(included);
}

/* 그래프 출력 */
void graph_print(const Graph* graph) {
    printf("\nGraph Adjacency Matrix:\n");
    printf("    ");
    for (int i = 0; i < graph->num_vertices; i++) {
        printf("%4d", i);
    }
    printf("\n");

    for (int i = 0; i < graph->num_vertices; i++) {
        printf("%2d: ", i);
        for (int j = 0; j < graph->num_vertices; j++) {
            if (graph->graph[i][j] == INF) {
                printf("  ∞ ");
            }
            else {
                printf("%4d", graph->graph[i][j]);
            }
        }
        printf("\n");
    }
}

/* 그래프 메모리 해제 */
void graph_destroy(Graph* graph) {
    free(graph);
}

/* 메뉴 출력 */
void print_menu(void) {
    printf("\n=== Prim's Algorithm Menu ===\n");
    printf("1. Add edge\n");
    printf("2. Find MST\n");
    printf("3. Print graph\n");
    printf("0. Exit\n");
    printf("Choice: ");
}

int main(void) {
    int vertices;
    printf("Enter number of vertices: ");
    scanf("%d", &vertices);

    Graph* graph = graph_create(vertices);
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
            int src, dest, weight;
            printf("Enter source vertex: ");
            scanf("%d", &src);
            printf("Enter destination vertex: ");
            scanf("%d", &dest);
            printf("Enter weight: ");
            scanf("%d", &weight);

            if (graph_add_edge(graph, src, dest, weight)) {
                printf("Edge added successfully\n");
            }
            else {
                printf("Failed to add edge\n");
            }
            break;
        }

        case 2: {  // Find MST
            int start;
            printf("Enter starting vertex: ");
            scanf("%d", &start);

            if (start >= 0 && start < graph->num_vertices) {
                prim_mst(graph, start);
            }
            else {
                printf("Invalid starting vertex\n");
            }
            break;
        }

        case 3:  // Print graph
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

1. Prim 알고리즘의 특징
-------------------
- 정점 기반 접근
- 하나의 트리 확장
- 인접 정점 중 최소 가중치 선택
- Cut Property 활용

2. Kruskal과의 차이점
-----------------
메모리 사용:
- Prim: 인접 행렬/리스트
- Kruskal: 간선 리스트

성능 특성:
- Prim: 밀집 그래프에 유리
- Kruskal: 희소 그래프에 유리

구현 방식:
- Prim: 우선순위 큐
- Kruskal: Union-Find

3. 시간 복잡도
-----------
기본 구현: O(V²)
- 정점 선택: O(V)
- 키 갱신: O(V)

개선된 구현: O(E log V)
- 우선순위 큐 사용
- 희소 그래프에서 효율적

4. 활용 분야
----------
- 네트워크 설계
- 파이프라인 설계
- 회로 설계
- 통신망 구축

5. 장단점
-------
장점:
- 밀집 그래프에 효율적
- 지역성 좋음
- 간선 정렬 불필요
- 진행 상황 확인 용이

단점:
- 희소 그래프에서 비효율
- 메모리 사용량 多
- 분산 처리 어려움
- 구현 복잡

이 구현은 Prim 알고리즘의 기본
버전을 보여주며, 우선순위 큐를
사용한 개선된 버전으로의 확장이
가능합니다.
*/