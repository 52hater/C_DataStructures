#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

/*
동적 계획법 (Dynamic Programming)
============================

핵심 개념:
1. 최적 부분 구조 (Optimal Substructure)
   - 큰 문제의 최적해가 작은 문제들의 최적해를 포함
   - F(n) = F(n-1) + F(n-2) 형태로 표현 가능
   - 상향식/하향식 접근 모두 가능

2. 중복되는 부분 문제 (Overlapping Subproblems)
   - 동일한 작은 문제들이 여러 번 등장
   - 메모리에 저장하여 재사용 (메모이제이션)
   - 중복 계산 제거로 성능 향상

3. 구현 방식
   a) Top-down (하향식)
      - 재귀적 접근
      - 필요한 부분만 계산
      - 메모이제이션 활용

   b) Bottom-up (상향식)
      - 반복적 접근
      - 작은 문제부터 순차적 해결
      - 테이블 사용
*/

// === 1. 기본 재귀 방식 ===
long long fib_recursive(int n, bool print_steps) {
    static int depth = 0;  // 재귀 깊이 추적
    depth++;

    // 들여쓰기로 재귀 깊이 표현
    if (print_steps) {
        for (int i = 1; i < depth; i++) printf("  ");
        printf("Calculating F(%d)\n", n);
    }

    // 기저 조건
    if (n <= 1) {
        if (print_steps) {
            for (int i = 1; i < depth; i++) printf("  ");
            printf("Base case: F(%d) = %d\n", n, n);
        }
        depth--;
        return n;
    }

    // 재귀적 계산
    long long result = fib_recursive(n - 1, print_steps) +
        fib_recursive(n - 2, print_steps);

    if (print_steps) {
        for (int i = 1; i < depth; i++) printf("  ");
        printf("F(%d) = F(%d) + F(%d) = %lld\n", n, n - 1, n - 2, result);
    }

    depth--;
    return result;
}

// === 2. 메모이제이션 (Top-down) ===
long long* memo;
int* calculation_count;  // 각 n에 대한 계산 횟수 추적

void init_memoization(int size) {
    memo = (long long*)malloc(size * sizeof(long long));
    calculation_count = (int*)calloc(size, sizeof(int));

    for (int i = 0; i < size; i++) {
        memo[i] = -1;  // 미계산 상태 표시
    }
}

void cleanup_memoization(void) {
    free(memo);
    free(calculation_count);
}

long long fib_memoization(int n, bool print_steps) {
    static int depth = 0;
    depth++;

    // 진행 상황 출력
    if (print_steps) {
        for (int i = 1; i < depth; i++) printf("  ");
        printf("Checking F(%d)", n);
        if (memo[n] != -1) {
            printf(" [Cached value: %lld]\n", memo[n]);
        }
        else {
            printf(" [Not calculated yet]\n");
        }
    }

    // 기저 조건
    if (n <= 1) {
        calculation_count[n]++;
        if (print_steps) {
            for (int i = 1; i < depth; i++) printf("  ");
            printf("Base case: F(%d) = %d\n", n, n);
        }
        depth--;
        return n;
    }

    // 이미 계산된 값이면 재사용
    if (memo[n] != -1) {
        if (print_steps) {
            for (int i = 1; i < depth; i++) printf("  ");
            printf("Reusing F(%d) = %lld\n", n, memo[n]);
        }
        depth--;
        return memo[n];
    }

    // 새로운 값 계산
    calculation_count[n]++;
    memo[n] = fib_memoization(n - 1, print_steps) +
        fib_memoization(n - 2, print_steps);

    if (print_steps) {
        for (int i = 1; i < depth; i++) printf("  ");
        printf("Calculated F(%d) = %lld\n", n, memo[n]);
    }

    depth--;
    return memo[n];
}

// === 3. 타뷸레이션 (Bottom-up) ===
typedef struct {
    long long* table;
    int size;
} FibTable;

FibTable create_fib_table(int size) {
    FibTable table;
    table.table = (long long*)malloc(size * sizeof(long long));
    table.size = size;

    // 기저 조건 초기화
    if (size > 0) table.table[0] = 0;
    if (size > 1) table.table[1] = 1;

    return table;
}

void cleanup_fib_table(FibTable* table) {
    free(table->table);
}

