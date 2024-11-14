#include <stdio.h>
#include <stdlib.h>

/*
탐욕 알고리즘 - 거스름돈 문제
==========================

특징:
1. 가장 큰 단위부터 선택
2. 각 선택이 이후 선택에 영향
3. 한국/미국 화폐처럼 큰 단위가 작은 단위의 배수일 때 최적해 보장
4. 임의의 화폐 단위에서는 최적해 보장하지 않음
*/

typedef struct {
    int* coins;         // 동전 종류
    int* counts;        // 각 동전의 사용 개수
    int num_types;      // 동전 종류의 수
    int total_coins;    // 총 사용된 동전 수
} ChangeResult;

/* 거스름돈 계산 함수 */
ChangeResult* calculate_change(int amount, int* denominations, int num_denoms) {
    ChangeResult* result = (ChangeResult*)malloc(sizeof(ChangeResult));
    result->coins = (int*)malloc(num_denoms * sizeof(int));
    result->counts = (int*)calloc(num_denoms, sizeof(int));
    result->num_types = num_denoms;
    result->total_coins = 0;

    printf("\n=== 거스름돈 계산 과정 ===\n");
    printf("목표 금액: %d\n\n", amount);
    int remaining = amount;

    // 큰 단위부터 처리
    for (int i = 0; i < num_denoms; i++) {
        result->coins[i] = denominations[i];
        if (remaining >= denominations[i]) {
            result->counts[i] = remaining / denominations[i];
            remaining = remaining % denominations[i];
            result->total_coins += result->counts[i];

            printf("단위 %d원 사용: %d개\n",
                denominations[i], result->counts[i]);
            printf("남은 금액: %d원\n\n", remaining);
        }
    }

    if (remaining > 0) {
        printf("경고: 거스름돈을 완전히 만들 수 없습니다!\n");
        printf("부족한 금액: %d원\n", remaining);
    }

    return result;
}

/* 결과 출력 함수 */
void print_change_result(ChangeResult* result) {
    printf("\n=== 최종 거스름돈 내역 ===\n");
    for (int i = 0; i < result->num_types; i++) {
        if (result->counts[i] > 0) {
            printf("%d원: %d개 (총 %d원)\n",
                result->coins[i],
                result->counts[i],
                result->coins[i] * result->counts[i]);
        }
    }
    printf("\n총 사용된 동전 개수: %d개\n", result->total_coins);
}

/* 반례 케이스 검사 함수 */
void check_counterexample(void) {
    printf("\n=== 반례 케이스 검사 ===\n");
    int amount = 6;
    int denoms[] = { 4, 3, 1 };  // 비최적 케이스
    int num_denoms = sizeof(denoms) / sizeof(denoms[0]);

    printf("금액: %d\n", amount);
    printf("화폐 단위: ");
    for (int i = 0; i < num_denoms; i++) {
        printf("%d ", denoms[i]);
    }
    printf("\n\n");

    printf("탐욕적 방법 결과:\n");
    ChangeResult* greedy = calculate_change(amount, denoms, num_denoms);
    print_change_result(greedy);

    printf("\n최적해: 3원 2개 = 2개\n");
    printf("탐욕적 해: 4원 1개 + 1원 2개 = 3개\n");
    printf("\n이 경우 탐욕적 방법은 최적해를 찾지 못함\n");

    free(greedy->coins);
    free(greedy->counts);
    free(greedy);
}

/* 메모리 해제 함수 */
void cleanup_change_result(ChangeResult* result) {
    free(result->coins);
    free(result->counts);
    free(result);
}

int main(void) {
    int default_denoms[] = { 500, 100, 50, 10, 1 };  // 한국 화폐 단위
    int num_default_denoms = sizeof(default_denoms) / sizeof(default_denoms[0]);

    printf("=== 거스름돈 계산 프로그램 ===\n");
    printf("1. 기본 화폐 단위 사용 (한국 원화)\n");
    printf("2. 사용자 정의 화폐 단위\n");
    printf("3. 반례 케이스 확인\n");
    printf("선택: ");

    int choice;
    scanf("%d", &choice);

    if (choice == 1 || choice == 2) {
        int amount;
        printf("금액 입력: ");
        scanf("%d", &amount);

        ChangeResult* result;
        if (choice == 1) {
            result = calculate_change(amount, default_denoms, num_default_denoms);
        }
        else {
            int num_denoms;
            printf("화폐 단위 개수 입력: ");
            scanf("%d", &num_denoms);

            int* denoms = (int*)malloc(num_denoms * sizeof(int));
            printf("화폐 단위 입력 (큰 단위부터): ");
            for (int i = 0; i < num_denoms; i++) {
                scanf("%d", &denoms[i]);
            }

            result = calculate_change(amount, denoms, num_denoms);
            free(denoms);
        }

        print_change_result(result);
        cleanup_change_result(result);
    }
    else if (choice == 3) {
        check_counterexample();
    }

    return 0;
}

/*
거스름돈 문제 분석
==============

1. 탐욕적 선택의 특성
-----------------
- 매 단계에서 가장 큰 단위 선택
- 이전 선택을 번복하지 않음
- 지역적 최적해 추구

2. 최적해 보장 조건
---------------
- 큰 단위가 작은 단위의 배수
- 연속된 단위 간 적절한 배수 관계
- 최소 단위가 1인 경우

3. 시간 복잡도
-----------
- O(N): N은 화폐 단위의 개수
- 각 단위에 대해 한 번씩만 계산
- 매우 효율적인 알고리즘

4. 응용 및 확장
-----------
- 자판기 거스름돈 계산
- ATM 출금 금액 계산
- 화폐 단위 최적화
- 동전 수 최소화

5. 한계점
-------
- 임의의 화폐 단위에서 최적해 미보장
- 특정 케이스에서 비효율적 해 도출
- 제약 조건 존재 시 적용 어려움

이 구현은 거스름돈 문제를 통해
탐욕 알고리즘의 기본 개념과
한계점을 명확히 보여줍니다.
*/