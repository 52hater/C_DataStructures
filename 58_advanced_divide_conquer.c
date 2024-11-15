```c
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>
#include <time.h>

/*
�˰��� �з�: �˰��� ���� ���
���� �з�: ���� ����
���̵�: ��
���� �˰���: ���� ����, �� ����

���� ���� ��ȭ ����:
1. ���� ����� ���� �� ã��
2. ��Ʈ�� ��� ����
3. ���� ������ �̿��� �ŵ�����
*/

// === ���� ����� ���� �� ã�� ===
typedef struct {
    double x, y;
} Point;

typedef struct {
    Point p1, p2;
    double distance;
} PointPair;

// �� �� ������ �Ÿ� ���
double dist(Point p1, Point p2) {
    return sqrt((p1.x - p2.x) * (p1.x - p2.x) +
        (p1.y - p2.y) * (p1.y - p2.y));
}

// x��ǥ ���� ����
int compare_x(const void* a, const void* b) {
    return ((Point*)a)->x > ((Point*)b)->x;
}

// y��ǥ ���� ����
int compare_y(const void* a, const void* b) {
    return ((Point*)a)->y > ((Point*)b)->y;
}

// ���Ʈ ������ �ֱ��� �� ã��
PointPair brute_force(Point* points, int n) {
    PointPair result;
    result.distance = INFINITY;

    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            double d = dist(points[i], points[j]);
            if (d < result.distance) {
                result.p1 = points[i];
                result.p2 = points[j];
                result.distance = d;
            }
        }
    }
    return result;
}

// �߰� �������� �ֱ��� �� ã��
PointPair strip_closest(Point* strip, int size, double d) {
    PointPair result;
    result.distance = d;

    qsort(strip, size, sizeof(Point), compare_y);

    for (int i = 0; i < size; i++) {
        for (int j = i + 1; j < size && (strip[j].y - strip[i].y) < d; j++) {
            double dist_ij = dist(strip[i], strip[j]);
            if (dist_ij < result.distance) {
                result.distance = dist_ij;
                result.p1 = strip[i];
                result.p2 = strip[j];
            }
        }
    }
    return result;
}

// ���� �������� �ֱ��� �� ã��
PointPair closest_pair_recursive(Point* points, int n) {
    if (n <= 3)
        return brute_force(points, n);

    int mid = n / 2;
    Point midPoint = points[mid];

    PointPair left = closest_pair_recursive(points, mid);
    PointPair right = closest_pair_recursive(points + mid, n - mid);

    PointPair result = (left.distance < right.distance) ? left : right;

    Point* strip = (Point*)malloc(n * sizeof(Point));
    int j = 0;

    for (int i = 0; i < n; i++) {
        if (fabs(points[i].x - midPoint.x) < result.distance) {
            strip[j] = points[i];
            j++;
        }
    }

    PointPair strip_result = strip_closest(strip, j, result.distance);
    free(strip);

    return (strip_result.distance < result.distance) ? strip_result : result;
}

// �ֱ��� �� ã�� ���� �Լ�
PointPair find_closest_pair(Point* points, int n) {
    qsort(points, n, sizeof(Point), compare_x);
    return closest_pair_recursive(points, n);
}

// === ��Ʈ�� ��� ���� ===
void add_matrix(int** A, int** B, int** C, int n) {
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            C[i][j] = A[i][j] + B[i][j];
}

void subtract_matrix(int** A, int** B, int** C, int n) {
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            C[i][j] = A[i][j] - B[i][j];
}

int** allocate_matrix(int n) {
    int** matrix = (int**)malloc(n * sizeof(int*));
    for (int i = 0; i < n; i++)
        matrix[i] = (int*)calloc(n, sizeof(int));
    return matrix;
}

void free_matrix(int** matrix, int n) {
    for (int i = 0; i < n; i++)
        free(matrix[i]);
    free(matrix);
}

void strassen_multiply(int** A, int** B, int** C, int n) {
    if (n <= 64) {  // ���� ũ��� �Ϲ� ����
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++) {
                C[i][j] = 0;
                for (int k = 0; k < n; k++)
                    C[i][j] += A[i][k] * B[k][j];
            }
        return;
    }

    int k = n / 2;

    // �κ� ��ĵ� �Ҵ�
    int** A11 = allocate_matrix(k);
    int** A12 = allocate_matrix(k);
    int** A21 = allocate_matrix(k);
    int** A22 = allocate_matrix(k);
    int** B11 = allocate_matrix(k);
    int** B12 = allocate_matrix(k);
    int** B21 = allocate_matrix(k);
    int** B22 = allocate_matrix(k);

    // �κ� ��ķ� ����
    for (int i = 0; i < k; i++) {
        for (int j = 0; j < k; j++) {
            A11[i][j] = A[i][j];
            A12[i][j] = A[i][j + k];
            A21[i][j] = A[i + k][j];
            A22[i][j] = A[i + k][j + k];
            B11[i][j] = B[i][j];
            B12[i][j] = B[i][j + k];
            B21[i][j] = B[i + k][j];
            B22[i][j] = B[i + k][j + k];
        }
    }

    // �ӽ� ��ĵ� �Ҵ�
    int** P1 = allocate_matrix(k);
    int** P2 = allocate_matrix(k);
    int** P3 = allocate_matrix(k);
    int** P4 = allocate_matrix(k);
    int** P5 = allocate_matrix(k);
    int** P6 = allocate_matrix(k);
    int** P7 = allocate_matrix(k);
    int** temp1 = allocate_matrix(k);
    int** temp2 = allocate_matrix(k);

    // Strassen �˰��� ����
    add_matrix(A11, A22, temp1, k);
    add_matrix(B11, B22, temp2, k);
    strassen_multiply(temp1, temp2, P1, k);      // P1 = (A11+A22)(B11+B22)

    add_matrix(A21, A22, temp1, k);
    strassen_multiply(temp1, B11, P2, k);        // P2 = (A21+A22)B11

    subtract_matrix(B12, B22, temp1, k);
    strassen_multiply(A11, temp1, P3, k);        // P3 = A11(B12-B22)

    subtract_matrix(B21, B11, temp1, k);
    strassen_multiply(A22, temp1, P4, k);        // P4 = A22(B21-B11)

    add_matrix(A11, A12, temp1, k);
    strassen_multiply(temp1, B22, P5, k);        // P5 = (A11+A12)B22

    subtract_matrix(A21, A11, temp1, k);
    add_matrix(B11, B12, temp2, k);
    strassen_multiply(temp1, temp2, P6, k);      // P6 = (A21-A11)(B11+B12)

    subtract_matrix(A12, A22, temp1, k);
    add_matrix(B21, B22, temp2, k);
    strassen_multiply(temp1, temp2, P7, k);      // P7 = (A12-A22)(B21+B22)

    // C11 = P1 + P4 - P5 + P7
    add_matrix(P1, P4, temp1, k);
    subtract_matrix(temp1, P5, temp2, k);
    add_matrix(temp2, P7, temp1, k);
    for (int i = 0; i < k; i++)
        for (int j = 0; j < k; j++)
            C[i][j] = temp1[i][j];

    // C12 = P3 + P5
    add_matrix(P3, P5, temp1, k);
    for (int i = 0; i < k; i++)
        for (int j = 0; j < k; j++)
            C[i][j + k] = temp1[i][j];

    // C21 = P2 + P4
    add_matrix(P2, P4, temp1, k);
    for (int i = 0; i < k; i++)
        for (int j = 0; j < k; j++)
            C[i + k][j] = temp1[i][j];

    // C22 = P1 + P3 - P2 + P6
    add_matrix(P1, P3, temp1, k);
    subtract_matrix(temp1, P2, temp2, k);
    add_matrix(temp2, P6, temp1, k);
    for (int i = 0; i < k; i++)
        for (int j = 0; j < k; j++)
            C[i + k][j + k] = temp1[i][j];

    // �޸� ����
    free_matrix(A11, k); free_matrix(A12, k);
    free_matrix(A21, k); free_matrix(A22, k);
    free_matrix(B11, k); free_matrix(B12, k);
    free_matrix(B21, k); free_matrix(B22, k);
    free_matrix(P1, k); free_matrix(P2, k);
    free_matrix(P3, k); free_matrix(P4, k);
    free_matrix(P5, k); free_matrix(P6, k);
    free_matrix(P7, k);
    free_matrix(temp1, k); free_matrix(temp2, k);
}

// === ���� ���� �ŵ����� ===
typedef unsigned long long ull;

ull power_recursive(ull base, ull exponent, ull modulus) {
    if (exponent == 0) return 1;
    if (exponent == 1) return base % modulus;

    ull half = power_recursive(base, exponent / 2, modulus);
    ull result = (half * half) % modulus;

    if (exponent % 2 == 1)
        result = (result * base) % modulus;

    return result;
}

int main(void) {
    int choice;
    printf("=== ���� ���� ��ȭ �˰��� ===\n");

    while (1) {
        printf("\n1. �ֱ��� ���� �� ã��\n");
        printf("2. ��Ʈ�� ��� ����\n");
        printf("3. ���� ���� �ŵ�����\n");
        printf("0. ����\n");
        printf("����: ");
        scanf("%d", &choice);

        switch (choice) {
        case 1: {
            int n;
            printf("���� ���� �Է�: ");
            scanf("%d", &n);

            Point* points = (Point*)malloc(n * sizeof(Point));
            printf("������ ��ǥ �Է� (x y):\n");
            for (int i = 0; i < n; i++) {
                scanf("%lf %lf", &points[i].x, &points[i].y);
            }

            PointPair result = find_closest_pair(points, n);
            printf("\n���� ����� �� ��:\n");
            printf("��1: (%.2f, %.2f)\n", result.p1.x, result.p1.y);
            printf("��2: (%.2f, %.2f)\n", result.p2.x, result.p2.y);
            printf("�Ÿ�: %.2f\n", result.distance);

            free(points);
            break;
        }

        case 2: {
            int n;
            printf("��� ũ�� �Է� (2�� �ŵ�����): ");
            scanf("%d", &n);

            int** A = allocate_matrix(n);
            int** B = allocate_matrix(n);
            int** C = allocate_matrix(n);

            printf("ù ��° ��� �Է�:\n");
            for (int i = 0; i < n; i++)
                for (int j = 0; j < n; j++)
                    scanf("%d", &A[i][j]);

            printf("�� ��° ��� �Է�:\n");
            for (int i = 0; i < n; i++)
                for (int j = 0; j < n; j++)
                    scanf("%d", &B[i][j]);

            strassen_multiply(A, B, C, n);

            printf("\n��� ���:\n");
            for (int i = 0; i < n; i++) {
                for (int j = 0; j < n; j++)
                    printf("%d ", C[i][j]);
                printf("\n");
            }

            free_matrix(A, n);
            free_matrix(B, n);
            free_matrix(C, n);
            break;
        }

        case 3: {
            ull base, exponent, modulus;
            printf("�ؼ� �Է�: ");
            scanf("%llu", &base);
            printf("���� �Է�: ");
            scanf("%llu", &exponent);
            printf("��ⷯ �Է�: ");
            scanf("%llu", &modulus);

            ull result = power_recursive(base, exponent, modulus);
            printf("\n���: %llu\n", result);
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
���� ���� ��ȭ �м�
===============

1. �ֱ��� ���� �� ã��
------------------
�ð� ���⵵:
- ���Ʈ ����: O(n��)
- ���� ����: O(n log n)
- ���� �ܰ�: O(n log n)

���� ���⵵:
- O(n) �߰� ����

����ȭ:
- ��� ����
- ��Ʈ�� ó��
- ���� ���� ����

2. ��Ʈ�� ��� ����
-----------------
�ð� ���⵵:
- �Ϲ� ����: O(n��)
- ��Ʈ��: O(n^2.807)

���� ���⵵:
- O(n��) �߰� ����

Ư¡:
- ū ��Ŀ����� ȿ����
- ĳ�� ���� ��� �ʿ�
- ��ġ ������ ����

3. ���� ���� �ŵ�����
-----------------
�ð� ���⵵:
- O(log n)

���� ���⵵:
- O(log n) ��� ����

Ư¡:
- ��ⷯ ���� ����
- �����÷ο� ����
- ū �� ó�� ����

�Ϲ��� ����
=========
1. ���� ���� ����
2. ����� ȿ����
3. ���� ��� ó��
4. �޸� ����
5. ��ġ ������

�� �������� ���� ������
��� ������ �����ָ�,
���� ���� �ذῡ ���Ǵ�
����ȭ ������� �����մϴ�.
*/