long long fib_tabulation(int n, bool print_steps) {
    if (n < 0) return -1;

    FibTable table = create_fib_table(n + 1);

    if (print_steps) {
        printf("\n=== Bottom-up Calculation Steps ===\n");
        if (n >= 0) printf("Initialize F(0) = 0\n");
        if (n >= 1) printf("Initialize F(1) = 1\n");
    }

    // Bottom-up으로 값 계산
    for (int i = 2; i <= n; i++) {
        table.table[i] = table.table[i - 1] + table.table[i - 2];

        if (print_steps) {
            printf("F(%d) = F(%d) + F(%d) = %lld + %lld = %lld\n",
                i, i - 1, i - 2, table.table[i - 1], table.table[i - 2], table.table[i]);
        }
    }

    long long result = table.table[n];
    cleanup_fib_table(&table);
    return result;
}

// === 4. 공간 최적화 버전 ===
long long fib_optimized(int n, bool print_steps) {
    if (n <= 1) return n;

    long long prev = 0, curr = 1;

    if (print_steps) {
        printf("\n=== Space-Optimized Calculation Steps ===\n");
        printf("Initialize: prev = 0, curr = 1\n");
    }

    for (int i = 2; i <= n; i++) {
        long long next = prev + curr;

        if (print_steps) {
            printf("Step %d: %lld + %lld = %lld\n", i, prev, curr, next);
        }

        prev = curr;
        curr = next;
    }

    return curr;
}

// === 성능 분석 및 비교 ===
void analyze_performance(int n) {
    clock_t start, end;
    double cpu_time;

    printf("\n=== Performance Analysis for F(%d) ===\n", n);

    // 1. 기본 재귀
    if (n <= 40) {
        start = clock();
        long long result1 = fib_recursive(n, false);
        end = clock();
        cpu_time = ((double)(end - start)) / CLOCKS_PER_SEC;
        printf("\n1. Basic Recursive:\n");
        printf("   Result: %lld\n", result1);
        printf("   Time: %.6f seconds\n", cpu_time);
        printf("   Space: O(n) - 재귀 스택\n");
    }
    else {
        printf("\n1. Basic Recursive: n > 40 - 시간 초과 예상\n");
    }

    // 2. 메모이제이션
    start = clock();
    init_memoization(n + 1);
    long long result2 = fib_memoization(n, false);
    end = clock();
    cpu_time = ((double)(end - start)) / CLOCKS_PER_SEC;

    printf("\n2. Memoization:\n");
    printf("   Result: %lld\n", result2);
    printf("   Time: %.6f seconds\n", cpu_time);
    printf("   Calculation counts:\n");
    for (int i = 0; i <= n; i++) {
        if (calculation_count[i] > 0) {
            printf("   F(%d): %d times\n", i, calculation_count[i]);
        }
    }
    cleanup_memoization();

    // 3. 타뷸레이션
    start = clock();
    long long result3 = fib_tabulation(n, false);
    end = clock();
    cpu_time = ((double)(end - start)) / CLOCKS_PER_SEC;

    printf("\n3. Tabulation:\n");
    printf("   Result: %lld\n", result3);
    printf("   Time: %.6f seconds\n", cpu_time);
    printf("   Space: O(n) - 테이블\n");

    // 4. 공간 최적화
    start = clock();
    long long result4 = fib_optimized(n, false);
    end = clock();
    cpu_time = ((double)(end - start)) / CLOCKS_PER_SEC;

    printf("\n4. Space Optimized:\n");
    printf("   Result: %lld\n", result4);
    printf("   Time: %.6f seconds\n", cpu_time);
    printf("   Space: O(1) - 상수 공간\n");
}

