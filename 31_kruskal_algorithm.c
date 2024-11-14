#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/*
Kruskal 알고리즘:
- 최소 신장 트리(MST) 구현
- Union-Find 자료구조 사용
- 간선을 가중치 순으로 정렬
- 사이클 없이 간선 선택
*/

// 간선 구조체
typedef struct {
    int src;      // 시작 정점
    int dest;     // 도착 정점
    int weight;   // 가중치
} Edge;

// 그래프 구조체
typedef struct {
    int num_vertices;
    int num_edges;
    Edge* edges;
} Graph;

// Union-Find를 위한 부모 배열과 랭크 배열
typedef struct {
    int* parent;
    int* rank;
} DisjointSet;

/* Union-Find 초기화 */
DisjointSet* disjoint_set_create(int size) {
    DisjointSet* set = (DisjointSet*)malloc(sizeof(DisjointSet));
    if (!set) return NULL;

    set->parent = (int*)malloc(size * sizeof(int));
    set->rank = (int*)malloc(size * sizeof(int));

    if (!set->parent || !set->rank) {
        free(set->parent);
        free(set->rank);
        free(set);
        return NULL;
    }

    // 초기화: 각 원소는 자신이 대표
    for (int i = 0; i < size; i++) {
        set->parent[i] = i;
        set->rank[i] = 0;
    }

    return set;
}

/* Find 연산 (경로 압축 포함)
 * - 시간복잡도: O(α(n)) (거의 상수 시간)
 */
int find_set(DisjointSet* set, int x) {
    if (set->parent[x] != x) {
        set->parent[x] = find_set(set, set->parent[x]);  // 경로 압축
    }
    return set->parent[x];
}

/* Union 연산 (랭크에 따른 합병)
 * - 시간복잡도: O(α(n))
 */
