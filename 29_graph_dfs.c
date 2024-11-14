#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/*
DFS (Depth First Search):
- ���� �켱 Ž��
- ��� �Ǵ� �������� ����
- �� ��θ� ������ Ž��
- ��Ʈ��ŷ�� �⺻�� �Ǵ� �˰���
*/

#define MAX_VERTICES 100

// �ռ� ������ ���� ����Ʈ ��� �׷��� ���� ����
typedef struct Node {
    int vertex;
    struct Node* next;
} Node;

typedef struct {
    Node* adj_list[MAX_VERTICES];
    bool visited[MAX_VERTICES];  // �湮 ���� �迭 �߰�
    int num_vertices;
} Graph;

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

    // ���� ����Ʈ�� �湮 �迭 �ʱ�ȭ
    for (int i = 0; i < vertices; i++) {
        graph->adj_list[i] = NULL;
        graph->visited[i] = false;
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

/* �湮 �迭 �ʱ�ȭ */
void reset_visited(Graph* graph) {
    for (int i = 0; i < graph->num_vertices; i++) {
        graph->visited[i] = false;
    }
}

/* DFS ��� ����
 * - �ð����⵵: O(V + E)
 * - �������⵵: O(V) (��� ����)
 */
void dfs_recursive(Graph* graph, int vertex) {
    printf("%d ", vertex);  // ���� ���� �湮
    graph->visited[vertex] = true;

    // ������ ��� ������ ���� ��������� DFS ����
    for (Node* temp = graph->adj_list[vertex]; temp != NULL; temp = temp->next) {
        if (!graph->visited[temp->vertex]) {
            dfs_recursive(graph, temp->vertex);
        }
    }
}

/* ���� ���� (�ݺ��� DFS��) */
typedef struct {
    int items[MAX_VERTICES];
    int top;
} Stack;

/* ���� �ʱ�ȭ */
void stack_init(Stack* s) {
    s->top = -1;
}

/* ������ ����ִ��� Ȯ�� */
bool stack_is_empty(Stack* s) {
    return s->top == -1;
}

/* ���ÿ� ���� �߰� */
void stack_push(Stack* s, int value) {
    s->items[++s->top] = value;
}

/* ���ÿ��� ���� ���� */
int stack_pop(Stack* s) {
    return s->items[s->top--];
}

/* DFS �ݺ� ���� (���� ���)
 * - �ð����⵵: O(V + E)
 * - �������⵵: O(V)
 */
void dfs_iterative(Graph* graph, int start) {
    Stack stack;
    stack_init(&stack);
    reset_visited(graph);

    // ���� ���� ó��
    stack_push(&stack, start);

    while (!stack_is_empty(&stack)) {
        int vertex = stack_pop(&stack);

        if (!graph->visited[vertex]) {
            printf("%d ", vertex);
            graph->visited[vertex] = true;

            // ������ �������� ���ÿ� �߰� (��������)
            for (Node* temp = graph->adj_list[vertex]; temp != NULL; temp = temp->next) {
                if (!graph->visited[temp->vertex]) {
                    stack_push(&stack, temp->vertex);
                }
            }
        }
    }
    printf("\n");
}

/* ���� ��� ã��
 * - DFS�� ����Ͽ� �׷����� ���� ��� ���� ���
 */
int find_connected_components(Graph* graph) {
    reset_visited(graph);
    int components = 0;

    for (int v = 0; v < graph->num_vertices; v++) {
        if (!graph->visited[v]) {
            printf("Component %d: ", components + 1);
            dfs_recursive(graph, v);
            printf("\n");
            components++;
        }
    }

    return components;
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
    printf("\n=== Graph DFS Menu ===\n");
    printf("1. Add edge\n");
    printf("2. Print graph\n");
    printf("3. DFS (recursive)\n");
    printf("4. DFS (iterative)\n");
    printf("5. Find connected components\n");
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

        case 3: {  // DFS recursive
            int start;
            printf("Enter starting vertex: ");
            scanf("%d", &start);

            printf("DFS traversal (recursive): ");
            reset_visited(graph);
            dfs_recursive(graph, start);
            printf("\n");
            break;
        }

        case 4: {  // DFS iterative
            int start;
            printf("Enter starting vertex: ");
            scanf("%d", &start);

            printf("DFS traversal (iterative): ");
            dfs_iterative(graph, start);
            break;
        }

        case 5:  // Connected components
            printf("\nFinding connected components...\n");
            int components = find_connected_components(graph);
            printf("Total number of connected components: %d\n", components);
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

1. DFS�� �⺻ ����
--------------
- ������ ���� ���� Ž��
- ���ٸ� �濡�� ��Ʈ��ŷ
- ��� ��� Ž�� ����
- ����/��� Ȱ��

2. ���� ���
----------
����� ����:
- �ý��� ���� Ȱ��
- ������ �ܼ�
- �������� ����
- ���� ��� ����

�ݺ��� ����:
- ����� ���� ���
- �޸� ���� ����
- ���� �����÷ο� ����
- �ణ ������ ����

3. �ð� ���⵵
-----------
- ��ü: O(V + E)
- ���� �湮: O(V)
- ���� Ž��: O(E)
- ���� ���: O(V + E)

4. ���� ���⵵
-----------
- �湮 �迭: O(V)
- ���� ����: O(V)
- ���� ����Ʈ: O(E)
- ��ü: O(V + E)

5. Ȱ�� �о�
----------
- ���� ����
- ���� ��� ã��
- ����Ŭ Ž��
- ��� Ž��

6. �ֿ� Ư¡
----------
- ���� Ž��
- ��Ʈ��ŷ ���
- �޸� ȿ����
- ���� ���� ����

�� ������ DFS�� �� ���� �����
��� �����ϸ�, ���� ���뿡��
���� ���Ǵ� ���� ��� ã��
��ɵ� �����մϴ�.
*/