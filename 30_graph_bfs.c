#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/*
BFS (Breadth First Search):
- �ʺ� �켱 Ž��
- ť�� ����Ͽ� ����
- ���� ������ �������� ���� Ž��
- �ִ� ��� ã���� �⺻�� �Ǵ� �˰���
*/

#define MAX_VERTICES 100
#define INFINITY 999999

// ���� ����Ʈ �׷��� ���� ����
typedef struct Node {
    int vertex;
    struct Node* next;
} Node;

typedef struct {
    Node* adj_list[MAX_VERTICES];
    bool visited[MAX_VERTICES];   // �湮 �迭
    int level[MAX_VERTICES];      // �� ������ ����(���������κ����� �Ÿ�)
    int parent[MAX_VERTICES];     // �� ������ �θ� ����
    int num_vertices;
} Graph;

/* ť ���� */
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

/* ���ο� ��� ���� */
Node* create_node(int v) {
    Node* new_node = (Node*)malloc(sizeof(Node));
    if (new_node) {
        new_node->vertex = v;
        new_node->next = NULL;
    }
    return new_node;
}

/* �׷��� ���� */
Graph* graph_create(int vertices) {
    Graph* graph = (Graph*)malloc(sizeof(Graph));
    if (!graph) return NULL;

    graph->num_vertices = vertices;

    for (int i = 0; i < vertices; i++) {
        graph->adj_list[i] = NULL;
        graph->visited[i] = false;
        graph->level[i] = INFINITY;
        graph->parent[i] = -1;
    }

    return graph;
}

/* ���� �߰� */
bool graph_add_edge(Graph* graph, int src, int dest) {
    // src���� dest���� ���� �߰�
    Node* new_node = create_node(dest);
    if (!new_node) return false;

    new_node->next = graph->adj_list[src];
    graph->adj_list[src] = new_node;

    // ������ �׷����� ���� �ݴ� ���� ���� �߰�
    new_node = create_node(src);
    if (!new_node) return false;

    new_node->next = graph->adj_list[dest];
    graph->adj_list[dest] = new_node;

    return true;
}

/* �湮 �迭�� ���� �迭 �ʱ�ȭ */
void reset_arrays(Graph* graph) {
    for (int i = 0; i < graph->num_vertices; i++) {
        graph->visited[i] = false;
        graph->level[i] = INFINITY;
        graph->parent[i] = -1;
    }
}

/* BFS ����
 * - �ð����⵵: O(V + E)
 * - �������⵵: O(V)
 */
void bfs(Graph* graph, int start) {
    reset_arrays(graph);
    Queue queue;
    queue_init(&queue);

    // ���� ���� ó��
    graph->visited[start] = true;
    graph->level[start] = 0;
    queue_enqueue(&queue, start);

    printf("BFS traversal: ");

    while (!queue_is_empty(&queue)) {
        int current = queue_dequeue(&queue);
        printf("%d ", current);

        // ������ ��� ������ ����
        for (Node* temp = graph->adj_list[current]; temp != NULL; temp = temp->next) {
            if (!graph->visited[temp->vertex]) {
                graph->visited[temp->vertex] = true;
                graph->level[temp->vertex] = graph->level[current] + 1;
                graph->parent[temp->vertex] = current;
                queue_enqueue(&queue, temp->vertex);
            }
        }
    }
    printf("\n");
}

/* Ư�� ���������� �ִ� ��� ��� */
void print_shortest_path(const Graph* graph, int start, int end) {
    if (start == end) {
        printf("%d", start);
        return;
    }

    if (graph->parent[end] == -1) {
        printf("No path exists from %d to %d", start, end);
        return;
    }

    print_shortest_path(graph, start, graph->parent[end]);
    printf(" -> %d", end);
}

/* ���� �� ���� ��� */
void print_levels(const Graph* graph) {
    printf("\nLevels from source:\n");
    for (int i = 0; i < graph->num_vertices; i++) {
        if (graph->level[i] != INFINITY) {
            printf("Vertex %d: Level %d\n", i, graph->level[i]);
        }
    }
}

