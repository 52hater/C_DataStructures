#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/*
���� ���� (Topological Sort):
- ���� ���ȯ �׷���(DAG)���� �������� ������ ������ ã�� �˰���
- ���� ���谡 �ִ� �۾����� ������� ����
- ����:
  * ���� ���� ��û (�������� ����)
  * ������Ʈ ���� ����
  * ������ �ִ� ��Ű�� ��ġ

Ư¡:
1. DAG������ ���� (����Ŭ�� ����� ��)
2. ����� �������� ���� �� ����
3. ���� ����(in-degree)�� ���
4. DFS �Ǵ� ���� ���� ������� ���� ����

���⼭�� �� ���� ��� ��� ����:
1. DFS + ����
2. ���� ���� + ť
*/

#define MAX_VERTICES 100

// ť ����ü �� �Լ�
typedef struct {
    int items[MAX_VERTICES];
    int front;
    int rear;
} Queue;

/* ť �ʱ�ȭ */
void queue_init(Queue* q) {
    q->front = -1;
    q->rear = -1;
}

/* ť�� ����ִ��� Ȯ�� */
bool queue_is_empty(const Queue* q) {
    return q->front == -1;
}

/* ť�� ���� �߰� */
void queue_enqueue(Queue* q, int value) {
    if (q->front == -1)
        q->front = 0;
    q->items[++q->rear] = value;
}

/* ť���� ���� ���� */
int queue_dequeue(Queue* q) {
    int item = q->items[q->front];
    if (q->front == q->rear) {
        q->front = -1;
        q->rear = -1;
    }
    else {
        q->front++;
    }
    return item;
}

// �׷��� ����ü
typedef struct {
    int num_vertices;
    int** adj_matrix;     // ���� ���
    int* in_degree;       // ���� ���� �迭
} Graph;

/* �׷��� ���� */
Graph* graph_create(int vertices) {
    Graph* graph = (Graph*)malloc(sizeof(Graph));
    if (!graph) return NULL;

    graph->num_vertices = vertices;

    // ���� ��� �Ҵ�
    graph->adj_matrix = (int**)malloc(vertices * sizeof(int*));
    for (int i = 0; i < vertices; i++) {
        graph->adj_matrix[i] = (int*)calloc(vertices, sizeof(int));
    }

    // ���� ���� �迭 �Ҵ�
    graph->in_degree = (int*)calloc(vertices, sizeof(int));

    return graph;
}

/* ���� �߰� */
bool graph_add_edge(Graph* graph, int src, int dest) {
    if (!graph || src < 0 || src >= graph->num_vertices ||
        dest < 0 || dest >= graph->num_vertices) {
        return false;
    }

    graph->adj_matrix[src][dest] = 1;
    graph->in_degree[dest]++;  // ���� ���� ����
    return true;
}

/* DFS ��� ���� ������ ���� ��� �Լ� */
void topological_sort_dfs_util(Graph* graph, int vertex, bool visited[],
    int stack[], int* stack_index) {
    visited[vertex] = true;

    // ������ ��� ������ ����
    for (int i = 0; i < graph->num_vertices; i++) {
        if (graph->adj_matrix[vertex][i] && !visited[i]) {
            topological_sort_dfs_util(graph, i, visited, stack, stack_index);
        }
    }

    // ���� ������ ���ÿ� �߰�
    stack[(*stack_index)++] = vertex;
}

/* DFS ��� ���� ���� (���� ���)
 * - �ð����⵵: O(V + E)
 */
void topological_sort_dfs(Graph* graph) {
    bool* visited = (bool*)calloc(graph->num_vertices, sizeof(bool));
    int* stack = (int*)malloc(graph->num_vertices * sizeof(int));
    int stack_index = 0;

    // ��� ������ ���� DFS ����
    for (int i = 0; i < graph->num_vertices; i++) {
        if (!visited[i]) {
            topological_sort_dfs_util(graph, i, visited, stack, &stack_index);
        }
    }

    printf("Topological Sort (DFS): ");
    for (int i = stack_index - 1; i >= 0; i--) {
        printf("%d ", stack[i]);
    }
    printf("\n");

    free(visited);
    free(stack);
}

/* ���� ���� ��� ���� ���� (ť ���)
 * - �ð����⵵: O(V + E)
 */
