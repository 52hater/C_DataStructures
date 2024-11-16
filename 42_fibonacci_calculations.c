#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

/*
피보나치 수의 다양한 계산 방법:

1. 기본 재귀 방법 (O(2^n))
   - 직관적이지만 매우 비효율적
   - 중복 계산이 많음

2. 동적 계획법 (O(n))
   - 중복 계산 제거
   - 메모리 O(n) 또는 O(1)

3. 행렬 거듭제곱 방법 (O(log n))
   - 분할 정복의 핵심 예시
   - [[1 1], [1 0]]^n = [[F(n+1) F(n)], [F(n) F(n-1)]]

4. 수학적 공식 (O(1))
   - 빠르지만 큰 수에서 정밀도 문제
   - Binet's Formula 활용
*/

// 2x2 행렬 구조체
typedef struct {
    long long m[2][2];
} Matrix2x2;

/* 행렬 곱셈
 * - 2x2 행렬의 곱셈 연산
 */
Matrix2x2 matrix_multiply(Matrix2x2 a, Matrix2x2 b) {
    Matrix2x2 result;
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            result.m[i][j] = 0;
            for (int k = 0; k < 2; k++) {
                result.m[i][j] += a.m[i][k] * b.m[k][j];
            }
        }
    }
    return result;
}

/* 기본 재귀 방식 피보나치
 * - 시간복잡도: O(2^n)
 * - 직관적이지만 매우 비효율적
 */
long long fib_recursive(int n, bool print_steps) {
    if (print_steps) {
        printf("Computing F(%d)\n", n);
    }

    if (n <= 1) return n;

    long long result = fib_recursive(n - 1, print_steps) +
        fib_recursive(n - 2, print_steps);

    if (print_steps) {
        printf("F(%d) = %lld\n", n, result);
    }

    return result;
}

/* 동적 계획법 피보나치
 * - 시간복잡도: O(n)
 * - 공간복잡도: O(1)
 */
long long fib_dynamic(int n, bool print_steps) {
    if (n <= 1) return n;

    long long prev = 0, curr = 1;

    for (int i = 2; i <= n; i++) {
        long long next = prev + curr;
        if (print_steps) {
            printf("F(%d) = F(%d) + F(%d) = %lld + %lld = %lld\n",
                i, i - 2, i - 1, prev, curr, next);
        }
        prev = curr;
        curr = next;
    }

    return curr;
}

/* 행렬 거듭제곱을 이용한 피보나치
 * - 시간복잡도: O(log n)
 * - 분할 정복 사용
 */
Matrix2x2 matrix_power(Matrix2x2 base, int exp, bool print_steps) {
    if (print_steps) {
        printf("Computing matrix power for n = %d\n", exp);
    }

    if (exp == 0) {
        Matrix2x2 identity = { {1, 0}, {0, 1} };
        return identity;
    }
    if (exp == 1) return base;

    Matrix2x2 half = matrix_power(base, exp / 2, print_steps);
    Matrix2x2 result = matrix_multiply(half, half);

    if (exp % 2 == 1) {
        result = matrix_multiply(result, base);
    }

    if (print_steps) {
        printf("Matrix power %d result:\n", exp);
        printf("[[%lld %lld]\n", result.m[0][0], result.m[0][1]);
        printf(" [%lld %lld]]\n", result.m[1][0], result.m[1][1]);
    }

    return result;
}

/* 행렬 방식 피보나치
 * - F(n)을 행렬 거듭제곱으로 계산
 */
long long fib_matrix(int n, bool print_steps) {
    if (n <= 1) return n;

    Matrix2x2 base = { {1, 1}, {1, 0} };
    if (print_steps) {
        printf("Base matrix:\n[[1 1]\n [1 0]]\n\n");
    }

    Matrix2x2 result = matrix_power(base, n - 1, print_steps);

    return result.m[0][0];
}

/* 수학적 공식을 이용한 피보나치 (Binet's Formula)
 * - 시간복잡도: O(1)
 * - 큰 수에서 정밀도 문제 있음
 */
double fib_binet(int n) {
    const double phi = (1 + sqrt(5)) / 2;  // 황금비
    return round((pow(phi, n) - pow(-phi, -n)) / sqrt(5));
}

/* 실행 시간 측정 및 비교 */
void compare_methods(int n) {
    printf("\nComparing different methods for F(%d):\n", n);
    printf("----------------------------------------\n");

    // 동적 계획법
    clock_t start = clock();
    long long result_dp = fib_dynamic(n, false);
    clock_t end = clock();
    printf("Dynamic Programming: %lld (%.6f seconds)\n",
        result_dp, ((double)(end - start)) / CLOCKS_PER_SEC);

    // 행렬 방식
    start = clock();
    long long result_matrix = fib_matrix(n, false);
    end = clock();
    printf("Matrix Method: %lld (%.6f seconds)\n",
        result_matrix, ((double)(end - start)) / CLOCKS_PER_SEC);

    // Binet 공식
    start = clock();
    double result_binet = fib_binet(n);
    end = clock();
    printf("Binet's Formula: %.0f (%.6f seconds)\n",
        result_binet, ((double)(end - start)) / CLOCKS_PER_SEC);

    // 기본 재귀 (작은 수만)
    if (n <= 40) {
        start = clock();
        long long result_rec = fib_recursive(n, false);
        end = clock();
        printf("Basic Recursive: %lld (%.6f seconds)\n",
            result_rec, ((double)(end - start)) / CLOCKS_PER_SEC);
    }
    else {
        printf("Basic Recursive: Too large to compute\n");
    }
}

