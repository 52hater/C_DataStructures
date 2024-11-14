#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/*
Kruskal �˰���:
- �ּ� ���� Ʈ��(MST) ����
- Union-Find �ڷᱸ�� ���
- ������ ����ġ ������ ����
- ����Ŭ ���� ���� ����
*/

// ���� ����ü
typedef struct {
    int src;      // ���� ����
    int dest;     // ���� ����
    int weight;   // ����ġ
} Edge;

// �׷��� ����ü
typedef struct {
    int num_vertices;
    int num_edges;
    Edge* edges;
} Graph;

// Union-Find�� ���� �θ� �迭�� ��ũ �迭
typedef struct {
    int* parent;
    int* rank;
} DisjointSet;

/* Union-Find �ʱ�ȭ */
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

    // �ʱ�ȭ: �� ���Ҵ� �ڽ��� ��ǥ
    for (int i = 0; i < size; i++) {
        set->parent[i] = i;
        set->rank[i] = 0;
    }

    return set;
}

/* Find ���� (��� ���� ����)
 * - �ð����⵵: O(��(n)) (���� ��� �ð�)
 */
int find_set(DisjointSet* set, int x) {
    if (set->parent[x] != x) {
        set->parent[x] = find_set(set, set->parent[x]);  // ��� ����
    }
    return set->parent[x];
}

/* Union ���� (��ũ�� ���� �պ�)
 * - �ð����⵵: O(��(n))
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

/* �׷��� ���� */
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

/* ���� �� �Լ� (qsort��) */
int compare_edges(const void* a, const void* b) {
    return ((Edge*)a)->weight - ((Edge*)b)->weight;
}

/* Kruskal �˰���
 * - �ð����⵵: O(E log E)
 * - �������⵵: O(V)
 */
Edge* kruskal_mst(Graph* graph, int* mst_size) {
    Edge* mst = (Edge*)malloc((graph->num_vertices - 1) * sizeof(Edge));
    if (!mst) return NULL;

    // ������ ����ġ �������� ����
    qsort(graph->edges, graph->num_edges, sizeof(Edge), compare_edges);

    // Union-Find �ڷᱸ�� �ʱ�ȭ
    DisjointSet* set = disjoint_set_create(graph->num_vertices);
    if (!set) {
        free(mst);
        return NULL;
    }

    *mst_size = 0;
    int mst_weight = 0;  // MST ��ü ����ġ

    // ����ġ�� ���� �������� ����
    for (int i = 0; i < graph->num_edges && *mst_size < graph->num_vertices - 1; i++) {
        int src = graph->edges[i].src;
        int dest = graph->edges[i].dest;

        // ����Ŭ�� �������� �ʴ� ��쿡�� ����
        if (find_set(set, src) != find_set(set, dest)) {
            mst[*mst_size] = graph->edges[i];
            mst_weight += graph->edges[i].weight;
            (*mst_size)++;
            union_sets(set, src, dest);

            // ���õ� ���� ���
            printf("Selected edge: %d -- %d (weight: %d)\n",
                src, dest, graph->edges[i].weight);
        }
    }

    printf("Total MST weight: %d\n", mst_weight);

    // �޸� ����
    free(set->parent);
    free(set->rank);
    free(set);

    return mst;
}

/* �׷����� MST �ð�ȭ */
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

/* �׷��� �޸� ���� */
void graph_destroy(Graph* graph) {
    if (graph) {
        free(graph->edges);
        free(graph);
    }
}

/* �޴� ��� */
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

    max_edges = vertices * (vertices - 1) / 2;  // �ִ� ���� ��
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
�� ���� �� �ֿ� ����
==========================================

1. Kruskal �˰����� ����
----------------------
- Ž����(Greedy) ����
- �ּ� ����ġ ���� ����
- ����Ŭ ���� ����
- Union-Find Ȱ��

2. Union-Find �ڷᱸ��
-------------------
- �и� ���� ǥ��
- ��� ����
- ��ũ ��� �պ�
- ���� ��� �ð� ����

3. �ð� ���⵵
-----------
- ���� ����: O(E log E)
- Union-Find: O(��(V))
- ��ü: O(E log E)
- ��(n)�� ��Ŀ�� �Լ��� ���Լ�

4. ���� ���⵵
-----------
- ���� �迭: O(E)
- Union-Find: O(V)
- ��ü: O(E)

5. ���� Ư¡
----------
- ���� ����Ʈ ���
- ����ġ ��� ����
- ����Ŭ �˻�
- ������ ����

6. �����
-------
����:
- ���� �ܼ�
- ��� �׷����� ȿ����
- ������ ����
- ���� �߽� ����

����:
- ���� ���� �ʿ�
- ���� �׷����� ��ȿ��
- �߰� �ڷᱸ�� �ʿ�
- �޸� ��뷮

�� ������ Kruskal �˰�����
�⺻ ������ Union-Find �ڷᱸ����
��Ȯ�ϰ� �����ָ�, �ּ� ���� Ʈ����
������ �����ϴ� �� ������ �˴ϴ�.
*/