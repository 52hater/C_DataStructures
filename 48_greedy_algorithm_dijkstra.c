#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>

/*
����ũ��Ʈ�� �˰����� Ž���� Ư��
=============================

1. Ž���� ����
- ���� �˷��� �ִ� �Ÿ��� ���� ���� ����
- ���� �� ���� �������� �Ÿ� ����
- �� �ܰ� ������ ���� ���� �Ϻ�

2. ������ ����
- ���� ����ġ�� ���� ��� ������ ����
- �ﰢ �ε�Ŀ� ���� ���缺
*/

#define MAX_VERTICES 100
#define INF INT_MAX

typedef struct {
    int graph[MAX_VERTICES][MAX_VERTICES];
    int num_vertices;
    bool directed;  // ���� �׷��� ����
} Graph;

typedef struct {
    int distance;
    int parent;
    bool visited;
} VertexInfo;

// �׷��� ����
Graph* create_graph(int num_vertices, bool directed) {
    Graph* g = malloc(sizeof(Graph));
    g->num_vertices = num_vertices;
    g->directed = directed;

    for (int i = 0; i < num_vertices; i++) {
        for (int j = 0; j < num_vertices; j++) {
            g->graph[i][j] = INF;
        }
        g->graph[i][i] = 0;
    }
    return g;
}

// ���� �߰�
void add_edge(Graph* g, int src, int dest, int weight) {
    g->graph[src][dest] = weight;
    if (!g->directed) {
        g->graph[dest][src] = weight;
    }
}

// ���� ���� ���
void print_current_state(VertexInfo* info, int num_vertices, int current) {
    printf("\n���� ����:\n");
    printf("���õ� ����: %d\n", current);
    printf("���� | �Ÿ� | ���� ����\n");
    printf("-----+------+----------\n");

    for (int i = 0; i < num_vertices; i++) {
        printf("%4d | ", i);
        if (info[i].distance == INF)
            printf(" INF |");
        else
            printf("%4d |", info[i].distance);

        if (info[i].parent == -1)
            printf("   - \n");
        else
            printf("%4d\n", info[i].parent);
    }
}

// �ִ� ��� ���� �� ���
void print_path(int dest, VertexInfo* info) {
    if (info[dest].parent == -1) {
        printf("%d", dest);
        return;
    }
    print_path(info[dest].parent, info);
    printf(" -> %d", dest);
}

// Ž���� ���� ������ �����ִ� ����ũ��Ʈ�� �˰���
void dijkstra_with_steps(Graph* g, int start) {
    VertexInfo* info = malloc(g->num_vertices * sizeof(VertexInfo));

    // �ʱ�ȭ
    printf("\n=== ����ũ��Ʈ�� �˰��� ���� ���� ===\n");
    printf("���� ����: %d\n", start);
    printf("\n�ʱ�ȭ �ܰ�:\n");

    for (int i = 0; i < g->num_vertices; i++) {
        info[i].distance = INF;
        info[i].parent = -1;
        info[i].visited = false;
    }
    info[start].distance = 0;

    print_current_state(info, g->num_vertices, start);

    // ���� ����
    for (int count = 0; count < g->num_vertices - 1; count++) {
        // Ž���� ����: �ּ� �Ÿ� ����
        int min_distance = INF;
        int min_vertex = -1;

        for (int v = 0; v < g->num_vertices; v++) {
            if (!info[v].visited && info[v].distance < min_distance) {
                min_distance = info[v].distance;
                min_vertex = v;
            }
        }

        if (min_vertex == -1) break;  // ������� ���� �׷���

        info[min_vertex].visited = true;
        printf("\n���õ� ����: %d (���� �Ÿ�: %d)\n",
            min_vertex, info[min_vertex].distance);

        // ���� ���� �Ÿ� ����
        for (int v = 0; v < g->num_vertices; v++) {
            if (!info[v].visited &&
                g->graph[min_vertex][v] != INF &&
                info[min_vertex].distance != INF &&
                info[min_vertex].distance + g->graph[min_vertex][v] < info[v].distance) {

                printf("���� %d�� �Ÿ� ����: %d -> %d (����: %d)\n",
                    v,
                    info[v].distance == INF ? -1 : info[v].distance,
                    info[min_vertex].distance + g->graph[min_vertex][v],
                    min_vertex);

                info[v].distance = info[min_vertex].distance + g->graph[min_vertex][v];
                info[v].parent = min_vertex;
            }
        }

        print_current_state(info, g->num_vertices, min_vertex);
    }

    // ��� ���
    printf("\n=== ���� ��� ===\n");
    for (int i = 0; i < g->num_vertices; i++) {
        if (i != start && info[i].distance != INF) {
            printf("\n%d���� %d������ �ִ� ��� (�Ÿ�: %d):\n",
                start, i, info[i].distance);
            print_path(i, info);
            printf("\n");
        }
    }

    free(info);
}

int main(void) {
    int num_vertices, num_edges, start;
    bool directed;

    printf("���� �� �Է�: ");
    scanf("%d", &num_vertices);
    printf("���� �� �Է�: ");
    scanf("%d", &num_edges);
    printf("���� �׷����Դϱ�? (1: ��, 0: �ƴϿ�): ");
    scanf("%d", &directed);

    Graph* graph = create_graph(num_vertices, directed);

    printf("\n���� ���� �Է� (������ ������ ����ġ):\n");
    for (int i = 0; i < num_edges; i++) {
        int src, dest, weight;
        scanf("%d %d %d", &src, &dest, &weight);
        add_edge(graph, src, dest, weight);
    }

    printf("\n���� ���� �Է�: ");
    scanf("%d", &start);

    dijkstra_with_steps(graph, start);

    free(graph);
    return 0;
}

/*
����ũ��Ʈ�� �˰����� Ž���� ���缺
==============================

1. ������ ����
-----------
- �ﰢ �ε�� ���� Ȱ��
- ���� ����ġ�� ���ٴ� ���� �ʿ�
- �湮 ������ ������

2. ������ ����
-----------
- �κ� ����� ������
- Relaxation�� ������
- �ִ� ����� �κ� ����

3. Ž���� ������ �ǹ�
----------------
- �� �ܰ��� ������ �������� ����
- ������ �����ذ� ���� �����ط� ����
- ������ ������ ����� ���� ����

4. ���� ����ȭ
-----------
- �켱���� ť ��� ����
- ���ʿ��� ���� �湮 ����
- ���� ���� ���� Ȱ��

�� ������ ����ũ��Ʈ�� �˰�����
Ž���� Ư���� �����ϰ�,
�� ���� ������ �� ������
�ð������� �����ݴϴ�.
*/