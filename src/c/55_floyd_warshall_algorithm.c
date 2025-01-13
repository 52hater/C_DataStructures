#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

/*
알고리즘 분류: 그래프 알고리즘
하위 분류: 최단 경로 알고리즘
난이도: 중상
관련 알고리즘: 벨만-포드, 다익스트라

플로이드-워셜 알고리즘:
- 모든 정점 쌍 간의 최단 경로
- 동적 계획법 기반
- 음수 가중치 허용 (사이클 없는 경우)
- 경로 복원 가능
*/

#define INF INT_MAX

typedef struct {
    int** dist;      // 거리 행렬
    int** next;      // 경로 복원용 행렬
    int V;           // 정점 수
} Graph;

// 그래프 생성
Graph* create_graph(int V) {
    Graph* graph = (Graph*)malloc(sizeof(Graph));
    graph->V = V;

    // 거리 행렬 할당
    graph->dist = (int**)malloc(V * sizeof(int*));
    graph->next = (int**)malloc(V * sizeof(int*));

    for (int i = 0; i < V; i++) {
        graph->dist[i] = (int*)malloc(V * sizeof(int));
        graph->next[i] = (int*)malloc(V * sizeof(int));

        for (int j = 0; j < V; j++) {
            if (i == j)
                graph->dist[i][j] = 0;
            else
                graph->dist[i][j] = INF;
            graph->next[i][j] = -1;
        }
    }

    return graph;
}

// 그래프 출력
void print_graph(Graph* graph) {
    printf("\n현재 거리 행렬:\n");
    printf("    ");
    for (int i = 0; i < graph->V; i++)
        printf("%4d", i);
    printf("\n");

    for (int i = 0; i < graph->V; i++) {
        printf("%2d: ", i);
        for (int j = 0; j < graph->V; j++) {
            if (graph->dist[i][j] == INF)
                printf(" INF");
            else
                printf("%4d", graph->dist[i][j]);
        }
        printf("\n");
    }
}

// 경로 출력
void print_path(Graph* graph, int start, int end) {
    if (graph->next[start][end] == -1) {
        printf("경로가 존재하지 않습니다.\n");
        return;
    }

    printf("경로: %d", start);
    int current = start;
    while (current != end) {
        current = graph->next[current][end];
        printf(" -> %d", current);
    }
    printf("\n거리: %d\n", graph->dist[start][end]);
}

// 플로이드-워셜 알고리즘
void floyd_warshall(Graph* graph, bool print_steps) {
    printf("\n=== 플로이드-워셜 알고리즘 실행 ===\n");

    // k: 중간 정점
    for (int k = 0; k < graph->V; k++) {
        if (print_steps) {
            printf("\n중간 정점 %d 사용:\n", k);
        }

        // i: 시작 정점
        for (int i = 0; i < graph->V; i++) {
            // j: 도착 정점
            for (int j = 0; j < graph->V; j++) {
                // 오버플로우 방지
                if (graph->dist[i][k] != INF &&
                    graph->dist[k][j] != INF) {
                    int new_dist = graph->dist[i][k] + graph->dist[k][j];

                    if (new_dist < graph->dist[i][j]) {
                        graph->dist[i][j] = new_dist;
                        graph->next[i][j] = graph->next[i][k];

                        if (print_steps) {
                            printf("%d->%d 경로 갱신: %d (경유: %d)\n",
                                i, j, new_dist, k);
                        }
                    }
                }
            }
        }

        if (print_steps) {
            print_graph(graph);
        }
    }

    // 음수 사이클 검사
    for (int i = 0; i < graph->V; i++) {
        if (graph->dist[i][i] < 0) {
            printf("\n경고: 음수 사이클 발견!\n");
            return;
        }
    }
}

// 경로 초기화
void initialize_paths(Graph* graph) {
    for (int i = 0; i < graph->V; i++) {
        for (int j = 0; j < graph->V; j++) {
            if (graph->dist[i][j] != INF && i != j) {
                graph->next[i][j] = j;
            }
        }
    }
}

// 메모리 해제
void free_graph(Graph* graph) {
    for (int i = 0; i < graph->V; i++) {
        free(graph->dist[i]);
        free(graph->next[i]);
    }
    free(graph->dist);
    free(graph->next);
    free(graph);
}

int main(void) {
    int V, E;
    printf("정점 수 입력: ");
    scanf("%d", &V);
    printf("간선 수 입력: ");
    scanf("%d", &E);

    Graph* graph = create_graph(V);

    printf("\n간선 정보 입력 (시작점 도착점 가중치):\n");
    for (int i = 0; i < E; i++) {
        int src, dest, weight;
        scanf("%d %d %d", &src, &dest, &weight);
        graph->dist[src][dest] = weight;
    }

    initialize_paths(graph);
    printf("\n초기 상태:");
    print_graph(graph);

    bool print_steps;
    printf("\n과정을 출력하시겠습니까? (1/0): ");
    scanf("%d", &print_steps);

    floyd_warshall(graph, print_steps);

    printf("\n=== 모든 쌍 최단 경로 ===\n");
    for (int i = 0; i < V; i++) {
        for (int j = 0; j < V; j++) {
            if (i != j && graph->dist[i][j] != INF) {
                printf("\n%d에서 %d까지:\n", i, j);
                print_path(graph, i, j);
            }
        }
    }

    free_graph(graph);
    return 0;
}

/*
플로이드-워셜 알고리즘 분석
=====================

1. 작동 원리
----------
- 모든 정점 쌍에 대해 최단 경로 계산
- k를 중간 정점으로 사용
- 동적 계획법 기반의 점화식
- dist[i][j] = min(dist[i][j],
                   dist[i][k] + dist[k][j])

2. 시간 복잡도
-----------
- 삼중 반복문: O(V³)
- 모든 쌍을 동시에 계산
- 희소 그래프에서는 비효율적

3. 공간 복잡도
-----------
- 거리 행렬: O(V²)
- 경로 행렬: O(V²)
- 총 공간: O(V²)

4. 다른 알고리즘과의 비교
-------------------
플로이드-워셜:
- 모든 쌍 최단 경로
- O(V³) 시간복잡도
- 단순한 구현

다익스트라 V번:
- O(V * E log V)
- 희소 그래프에서 유리
- 음수 가중치 불가

5. 실제 응용
---------
- 네트워크 라우팅
- 차익 거래 탐지
- 전이적 폐쇄
- 정점 중심성 계산

6. 최적화 기법
-----------
- 캐시 지역성 활용
- 병렬화 가능성
- 희소 행렬 활용
- SIMD 최적화

이 구현은 그래프에서
모든 정점 쌍 간의 최단 경로를
찾는 기본적인 방법을
보여줍니다.
*/