// === 메인 함수 ===
int main(void) {
    printf("=== 동적 계획법 학습: 피보나치 수열 ===\n");
    printf("이 프로그램은 네 가지 다른 구현 방식을 비교합니다:\n");
    printf("1. 기본 재귀 (educational)\n");
    printf("2. 메모이제이션 (top-down)\n");
    printf("3. 타뷸레이션 (bottom-up)\n");
    printf("4. 공간 최적화 버전\n\n");

    int choice, n;
    bool running = true;

    while (running) {
        printf("\n=== 메뉴 ===\n");
        printf("1. 기본 재귀 실행 (단계 표시)\n");
        printf("2. 메모이제이션 실행 (단계 표시)\n");
        printf("3. 타뷸레이션 실행 (단계 표시)\n");
        printf("4. 공간 최적화 실행 (단계 표시)\n");
        printf("5. 모든 방식 성능 비교\n");
        printf("0. 종료\n");
        printf("선택: ");
        scanf("%d", &choice);

        if (choice == 0) {
            running = false;
            continue;
        }

        printf("계산할 피보나치 수 입력 (n): ");
        scanf("%d", &n);

        if (n < 0) {
            printf("음수는 계산할 수 없습니다.\n");
            continue;
        }

        switch (choice) {
        case 1:
            if (n > 40) {
                printf("n이 너무 큽니다. 40 이하를 입력하세요.\n");
                break;
            }
            printf("\n결과: %lld\n", fib_recursive(n, true));
            break;

        case 2:
            init_memoization(n + 1);
            printf("\n결과: %lld\n", fib_memoization(n, true));
            cleanup_memoization();
            break;

        case 3:
            printf("\n결과: %lld\n", fib_tabulation(n, true));
            break;

        case 4:
            printf("\n결과: %lld\n", fib_optimized(n, true));
            break;

        case 5:
            analyze_performance(n);
            break;

        default:
            printf("잘못된 선택입니다.\n");
        }
    }

    return 0;
}

/*
구현 분석 및 교훈
==============

1. 시간 복잡도 분석
----------------
기본 재귀:
- O(2^n)
- 매 호출마다 2번의 재귀 호출
- 중복 계산이 매우 많음

메모이제이션:
- O(n)
- 각 부분 문제를 한 번만 계산
- 캐시 적중으로 재계산 방지

타뷸레이션:
- O(n)
- 순차적 계산으로 오버헤드 최소화
- 모든 값을 한 번씩만 계산

공간 최적화:
- O(n) 시간
- O(1) 공간
- 가장 효율적인 구현

2. 공간 복잡도 분석
----------------
기본 재귀:
- O(n) 스택 공간
- 재귀 호출마다 스택 프레임 사용
- 스택 오버플로우 위험

메모이제이션:
- O(n) 메모리
- 모든 부분 문제의 결과 저장
- 스택 공간 추가 필요

타뷸레이션:
- O(n) 메모리
- 테이블에 모든 값 저장
- 스택 사용 없음

공간 최적화:
- O(1) 메모리
- 필요한 최소한의 값만 유지
- 추가 공간 없음

3. 교훈
------
중복 계산의 영향:
- 기본 재귀에서 지수적 증가
- 메모이제이션으로 극적인 성능 향상
- 같은 계산 반복이 성능 저하의 주원인

메모리-시간 트레이드오프:
- 공간을 사용해 시간 절약
- 메모리 사용량과 실행 속도의 균형
- 상황에 따른 적절한 선택

구현 방식의 선택:
- 문제 크기와 제약 조건 고려
- 가독성과 유지보수성 감안
- 실제 환경에서의 적용성 검토

4. 최적화 가능성
-------------
행렬 거듭제곱:
- O(log n) 시간 복잡도
- 분할 정복 기법 활용
- 대규모 수에 효과적

모듈러 연산:
- 오버플로우 방지
- 암호학적 응용
- 매우 큰 수 처리

병렬화:
- 독립적 부분 문제 병렬 처리
- 하드웨어 활용 최적화
- 대규모 계산에 유용

5. 실제 응용 분야
--------------
알고리즘 설계:
- 동적 계획법의 기초 예제
- 분할 정복과의 비교
- 최적화 기법 학습

시스템 설계:
- 캐싱 시스템 설계
- 메모리 관리 전략
- 성능 최적화 기법

수학적 응용:
- 수열과 패턴
- 황금비와의 관계
- 자연계 현상 모델링

6. 구현 시 고려사항
---------------
오버플로우 처리:
- 큰 수 처리 방법
- 자료형 선택
- 예외 처리

메모리 관리:
- 동적 할당 및 해제
- 메모리 누수 방지
- 효율적 자원 사용

디버깅 용이성:
- 단계별 진행 상황 출력
- 중간 결과 확인
- 오류 추적 지원

7. 성능 향상 전략
--------------
캐시 효율성:
- 메모리 접근 패턴 최적화
- 지역성 원리 활용
- 캐시 미스 최소화

메모리 재사용:
- 불필요한 할당 제거
- 버퍼 재활용
- 임시 객체 최소화

알고리즘 선택:
- 입력 크기별 최적 방법
- 하드웨어 특성 고려
- 실행 환경 적응

이 구현은 동적 계획법의 기본을
피보나치 수열을 통해 설명하며,
다양한 최적화 기법과 실제 응용
가능성을 보여줍니다.
*/