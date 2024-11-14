#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

/*
거듭제곱의 분할 정복:
- x^n = x^(n/2) * x^(n/2)     (n이 짝수일 때)
- x^n = x^(n/2) * x^(n/2) * x (n이 홀수일 때)

예시: 2^10 계산
1. 2^10 = (2^5)^2
2. 2^5 = 2^4 * 2
3. 2^4 = (2^2)^2
4. 2^2 = (2^1)^2
5. 2^1 = 2

일반적 방법: O(n) -> 분할 정복: O(log n)
*/

/* 일반적인 거듭제곱 계산
 * - 시간복잡도: O(n)
 */
long long power_naive(long long base, int exp, bool print_steps) {
    long long result = 1;

    for (int i = 0; i < exp; i++) {
        result *= base;
        if (print_steps) {
            printf("Step %d: %lld^%d = %lld\n",
                i + 1, base, i + 1, result);
        }
    }

    return result;
}

/* 분할 정복을 이용한 거듭제곱 계산
 * - 시간복잡도: O(log n)
 */
long long power_divide_conquer(long long base, int exp, bool print_steps) {
    if (print_steps) {
        printf("Calculating %lld^%d\n", base, exp);
    }

    if (exp == 0) {
        if (print_steps) printf("Base case: %lld^0 = 1\n", base);
        return 1;
    }
    if (exp == 1) {
        if (print_steps) printf("Base case: %lld^1 = %lld\n", base, base);
        return base;
    }

    // 분할: 지수를 절반으로 나눔
    long long half = power_divide_conquer(base, exp / 2, print_steps);
    long long result;

    if (exp % 2 == 0) {
        // 짝수: half * half
        result = half * half;
        if (print_steps) {
            printf("Even exp: %lld^%d = %lld^%d * %lld^%d = %lld * %lld = %lld\n",
                base, exp, base, exp / 2, base, exp / 2, half, half, result);
        }
    }
    else {
        // 홀수: half * half * base
        result = half * half * base;
        if (print_steps) {
            printf("Odd exp: %lld^%d = %lld^%d * %lld^%d * %lld = %lld * %lld * %lld = %lld\n",
                base, exp, base, exp / 2, base, exp / 2, base, half, half, base, result);
        }
    }

    return result;
}

/* 모듈러 연산을 포함한 거듭제곱 계산
 * - 시간복잡도: O(log n)
 * - 큰 수의 거듭제곱을 안전하게 계산
 */
long long power_modular(long long base, int exp, long long mod, bool print_steps) {
    if (print_steps) {
        printf("Calculating %lld^%d mod %lld\n", base, exp, mod);
    }

    if (exp == 0) {
        if (print_steps) printf("Base case: %lld^0 mod %lld = 1\n", base, mod);
        return 1;
    }

    // 분할: 지수를 절반으로 나눔
    long long half = power_modular(base, exp / 2, mod, print_steps);
    long long result;

    if (exp % 2 == 0) {
        result = (half * half) % mod;
        if (print_steps) {
            printf("Even exp: %lld^%d mod %lld = (%lld * %lld) mod %lld = %lld\n",
                base, exp, mod, half, half, mod, result);
        }
    }
    else {
        result = ((half * half) % mod * base % mod) % mod;
        if (print_steps) {
            printf("Odd exp: %lld^%d mod %lld = (%lld * %lld * %lld) mod %lld = %lld\n",
                base, exp, mod, half, half, base, mod, result);
        }
    }

    return result;
}

/* 이진수 표현을 이용한 거듭제곱 계산
 * - 시간복잡도: O(log n)
 * - 비트 연산 활용
 */
long long power_binary(long long base, int exp, bool print_steps) {
    long long result = 1;

    if (print_steps) {
        printf("Binary representation of %d: ", exp);
        for (int i = 31; i >= 0; i--) {
            if (exp & (1 << i)) printf("1");
            else printf("0");
        }
        printf("\n");
    }

    while (exp > 0) {
        if (exp & 1) {  // 현재 비트가 1이면
            if (print_steps) {
                printf("Current bit is 1: multiply result by %lld\n", base);
            }
            result *= base;
        }

        base *= base;  // 다음 비트를 위해 base를 제곱
        exp >>= 1;     // 다음 비트로 이동

        if (print_steps && exp > 0) {
            printf("Square base: %lld, Next exp: %d\n", base, exp);
        }
    }

    return result;
}

