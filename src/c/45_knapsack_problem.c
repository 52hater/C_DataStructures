#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/*
배낭 문제 (0/1 Knapsack Problem)
==============================

특징:
1. 각 물건을 통째로 넣거나 안 넣거나 (0 또는 1)
2. 제한된 무게 내에서 최대 가치 선택
3. 분할 불가능한 물건들의 조합 최적화

기본 개념:
- 물건 i의 무게: w[i]
- 물건 i의 가치: v[i]
- 배낭 용량: W
- 최적해: dp[i][w]
*/

// === 물건 구조체 ===
typedef struct {
    int weight;     // 무게
    int value;      // 가치
    char* name;     // 물건 이름
} Item;

// === 배낭 테이블 구조체 ===
typedef struct {
    int** table;    // DP 테이블
    bool** selected; // 선택된 물건 추적
    int rows;       // 물건 수 + 1
    int cols;       // 배낭 용량 + 1
} KnapsackTable;

// === 배낭 결과 구조체 ===
typedef struct {
    int maxValue;           // 최대 가치
    bool* selectedItems;    // 선택된 물건들
    int numSelected;        // 선택된 물건 수
} KnapsackResult;

// === 배낭 테이블 생성 ===
KnapsackTable* create_knapsack_table(int numItems, int capacity) {
    KnapsackTable* table = (KnapsackTable*)malloc(sizeof(KnapsackTable));
    table->rows = numItems + 1;
    table->cols = capacity + 1;

    // DP 테이블 할당
    table->table = (int**)malloc(table->rows * sizeof(int*));
    table->selected = (bool**)malloc(table->rows * sizeof(bool*));

    for (int i = 0; i < table->rows; i++) {
        table->table[i] = (int*)calloc(table->cols, sizeof(int));
        table->selected[i] = (bool*)calloc(table->cols, sizeof(bool));
    }

    return table;
}

// === 배낭 테이블 메모리 해제 ===
void destroy_knapsack_table(KnapsackTable* table) {
    for (int i = 0; i < table->rows; i++) {
        free(table->table[i]);
        free(table->selected[i]);
    }
    free(table->table);
    free(table->selected);
    free(table);
}

// === 배낭 테이블 출력 ===
void print_knapsack_table(KnapsackTable* table, Item* items) {
    printf("\n=== 배낭 문제 DP 테이블 ===\n");
    printf("    ");
    for (int w = 0; w < table->cols; w++) {
        printf("%4d ", w);
    }
    printf("\n");

    for (int i = 0; i < table->rows; i++) {
        if (i == 0) printf("0   ");
        else printf("%-3s ", items[i - 1].name);

        for (int w = 0; w < table->cols; w++) {
            printf("%4d ", table->table[i][w]);
        }
        printf("\n");
    }
}

// === 배낭 문제 해결 ===
KnapsackResult* solve_knapsack(Item* items, int numItems, int capacity, bool print_steps) {
    KnapsackTable* table = create_knapsack_table(numItems, capacity);

    if (print_steps) {
        printf("\n=== 배낭 문제 해결 과정 ===\n");
    }

    // Bottom-up 방식으로 테이블 채우기
    for (int i = 1; i <= numItems; i++) {
        for (int w = 0; w <= capacity; w++) {
            if (items[i - 1].weight <= w) {
                int value_with_item = table->table[i - 1][w - items[i - 1].weight]
                    + items[i - 1].value;
                int value_without_item = table->table[i - 1][w];

                if (value_with_item > value_without_item) {
                    table->table[i][w] = value_with_item;
                    table->selected[i][w] = true;

                    if (print_steps) {
                        printf("물건 %s 선택 (무게: %d, 가치: %d)\n",
                            items[i - 1].name, items[i - 1].weight, items[i - 1].value);
                    }
                }
                else {
                    table->table[i][w] = value_without_item;
                }
            }
            else {
                table->table[i][w] = table->table[i - 1][w];
            }
        }

        if (print_steps) {
            print_knapsack_table(table, items);
            printf("\n");
        }
    }

    // 결과 생성
    KnapsackResult* result = (KnapsackResult*)malloc(sizeof(KnapsackResult));
    result->maxValue = table->table[numItems][capacity];
    result->selectedItems = (bool*)calloc(numItems, sizeof(bool));
    result->numSelected = 0;

    // 선택된 물건 추적
    int i = numItems;
    int w = capacity;
    while (i > 0 && w > 0) {
        if (table->selected[i][w]) {
            result->selectedItems[i - 1] = true;
            result->numSelected++;
            w -= items[i - 1].weight;
        }
        i--;
    }

    destroy_knapsack_table(table);
    return result;
}