void union_sets(DisjointSet* set, int x, int y) {
    int root_x = find_set(set, x);
    int root_y = find_set(set, y);

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

/* 그래프 생성 */
Graph* graph_create(int vertices, int edges) {
    Graph* graph = (Graph*)malloc(sizeof(Graph));
    if (!graph) return NULL;

    graph->num_vertices = vertices;
    graph->num_edges = edges;
    graph->edges = (Edge*)malloc(edges * sizeof(Edge));

    if (!graph->edges) {
        free(graph);
        return NULL;
    }

    return graph;
}

/* 간선 비교 함수 (qsort용) */
int compare_edges(const void* a, const void* b) {
    return ((Edge*)a)->weight - ((Edge*)b)->weight;
}

/* Kruskal 알고리즘
 * - 시간복잡도: O(E log E)
 * - 공간복잡도: O(V)
 */
Edge* kruskal_mst(Graph* graph, int* mst_size) {
    Edge* mst = (Edge*)malloc((graph->num_vertices - 1) * sizeof(Edge));
    if (!mst) return NULL;

    // 간선을 가중치 기준으로 정렬
    qsort(graph->edges, graph->num_edges, sizeof(Edge), compare_edges);

    // Union-Find 자료구조 초기화
    DisjointSet* set = disjoint_set_create(graph->num_vertices);
    if (!set) {
        free(mst);
        return NULL;
    }

    *mst_size = 0;
    int mst_weight = 0;  // MST 전체 가중치

    // 가중치가 작은 간선부터 선택
    for (int i = 0; i < graph->num_edges && *mst_size < graph->num_vertices - 1; i++) {
        int src = graph->edges[i].src;
        int dest = graph->edges[i].dest;

        // 사이클을 형성하지 않는 경우에만 선택
        if (find_set(set, src) != find_set(set, dest)) {
            mst[*mst_size] = graph->edges[i];
            mst_weight += graph->edges[i].weight;
            (*mst_size)++;
            union_sets(set, src, dest);

            // 선택된 간선 출력
            printf("Selected edge: %d -- %d (weight: %d)\n",
                src, dest, graph->edges[i].weight);
        }
    }

    printf("Total MST weight: %d\n", mst_weight);

    // 메모리 해제
    free(set->parent);
    free(set->rank);
    free(set);

    return mst;
}

/* 그래프와 MST 시각화 */
void print_graph_and_mst(const Graph* graph, const Edge* mst, int mst_size) {
    printf("\nOriginal Graph Edges:\n");
    for (int i = 0; i < graph->num_edges; i++) {
        printf("%d -- %d (weight: %d)\n",
            graph->edges[i].src,
            graph->edges[i].dest,
            graph->edges[i].weight);
    }

    printf("\nMST Edges:\n");
    int total_weight = 0;
    for (int i = 0; i < mst_size; i++) {
        printf("%d -- %d (weight: %d)\n",
            mst[i].src,
            mst[i].dest,
            mst[i].weight);
        total_weight += mst[i].weight;
    }
    printf("Total MST weight: %d\n", total_weight);
}

/* 그래프 메모리 해제 */
void graph_destroy(Graph* graph) {
    if (graph) {
        free(graph->edges);
        free(graph);
    }
}

/* 메뉴 출력 */
void print_menu(void) {
    printf("\n=== Kruskal's Algorithm Menu ===\n");
    printf("1. Add edge\n");
    printf("2. Find MST\n");
    printf("3. Print graph\n");
    printf("0. Exit\n");
    printf("Choice: ");
}

int main(void) {
    int vertices, max_edges;
    printf("Enter number of vertices: ");
    scanf("%d", &vertices);

    max_edges = vertices * (vertices - 1) / 2;  // 최대 간선 수
    Graph* graph = graph_create(vertices, max_edges);
    if (!graph) {
        printf("Failed to create graph\n");
        return 1;
    }

    int edge_count = 0;
    int choice;

    do {
        print_menu();
        scanf("%d", &choice);

        switch (choice) {
        case 1: {  // Add edge
            if (edge_count >= max_edges) {
                printf("Maximum number of edges reached\n");
                break;
            }

            printf("Enter source vertex: ");
            scanf("%d", &graph->edges[edge_count].src);
            printf("Enter destination vertex: ");
            scanf("%d", &graph->edges[edge_count].dest);
            printf("Enter weight: ");
            scanf("%d", &graph->edges[edge_count].weight);

            if (graph->edges[edge_count].src >= 0 &&
                graph->edges[edge_count].src < vertices &&
                graph->edges[edge_count].dest >= 0 &&
                graph->edges[edge_count].dest < vertices) {
                edge_count++;
                graph->num_edges = edge_count;
                printf("Edge added successfully\n");
            }
            else {
                printf("Invalid vertices\n");
            }
            break;
        }

        case 2: {  // Find MST
            if (edge_count < vertices - 1) {
                printf("Not enough edges to form MST\n");
                break;
            }

            int mst_size;
            Edge* mst = kruskal_mst(graph, &mst_size);

            if (mst) {
                print_graph_and_mst(graph, mst, mst_size);
                free(mst);
            }
            else {
                printf("Failed to find MST\n");
            }
            break;
        }

        case 3:  // Print graph
            printf("\nCurrent Graph:\n");
            for (int i = 0; i < edge_count; i++) {
                printf("%d -- %d (weight: %d)\n",
                    graph->edges[i].src,
                    graph->edges[i].dest,
                    graph->edges[i].weight);
            }
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

1. Kruskal 알고리즘의 원리
----------------------
- 탐욕적(Greedy) 접근
- 최소 가중치 간선 선택
- 사이클 형성 방지
- Union-Find 활용

2. Union-Find 자료구조
-------------------
- 분리 집합 표현
- 경로 압축
- 랭크 기반 합병
- 거의 상수 시간 연산

3. 시간 복잡도
-----------
- 간선 정렬: O(E log E)
- Union-Find: O(α(V))
- 전체: O(E log E)
- α(n)은 애커만 함수의 역함수

4. 공간 복잡도
-----------
- 간선 배열: O(E)
- Union-Find: O(V)
- 전체: O(E)

5. 구현 특징
----------
- 간선 리스트 사용
- 가중치 기반 정렬
- 사이클 검사
- 점진적 구성

6. 장단점
-------
장점:
- 구현 단순
- 희소 그래프에 효율적
- 최적해 보장
- 간선 중심 접근

단점:
- 간선 정렬 필요
- 밀집 그래프에 비효율
- 추가 자료구조 필요
- 메모리 사용량

이 구현은 Kruskal 알고리즘의
기본 원리와 Union-Find 자료구조를
명확하게 보여주며, 최소 신장 트리의
개념을 이해하는 데 도움이 됩니다.
*/