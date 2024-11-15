#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

/*
�˰��� �з�: �׷��� �˰���
���� �з�: �ִ� ��� �˰���
���̵�: �߻�
���� �˰���: ����-����, ���ͽ�Ʈ��

�÷��̵�-���� �˰���:
- ��� ���� �� ���� �ִ� ���
- ���� ��ȹ�� ���
- ���� ����ġ ��� (����Ŭ ���� ���)
- ��� ���� ����
*/

#define INF INT_MAX

typedef struct {
    int** dist;      // �Ÿ� ���
    int** next;      // ��� ������ ���
    int V;           // ���� ��
} Graph;

// �׷��� ����
Graph* create_graph(int V) {
    Graph* graph = (Graph*)malloc(sizeof(Graph));
    graph->V = V;

    // �Ÿ� ��� �Ҵ�
    graph->dist = (int**)malloc(V * sizeof(int*));
    graph->next = (int**)malloc(V * sizeof(int*));

    for (int i = 0; i < V; i++) {
        graph->dist[i] = (int*)malloc(V * sizeof(int));
        graph->next[i] = (int*)malloc(V * sizeof(int));

        for (int j = 0; j < V; j++) {
            if (i == j)
                graph->dist[i][j] = 0;
            else
                graph->dist[i][j] = INF;
            graph->next[i][j] = -1;
        }
    }

    return graph;
}

// �׷��� ���
void print_graph(Graph* graph) {
    printf("\n���� �Ÿ� ���:\n");
    printf("    ");
    for (int i = 0; i < graph->V; i++)
        printf("%4d", i);
    printf("\n");

    for (int i = 0; i < graph->V; i++) {
        printf("%2d: ", i);
        for (int j = 0; j < graph->V; j++) {
            if (graph->dist[i][j] == INF)
                printf(" INF");
            else
                printf("%4d", graph->dist[i][j]);
        }
        printf("\n");
    }
}

// ��� ���
void print_path(Graph* graph, int start, int end) {
    if (graph->next[start][end] == -1) {
        printf("��ΰ� �������� �ʽ��ϴ�.\n");
        return;
    }

    printf("���: %d", start);
    int current = start;
    while (current != end) {
        current = graph->next[current][end];
        printf(" -> %d", current);
    }
    printf("\n�Ÿ�: %d\n", graph->dist[start][end]);
}

// �÷��̵�-���� �˰���
void floyd_warshall(Graph* graph, bool print_steps) {
    printf("\n=== �÷��̵�-���� �˰��� ���� ===\n");

    // k: �߰� ����
    for (int k = 0; k < graph->V; k++) {
        if (print_steps) {
            printf("\n�߰� ���� %d ���:\n", k);
        }

        // i: ���� ����
        for (int i = 0; i < graph->V; i++) {
            // j: ���� ����
            for (int j = 0; j < graph->V; j++) {
                // �����÷ο� ����
                if (graph->dist[i][k] != INF &&
                    graph->dist[k][j] != INF) {
                    int new_dist = graph->dist[i][k] + graph->dist[k][j];

                    if (new_dist < graph->dist[i][j]) {
                        graph->dist[i][j] = new_dist;
                        graph->next[i][j] = graph->next[i][k];

                        if (print_steps) {
                            printf("%d->%d ��� ����: %d (����: %d)\n",
                                i, j, new_dist, k);
                        }
                    }
                }
            }
        }

        if (print_steps) {
            print_graph(graph);
        }
    }

    // ���� ����Ŭ �˻�
    for (int i = 0; i < graph->V; i++) {
        if (graph->dist[i][i] < 0) {
            printf("\n���: ���� ����Ŭ �߰�!\n");
            return;
        }
    }
}

// ��� �ʱ�ȭ
void initialize_paths(Graph* graph) {
    for (int i = 0; i < graph->V; i++) {
        for (int j = 0; j < graph->V; j++) {
            if (graph->dist[i][j] != INF && i != j) {
                graph->next[i][j] = j;
            }
        }
    }
}

// �޸� ����
void free_graph(Graph* graph) {
    for (int i = 0; i < graph->V; i++) {
        free(graph->dist[i]);
        free(graph->next[i]);
    }
    free(graph->dist);
    free(graph->next);
    free(graph);
}

int main(void) {
    int V, E;
    printf("���� �� �Է�: ");
    scanf("%d", &V);
    printf("���� �� �Է�: ");
    scanf("%d", &E);

    Graph* graph = create_graph(V);

    printf("\n���� ���� �Է� (������ ������ ����ġ):\n");
    for (int i = 0; i < E; i++) {
        int src, dest, weight;
        scanf("%d %d %d", &src, &dest, &weight);
        graph->dist[src][dest] = weight;
    }

    initialize_paths(graph);
    printf("\n�ʱ� ����:");
    print_graph(graph);

    bool print_steps;
    printf("\n������ ����Ͻðڽ��ϱ�? (1/0): ");
    scanf("%d", &print_steps);

    floyd_warshall(graph, print_steps);

    printf("\n=== ��� �� �ִ� ��� ===\n");
    for (int i = 0; i < V; i++) {
        for (int j = 0; j < V; j++) {
            if (i != j && graph->dist[i][j] != INF) {
                printf("\n%d���� %d����:\n", i, j);
                print_path(graph, i, j);
            }
        }
    }

    free_graph(graph);
    return 0;
}

/*
�÷��̵�-���� �˰��� �м�
=====================

1. �۵� ����
----------
- ��� ���� �ֿ� ���� �ִ� ��� ���
- k�� �߰� �������� ���
- ���� ��ȹ�� ����� ��ȭ��
- dist[i][j] = min(dist[i][j],
                   dist[i][k] + dist[k][j])

2. �ð� ���⵵
-----------
- ���� �ݺ���: O(V��)
- ��� ���� ���ÿ� ���
- ��� �׷��������� ��ȿ����

3. ���� ���⵵
-----------
- �Ÿ� ���: O(V��)
- ��� ���: O(V��)
- �� ����: O(V��)

4. �ٸ� �˰������ ��
-------------------
�÷��̵�-����:
- ��� �� �ִ� ���
- O(V��) �ð����⵵
- �ܼ��� ����

���ͽ�Ʈ�� V��:
- O(V * E log V)
- ��� �׷������� ����
- ���� ����ġ �Ұ�

5. ���� ����
---------
- ��Ʈ��ũ �����
- ���� �ŷ� Ž��
- ������ ���
- ���� �߽ɼ� ���

6. ����ȭ ���
-----------
- ĳ�� ������ Ȱ��
- ����ȭ ���ɼ�
- ��� ��� Ȱ��
- SIMD ����ȭ

�� ������ �׷�������
��� ���� �� ���� �ִ� ��θ�
ã�� �⺻���� �����
�����ݴϴ�.
*/