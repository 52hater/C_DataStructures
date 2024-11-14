#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>

/*
최단 경로 (Shortest Path):
- 두 정점 사이의 경로 중 간선의 가중치 합이 최소인 경로
- 네비게이션, 네트워크 라우팅 등에서 핵심 개념
- 다양한 변형: 단일 출발점, 모든 쌍, 음수 가중치 등

Dijkstra 알고리즘:
- 단일 출발점 최단 경로 알고리즘
- 그리디(Greedy) 방식으로 동작
- 음수 가중치가 없는 그래프에서만 사용 가능
- 우선순위 큐를 사용하여 최적화 가능

다른 최단 경로 알고리즘과의 비교:
1. Bellman-Ford: 음수 가중치 처리 가능, 더 느림
2. Floyd-Warshall: 모든 정점 쌍의 최단 경로, O(V³)
3. A*: 휴리스틱을 사용한 방향성 있는 탐색
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
        dest < 0 || dest >= graph->num_vertices || weight < 0) {
        return false;
    }

    graph->graph[src][dest] = weight;
    return true;
}

/* 최소 거리 정점 찾기
 * - 아직 방문하지 않은 정점 중 최소 거리를 가진 정점 반환
 */
int find_min_distance(const int dist[], const bool visited[], int vertices) {
    int min = INF;
    int min_index = -1;

    for (int v = 0; v < vertices; v++) {
        if (!visited[v] && dist[v] < min) {
            min = dist[v];
            min_index = v;
        }
    }

    return min_index;
}

/* 경로 출력
 * - 시작점에서 목적지까지의 경로를 재귀적으로 출력
 */
void print_path(const int parent[], int dest) {
    if (parent[dest] == -1) {
        printf("%d", dest);
        return;
    }
    print_path(parent, parent[dest]);
    printf(" -> %d", dest);
}

/* Dijkstra 알고리즘
 * - 시간복잡도: O(V²)
 * - 공간복잡도: O(V)
 */
void dijkstra(Graph* graph, int start) {
    int vertices = graph->num_vertices;
    int* dist = (int*)malloc(vertices * sizeof(int));     // 최단 거리
    bool* visited = (bool*)malloc(vertices * sizeof(bool)); // 방문 여부
    int* parent = (int*)malloc(vertices * sizeof(int));    // 경로 추적용

    // 초기화
    for (int i = 0; i < vertices; i++) {
        dist[i] = INF;
        visited[i] = false;
        parent[i] = -1;
    }

    // 시작 정점 설정
    dist[start] = 0;

    // 모든 정점에 대해
    for (int count = 0; count < vertices - 1; count++) {
        // 최소 거리 정점 찾기
        int u = find_min_distance(dist, visited, vertices);
        if (u == -1) break;  // 연결되지 않은 정점이 있는 경우

        visited[u] = true;

        // 선택된 정점의 인접 정점들의 거리 갱신
        for (int v = 0; v < vertices; v++) {
            // 방문하지 않은 정점이고, 간선이 존재하고
            // 현재까지의 경로보다 새로운 경로가 더 짧은 경우
            if (!visited[v] &&
                graph->graph[u][v] != INF &&
                dist[u] != INF &&
                dist[u] + graph->graph[u][v] < dist[v]) {
                dist[v] = dist[u] + graph->graph[u][v];
                parent[v] = u;
            }
        }

        // 현재 상태 출력
        printf("\nIteration %d:\n", count + 1);
        printf("Selected vertex: %d\n", u);
        printf("Current distances: ");
        for (int i = 0; i < vertices; i++) {
            if (dist[i] == INF)
                printf("INF ");
            else
                printf("%3d ", dist[i]);
        }
        printf("\n");
    }

    // 결과 출력
    printf("\nFinal Shortest Paths from vertex %d:\n", start);
    for (int i = 0; i < vertices; i++) {
        if (i != start && dist[i] != INF) {
            printf("To %d (distance = %d): ", i, dist[i]);
            print_path(parent, i);
            printf("\n");
        }
    }

    free(dist);
    free(visited);
    free(parent);
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
    printf("\n=== Dijkstra's Algorithm Menu ===\n");
    printf("1. Add edge\n");
    printf("2. Find shortest paths\n");
    printf("3. Print graph\n");
    printf("0. Exit\n");
    printf("Choice: ");
}

/* 메인 함수 */
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
            printf("Enter weight (non-negative): ");
            scanf("%d", &weight);

            if (graph_add_edge(graph, src, dest, weight)) {
                printf("Edge added successfully\n");
            }
            else {
                printf("Failed to add edge\n");
            }
            break;
        }

        case 2: {  // Find shortest paths
            int start;
            printf("Enter starting vertex: ");
            scanf("%d", &start);

            if (start >= 0 && start < graph->num_vertices) {
                dijkstra(graph, start);
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

1. 최단 경로의 특성
----------------
- 최적 부분 구조
- 삼각 부등식 성질
- 음수 사이클 없어야 함
- 유일성은 보장되지 않음

2. Dijkstra 알고리즘 원리
---------------------
- 그리디 선택 속성
- 점진적 경로 확장
- 최단 거리 확정
- 경로 갱신

3. 시간 복잡도 분석
---------------
기본 구현: O(V²)
- 정점 선택: O(V)
- 거리 갱신: O(V)

개선된 구현: O(E log V)
- 우선순위 큐 사용
- 희소 그래프에 효과적

4. 활용 분야
----------
- 네트워크 라우팅
- 네비게이션
- 소셜 네트워크
- 게임 AI 경로찾기

5. 알고리즘의 제약사항
------------------
- 음수 가중치 불가
- 밀집 그래프에서 비효율
- 메모리 사용량 多
- 병렬화 어려움

6. 최적화 기법
-----------
- 우선순위 큐 사용
- 양방향 검색
- 목적지 기반 가지치기
- 휴리스틱 활용 가능

이 구현은 Dijkstra 알고리즘의
기본 버전을 보여주며, 실제
경로와 진행 과정을 시각적으로
확인할 수 있도록 했습니다.
*/