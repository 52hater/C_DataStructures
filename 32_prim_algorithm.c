#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>

/*
�ּ� ���� Ʈ�� (Minimum Spanning Tree, MST):
- �׷����� ��� ������ �����ϴ� �κ� �׷���
- ����Ŭ�� ���� Ʈ�� ����
- �������� ����ġ ���� �ּ�
- ��Ʈ��ũ ����, ���/���� ���� � Ȱ��

Prim vs Kruskal �˰���:
1. ���� ���
   - Prim: ���� �߽�, �ϳ��� Ʈ���� Ȯ��
   - Kruskal: ���� �߽�, ���� Ʈ���� �պ�

2. ������ ���
   - Prim: ���� �׷���(������ ���� ���)
   - Kruskal: ��� �׷���(������ ���� ���)

3. �ð� ���⵵
   - Prim: O(V��) �⺻ ����, O(E log V) �� ����
   - Kruskal: O(E log E) ���� ������ ������

4. ���� ���⵵
   - Prim: �켱���� ť ���
   - Kruskal: Union-Find �ڷᱸ�� ���
*/

#define MAX_VERTICES 100
#define INF INT_MAX

typedef struct {
    int num_vertices;
    int graph[MAX_VERTICES][MAX_VERTICES];  // ���� ���
} Graph;

/* �׷��� ���� */
Graph* graph_create(int vertices) {
    Graph* graph = (Graph*)malloc(sizeof(Graph));
    if (!graph) return NULL;

    graph->num_vertices = vertices;

    // ���� ��� �ʱ�ȭ
    for (int i = 0; i < vertices; i++) {
        for (int j = 0; j < vertices; j++) {
            graph->graph[i][j] = i == j ? 0 : INF;
        }
    }

    return graph;
}

/* ���� �߰� */
bool graph_add_edge(Graph* graph, int src, int dest, int weight) {
    if (!graph || src < 0 || src >= graph->num_vertices ||
        dest < 0 || dest >= graph->num_vertices) {
        return false;
    }

    // ������ �׷����̹Ƿ� ���ʿ� �߰�
    graph->graph[src][dest] = weight;
    graph->graph[dest][src] = weight;
    return true;
}

/* �ּ� Ű ���� ���� ���� ã��
 * - ���� MST�� ���Ե��� ���� ���� �߿��� ã��
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

/* Prim �˰���
 * - �ð����⵵: O(V��)
 * - �������⵵: O(V)
 */
void prim_mst(Graph* graph, int start) {
    int vertices = graph->num_vertices;
    int* parent = (int*)malloc(vertices * sizeof(int));  // MST�� ������ �迭
    int* key = (int*)malloc(vertices * sizeof(int));     // �ּ� ����ġ
    bool* included = (bool*)malloc(vertices * sizeof(bool));  // MST ���� ����

    // �ʱ�ȭ
    for (int i = 0; i < vertices; i++) {
        key[i] = INF;
        included[i] = false;
    }

    // ���� ���� ����
    key[start] = 0;
    parent[start] = -1;  // ���� ������ �θ� ����

    // MST ���� (���� ���� - 1���� ���� ����)
    for (int count = 0; count < vertices - 1; count++) {
        // �ּ� Ű ���� ���� ���� ����
        int u = find_min_key(key, included, vertices);
        included[u] = true;

        // ���õ� ������ ����� �������� Ű �� ����
        for (int v = 0; v < vertices; v++) {
            if (graph->graph[u][v] && !included[v] &&
                graph->graph[u][v] < key[v]) {
                parent[v] = u;
                key[v] = graph->graph[u][v];
            }
        }
    }

    // MST ���
    int total_weight = 0;
    printf("\nMinimum Spanning Tree edges:\n");
    for (int i = 1; i < vertices; i++) {
        printf("%d -- %d (weight: %d)\n",
            parent[i], i, graph->graph[i][parent[i]]);
        total_weight += graph->graph[i][parent[i]];
    }
    printf("Total MST weight: %d\n", total_weight);

    // MST�� ���� ���� ���
    printf("\nMST Level Structure (from vertex %d):\n", start);
    int level[MAX_VERTICES] = { 0 };
    bool visited[MAX_VERTICES] = { false };

    // BFS�� ����Ͽ� ���� ���
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

/* �׷��� ��� */
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
                printf("  �� ");
            }
            else {
                printf("%4d", graph->graph[i][j]);
            }
        }
        printf("\n");
    }
}

/* �׷��� �޸� ���� */
void graph_destroy(Graph* graph) {
    free(graph);
}

/* �޴� ��� */
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
�� ���� �� �ֿ� ����
==========================================

1. Prim �˰����� Ư¡
-------------------
- ���� ��� ����
- �ϳ��� Ʈ�� Ȯ��
- ���� ���� �� �ּ� ����ġ ����
- Cut Property Ȱ��

2. Kruskal���� ������
-----------------
�޸� ���:
- Prim: ���� ���/����Ʈ
- Kruskal: ���� ����Ʈ

���� Ư��:
- Prim: ���� �׷����� ����
- Kruskal: ��� �׷����� ����

���� ���:
- Prim: �켱���� ť
- Kruskal: Union-Find

3. �ð� ���⵵
-----------
�⺻ ����: O(V��)
- ���� ����: O(V)
- Ű ����: O(V)

������ ����: O(E log V)
- �켱���� ť ���
- ��� �׷������� ȿ����

4. Ȱ�� �о�
----------
- ��Ʈ��ũ ����
- ���������� ����
- ȸ�� ����
- ��Ÿ� ����

5. �����
-------
����:
- ���� �׷����� ȿ����
- ������ ����
- ���� ���� ���ʿ�
- ���� ��Ȳ Ȯ�� ����

����:
- ��� �׷������� ��ȿ��
- �޸� ��뷮 ��
- �л� ó�� �����
- ���� ����

�� ������ Prim �˰����� �⺻
������ �����ָ�, �켱���� ť��
����� ������ ���������� Ȯ����
�����մϴ�.
*/