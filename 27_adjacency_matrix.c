#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/*
���� ��� ��� �׷���:
- 2���� �迭�� �׷��� ǥ��
- ���� ���� ���θ� ��ķ� ǥ��
- ������ �ܼ��ϰ� ������
- ���� Ȯ���� O(1)�� ����
*/

#define MAX_VERTICES 100
#define INF 999999  // ���Ѵ� �� (������� ���� ����)

typedef struct {
    int num_vertices;                         // ������ ����
    int matrix[MAX_VERTICES][MAX_VERTICES];   // ���� ���
} Graph;

/* �׷��� ����
 * - �Ű�����: vertices - ������ ����
 * - ��ȯ��: ������ �׷����� ������
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

    // ��� ������ �ʱ�ȭ
    for (int i = 0; i < vertices; i++) {
        for (int j = 0; j < vertices; j++) {
            if (i == j) {
                graph->matrix[i][j] = 0;  // �ڱ� �ڽ������� ������ 0
            }
            else {
                graph->matrix[i][j] = INF;  // �������� ���Ѵ�� �ʱ�ȭ
            }
        }
    }

    return graph;
}

/* ���� �߰�
 * - �Ű�����: graph - �׷���
 *            src - ��� ����
 *            dest - ���� ����
 *            weight - ����ġ
 *            directed - ���� �׷��� ����
 * - ��ȯ��: ���� �� true, ���� �� false
 */
bool graph_add_edge(Graph* graph, int src, int dest, int weight, bool directed) {
    if (!graph || src < 0 || src >= graph->num_vertices ||
        dest < 0 || dest >= graph->num_vertices) {
        return false;
    }

    graph->matrix[src][dest] = weight;

    if (!directed) {  // ������ �׷����� ���
        graph->matrix[dest][src] = weight;
    }

    return true;
}

/* ���� ����
 * - �Ű�����: graph - �׷���
 *            src - ��� ����
 *            dest - ���� ����
 *            directed - ���� �׷��� ����
 * - ��ȯ��: ���� �� true, ���� �� false
 */
bool graph_remove_edge(Graph* graph, int src, int dest, bool directed) {
    if (!graph || src < 0 || src >= graph->num_vertices ||
        dest < 0 || dest >= graph->num_vertices) {
        return false;
    }

    graph->matrix[src][dest] = INF;

    if (!directed) {  // ������ �׷����� ���
        graph->matrix[dest][src] = INF;
    }

    return true;
}

/* ���� ���� ���� Ȯ��
 * - �ð����⵵: O(1)
 */
bool graph_has_edge(const Graph* graph, int src, int dest) {
    if (!graph || src < 0 || src >= graph->num_vertices ||
        dest < 0 || dest >= graph->num_vertices) {
        return false;
    }

    return graph->matrix[src][dest] != INF;
}

/* ������ ���� ���
 * - ������ �׷����� ��� ����� ��� ������ ��
 * - ���� �׷����� ��� ���� ������ ���� ������ ��
 */
int graph_degree(const Graph* graph, int vertex) {
    if (!graph || vertex < 0 || vertex >= graph->num_vertices) {
        return -1;
    }

    int degree = 0;

    // ���� ���� ���
    for (int i = 0; i < graph->num_vertices; i++) {
        if (graph->matrix[vertex][i] != INF) {
            degree++;
        }
    }

    // ���� ���� ���
    for (int i = 0; i < graph->num_vertices; i++) {
        if (graph->matrix[i][vertex] != INF) {
            degree++;
        }
    }

    return degree;
}

/* �׷��� ���
 * - ���� ��� ���·� ���
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

/* �׷��� ���� ���
 * - �� ������ ���� ���� ���
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

/* �׷��� �޸� ���� */
void graph_destroy(Graph* graph) {
    free(graph);
}

/* �޴� ��� */
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
�� ���� �� �ֿ� ����
==========================================

1. ���� ����� Ư¡
---------------
����:
- ������ �ܼ�
- ���� Ȯ�� O(1)
- ��� ���� ����
- ������ ������

����:
- ���� ���⵵ O(V��)
- �޸� ����
- ���� ��ȸ O(V)
- ��� �׷����� ��ȿ��

2. �ð� ���⵵
-----------
�⺻ ����:
- ���� �߰�: O(1)
- ���� ����: O(1)
- ���� Ȯ��: O(1)

���� ����:
- ���� ���: O(V)
- ���� ����: O(V)
- ��ü ��ȸ: O(V��)

3. ���� ���⵵
-----------
- O(V��)
- ���� ũ�� �迭
- ����/������ ����
- �߰� ���� ���ʿ�

4. ���� Ư¡
----------
- 2���� �迭 ���
- ����ġ ǥ�� ����
- ���⼺ ó�� ����
- INF �� Ȱ��

5. Ȱ�� �о�
----------
- ���� �׷���
- ���� �׷���
- ���� ���� �ʿ�
- ��� ���� �ʿ�

6. ���ǻ���
---------
- �޸� ��뷮
- ���� ���� ����
- ���� Ȯ�� �����
- ��Ī�� ���

�� ������ �׷����� ���� �⺻����
ǥ�� ����� �����ָ�, ���� �ܰ���
���� ����Ʈ ������ �� ������
�˴ϴ�.
*/