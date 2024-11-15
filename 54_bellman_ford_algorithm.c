#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

/*
�˰��� �з�: �׷��� �˰���
���� �з�: �ִ� ��� �˰���
���̵�: �߻�
���� �˰���: ���ͽ�Ʈ��, �÷��̵�-����

����-���� �˰���:
- ���� ����ġ ���
- ���� ����Ŭ ���� ����
- ���� ����� �ִ� ���
- O(VE) �ð����⵵
*/

typedef struct {
    int source;     // ���� ����
    int dest;       // ���� ����
    int weight;     // ����ġ
} Edge;

typedef struct {
    Edge* edges;    // ���� �迭
    int V;          // ���� ��
    int E;          // ���� ��
} Graph;

// �׷��� ����
Graph* create_graph(int V, int E) {
    Graph* graph = (Graph*)malloc(sizeof(Graph));
    graph->V = V;
    graph->E = E;
    graph->edges = (Edge*)malloc(E * sizeof(Edge));
    return graph;
}

// �Ÿ� �迭�� ��� �迭 �ʱ�ȭ
void initialize_single_source(Graph* graph, int* dist, int* prev, int source) {
    for (int i = 0; i < graph->V; i++) {
        dist[i] = INT_MAX;
        prev[i] = -1;
    }
    dist[source] = 0;
}

// ��� ���
void print_path(int* prev, int vertex) {
    if (vertex == -1) return;
    print_path(prev, prev[vertex]);
    printf("%d ", vertex);
}

// ����-���� �˰���
bool bellman_ford(Graph* graph, int source, bool print_steps) {
    int* dist = (int*)malloc(graph->V * sizeof(int));
    int* prev = (int*)malloc(graph->V * sizeof(int));

    initialize_single_source(graph, dist, prev, source);

    printf("\n=== ����-���� �˰��� ���� ===\n");
    printf("��� ����: %d\n", source);

    // ���� �� - 1�� �ݺ�
    for (int i = 1; i <= graph->V - 1; i++) {
        if (print_steps) {
            printf("\n�ݺ� %d:\n", i);
        }

        // ��� ������ ���� ��ȭ(relax) ����
        for (int j = 0; j < graph->E; j++) {
            int u = graph->edges[j].source;
            int v = graph->edges[j].dest;
            int weight = graph->edges[j].weight;

            if (dist[u] != INT_MAX &&
                dist[u] + weight < dist[v]) {
                dist[v] = dist[u] + weight;
                prev[v] = u;

                if (print_steps) {
                    printf("���� %d->%d ��ȭ: �Ÿ�[%d] = %d\n",
                        u, v, v, dist[v]);
                }
            }
        }
    }

    // ���� ����Ŭ �˻�
    for (int i = 0; i < graph->E; i++) {
        int u = graph->edges[i].source;
        int v = graph->edges[i].dest;
        int weight = graph->edges[i].weight;

        if (dist[u] != INT_MAX &&
            dist[u] + weight < dist[v]) {
            printf("\n���: ���� ����Ŭ �߰�!\n");
            free(dist);
            free(prev);
            return false;
        }
    }

    // ��� ���
    printf("\n=== �ִ� ��� ��� ===\n");
    for (int i = 0; i < graph->V; i++) {
        if (i != source && dist[i] != INT_MAX) {
            printf("\n%d���� %d������ �ִ� ��� (�Ÿ�: %d):\n",
                source, i, dist[i]);
            print_path(prev, i);
            printf("\n");
        }
    }

    free(dist);
    free(prev);
    return true;
}

// �޸� ����
void free_graph(Graph* graph) {
    free(graph->edges);
    free(graph);
}

int main(void) {
    int V, E, source;

    printf("���� �� �Է�: ");
    scanf("%d", &V);
    printf("���� �� �Է�: ");
    scanf("%d", &E);

    Graph* graph = create_graph(V, E);

    printf("\n���� ���� �Է� (������ ������ ����ġ):\n");
    for (int i = 0; i < E; i++) {
        scanf("%d %d %d",
            &graph->edges[i].source,
            &graph->edges[i].dest,
            &graph->edges[i].weight);
    }

    printf("\n���� ���� �Է�: ");
    scanf("%d", &source);

    bool print_steps;
    printf("������ ����Ͻðڽ��ϱ�? (1/0): ");
    scanf("%d", &print_steps);

    if (bellman_ford(graph, source, print_steps)) {
        printf("\n��� �ִ� ��θ� ã�ҽ��ϴ�.\n");
    }
    else {
        printf("\n���� ����Ŭ�� �����Ͽ� �ִ� ��θ� ã�� �� �����ϴ�.\n");
    }

    free_graph(graph);
    return 0;
}

/*
����-���� �˰��� �м�
===================

1. �۵� ����
----------
- ��� ������ ���� V-1�� �ݺ�
- �� �ݺ����� ��� ���� ��ȭ
- ���� ����Ŭ ���� ����

2. �ð� ���⵵
-----------
- �⺻ ����: O(VE)
- ���� ����Ŭ �˻�: O(E)
- �� ���⵵: O(VE)

3. ���� ���⵵
-----------
- �Ÿ� �迭: O(V)
- ���� ���� �迭: O(V)
- �� ����: O(V)

4. ���ͽ�Ʈ����� ����
-----------------
����-����:
- ���� ����ġ ó�� ����
- ���� ����Ŭ ����
- O(VE) �ð����⵵

���ͽ�Ʈ��:
- ���� ����ġ �Ұ�
- �� ���� O(E log V)
- �켱���� ť ���

5. ���� ����
---------
- ��Ʈ��ũ �����
- �Ÿ� ���� ��������
- ���� ����� �ִ� �ý���
- ���� �ŷ� �ý���

6. ����ȭ ���
-----------
- ���� ���� ����
- SPFA �˰���
- ����ȭ ���ɼ�

�� ������ ���� ����ġ��
�����ϴ� �׷���������
�ִ� ��� ã���� �⺻��
�����ݴϴ�.
*/