/* �׷��� ��� */
void graph_print(const Graph* graph) {
    printf("\nGraph Adjacency List:\n");
    for (int i = 0; i < graph->num_vertices; i++) {
        printf("[%d]", i);
        for (Node* temp = graph->adj_list[i]; temp != NULL; temp = temp->next) {
            printf(" -> %d", temp->vertex);
        }
        printf("\n");
    }
}

/* �׷��� �޸� ���� */
void graph_destroy(Graph* graph) {
    if (!graph) return;

    for (int i = 0; i < graph->num_vertices; i++) {
        Node* current = graph->adj_list[i];
        while (current) {
            Node* temp = current;
            current = current->next;
            free(temp);
        }
    }

    free(graph);
}

/* �޴� ��� */
void print_menu(void) {
    printf("\n=== Graph BFS Menu ===\n");
    printf("1. Add edge\n");
    printf("2. Print graph\n");
    printf("3. BFS traversal\n");
    printf("4. Print shortest path\n");
    printf("5. Print vertex levels\n");
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
            int src, dest;
            printf("Enter source vertex: ");
            scanf("%d", &src);
            printf("Enter destination vertex: ");
            scanf("%d", &dest);

            if (graph_add_edge(graph, src, dest)) {
                printf("Edge added successfully\n");
            }
            else {
                printf("Failed to add edge\n");
            }
            break;
        }

        case 2:  // Print graph
            graph_print(graph);
            break;

        case 3: {  // BFS traversal
            int start;
            printf("Enter starting vertex: ");
            scanf("%d", &start);

            if (start >= 0 && start < graph->num_vertices) {
                bfs(graph, start);
            }
            else {
                printf("Invalid vertex\n");
            }
            break;
        }

        case 4: {  // Print shortest path
            int start, end;
            printf("Enter start vertex: ");
            scanf("%d", &start);
            printf("Enter end vertex: ");
            scanf("%d", &end);

            if (start >= 0 && start < graph->num_vertices &&
                end >= 0 && end < graph->num_vertices) {
                bfs(graph, start);  // BFS �����Ͽ� �ִ� ��� ���� ����
                printf("Shortest path: ");
                print_shortest_path(graph, start, end);
                printf("\nDistance: %d\n", graph->level[end]);
            }
            else {
                printf("Invalid vertices\n");
            }
            break;
        }

        case 5:  // Print levels
            print_levels(graph);
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

1. BFS�� �⺻ ����
--------------
- ���� ���� Ž��
- ť �ڷᱸ�� ���
- �ִ� ��� ����
- ��� ���� �湮

2. ���� Ư¡
----------
- ť ��� ����
- ���� ���� ����
- �θ� ���� ����
- ��� ���� ����

3. �ð� ���⵵
-----------
- ��ü: O(V + E)
- ���� �湮: O(V)
- ���� Ž��: O(E)
- ��� ���: O(V)

4. ���� ���⵵
-----------
- ť ����: O(V)
- �湮 �迭: O(V)
- ����/�θ� �迭: O(V)
- ��ü: O(V)

5. Ȱ�� �о�
----------
- �ִ� ��� ã��
- ��Ʈ��ũ ȩ ���
- �� ũ�Ѹ�
- �ּ� �̵� �Ÿ�

6. �����
-------
����:
- �ִ� ��� ����
- ���� ���� ó��
- ���� Ž��
- ���� �ܼ�

����:
- �޸� ��뷮
- ���� ��� ��ȿ��
- ����ġ ��� ����
- ���� ���⵵

�� ������ BFS�� �⺻ ��ɰ� �Բ�
�ִ� ��� ã��, ���� ���� Ȯ�� ��
���������� �߿��� ��ɵ���
�����ϰ� �ֽ��ϴ�.
*/