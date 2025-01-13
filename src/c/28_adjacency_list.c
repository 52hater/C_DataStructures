#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/*
인접 리스트 기반 그래프:
- 각 정점마다 연결된 정점들의 리스트 유지
- 희소 그래프에 효율적
- 메모리 효율적인 구조
- 실제 많이 사용되는 방식
*/

typedef struct Node {
    int vertex;         // 도착 정점
    int weight;         // 간선의 가중치
    struct Node* next;  // 다음 간선
} Node;

typedef struct {
    int num_vertices;           // 정점의 개수
    Node** adj_list;           // 인접 리스트 배열
} Graph;

/* 새로운 노드 생성 */
Node* create_node(int v, int weight) {
    Node* new_node = (Node*)malloc(sizeof(Node));
    if (new_node) {
        new_node->vertex = v;
        new_node->weight = weight;
        new_node->next = NULL;
    }
    return new_node;
}

/* 그래프 생성 */
Graph* graph_create(int vertices) {
    Graph* graph = (Graph*)malloc(sizeof(Graph));
    if (!graph) return NULL;

    graph->adj_list = (Node**)malloc(vertices * sizeof(Node*));
    if (!graph->adj_list) {
        free(graph);
        return NULL;
    }

    graph->num_vertices = vertices;

    // 인접 리스트 초기화
    for (int i = 0; i < vertices; i++) {
        graph->adj_list[i] = NULL;
    }

    return graph;
}

/* 간선 추가 */
bool graph_add_edge(Graph* graph, int src, int dest, int weight, bool directed) {
    if (!graph || src < 0 || src >= graph->num_vertices ||
        dest < 0 || dest >= graph->num_vertices) {
        return false;
    }

    // src에서 dest로의 간선 추가
    Node* new_node = create_node(dest, weight);
    if (!new_node) return false;

    new_node->next = graph->adj_list[src];
    graph->adj_list[src] = new_node;

    if (!directed) {  // 무방향 그래프인 경우
        new_node = create_node(src, weight);
        if (!new_node) return false;

        new_node->next = graph->adj_list[dest];
        graph->adj_list[dest] = new_node;
    }

    return true;
}

/* 간선 제거 */
bool graph_remove_edge(Graph* graph, int src, int dest, bool directed) {
    if (!graph || src < 0 || src >= graph->num_vertices ||
        dest < 0 || dest >= graph->num_vertices) {
        return false;
    }

    // src에서 dest로의 간선 제거
    Node* current = graph->adj_list[src];
    Node* prev = NULL;

    while (current && current->vertex != dest) {
        prev = current;
        current = current->next;
    }

    if (!current) return false;  // 간선이 존재하지 않음

    if (prev) {
        prev->next = current->next;
    }
    else {
        graph->adj_list[src] = current->next;
    }

    free(current);

    if (!directed) {  // 무방향 그래프인 경우 반대 방향도 제거
        current = graph->adj_list[dest];
        prev = NULL;

        while (current && current->vertex != src) {
            prev = current;
            current = current->next;
        }

        if (current) {
            if (prev) {
                prev->next = current->next;
            }
            else {
                graph->adj_list[dest] = current->next;
            }
            free(current);
        }
    }

    return true;
}

/* 간선 존재 여부 확인 */
bool graph_has_edge(const Graph* graph, int src, int dest) {
    if (!graph || src < 0 || src >= graph->num_vertices ||
        dest < 0 || dest >= graph->num_vertices) {
        return false;
    }

    Node* current = graph->adj_list[src];
    while (current) {
        if (current->vertex == dest) {
            return true;
        }
        current = current->next;
    }

    return false;
}

/* 정점의 차수 계산 */
int graph_degree(const Graph* graph, int vertex) {
    if (!graph || vertex < 0 || vertex >= graph->num_vertices) {
        return -1;
    }

    int degree = 0;
    Node* current = graph->adj_list[vertex];

    while (current) {
        degree++;
        current = current->next;
    }

    return degree;
}

/* 그래프 출력 */
void graph_print(const Graph* graph) {
    if (!graph) return;

    printf("\nAdjacency List:\n");
    for (int i = 0; i < graph->num_vertices; i++) {
        printf("[%d]", i);
        Node* current = graph->adj_list[i];

        while (current) {
            printf(" -> %d(w:%d)", current->vertex, current->weight);
            current = current->next;
        }
        printf("\n");
    }
}

/* 정점별 인접 정점 정보 출력 */
void graph_info(const Graph* graph) {
    if (!graph) return;

    printf("\nGraph Information:\n");
    printf("Number of vertices: %d\n", graph->num_vertices);

    for (int i = 0; i < graph->num_vertices; i++) {
        printf("\nVertex %d:", i);
        printf("\n  Adjacent vertices: ");

        Node* current = graph->adj_list[i];
        if (!current) {
            printf("None");
        }

        while (current) {
            printf("%d(weight:%d) ", current->vertex, current->weight);
            current = current->next;
        }

        printf("\n  Degree: %d\n", graph_degree(graph, i));
    }
}

/* 그래프 메모리 해제 */
void graph_destroy(Graph* graph) {
    if (!graph) return;

    // 각 정점의 인접 리스트 해제
    for (int i = 0; i < graph->num_vertices; i++) {
        Node* current = graph->adj_list[i];
        while (current) {
            Node* temp = current;
            current = current->next;
            free(temp);
        }
    }

    free(graph->adj_list);
    free(graph);
}

/* 메뉴 출력 */
void print_menu(void) {
    printf("\n=== Graph Menu ===\n");
    printf("1. Add edge\n");
    printf("2. Remove edge\n");
    printf("3. Check if edge exists\n");
    printf("4. Print graph\n");
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
                printf("Edge exists\n");
            }
            else {
                printf("Edge does not exist\n");
            }
            break;
        }

        case 4:  // Print graph
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

1. 인접 리스트의 특징
-----------------
장점:
- 희소 그래프에 효율적
- 공간 복잡도 O(V + E)
- 정점 순회 효율적
- 메모리 효율적

단점:
- 간선 존재 확인 O(V)
- 구현이 복잡함
- 캐시 지역성 낮음
- 메모리 단편화

2. 시간 복잡도
-----------
기본 연산:
- 간선 추가: O(1)
- 간선 제거: O(V)
- 간선 확인: O(V)

정점 관련:
- 차수 계산: O(V)
- 인접 정점: O(1)
- 전체 순회: O(V + E)

3. 공간 복잡도
-----------
- O(V + E)
- 동적 메모리 사용
- 필요한 만큼만 사용
- 추가 포인터 필요

4. 구현 특징
----------
- 연결 리스트 사용
- 동적 메모리 관리
- 포인터 조작
- 가중치 표현

5. 활용 분야
----------
- 희소 그래프
- 큰 그래프
- SNS 관계망
- 웹 그래프

6. 주의사항
---------
- 메모리 관리
- 포인터 처리
- 동적 할당 실패
- 메모리 누수

이 구현은 인접 행렬과 대비되는
인접 리스트의 특징을 잘 보여주며,
다음 단계인 그래프 순회 구현의
기초가 됩니다.
*/