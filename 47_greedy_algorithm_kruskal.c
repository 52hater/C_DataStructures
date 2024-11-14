#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/*
크루스칼 알고리즘의 탐욕적 특성
===========================

1. 탐욕적 선택
- 가장 작은 가중치의 간선부터 선택
- 사이클 형성하지 않는 조건 하에서 결정
- 선택 시점에서 최선의 선택

2. 최적 부분 구조
- 부분 MST도 최소 신장 트리
- 선택된 간선들의 부분집합도 최적
*/

typedef struct {
    int src;
    int dest;
    int weight;
} Edge;

typedef struct {
    Edge* edges;
    int num_vertices;
    int num_edges;
} Graph;

typedef struct {
    int* parent;
    int* rank;
} DisjointSet;

// 간선 비교 함수
int compare_edges(const void* a, const void* b) {
    return ((Edge*)a)->weight - ((Edge*)b)->weight;
}

// Union-Find 구현
DisjointSet* create_disjoint_set(int size) {
    DisjointSet* set = malloc(sizeof(DisjointSet));
    set->parent = malloc(size * sizeof(int));
    set->rank = calloc(size, sizeof(int));

    for (int i = 0; i < size; i++) {
        set->parent[i] = i;
    }
    return set;
}

int find(DisjointSet* set, int x) {
    if (set->parent[x] != x) {
        set->parent[x] = find(set, set->parent[x]);
    }
    return set->parent[x];
}

void union_sets(DisjointSet* set, int x, int y) {
    int root_x = find(set, x);
    int root_y = find(set, y);

    if (root_x != root_y) {
        if (set->rank[root_x] < set->rank[root_y]) {
            set->parent[root_x] = root_y;
        }
        else if (set->rank[root_x] > set->rank[root_y]) {
            set->parent[root_y] = root_x;
        }
        else {
            set->parent[root_y] = root_x;
            set->rank[root_x]++;
        }
    }
}

// 탐욕적 선택 과정을 보여주는 크루스칼 알고리즘
void kruskal_mst_with_steps(Graph* graph) {
    qsort(graph->edges, graph->num_edges, sizeof(Edge), compare_edges);
    DisjointSet* set = create_disjoint_set(graph->num_vertices);

    printf("\n=== 크루스칼 알고리즘 실행 과정 ===\n");
    printf("간선들을 가중치 순으로 정렬:\n");
    for (int i = 0; i < graph->num_edges; i++) {
        printf("(%d-%d, 가중치: %d)\n",
            graph->edges[i].src,
            graph->edges[i].dest,
            graph->edges[i].weight);
    }

    int mst_weight = 0;
    int edges_used = 0;
    printf("\n간선 선택 과정:\n");

    for (int i = 0; i < graph->num_edges && edges_used < graph->num_vertices - 1; i++) {
        Edge current = graph->edges[i];
        printf("\n현재 검사할 간선: %d-%d (가중치: %d)\n",
            current.src, current.dest, current.weight);

        int src_root = find(set, current.src);
        int dest_root = find(set, current.dest);

        if (src_root != dest_root) {
            union_sets(set, current.src, current.dest);
            mst_weight += current.weight;
            edges_used++;

            printf("선택됨 - 사이클을 형성하지 않음\n");
            printf("현재까지의 MST 가중치: %d\n", mst_weight);
        }
        else {
            printf("제외됨 - 사이클 형성 위험\n");
        }

        printf("현재까지 선택된 간선 수: %d\n", edges_used);
    }

    printf("\n=== 최종 결과 ===\n");
    printf("MST 총 가중치: %d\n", mst_weight);
    printf("사용된 간선 수: %d\n", edges_used);

    free(set->parent);
    free(set->rank);
    free(set);
}

// 메인 함수
int main(void) {
    int num_vertices, num_edges;
    printf("정점 수 입력: ");
    scanf("%d", &num_vertices);
    printf("간선 수 입력: ");
    scanf("%d", &num_edges);

    Graph* graph = malloc(sizeof(Graph));
    graph->num_vertices = num_vertices;
    graph->num_edges = num_edges;
    graph->edges = malloc(num_edges * sizeof(Edge));

    printf("\n간선 정보 입력 (시작점 도착점 가중치):\n");
    for (int i = 0; i < num_edges; i++) {
        scanf("%d %d %d",
            &graph->edges[i].src,
            &graph->edges[i].dest,
            &graph->edges[i].weight);
    }

    kruskal_mst_with_steps(graph);

    free(graph->edges);
    free(graph);
    return 0;
}

/*
크루스칼 알고리즘의 탐욕적 정당성
===========================

1. 안전성 (Safety Property)
------------------------
- 가장 작은 가중치의 간선 선택이
  항상 MST의 일부가 될 수 있음
- Cut Property에 의해 증명 가능

2. 최적성 (Optimality)
-------------------
- 탐욕적 선택을 통해 전역 최적해 도달
- Exchange Argument로 증명 가능
- 선택된 간선은 항상 최적해의 일부

3. 탐욕적 선택의 장점
-----------------
- 단순하고 직관적인 구현
- 효율적인 실행 시간
- 증분적 해 구성
- 부분해의 최적성 보장

4. 시간 복잡도
-----------
- 정렬: O(E log E)
- Union-Find: O(α(V))
- 전체: O(E log E)

이 구현은 크루스칼 알고리즘의
탐욕적 특성을 강조하고,
각 선택 과정을 시각화하여
보여줍니다.
*/