/* 메모리 사용량 분석 */
void analyze_memory_usage(void) {
    printf("\nMemory Usage Analysis:\n");
    printf("---------------------\n");
    printf("Basic Recursive: O(n) stack space\n");
    printf("Dynamic Programming: O(1) auxiliary space\n");
    printf("Matrix Method: O(1) auxiliary space\n");
    printf("Binet's Formula: O(1) auxiliary space\n\n");

    printf("Detailed explanation:\n");
    printf("1. Basic Recursive:\n");
    printf("   - Requires stack space for each recursive call\n");
    printf("   - Maximum stack depth = n\n");
    printf("   - Can cause stack overflow for large n\n\n");

    printf("2. Dynamic Programming:\n");
    printf("   - Only stores previous two values\n");
    printf("   - Constant extra space\n");
    printf("   - Very memory efficient\n\n");

    printf("3. Matrix Method:\n");
    printf("   - Uses 2x2 matrix operations\n");
    printf("   - Constant extra space\n");
    printf("   - Logarithmic time complexity\n\n");

    printf("4. Binet's Formula:\n");
    printf("   - No extra space needed\n");
    printf("   - Limited by floating-point precision\n");
    printf("   - Best for small numbers\n");
}

/* 메뉴 출력 */
void print_menu(void) {
    printf("\n=== Fibonacci Calculation Menu ===\n");
    printf("1. Basic recursive method\n");
    printf("2. Dynamic programming method\n");
    printf("3. Matrix power method\n");
    printf("4. Compare all methods\n");
    printf("5. Analyze memory usage\n");
    printf("0. Exit\n");
    printf("Choice: ");
}

int main(void) {
    int choice;
    do {
        print_menu();
        scanf("%d", &choice);

        switch (choice) {
        case 1:  // Basic recursive
        case 2:  // Dynamic programming
        case 3: {  // Matrix power
            int n;
            printf("Enter n: ");
            scanf("%d", &n);

            if (n < 0) {
                printf("Please enter a non-negative number.\n");
                break;
            }

            if (choice == 1 && n > 40) {
                printf("Warning: n > 40 will take very long with recursive method.\n");
                printf("Do you want to continue? (1/0): ");
                int confirm;
                scanf("%d", &confirm);
                if (!confirm) break;
            }

            long long result;
            switch (choice) {
            case 1:
                result = fib_recursive(n, true);
                break;
            case 2:
                result = fib_dynamic(n, true);
                break;
            case 3:
                result = fib_matrix(n, true);
                break;
            }

            printf("\nF(%d) = %lld\n", n, result);
            break;
        }

        case 4: {  // Compare methods
            int n;
            printf("Enter n: ");
            scanf("%d", &n);
            if (n < 0) {
                printf("Please enter a non-negative number.\n");
                break;
            }
            compare_methods(n);
            break;
        }

        case 5:  // Analyze memory
            analyze_memory_usage();
            break;

        case 0:  // Exit
            printf("Exiting program\n");
            break;

        default:
            printf("Invalid choice\n");
        }
    } while (choice != 0);

    return 0;
}

/*
==========================================
상세 설명 및 주요 개념
==========================================

1. 다양한 접근 방식
---------------
기본 재귀:
- 가장 직관적인 구현
- F(n) = F(n-1) + F(n-2)
- 지수 시간 복잡도
- 교육용으로 적합

동적 계획법:
- 중복 계산 제거
- 선형 시간 복잡도
- 메모리 효율적
- 실용적인 방법

행렬 거듭제곱:
- 분할 정복의 핵심
- 로그 시간 복잡도
- 큰 수에 효율적
- 고급 수학 개념

2. 행렬 방식의 이해
---------------
기본 원리:
[[1 1], [1 0]]^n = [[F(n+1) F(n)], [F(n) F(n-1)]]

수학적 증명:
1) n = 1일 때 성립
2) n -> n+1로의 귀납적 증명
3) 행렬 곱셈의 성질 활용

장점:
- 빠른 계산 속도
- 큰 수 처리 가능
- 수학적 우아함

3. 성능 분석
----------
시간 복잡도:
- 재귀: O(2^n)
- 동적 계획법: O(n)
- 행렬: O(log n)
- Binet: O(1)

공간 복잡도:
- 재귀: O(n)
- 동적 계획법: O(1)
- 행렬: O(1)
- Binet: O(1)

4. 수학적 통찰
-----------
피보나치 수의 성질:
- 황금비와의 관계
- 행렬 표현의 의미
- 지수적 증가
- 수학적 패턴

응용 분야:
- 자연계의 피보나치 수열
- 암호학
- 알고리즘 설계
- 수학적 모델링

5. 최적화 기법
-----------
- 행렬 곱셈 최적화
- 캐시 활용
- 모듈러 연산
- 병렬화 가능성

이 구현은 피보나치 수 계산의
다양한 방법을 보여주며, 각 방식의
장단점을 실제로 비교할 수 있게
합니다.
*/