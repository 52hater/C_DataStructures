#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/*
ũ�罺Į �˰����� Ž���� Ư��
===========================

1. Ž���� ����
- ���� ���� ����ġ�� �������� ����
- ����Ŭ �������� �ʴ� ���� �Ͽ��� ����
- ���� �������� �ּ��� ����

2. ���� �κ� ����
- �κ� MST�� �ּ� ���� Ʈ��
- ���õ� �������� �κ����յ� ����
*/

typedef struct {
    int src;
    int dest;
    int weight;
} Edge;

typedef struct {
    Edge* edges;
    int num_vertices;
    int num_edges;
} Graph;

typedef struct {
    int* parent;
    int* rank;
} DisjointSet;

// ���� �� �Լ�
int compare_edges(const void* a, const void* b) {
    return ((Edge*)a)->weight - ((Edge*)b)->weight;
}

// Union-Find ����
DisjointSet* create_disjoint_set(int size) {
    DisjointSet* set = malloc(sizeof(DisjointSet));
    set->parent = malloc(size * sizeof(int));
    set->rank = calloc(size, sizeof(int));

    for (int i = 0; i < size; i++) {
        set->parent[i] = i;
    }
    return set;
}

int find(DisjointSet* set, int x) {
    if (set->parent[x] != x) {
        set->parent[x] = find(set, set->parent[x]);
    }
    return set->parent[x];
}

void union_sets(DisjointSet* set, int x, int y) {
    int root_x = find(set, x);
    int root_y = find(set, y);

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

// Ž���� ���� ������ �����ִ� ũ�罺Į �˰���
void kruskal_mst_with_steps(Graph* graph) {
    qsort(graph->edges, graph->num_edges, sizeof(Edge), compare_edges);
    DisjointSet* set = create_disjoint_set(graph->num_vertices);

    printf("\n=== ũ�罺Į �˰��� ���� ���� ===\n");
    printf("�������� ����ġ ������ ����:\n");
    for (int i = 0; i < graph->num_edges; i++) {
        printf("(%d-%d, ����ġ: %d)\n",
            graph->edges[i].src,
            graph->edges[i].dest,
            graph->edges[i].weight);
    }

    int mst_weight = 0;
    int edges_used = 0;
    printf("\n���� ���� ����:\n");

    for (int i = 0; i < graph->num_edges && edges_used < graph->num_vertices - 1; i++) {
        Edge current = graph->edges[i];
        printf("\n���� �˻��� ����: %d-%d (����ġ: %d)\n",
            current.src, current.dest, current.weight);

        int src_root = find(set, current.src);
        int dest_root = find(set, current.dest);

        if (src_root != dest_root) {
            union_sets(set, current.src, current.dest);
            mst_weight += current.weight;
            edges_used++;

            printf("���õ� - ����Ŭ�� �������� ����\n");
            printf("��������� MST ����ġ: %d\n", mst_weight);
        }
        else {
            printf("���ܵ� - ����Ŭ ���� ����\n");
        }

        printf("������� ���õ� ���� ��: %d\n", edges_used);
    }

    printf("\n=== ���� ��� ===\n");
    printf("MST �� ����ġ: %d\n", mst_weight);
    printf("���� ���� ��: %d\n", edges_used);

    free(set->parent);
    free(set->rank);
    free(set);
}

// ���� �Լ�
int main(void) {
    int num_vertices, num_edges;
    printf("���� �� �Է�: ");
    scanf("%d", &num_vertices);
    printf("���� �� �Է�: ");
    scanf("%d", &num_edges);

    Graph* graph = malloc(sizeof(Graph));
    graph->num_vertices = num_vertices;
    graph->num_edges = num_edges;
    graph->edges = malloc(num_edges * sizeof(Edge));

    printf("\n���� ���� �Է� (������ ������ ����ġ):\n");
    for (int i = 0; i < num_edges; i++) {
        scanf("%d %d %d",
            &graph->edges[i].src,
            &graph->edges[i].dest,
            &graph->edges[i].weight);
    }

    kruskal_mst_with_steps(graph);

    free(graph->edges);
    free(graph);
    return 0;
}

/*
ũ�罺Į �˰����� Ž���� ���缺
===========================

1. ������ (Safety Property)
------------------------
- ���� ���� ����ġ�� ���� ������
  �׻� MST�� �Ϻΰ� �� �� ����
- Cut Property�� ���� ���� ����

2. ������ (Optimality)
-------------------
- Ž���� ������ ���� ���� ������ ����
- Exchange Argument�� ���� ����
- ���õ� ������ �׻� �������� �Ϻ�

3. Ž���� ������ ����
-----------------
- �ܼ��ϰ� �������� ����
- ȿ������ ���� �ð�
- ������ �� ����
- �κ����� ������ ����

4. �ð� ���⵵
-----------
- ����: O(E log E)
- Union-Find: O(��(V))
- ��ü: O(E log E)

�� ������ ũ�罺Į �˰�����
Ž���� Ư���� �����ϰ�,
�� ���� ������ �ð�ȭ�Ͽ�
�����ݴϴ�.
*/