/* 실행 시간 측정 함수 */
double measure_time(long long (*func)(long long, int, bool),
    long long base, int exp, bool print_steps) {
    clock_t start = clock();
    long long result = func(base, exp, print_steps);
    clock_t end = clock();

    printf("Result: %lld\n", result);
    return ((double)(end - start)) / CLOCKS_PER_SEC;
}

/* 성능 비교 분석 */
void compare_performance(void) {
    printf("\nPerformance Comparison:\n");
    printf("--------------------\n");

    int exponents[] = { 10, 100, 1000, 10000 };
    int num_tests = sizeof(exponents) / sizeof(exponents[0]);

    for (int i = 0; i < num_tests; i++) {
        int exp = exponents[i];
        printf("\nTesting with base = 2, exponent = %d\n", exp);

        printf("Naive approach: ");
        double time_naive = measure_time(power_naive, 2, exp, false);
        printf("Time: %f seconds\n", time_naive);

        printf("Divide and conquer: ");
        double time_dc = measure_time(power_divide_conquer, 2, exp, false);
        printf("Time: %f seconds\n", time_dc);

        printf("Binary method: ");
        double time_bin = measure_time(power_binary, 2, exp, false);
        printf("Time: %f seconds\n", time_bin);
    }
}

/* 메뉴 출력 */
void print_menu(void) {
    printf("\n=== Fast Power Calculation Menu ===\n");
    printf("1. Calculate power (naive)\n");
    printf("2. Calculate power (divide and conquer)\n");
    printf("3. Calculate power with modulo\n");
    printf("4. Calculate power (binary method)\n");
    printf("5. Compare performance\n");
    printf("0. Exit\n");
    printf("Choice: ");
}

int main(void) {
    int choice;
    do {
        print_menu();
        scanf("%d", &choice);

        switch (choice) {
        case 1:  // Naive
        case 2:  // Divide and conquer
        case 4: { // Binary method
            long long base;
            int exp;
            printf("Enter base: ");
            scanf("%lld", &base);
            printf("Enter exponent: ");
            scanf("%d", &exp);

            long long result;
            if (choice == 1) {
                result = power_naive(base, exp, true);
            }
            else if (choice == 2) {
                result = power_divide_conquer(base, exp, true);
            }
            else {
                result = power_binary(base, exp, true);
            }

            printf("Result: %lld\n", result);
            break;
        }

        case 3: {  // Modular exponentiation
            long long base, mod;
            int exp;
            printf("Enter base: ");
            scanf("%lld", &base);
            printf("Enter exponent: ");
            scanf("%d", &exp);
            printf("Enter modulus: ");
            scanf("%lld", &mod);

            long long result = power_modular(base, exp, mod, true);
            printf("Result: %lld\n", result);
            break;
        }

        case 5:  // Compare performance
            compare_performance();
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

1. 분할 정복 접근법
---------------
기본 아이디어:
- 지수를 절반으로 나눔
- 부분 결과를 재사용
- 홀수/짝수 경우 구분

장점:
- 연산 횟수 감소
- 빠른 계산 속도
- 메모리 효율적

2. 시간 복잡도 분석
---------------
일반적 방법: O(n)
- n번의 곱셈 필요
- 선형 시간 복잡도

분할 정복: O(log n)
- 지수를 절반씩 감소
- 로그 시간 복잡도
- 재귀 호출 트리

3. 구현 방식
----------
재귀적 구현:
- 직관적 이해 용이
- 코드 간단
- 스택 사용

이진수 방식:
- 비트 연산 활용
- 반복적 구현
- 메모리 효율적

4. 응용 분야
----------
- 암호화 알고리즘
- 모듈러 연산
- 큰 수 계산
- 고속 지수 계산

5. 최적화 기법
-----------
- 모듈러 연산 활용
- 비트 연산 최적화
- 재귀 제거
- 중간 결과 재사용

이 구현은 거듭제곱 계산의 다양한
방법을 보여주며, 실제 성능 차이를
비교할 수 있습니다. 다음 단계인
피보나치 수 계산의 기초가 됩니다.
*/