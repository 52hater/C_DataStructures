#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/*
인접 행렬 기반 그래프:
- 2차원 배열로 그래프 표현
- 간선 존재 여부를 행렬로 표현
- 구현이 단순하고 직관적
- 간선 확인이 O(1)로 빠름
*/

#define MAX_VERTICES 100
#define INF 999999  // 무한대 값 (연결되지 않은 간선)

typedef struct {
    int num_vertices;                         // 정점의 개수
    int matrix[MAX_VERTICES][MAX_VERTICES];   // 인접 행렬
} Graph;

/* 그래프 생성
 * - 매개변수: vertices - 정점의 개수
 * - 반환값: 생성된 그래프의 포인터
 */
Graph* graph_create(int vertices) {
    if (vertices > MAX_VERTICES) {
        return NULL;
    }

    Graph* graph = (Graph*)malloc(sizeof(Graph));
    if (!graph) {
        return NULL;
    }

    graph->num_vertices = vertices;

    // 모든 간선을 초기화
    for (int i = 0; i < vertices; i++) {
        for (int j = 0; j < vertices; j++) {
            if (i == j) {
                graph->matrix[i][j] = 0;  // 자기 자신으로의 간선은 0
            }
            else {
                graph->matrix[i][j] = INF;  // 나머지는 무한대로 초기화
            }
        }
    }

    return graph;
}

/* 간선 추가
 * - 매개변수: graph - 그래프
 *            src - 출발 정점
 *            dest - 도착 정점
 *            weight - 가중치
 *            directed - 방향 그래프 여부
 * - 반환값: 성공 시 true, 실패 시 false
 */
bool graph_add_edge(Graph* graph, int src, int dest, int weight, bool directed) {
    if (!graph || src < 0 || src >= graph->num_vertices ||
        dest < 0 || dest >= graph->num_vertices) {
        return false;
    }

    graph->matrix[src][dest] = weight;

    if (!directed) {  // 무방향 그래프인 경우
        graph->matrix[dest][src] = weight;
    }

    return true;
}

/* 간선 제거
 * - 매개변수: graph - 그래프
 *            src - 출발 정점
 *            dest - 도착 정점
 *            directed - 방향 그래프 여부
 * - 반환값: 성공 시 true, 실패 시 false
 */
bool graph_remove_edge(Graph* graph, int src, int dest, bool directed) {
    if (!graph || src < 0 || src >= graph->num_vertices ||
        dest < 0 || dest >= graph->num_vertices) {
        return false;
    }

    graph->matrix[src][dest] = INF;

    if (!directed) {  // 무방향 그래프인 경우
        graph->matrix[dest][src] = INF;
    }

    return true;
}

/* 간선 존재 여부 확인
 * - 시간복잡도: O(1)
 */
bool graph_has_edge(const Graph* graph, int src, int dest) {
    if (!graph || src < 0 || src >= graph->num_vertices ||
        dest < 0 || dest >= graph->num_vertices) {
        return false;
    }

    return graph->matrix[src][dest] != INF;
}

/* 정점의 차수 계산
 * - 무방향 그래프의 경우 연결된 모든 간선의 수
 * - 방향 그래프의 경우 진입 차수와 진출 차수의 합
 */
int graph_degree(const Graph* graph, int vertex) {
    if (!graph || vertex < 0 || vertex >= graph->num_vertices) {
        return -1;
    }

    int degree = 0;

    // 진출 차수 계산
    for (int i = 0; i < graph->num_vertices; i++) {
        if (graph->matrix[vertex][i] != INF) {
            degree++;
        }
    }

    // 진입 차수 계산
    for (int i = 0; i < graph->num_vertices; i++) {
        if (graph->matrix[i][vertex] != INF) {
            degree++;
        }
    }

    return degree;
}

/* 그래프 출력
 * - 인접 행렬 형태로 출력
 */
void graph_print(const Graph* graph) {
    if (!graph) return;

    printf("\nAdjacency Matrix:\n");
    printf("   ");
    for (int i = 0; i < graph->num_vertices; i++) {
        printf("%3d ", i);
    }
    printf("\n");

    for (int i = 0; i < graph->num_vertices; i++) {
        printf("%2d ", i);
        for (int j = 0; j < graph->num_vertices; j++) {
            if (graph->matrix[i][j] == INF) {
                printf("INF ");
            }
            else {
                printf("%3d ", graph->matrix[i][j]);
            }
        }
        printf("\n");
    }
}