// === 결과 출력 ===
void print_knapsack_result(KnapsackResult* result, Item* items, int numItems) {
    printf("\n=== 배낭 문제 해결 결과 ===\n");
    printf("최대 가치: %d\n", result->maxValue);
    printf("\n선택된 물건들:\n");

    int totalWeight = 0;
    for (int i = 0; i < numItems; i++) {
        if (result->selectedItems[i]) {
            printf("- %s (무게: %d, 가치: %d)\n",
                items[i].name, items[i].weight, items[i].value);
            totalWeight += items[i].weight;
        }
    }
    printf("\n총 무게: %d\n", totalWeight);
}

// === 메인 함수 ===
int main(void) {
    printf("=== 배낭 문제 해결 프로그램 ===\n");

    int numItems, capacity;
    printf("\n물건 개수 입력: ");
    scanf("%d", &numItems);

    Item* items = (Item*)malloc(numItems * sizeof(Item));

    for (int i = 0; i < numItems; i++) {
        items[i].name = (char*)malloc(20 * sizeof(char));
        printf("\n물건 %d 정보 입력:\n", i + 1);
        printf("이름: ");
        scanf("%s", items[i].name);
        printf("무게: ");
        scanf("%d", &items[i].weight);
        printf("가치: ");
        scanf("%d", &items[i].value);
    }

    printf("\n배낭 용량 입력: ");
    scanf("%d", &capacity);

    bool print_steps;
    printf("\n과정을 출력하시겠습니까? (1: 예, 0: 아니오): ");
    scanf("%d", &print_steps);

    KnapsackResult* result = solve_knapsack(items, numItems, capacity, print_steps);
    print_knapsack_result(result, items, numItems);

    // 메모리 해제
    for (int i = 0; i < numItems; i++) {
        free(items[i].name);
    }
    free(items);
    free(result->selectedItems);
    free(result);

    return 0;
}

/*
배낭 문제 분석
===========

1. 시간 복잡도
-----------
- O(nW): n은 물건 수, W는 배낭 용량
- 모든 부분 문제를 한 번씩 계산
- 각 셀마다 O(1) 연산

2. 공간 복잡도
-----------
- O(nW): 2차원 DP 테이블
- 선택 추적을 위한 추가 배열
- 입력 데이터 저장 공간

3. 최적화 가능성
------------
공간 최적화:
- O(W) 공간으로 줄일 수 있음
- 한 행만 사용하는 방식
- 선택 추적 포기 시 가능

병렬화:
- 각 행의 계산 병렬 처리
- GPU 활용 가능
- 대규모 데이터 처리

4. 응용 분야
---------
- 리소스 할당
- 포트폴리오 최적화
- 예산 계획
- 화물 적재
- 시간 관리

5. 변형 문제
---------
분할 가능한 배낭:
- Fractional Knapsack
- Greedy 접근 가능
- 더 단순한 해결

다중 제약:
- Multiple Constraints
- 더 복잡한 DP 필요
- 차원 증가

6. 구현 고려사항
------------
정밀도:
- 정수 가정
- 오버플로우 주의
- 부동소수점 처리

메모리 관리:
- 동적 할당 주의
- 메모리 재사용
- 큰 입력 처리

시각화:
- 단계별 출력
- 결과 표현
- 사용자 인터페이스

이 구현은 교육용으로 적합하며
실제 응용에도 활용 가능한
완전한 배낭 문제 해결기를
제공합니다.
*/