void topological_sort_indegree(Graph* graph) {
    Queue queue;
    queue_init(&queue);

    // ���� ���� �迭 ����
    int* in_degree = (int*)malloc(graph->num_vertices * sizeof(int));
    for (int i = 0; i < graph->num_vertices; i++) {
        in_degree[i] = graph->in_degree[i];
        if (in_degree[i] == 0) {
            queue_enqueue(&queue, i);
        }
    }

    printf("Topological Sort (In-degree): ");
    int visited_count = 0;

    while (!queue_is_empty(&queue)) {
        int vertex = queue_dequeue(&queue);
        printf("%d ", vertex);
        visited_count++;

        // ������ �������� ���� ���� ����
        for (int i = 0; i < graph->num_vertices; i++) {
            if (graph->adj_matrix[vertex][i]) {
                in_degree[i]--;
                if (in_degree[i] == 0) {
                    queue_enqueue(&queue, i);
                }
            }
        }
    }
    printf("\n");

    // ����Ŭ �˻�
    if (visited_count != graph->num_vertices) {
        printf("Graph contains a cycle!\n");
    }

    free(in_degree);
}

/* �׷��� ��� */
void graph_print(const Graph* graph) {
    printf("\nAdjacency Matrix:\n");
    for (int i = 0; i < graph->num_vertices; i++) {
        for (int j = 0; j < graph->num_vertices; j++) {
            printf("%d ", graph->adj_matrix[i][j]);
        }
        printf("\n");
    }

    printf("\nIn-degrees: ");
    for (int i = 0; i < graph->num_vertices; i++) {
        printf("%d ", graph->in_degree[i]);
    }
    printf("\n");
}

/* �׷��� �޸� ���� */
void graph_destroy(Graph* graph) {
    if (!graph) return;

    for (int i = 0; i < graph->num_vertices; i++) {
        free(graph->adj_matrix[i]);
    }
    free(graph->adj_matrix);
    free(graph->in_degree);
    free(graph);
}

/* �޴� ��� */
void print_menu(void) {
    printf("\n=== Topological Sort Menu ===\n");
    printf("1. Add edge\n");
    printf("2. Topological sort (DFS)\n");
    printf("3. Topological sort (In-degree)\n");
    printf("4. Print graph\n");
    printf("0. Exit\n");
    printf("Choice: ");
}

int main(void) {
    // ����: ���� ���� ��û �ó�����
    printf("College Course Prerequisites Example\n");
    printf("Courses: \n");
    printf("0: Introduction to Programming\n");
    printf("1: Data Structures\n");
    printf("2: Algorithms\n");
    printf("3: Database\n");
    printf("4: Web Development\n");

    Graph* graph = graph_create(5);
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
            int src, dest;
            printf("Enter prerequisite course (0-4): ");
            scanf("%d", &src);
            printf("Enter dependent course (0-4): ");
            scanf("%d", &dest);

            if (graph_add_edge(graph, src, dest)) {
                printf("Prerequisite added successfully\n");
            }
            else {
                printf("Failed to add prerequisite\n");
            }
            break;
        }

        case 2:  // Topological sort (DFS)
            topological_sort_dfs(graph);
            break;

        case 3:  // Topological sort (In-degree)
            topological_sort_indegree(graph);
            break;

        case 4:  // Print graph
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

1. ���� �����̶�?
-------------
- ���� �׷����� �������� �������� ����
- ��� ���� (u,v)�� ���� u�� v���� �տ� ��ġ
- ���� ���踦 �����ϴ� ���� ã��
- DAG������ ����

2. ���� ������ Ȱ��
---------------
����:
- ���� ���� ����
- �������� ü��

������Ʈ ����:
- �۾� ���� ����
- ���� ��ȹ

����Ʈ����:
- ���� ������
- ��Ű�� ��ġ ����

3. ���� ���
----------
DFS ���:
- ���� ���
- ����� ����
- �Ϸ� ���� ����

���� ���� ���:
- ť ���
- �ݺ��� ����
- 0�������� ó��

4. �ð� ���⵵
-----------
- �� ��� ��� O(V + E)
- ���� ���: O(V��)
- ���� ����Ʈ: O(V + E)

5. Ư¡�� ����
-----------
Ư¡:
- ��� �����ϼ�
- �κ� ���� ����
- ����Ŭ ���� ����

����:
- DAG�� ����
- ����Ŭ �Ұ�
- ��ȣ�� ����

6. ���� ����
----------
- �۾� �����ٸ�
- ���� ��û �ý���
- ������ �ؼ�
- ���� ��ųƮ��

�� ������ ���� ������ �� ����
�ֿ� ���� ����� ��� �����ָ�,
���� ���� ���(���� ��û)��
������ �����մϴ�.
*/