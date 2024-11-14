#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>

/*
�ִ� ��� (Shortest Path):
- �� ���� ������ ��� �� ������ ����ġ ���� �ּ��� ���
- �׺���̼�, ��Ʈ��ũ ����� ��� �ٽ� ����
- �پ��� ����: ���� �����, ��� ��, ���� ����ġ ��

Dijkstra �˰���:
- ���� ����� �ִ� ��� �˰���
- �׸���(Greedy) ������� ����
- ���� ����ġ�� ���� �׷��������� ��� ����
- �켱���� ť�� ����Ͽ� ����ȭ ����

�ٸ� �ִ� ��� �˰������ ��:
1. Bellman-Ford: ���� ����ġ ó�� ����, �� ����
2. Floyd-Warshall: ��� ���� ���� �ִ� ���, O(V��)
3. A*: �޸���ƽ�� ����� ���⼺ �ִ� Ž��
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
        dest < 0 || dest >= graph->num_vertices || weight < 0) {
        return false;
    }

    graph->graph[src][dest] = weight;
    return true;
}

/* �ּ� �Ÿ� ���� ã��
 * - ���� �湮���� ���� ���� �� �ּ� �Ÿ��� ���� ���� ��ȯ
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

/* ��� ���
 * - ���������� ������������ ��θ� ��������� ���
 */
void print_path(const int parent[], int dest) {
    if (parent[dest] == -1) {
        printf("%d", dest);
        return;
    }
    print_path(parent, parent[dest]);
    printf(" -> %d", dest);
}

/* Dijkstra �˰���
 * - �ð����⵵: O(V��)
 * - �������⵵: O(V)
 */
void dijkstra(Graph* graph, int start) {
    int vertices = graph->num_vertices;
    int* dist = (int*)malloc(vertices * sizeof(int));     // �ִ� �Ÿ�
    bool* visited = (bool*)malloc(vertices * sizeof(bool)); // �湮 ����
    int* parent = (int*)malloc(vertices * sizeof(int));    // ��� ������

    // �ʱ�ȭ
    for (int i = 0; i < vertices; i++) {
        dist[i] = INF;
        visited[i] = false;
        parent[i] = -1;
    }

    // ���� ���� ����
    dist[start] = 0;

    // ��� ������ ����
    for (int count = 0; count < vertices - 1; count++) {
        // �ּ� �Ÿ� ���� ã��
        int u = find_min_distance(dist, visited, vertices);
        if (u == -1) break;  // ������� ���� ������ �ִ� ���

        visited[u] = true;

        // ���õ� ������ ���� �������� �Ÿ� ����
        for (int v = 0; v < vertices; v++) {
            // �湮���� ���� �����̰�, ������ �����ϰ�
            // ��������� ��κ��� ���ο� ��ΰ� �� ª�� ���
            if (!visited[v] &&
                graph->graph[u][v] != INF &&
                dist[u] != INF &&
                dist[u] + graph->graph[u][v] < dist[v]) {
                dist[v] = dist[u] + graph->graph[u][v];
                parent[v] = u;
            }
        }

        // ���� ���� ���
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

    // ��� ���
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
    printf("\n=== Dijkstra's Algorithm Menu ===\n");
    printf("1. Add edge\n");
    printf("2. Find shortest paths\n");
    printf("3. Print graph\n");
    printf("0. Exit\n");
    printf("Choice: ");
}

/* ���� �Լ� */
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
�� ���� �� �ֿ� ����
==========================================

1. �ִ� ����� Ư��
----------------
- ���� �κ� ����
- �ﰢ �ε�� ����
- ���� ����Ŭ ����� ��
- ���ϼ��� ������� ����

2. Dijkstra �˰��� ����
---------------------
- �׸��� ���� �Ӽ�
- ������ ��� Ȯ��
- �ִ� �Ÿ� Ȯ��
- ��� ����

3. �ð� ���⵵ �м�
---------------
�⺻ ����: O(V��)
- ���� ����: O(V)
- �Ÿ� ����: O(V)

������ ����: O(E log V)
- �켱���� ť ���
- ��� �׷����� ȿ����

4. Ȱ�� �о�
----------
- ��Ʈ��ũ �����
- �׺���̼�
- �Ҽ� ��Ʈ��ũ
- ���� AI ���ã��

5. �˰����� �������
------------------
- ���� ����ġ �Ұ�
- ���� �׷������� ��ȿ��
- �޸� ��뷮 ��
- ����ȭ �����

6. ����ȭ ���
-----------
- �켱���� ť ���
- ����� �˻�
- ������ ��� ����ġ��
- �޸���ƽ Ȱ�� ����

�� ������ Dijkstra �˰�����
�⺻ ������ �����ָ�, ����
��ο� ���� ������ �ð�������
Ȯ���� �� �ֵ��� �߽��ϴ�.
*/