#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

/*
알고리즘 분류: 그래프 알고리즘
하위 분류: 최단 경로 알고리즘
난이도: 중상
관련 알고리즘: 다익스트라, 플로이드-워셜

벨만-포드 알고리즘:
- 음수 가중치 허용
- 음수 사이클 감지 가능
- 단일 출발점 최단 경로
- O(VE) 시간복잡도
*/

typedef struct {
    int source;     // 시작 정점
    int dest;       // 도착 정점
    int weight;     // 가중치
} Edge;

typedef struct {
    Edge* edges;    // 간선 배열
    int V;          // 정점 수
    int E;          // 간선 수
} Graph;

// 그래프 생성
Graph* create_graph(int V, int E) {
    Graph* graph = (Graph*)malloc(sizeof(Graph));
    graph->V = V;
    graph->E = E;
    graph->edges = (Edge*)malloc(E * sizeof(Edge));
    return graph;
}

// 거리 배열과 경로 배열 초기화
void initialize_single_source(Graph* graph, int* dist, int* prev, int source) {
    for (int i = 0; i < graph->V; i++) {
        dist[i] = INT_MAX;
        prev[i] = -1;
    }
    dist[source] = 0;
}

// 경로 출력
void print_path(int* prev, int vertex) {
    if (vertex == -1) return;
    print_path(prev, prev[vertex]);
    printf("%d ", vertex);
}

// 벨만-포드 알고리즘
bool bellman_ford(Graph* graph, int source, bool print_steps) {
    int* dist = (int*)malloc(graph->V * sizeof(int));
    int* prev = (int*)malloc(graph->V * sizeof(int));

    initialize_single_source(graph, dist, prev, source);

    printf("\n=== 벨만-포드 알고리즘 실행 ===\n");
    printf("출발 정점: %d\n", source);

    // 정점 수 - 1번 반복
    for (int i = 1; i <= graph->V - 1; i++) {
        if (print_steps) {
            printf("\n반복 %d:\n", i);
        }

        // 모든 간선에 대해 완화(relax) 수행
        for (int j = 0; j < graph->E; j++) {
            int u = graph->edges[j].source;
            int v = graph->edges[j].dest;
            int weight = graph->edges[j].weight;

            if (dist[u] != INT_MAX &&
                dist[u] + weight < dist[v]) {
                dist[v] = dist[u] + weight;
                prev[v] = u;

                if (print_steps) {
                    printf("간선 %d->%d 완화: 거리[%d] = %d\n",
                        u, v, v, dist[v]);
                }
            }
        }
    }

    // 음수 사이클 검사
    for (int i = 0; i < graph->E; i++) {
        int u = graph->edges[i].source;
        int v = graph->edges[i].dest;
        int weight = graph->edges[i].weight;

        if (dist[u] != INT_MAX &&
            dist[u] + weight < dist[v]) {
            printf("\n경고: 음수 사이클 발견!\n");
            free(dist);
            free(prev);
            return false;
        }
    }

    // 결과 출력
    printf("\n=== 최단 경로 결과 ===\n");
    for (int i = 0; i < graph->V; i++) {
        if (i != source && dist[i] != INT_MAX) {
            printf("\n%d에서 %d까지의 최단 경로 (거리: %d):\n",
                source, i, dist[i]);
            print_path(prev, i);
            printf("\n");
        }
    }

    free(dist);
    free(prev);
    return true;
}

// 메모리 해제
void free_graph(Graph* graph) {
    free(graph->edges);
    free(graph);
}

int main(void) {
    int V, E, source;

    printf("정점 수 입력: ");
    scanf("%d", &V);
    printf("간선 수 입력: ");
    scanf("%d", &E);

    Graph* graph = create_graph(V, E);

    printf("\n간선 정보 입력 (시작점 도착점 가중치):\n");
    for (int i = 0; i < E; i++) {
        scanf("%d %d %d",
            &graph->edges[i].source,
            &graph->edges[i].dest,
            &graph->edges[i].weight);
    }

    printf("\n시작 정점 입력: ");
    scanf("%d", &source);

    bool print_steps;
    printf("과정을 출력하시겠습니까? (1/0): ");
    scanf("%d", &print_steps);

    if (bellman_ford(graph, source, print_steps)) {
        printf("\n모든 최단 경로를 찾았습니다.\n");
    }
    else {
        printf("\n음수 사이클이 존재하여 최단 경로를 찾을 수 없습니다.\n");
    }

    free_graph(graph);
    return 0;
}

/*
벨만-포드 알고리즘 분석
===================

1. 작동 원리
----------
- 모든 간선에 대해 V-1번 반복
- 매 반복마다 모든 간선 완화
- 음수 사이클 검출 가능

2. 시간 복잡도
-----------
- 기본 동작: O(VE)
- 음수 사이클 검사: O(E)
- 총 복잡도: O(VE)

3. 공간 복잡도
-----------
- 거리 배열: O(V)
- 이전 정점 배열: O(V)
- 총 공간: O(V)

4. 다익스트라와의 차이
-----------------
벨만-포드:
- 음수 가중치 처리 가능
- 음수 사이클 감지
- O(VE) 시간복잡도

다익스트라:
- 음수 가중치 불가
- 더 빠른 O(E log V)
- 우선순위 큐 사용

5. 실제 응용
---------
- 네트워크 라우팅
- 거리 벡터 프로토콜
- 음수 비용이 있는 시스템
- 재정 거래 시스템

6. 최적화 기법
-----------
- 조기 종료 조건
- SPFA 알고리즘
- 병렬화 가능성

이 구현은 음수 가중치를
포함하는 그래프에서의
최단 경로 찾기의 기본을
보여줍니다.
*/