/* 그래프 정보 출력
 * - 각 정점의 연결 상태 출력
 */
void graph_info(const Graph* graph) {
    if (!graph) return;

    printf("\nGraph Information:\n");
    printf("Number of vertices: %d\n", graph->num_vertices);

    for (int i = 0; i < graph->num_vertices; i++) {
        printf("\nVertex %d:", i);
        printf("\n  Connected to: ");
        bool has_connection = false;

        for (int j = 0; j < graph->num_vertices; j++) {
            if (graph->matrix[i][j] != INF && i != j) {
                printf("%d(weight:%d) ", j, graph->matrix[i][j]);
                has_connection = true;
            }
        }

        if (!has_connection) {
            printf("No connections");
        }

        printf("\n  Degree: %d\n", graph_degree(graph, i));
    }
}

/* 그래프 메모리 해제 */
void graph_destroy(Graph* graph) {
    free(graph);
}

/* 메뉴 출력 */
void print_menu(void) {
    printf("\n=== Graph Menu ===\n");
    printf("1. Add edge\n");
    printf("2. Remove edge\n");
    printf("3. Check if edge exists\n");
    printf("4. Print graph (matrix)\n");
    printf("5. Print graph info\n");
    printf("6. Calculate vertex degree\n");
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
            int src, dest, weight;
            bool directed;

            printf("Enter source vertex: ");
            scanf("%d", &src);
            printf("Enter destination vertex: ");
            scanf("%d", &dest);
            printf("Enter weight: ");
            scanf("%d", &weight);
            printf("Is it directed? (1/0): ");
            scanf("%d", &directed);

            if (graph_add_edge(graph, src, dest, weight, directed)) {
                printf("Edge added successfully\n");
            }
            else {
                printf("Failed to add edge\n");
            }
            break;
        }

        case 2: {  // Remove edge
            int src, dest;
            bool directed;

            printf("Enter source vertex: ");
            scanf("%d", &src);
            printf("Enter destination vertex: ");
            scanf("%d", &dest);
            printf("Is it directed? (1/0): ");
            scanf("%d", &directed);

            if (graph_remove_edge(graph, src, dest, directed)) {
                printf("Edge removed successfully\n");
            }
            else {
                printf("Failed to remove edge\n");
            }
            break;
        }

        case 3: {  // Check edge
            int src, dest;

            printf("Enter source vertex: ");
            scanf("%d", &src);
            printf("Enter destination vertex: ");
            scanf("%d", &dest);

            if (graph_has_edge(graph, src, dest)) {
                printf("Edge exists with weight: %d\n",
                    graph->matrix[src][dest]);
            }
            else {
                printf("Edge does not exist\n");
            }
            break;
        }

        case 4:  // Print matrix
            graph_print(graph);
            break;

        case 5:  // Print info
            graph_info(graph);
            break;

        case 6: {  // Calculate degree
            int vertex;
            printf("Enter vertex: ");
            scanf("%d", &vertex);

            int degree = graph_degree(graph, vertex);
            if (degree >= 0) {
                printf("Degree of vertex %d: %d\n", vertex, degree);
            }
            else {
                printf("Invalid vertex\n");
            }
            break;
        }

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

1. 인접 행렬의 특징
---------------
장점:
- 구현이 단순
- 간선 확인 O(1)
- 행렬 연산 용이
- 구현이 직관적

단점:
- 공간 복잡도 O(V²)
- 메모리 낭비
- 정점 순회 O(V)
- 희소 그래프에 비효율

2. 시간 복잡도
-----------
기본 연산:
- 간선 추가: O(1)
- 간선 제거: O(1)
- 간선 확인: O(1)

정점 관련:
- 차수 계산: O(V)
- 인접 정점: O(V)
- 전체 순회: O(V²)

3. 공간 복잡도
-----------
- O(V²)
- 고정 크기 배열
- 방향/무방향 동일
- 추가 공간 불필요

4. 구현 특징
----------
- 2차원 배열 사용
- 가중치 표현 가능
- 방향성 처리 간단
- INF 값 활용

5. 활용 분야
----------
- 밀집 그래프
- 작은 그래프
- 빠른 접근 필요
- 행렬 연산 필요

6. 주의사항
---------
- 메모리 사용량
- 정점 개수 제한
- 동적 확장 어려움
- 대칭성 고려

이 구현은 그래프의 가장 기본적인
표현 방식을 보여주며, 다음 단계인
인접 리스트 구현의 비교 기준이
됩니다.
*/