```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

/*
�˰��� �з�: �˰��� ���� ���
���� �з�: ���� ��ȹ�� ��ȭ
���̵�: ��
���� �˰���: ���� ����, �׸���

��� ���� ��ȹ�� ����:
1. ���� �Ÿ� (Edit Distance)
2. ��� ü�� ���� (Matrix Chain Multiplication)
3. ���� ���� Ž�� Ʈ�� (Optimal BST)
*/

// === ���� �Ÿ� (Edit Distance) ===
typedef struct {
    int** dp;         // DP ���̺�
    char** ops;       // ���� ���� ���̺�
    int m, n;         // ���ڿ� ����
} EditDistance;

// �ּҰ� ��ȯ
int min3(int a, int b, int c) {
    int min = a;
    if (b < min) min = b;
    if (c < min) min = c;
    return min;
}

// ���� �Ÿ� �ʱ�ȭ
EditDistance* init_edit_distance(int m, int n) {
    EditDistance* ed = (EditDistance*)malloc(sizeof(EditDistance));
    ed->m = m;
    ed->n = n;

    ed->dp = (int**)malloc((m + 1) * sizeof(int*));
    ed->ops = (char**)malloc((m + 1) * sizeof(char*));

    for (int i = 0; i <= m; i++) {
        ed->dp[i] = (int*)malloc((n + 1) * sizeof(int));
        ed->ops[i] = (char*)malloc((n + 1) * sizeof(char));
    }

    return ed;
}

// ���� �Ÿ� ���
int edit_distance(char* str1, char* str2, bool print_steps) {
    int m = strlen(str1);
    int n = strlen(str2);
    EditDistance* ed = init_edit_distance(m, n);

    // ���� ��� �ʱ�ȭ
    for (int i = 0; i <= m; i++) {
        ed->dp[i][0] = i;
        ed->ops[i][0] = 'D';  // Delete
    }

    for (int j = 0; j <= n; j++) {
        ed->dp[0][j] = j;
        ed->ops[0][j] = 'I';  // Insert
    }

    // DP ���̺� ä���
    for (int i = 1; i <= m; i++) {
        for (int j = 1; j <= n; j++) {
            if (str1[i - 1] == str2[j - 1]) {
                ed->dp[i][j] = ed->dp[i - 1][j - 1];
                ed->ops[i][j] = 'M';  // Match
            }
            else {
                ed->dp[i][j] = 1 + min3(
                    ed->dp[i - 1][j],    // ����
                    ed->dp[i][j - 1],    // ����
                    ed->dp[i - 1][j - 1]   // ��ü
                );

                if (ed->dp[i][j] == 1 + ed->dp[i - 1][j])
                    ed->ops[i][j] = 'D';
                else if (ed->dp[i][j] == 1 + ed->dp[i][j - 1])
                    ed->ops[i][j] = 'I';
                else
                    ed->ops[i][j] = 'R';  // Replace
            }

            if (print_steps) {
                printf("dp[%d][%d] = %d (%c)\n",
                    i, j, ed->dp[i][j], ed->ops[i][j]);
            }
        }
    }

    // ���� ���� ���
    if (print_steps) {
        printf("\n���� ����:\n");
        int i = m, j = n;
        while (i > 0 || j > 0) {
            switch (ed->ops[i][j]) {
            case 'M':
                printf("����: %c\n", str1[i - 1]);
                i--; j--;
                break;
            case 'D':
                printf("����: %c\n", str1[i - 1]);
                i--;
                break;
            case 'I':
                printf("����: %c\n", str2[j - 1]);
                j--;
                break;
            case 'R':
                printf("��ü: %c -> %c\n", str1[i - 1], str2[j - 1]);
                i--; j--;
                break;
            }
        }
    }

    int result = ed->dp[m][n];

    // �޸� ����
    for (int i = 0; i <= m; i++) {
        free(ed->dp[i]);
        free(ed->ops[i]);
    }
    free(ed->dp);
    free(ed->ops);
    free(ed);

    return result;
}

// === ��� ü�� ���� ===
typedef struct {
    int** dp;         // DP ���̺�
    int** bracket;    // ��ȣ ��ġ ���̺�
    int n;           // ��� ����
} MatrixChain;

// ��� ü�� �ʱ�ȭ
MatrixChain* init_matrix_chain(int n) {
    MatrixChain* mc = (MatrixChain*)malloc(sizeof(MatrixChain));
    mc->n = n;

    mc->dp = (int**)malloc(n * sizeof(int*));
    mc->bracket = (int**)malloc(n * sizeof(int*));

    for (int i = 0; i < n; i++) {
        mc->dp[i] = (int*)malloc(n * sizeof(int));
        mc->bracket[i] = (int*)malloc(n * sizeof(int));
        for (int j = 0; j < n; j++) {
            mc->dp[i][j] = 0;
            mc->bracket[i][j] = 0;
        }
    }

    return mc;
}

// ��ȣ ��ġ ���
void print_parenthesis(MatrixChain* mc, int i, int j) {
    if (i == j) {
        printf("A%d", i);
        return;
    }

    printf("(");
    print_parenthesis(mc, i, mc->bracket[i][j]);
    print_parenthesis(mc, mc->bracket[i][j] + 1, j);
    printf(")");
}

// ��� ü�� ���� ����ȭ
int matrix_chain_mult(int* dimensions, int n, bool print_steps) {
    MatrixChain* mc = init_matrix_chain(n);

    // len�� ü���� ����
    for (int len = 2; len < n; len++) {
        for (int i = 1; i < n - len + 1; i++) {
            int j = i + len - 1;
            mc->dp[i][j] = INT_MAX;

            // k�� ���� ��ġ
            for (int k = i; k < j; k++) {
                int cost = mc->dp[i][k] + mc->dp[k + 1][j] +
                    dimensions[i - 1] * dimensions[k] * dimensions[j];

                if (cost < mc->dp[i][j]) {
                    mc->dp[i][j] = cost;
                    mc->bracket[i][j] = k;
                }

                if (print_steps) {
                    printf("dp[%d][%d] = %d (k=%d)\n", i, j, cost, k);
                }
            }
        }
    }

    if (print_steps) {
        printf("\n������ ��ȣ ��ġ:\n");
        print_parenthesis(mc, 1, n - 1);
        printf("\n");
    }

    int result = mc->dp[1][n - 1];

    // �޸� ����
    for (int i = 0; i < n; i++) {
        free(mc->dp[i]);
        free(mc->bracket[i]);
    }
    free(mc->dp);
    free(mc->bracket);
    free(mc);

    return result;
}

// === ���� ���� Ž�� Ʈ�� ===
typedef struct {
    double** dp;      // DP ���̺�
    int** root;       // ��Ʈ ���� ���̺�
    int n;           // Ű�� ����
} OptimalBST;

// ���� BST �ʱ�ȭ
OptimalBST* init_optimal_bst(int n) {
    OptimalBST* obst = (OptimalBST*)malloc(sizeof(OptimalBST));
    obst->n = n;

    obst->dp = (double**)malloc((n + 1) * sizeof(double*));
    obst->root = (int**)malloc(n * sizeof(int*));

    for (int i = 0; i <= n; i++) {
        obst->dp[i] = (double*)calloc(n + 1, sizeof(double));
    }

    for (int i = 0; i < n; i++) {
        obst->root[i] = (int*)malloc(n * sizeof(int));
    }

    return obst;
}

// ���� BST ���
double optimal_bst(int* keys, double* freq, int n, bool print_steps) {
    OptimalBST* obst = init_optimal_bst(n);
    double* sum = (double*)calloc(n + 1, sizeof(double));

    // ���� �󵵼� ���
    for (int i = 0; i < n; i++) {
        sum[i + 1] = sum[i] + freq[i];
    }

    // ���� 1�� ����Ʈ�� �ʱ�ȭ
    for (int i = 0; i < n; i++) {
        obst->dp[i][i + 1] = freq[i];
        obst->root[i][i] = i;
    }

    // ���� 2 �̻��� ����Ʈ��
    for (int len = 2; len <= n; len++) {
        for (int i = 0; i <= n - len; i++) {
            int j = i + len;
            obst->dp[i][j] = INT_MAX;

            for (int r = i; r < j; r++) {
                double cost = obst->dp[i][r] + obst->dp[r + 1][j] +
                    sum[j] - sum[i];

                if (cost < obst->dp[i][j]) {
                    obst->dp[i][j] = cost;
                    obst->root[i][j - 1] = r;
                }

                if (print_steps) {
                    printf("dp[%d][%d] = %.2f (root=%d)\n",
                        i, j, cost, r);
                }
            }
        }
    }

    if (print_steps) {
        printf("\n���� BST�� ��Ʈ ����:\n");
        for (int i = 0; i < n; i++) {
            for (int j = i; j < n; j++) {
                printf("����Ʈ�� [%d-%d]�� ��Ʈ: %d\n",
                    i, j, obst->root[i][j]);
            }
        }
    }

    double result = obst->dp[0][n];

    // �޸� ����
    for (int i = 0; i <= n; i++) {
        free(obst->dp[i]);
    }
    for (int i = 0; i < n; i++) {
        free(obst->root[i]);
    }
    free(obst->dp);
    free(obst->root);
    free(obst);
    free(sum);

    return result;
}

int main(void) {
    int choice;

    while (1) {
        printf("\n=== ��� ���� ��ȹ�� ===\n");
        printf("1. ���� �Ÿ�\n");
        printf("2. ��� ü�� ����\n");
        printf("3. ���� ���� Ž�� Ʈ��\n");
        printf("0. ����\n");
        printf("����: ");
        scanf("%d", &choice);

        switch (choice) {
        case 1: {
            char str1[100], str2[100];
            printf("\nù ��° ���ڿ�: ");
            scanf("%s", str1);
            printf("�� ��° ���ڿ�: ");
            scanf("%s", str2);

            int distance = edit_distance(str1, str2, true);
            printf("\n�ּ� ���� �Ÿ�: %d\n", distance);
            break;
        }

        case 2: {
            int n;
            printf("\n��� ����: ");
            scanf("%d", &n);

            int* dimensions = (int*)malloc((n + 1) * sizeof(int));
            printf("�� ����� ���� �Է�:\n");
            for (int i = 0; i <= n; i++) {
                scanf("%d", &dimensions[i]);
            }

            int min_mult = matrix_chain_mult(dimensions, n + 1, true);
            printf("\n�ּ� ���� Ƚ��: %d\n", min_mult);

            free(dimensions);
            break;
        }

        case 3: {
            int n;
            printf("\nŰ ����: ");
            scanf("%d", &n);

            int* keys = (int*)malloc(n * sizeof(int));
            double* freq = (double*)malloc(n * sizeof(double));

            printf("Ű�� �󵵼� �Է�:\n");
            for (int i = 0; i < n; i++) {
                scanf("%d %lf", &keys[i], &freq[i]);
            }

            double cost = optimal_bst(keys, freq, n, true);
            printf("\n���� BST�� ��� �˻� ���: %.2f\n", cost);

            free(keys);
            free(freq);
            break;
        }

        case 0:
            return 0;

        default:
            printf("�߸��� ����\n");
        }
    }

    return 0;
}

/*
��� ���� ��ȹ�� �м�
================

1. ���� �Ÿ�
---------
�ð� ���⵵:
- O(mn)

���� ���⵵:
- O(mn)

����:
- ����� �˻�
- DNA ���� ��
- ���ڿ� ���絵

2. ��� ü�� ����
---------
�ð� ���⵵:
- O(n��)

���� ���⵵:
-O(n��)

����:
- ��� ���� ����ȭ
- ���� �迭 ���
- �׷��Ƚ� ����������

3. ���� ���� Ž�� Ʈ��
----------

�ð� ���⵵:
- O(n��)

���� ���⵵:
- O(n��)

����:
- �˻� ����
- �����Ϸ� �ɺ� ���̺�
- �����ͺ��̽� �ε���

�Ϲ��� ����

1. ���� ������ �߿伺
- ���� ���ǰ� ���� ����
- �޸� ��뷮 ���
- ���� ���� ����ȭ


2. �κ� ���� �ߺ�
- �޸������̼� Ȱ��
- ���̺� ����
- ���� ���⵵ ����

3. ���� �κ� ����
- ����� ���� Ȱ��
- �κ��� ���� ���
- ���� ������ ����

4. ���� ����ȭ
- ĳ�� ������
- �޸� ����
- �ݺ��� ����ȭ

5. ���� ����
- �ǽð� ó��
- ��Ը� ������
- ��Ȯ�� vs ȿ����

�� �������� ���� ��ȹ����
��� ������ �����ָ�,
���� �ý��ۿ��� ���Ǵ�
����ȭ ������� �����մϴ�.
*/