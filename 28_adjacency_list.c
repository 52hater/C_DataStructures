#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/*
���� ����Ʈ ��� �׷���:
- �� �������� ����� �������� ����Ʈ ����
- ��� �׷����� ȿ����
- �޸� ȿ������ ����
- ���� ���� ���Ǵ� ���
*/

typedef struct Node {
    int vertex;         // ���� ����
    int weight;         // ������ ����ġ
    struct Node* next;  // ���� ����
} Node;

typedef struct {
    int num_vertices;           // ������ ����
    Node** adj_list;           // ���� ����Ʈ �迭
} Graph;

/* ���ο� ��� ���� */
Node* create_node(int v, int weight) {
    Node* new_node = (Node*)malloc(sizeof(Node));
    if (new_node) {
        new_node->vertex = v;
        new_node->weight = weight;
        new_node->next = NULL;
    }
    return new_node;
}

/* �׷��� ���� */
Graph* graph_create(int vertices) {
    Graph* graph = (Graph*)malloc(sizeof(Graph));
    if (!graph) return NULL;

    graph->adj_list = (Node**)malloc(vertices * sizeof(Node*));
    if (!graph->adj_list) {
        free(graph);
        return NULL;
    }

    graph->num_vertices = vertices;

    // ���� ����Ʈ �ʱ�ȭ
    for (int i = 0; i < vertices; i++) {
        graph->adj_list[i] = NULL;
    }

    return graph;
}

/* ���� �߰� */
bool graph_add_edge(Graph* graph, int src, int dest, int weight, bool directed) {
    if (!graph || src < 0 || src >= graph->num_vertices ||
        dest < 0 || dest >= graph->num_vertices) {
        return false;
    }

    // src���� dest���� ���� �߰�
    Node* new_node = create_node(dest, weight);
    if (!new_node) return false;

    new_node->next = graph->adj_list[src];
    graph->adj_list[src] = new_node;

    if (!directed) {  // ������ �׷����� ���
        new_node = create_node(src, weight);
        if (!new_node) return false;

        new_node->next = graph->adj_list[dest];
        graph->adj_list[dest] = new_node;
    }

    return true;
}

/* ���� ���� */
bool graph_remove_edge(Graph* graph, int src, int dest, bool directed) {
    if (!graph || src < 0 || src >= graph->num_vertices ||
        dest < 0 || dest >= graph->num_vertices) {
        return false;
    }

    // src���� dest���� ���� ����
    Node* current = graph->adj_list[src];
    Node* prev = NULL;

    while (current && current->vertex != dest) {
        prev = current;
        current = current->next;
    }

    if (!current) return false;  // ������ �������� ����

    if (prev) {
        prev->next = current->next;
    }
    else {
        graph->adj_list[src] = current->next;
    }

    free(current);

    if (!directed) {  // ������ �׷����� ��� �ݴ� ���⵵ ����
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

/* ���� ���� ���� Ȯ�� */
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

/* ������ ���� ��� */
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

/* �׷��� ��� */
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

/* ������ ���� ���� ���� ��� */
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

/* �׷��� �޸� ���� */
void graph_destroy(Graph* graph) {
    if (!graph) return;

    // �� ������ ���� ����Ʈ ����
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

/* �޴� ��� */
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
�� ���� �� �ֿ� ����
==========================================

1. ���� ����Ʈ�� Ư¡
-----------------
����:
- ��� �׷����� ȿ����
- ���� ���⵵ O(V + E)
- ���� ��ȸ ȿ����
- �޸� ȿ����

����:
- ���� ���� Ȯ�� O(V)
- ������ ������
- ĳ�� ������ ����
- �޸� ����ȭ

2. �ð� ���⵵
-----------
�⺻ ����:
- ���� �߰�: O(1)
- ���� ����: O(V)
- ���� Ȯ��: O(V)

���� ����:
- ���� ���: O(V)
- ���� ����: O(1)
- ��ü ��ȸ: O(V + E)

3. ���� ���⵵
-----------
- O(V + E)
- ���� �޸� ���
- �ʿ��� ��ŭ�� ���
- �߰� ������ �ʿ�

4. ���� Ư¡
----------
- ���� ����Ʈ ���
- ���� �޸� ����
- ������ ����
- ����ġ ǥ��

5. Ȱ�� �о�
----------
- ��� �׷���
- ū �׷���
- SNS �����
- �� �׷���

6. ���ǻ���
---------
- �޸� ����
- ������ ó��
- ���� �Ҵ� ����
- �޸� ����

�� ������ ���� ��İ� ���Ǵ�
���� ����Ʈ�� Ư¡�� �� �����ָ�,
���� �ܰ��� �׷��� ��ȸ ������
���ʰ� �˴ϴ�.
*/