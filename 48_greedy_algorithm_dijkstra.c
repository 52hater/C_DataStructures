#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>

/*
데이크스트라 알고리즘의 탐욕적 특성
=============================

1. 탐욕적 선택
- 현재 알려진 최단 거리를 가진 정점 선택
- 선택 후 인접 정점들의 거리 갱신
- 매 단계 선택이 최종 해의 일부

2. 최적성 증명
- 음수 가중치가 없는 경우 최적해 보장
- 삼각 부등식에 의한 정당성
*/

#define MAX_VERTICES 100
#define INF INT_MAX

typedef struct {
    int graph[MAX_VERTICES][MAX_VERTICES];
    int num_vertices;
    bool directed;  // 방향 그래프 여부
} Graph;

typedef struct {
    int distance;
    int parent;
    bool visited;
} VertexInfo;

// 그래프 생성
Graph* create_graph(int num_vertices, bool directed) {
    Graph* g = malloc(sizeof(Graph));
    g->num_vertices = num_vertices;
    g->directed = directed;

    for (int i = 0; i < num_vertices; i++) {
        for (int j = 0; j < num_vertices; j++) {
            g->graph[i][j] = INF;
        }
        g->graph[i][i] = 0;
    }
    return g;
}

// 간선 추가
void add_edge(Graph* g, int src, int dest, int weight) {
    g->graph[src][dest] = weight;
    if (!g->directed) {
        g->graph[dest][src] = weight;
    }
}

// 현재 상태 출력
void print_current_state(VertexInfo* info, int num_vertices, int current) {
    printf("\n현재 상태:\n");
    printf("선택된 정점: %d\n", current);
    printf("정점 | 거리 | 이전 정점\n");
    printf("-----+------+----------\n");

    for (int i = 0; i < num_vertices; i++) {
        printf("%4d | ", i);
        if (info[i].distance == INF)
            printf(" INF |");
        else
            printf("%4d |", info[i].distance);

        if (info[i].parent == -1)
            printf("   - \n");
        else
            printf("%4d\n", info[i].parent);
    }
}

// 최단 경로 추적 및 출력
void print_path(int dest, VertexInfo* info) {
    if (info[dest].parent == -1) {
        printf("%d", dest);
        return;
    }
    print_path(info[dest].parent, info);
    printf(" -> %d", dest);
}

// 탐욕적 선택 과정을 보여주는 데이크스트라 알고리즘
void dijkstra_with_steps(Graph* g, int start) {
    VertexInfo* info = malloc(g->num_vertices * sizeof(VertexInfo));

    // 초기화
    printf("\n=== 데이크스트라 알고리즘 실행 과정 ===\n");
    printf("시작 정점: %d\n", start);
    printf("\n초기화 단계:\n");

    for (int i = 0; i < g->num_vertices; i++) {
        info[i].distance = INF;
        info[i].parent = -1;
        info[i].visited = false;
    }
    info[start].distance = 0;

    print_current_state(info, g->num_vertices, start);

    // 메인 루프
    for (int count = 0; count < g->num_vertices - 1; count++) {
        // 탐욕적 선택: 최소 거리 정점
        int min_distance = INF;
        int min_vertex = -1;

        for (int v = 0; v < g->num_vertices; v++) {
            if (!info[v].visited && info[v].distance < min_distance) {
                min_distance = info[v].distance;
                min_vertex = v;
            }
        }

        if (min_vertex == -1) break;  // 연결되지 않은 그래프

        info[min_vertex].visited = true;
        printf("\n선택된 정점: %d (현재 거리: %d)\n",
            min_vertex, info[min_vertex].distance);

        // 인접 정점 거리 갱신
        for (int v = 0; v < g->num_vertices; v++) {
            if (!info[v].visited &&
                g->graph[min_vertex][v] != INF &&
                info[min_vertex].distance != INF &&
                info[min_vertex].distance + g->graph[min_vertex][v] < info[v].distance) {

                printf("정점 %d의 거리 갱신: %d -> %d (경유: %d)\n",
                    v,
                    info[v].distance == INF ? -1 : info[v].distance,
                    info[min_vertex].distance + g->graph[min_vertex][v],
                    min_vertex);

                info[v].distance = info[min_vertex].distance + g->graph[min_vertex][v];
                info[v].parent = min_vertex;
            }
        }

        print_current_state(info, g->num_vertices, min_vertex);
    }

    // 결과 출력
    printf("\n=== 최종 결과 ===\n");
    for (int i = 0; i < g->num_vertices; i++) {
        if (i != start && info[i].distance != INF) {
            printf("\n%d에서 %d까지의 최단 경로 (거리: %d):\n",
                start, i, info[i].distance);
            print_path(i, info);
            printf("\n");
        }
    }

    free(info);
}

int main(void) {
    int num_vertices, num_edges, start;
    bool directed;

    printf("정점 수 입력: ");
    scanf("%d", &num_vertices);
    printf("간선 수 입력: ");
    scanf("%d", &num_edges);
    printf("방향 그래프입니까? (1: 예, 0: 아니오): ");
    scanf("%d", &directed);

    Graph* graph = create_graph(num_vertices, directed);

    printf("\n간선 정보 입력 (시작점 도착점 가중치):\n");
    for (int i = 0; i < num_edges; i++) {
        int src, dest, weight;
        scanf("%d %d %d", &src, &dest, &weight);
        add_edge(graph, src, dest, weight);
    }

    printf("\n시작 정점 입력: ");
    scanf("%d", &start);

    dijkstra_with_steps(graph, start);

    free(graph);
    return 0;
}

/*
데이크스트라 알고리즘의 탐욕적 정당성
==============================

1. 안전성 증명
-----------
- 삼각 부등식 성질 활용
- 음수 가중치가 없다는 가정 필요
- 방문 순서의 독립성

2. 최적성 보장
-----------
- 부분 경로의 최적성
- Relaxation의 단조성
- 최단 경로의 부분 구조

3. 탐욕적 선택의 의미
----------------
- 각 단계의 선택이 번복되지 않음
- 지역적 최적해가 전역 최적해로 연결
- 선택의 순서가 결과에 영향 없음

4. 구현 최적화
-----------
- 우선순위 큐 사용 가능
- 불필요한 정점 방문 제거
- 조기 종료 조건 활용

이 구현은 데이크스트라 알고리즘의
탐욕적 특성을 강조하고,
각 선택 과정과 그 영향을
시각적으로 보여줍니다.
*/