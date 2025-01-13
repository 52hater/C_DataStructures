```c
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>
#include <time.h>

/*
알고리즘 분류: 알고리즘 설계 기법
하위 분류: 분할 정복
난이도: 상
관련 알고리즘: 병합 정렬, 퀵 정렬

분할 정복 심화 예제:
1. 가장 가까운 점의 쌍 찾기
2. 스트라센 행렬 곱셈
3. 분할 정복을 이용한 거듭제곱
*/

// === 가장 가까운 점의 쌍 찾기 ===
typedef struct {
    double x, y;
} Point;

typedef struct {
    Point p1, p2;
    double distance;
} PointPair;

// 두 점 사이의 거리 계산
double dist(Point p1, Point p2) {
    return sqrt((p1.x - p2.x) * (p1.x - p2.x) +
        (p1.y - p2.y) * (p1.y - p2.y));
}

// x좌표 기준 정렬
int compare_x(const void* a, const void* b) {
    return ((Point*)a)->x > ((Point*)b)->x;
}

// y좌표 기준 정렬
int compare_y(const void* a, const void* b) {
    return ((Point*)a)->y > ((Point*)b)->y;
}

// 브루트 포스로 최근접 쌍 찾기
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

// 중간 영역에서 최근접 쌍 찾기
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

// 분할 정복으로 최근접 쌍 찾기
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

// 최근접 쌍 찾기 메인 함수
PointPair find_closest_pair(Point* points, int n) {
    qsort(points, n, sizeof(Point), compare_x);
    return closest_pair_recursive(points, n);
}

// === 스트라센 행렬 곱셈 ===
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
    if (n <= 64) {  // 작은 크기는 일반 곱셈
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++) {
                C[i][j] = 0;
                for (int k = 0; k < n; k++)
                    C[i][j] += A[i][k] * B[k][j];
            }
        return;
    }

    int k = n / 2;

    // 부분 행렬들 할당
    int** A11 = allocate_matrix(k);
    int** A12 = allocate_matrix(k);
    int** A21 = allocate_matrix(k);
    int** A22 = allocate_matrix(k);
    int** B11 = allocate_matrix(k);
    int** B12 = allocate_matrix(k);
    int** B21 = allocate_matrix(k);
    int** B22 = allocate_matrix(k);

    // 부분 행렬로 분할
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

    // 임시 행렬들 할당
    int** P1 = allocate_matrix(k);
    int** P2 = allocate_matrix(k);
    int** P3 = allocate_matrix(k);
    int** P4 = allocate_matrix(k);
    int** P5 = allocate_matrix(k);
    int** P6 = allocate_matrix(k);
    int** P7 = allocate_matrix(k);
    int** temp1 = allocate_matrix(k);
    int** temp2 = allocate_matrix(k);

    // Strassen 알고리즘 수행
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

    // 메모리 해제
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

// === 분할 정복 거듭제곱 ===
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
    printf("=== 분할 정복 심화 알고리즘 ===\n");

    while (1) {
        printf("\n1. 최근접 점의 쌍 찾기\n");
        printf("2. 스트라센 행렬 곱셈\n");
        printf("3. 분할 정복 거듭제곱\n");
        printf("0. 종료\n");
        printf("선택: ");
        scanf("%d", &choice);

        switch (choice) {
        case 1: {
            int n;
            printf("점의 개수 입력: ");
            scanf("%d", &n);

            Point* points = (Point*)malloc(n * sizeof(Point));
            printf("점들의 좌표 입력 (x y):\n");
            for (int i = 0; i < n; i++) {
                scanf("%lf %lf", &points[i].x, &points[i].y);
            }

            PointPair result = find_closest_pair(points, n);
            printf("\n가장 가까운 두 점:\n");
            printf("점1: (%.2f, %.2f)\n", result.p1.x, result.p1.y);
            printf("점2: (%.2f, %.2f)\n", result.p2.x, result.p2.y);
            printf("거리: %.2f\n", result.distance);

            free(points);
            break;
        }

        case 2: {
            int n;
            printf("행렬 크기 입력 (2의 거듭제곱): ");
            scanf("%d", &n);

            int** A = allocate_matrix(n);
            int** B = allocate_matrix(n);
            int** C = allocate_matrix(n);

            printf("첫 번째 행렬 입력:\n");
            for (int i = 0; i < n; i++)
                for (int j = 0; j < n; j++)
                    scanf("%d", &A[i][j]);

            printf("두 번째 행렬 입력:\n");
            for (int i = 0; i < n; i++)
                for (int j = 0; j < n; j++)
                    scanf("%d", &B[i][j]);

            strassen_multiply(A, B, C, n);

            printf("\n결과 행렬:\n");
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
            printf("밑수 입력: ");
            scanf("%llu", &base);
            printf("지수 입력: ");
            scanf("%llu", &exponent);
            printf("모듈러 입력: ");
            scanf("%llu", &modulus);

            ull result = power_recursive(base, exponent, modulus);
            printf("\n결과: %llu\n", result);
            break;
        }

        case 0:
            return 0;

        default:
            printf("잘못된 선택\n");
        }
    }

    return 0;
}

/*
분할 정복 심화 분석
===============

1. 최근접 점의 쌍 찾기
------------------
시간 복잡도:
- 브루트 포스: O(n²)
- 분할 정복: O(n log n)
- 정렬 단계: O(n log n)

공간 복잡도:
- O(n) 추가 공간

최적화:
- 평면 분할
- 스트립 처리
- 정렬 상태 유지

2. 스트라센 행렬 곱셈
-----------------
시간 복잡도:
- 일반 곱셈: O(n³)
- 스트라센: O(n^2.807)

공간 복잡도:
- O(n²) 추가 공간

특징:
- 큰 행렬에서만 효율적
- 캐시 성능 고려 필요
- 수치 안정성 문제

3. 분할 정복 거듭제곱
-----------------
시간 복잡도:
- O(log n)

공간 복잡도:
- O(log n) 재귀 스택

특징:
- 모듈러 연산 포함
- 오버플로우 방지
- 큰 수 처리 가능

일반적 교훈
=========
1. 문제 분할 전략
2. 재결합 효율성
3. 기저 사례 처리
4. 메모리 관리
5. 수치 안정성

이 구현들은 분할 정복의
고급 응용을 보여주며,
실제 문제 해결에 사용되는
최적화 기법들을 포함합니다.
*/