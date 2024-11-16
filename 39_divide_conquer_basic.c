#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/*
분할 정복(Divide and Conquer):
- 문제를 더 작은 하위 문제로 분할(Divide)
- 하위 문제들을 재귀적으로 해결(Conquer)
- 해결된 결과를 결합(Combine)

일반적인 형태:
if (문제가 충분히 작음) {
    직접 해결
} else {
    분할: 문제를 여러 하위 문제로 나눔
    정복: 하위 문제들을 재귀적으로 해결
    결합: 해결된 하위 문제들을 결합
}
*/

/* 기본적인 분할 정복 예제들 */

/* 1. 최대값 찾기
 * - 배열을 둘로 나누어 각각의 최대값을 찾고 비교
 * - 시간복잡도: O(n)
 * - 분할 정복의 기본 패턴을 보여주는 간단한 예제
 */
int find_max(int arr[], int left, int right) {
    // 기저 사례: 원소가 하나인 경우
    if (left == right) {
        return arr[left];
    }

    // 분할: 배열을 둘로 나눔
    int mid = (left + right) / 2;

    // 정복: 각 부분의 최대값을 재귀적으로 찾음
    int left_max = find_max(arr, left, mid);
    int right_max = find_max(arr, mid + 1, right);

    // 결합: 두 최대값을 비교하여 더 큰 값 반환
    return (left_max > right_max) ? left_max : right_max;
}

/* 2. 이진 검색 (분할 정복 버전)
 * - 정렬된 배열에서 중간값과 비교하여 탐색 범위를 절반으로 줄임
 * - 시간복잡도: O(log n)
 */
int binary_search(int arr[], int left, int right, int target) {
    if (left > right) {
        return -1;  // 찾지 못함
    }

    // 분할: 중간 지점 계산
    int mid = left + (right - left) / 2;  // 오버플로우 방지

    // 기저 사례: 찾은 경우
    if (arr[mid] == target) {
        return mid;
    }

    // 정복: 적절한 절반에서 재귀적으로 검색
    if (arr[mid] > target) {
        return binary_search(arr, left, mid - 1, target);
    }
    else {
        return binary_search(arr, mid + 1, right, target);
    }
}

/* 3. 배열의 합 계산
 * - 배열을 둘로 나누어 각각의 합을 계산하고 더함
 * - 시간복잡도: O(n)
 * - 병렬 처리의 가능성을 보여주는 예제
 */
int array_sum(int arr[], int left, int right) {
    // 기저 사례: 원소가 하나인 경우
    if (left == right) {
        return arr[left];
    }

    // 분할: 배열을 둘로 나눔
    int mid = (left + right) / 2;

    // 정복: 각 부분의 합을 재귀적으로 계산
    int left_sum = array_sum(arr, left, mid);
    int right_sum = array_sum(arr, mid + 1, right);

    // 결합: 두 부분의 합을 더함
    return left_sum + right_sum;
}

/* 실행 시간 측정 함수 */
double measure_time(void (*func)(), void* args) {
    clock_t start = clock();
    func(args);
    clock_t end = clock();
    return ((double)(end - start)) / CLOCKS_PER_SEC;
}

/* 배열 출력 */
void print_array(int arr[], int size) {
    printf("[");
    for (int i = 0; i < size; i++) {
        printf("%d", arr[i]);
        if (i < size - 1) printf(", ");
    }
    printf("]\n");
}

/* 메뉴 출력 */
void print_menu(void) {
    printf("\n=== Divide and Conquer Examples ===\n");
    printf("1. Find maximum value\n");
    printf("2. Binary search\n");
    printf("3. Calculate array sum\n");
    printf("4. Generate new random array\n");
    printf("0. Exit\n");
    printf("Choice: ");
}

int main(void) {
    int size;
    printf("Enter array size: ");
    scanf("%d", &size);

    int* arr = (int*)malloc(size * sizeof(int));
    if (!arr) {
        printf("Memory allocation failed\n");
        return 1;
    }

    // 초기 배열 생성
    for (int i = 0; i < size; i++) {
        arr[i] = rand() % 100;  // 0-99 범위의 난수
    }

    int choice;
    do {
        print_menu();
        scanf("%d", &choice);

        switch (choice) {
        case 1: {  // Find maximum
            printf("\nArray: ");
            print_array(arr, size);

            clock_t start = clock();
            int max = find_max(arr, 0, size - 1);
            clock_t end = clock();

            printf("Maximum value: %d\n", max);
            printf("Time taken: %f seconds\n",
                ((double)(end - start)) / CLOCKS_PER_SEC);
            break;
        }

        case 2: {  // Binary search
            // 이진 검색을 위해 배열 정렬
            for (int i = 0; i < size - 1; i++) {
                for (int j = 0; j < size - i - 1; j++) {
                    if (arr[j] > arr[j + 1]) {
                        int temp = arr[j];
                        arr[j] = arr[j + 1];
                        arr[j + 1] = temp;
                    }
                }
            }

            printf("\nSorted array: ");
            print_array(arr, size);

            int target;
            printf("Enter value to search: ");
            scanf("%d", &target);

            clock_t start = clock();
            int index = binary_search(arr, 0, size - 1, target);
            clock_t end = clock();

            if (index != -1) {
                printf("Value %d found at index %d\n", target, index);
            }
            else {
                printf("Value %d not found\n", target);
            }
            printf("Time taken: %f seconds\n",
                ((double)(end - start)) / CLOCKS_PER_SEC);
            break;
        }

        case 3: {  // Array sum
            printf("\nArray: ");
            print_array(arr, size);

            clock_t start = clock();
            int sum = array_sum(arr, 0, size - 1);
            clock_t end = clock();

            printf("Array sum: %d\n", sum);
            printf("Time taken: %f seconds\n",
                ((double)(end - start)) / CLOCKS_PER_SEC);
            break;
        }

        case 4: {  // Generate new array
            for (int i = 0; i < size; i++) {
                arr[i] = rand() % 100;
            }
            printf("New array generated: ");
            print_array(arr, size);
            break;
        }

        case 0:  // Exit
            printf("Exiting program\n");
            break;

        default:
            printf("Invalid choice\n");
        }
    } while (choice != 0);

    free(arr);
    return 0;
}

/*
==========================================
상세 설명 및 주요 개념
==========================================

1. 분할 정복의 기본 원리
-------------------
분할(Divide):
- 문제를 더 작은 하위 문제로 나눔
- 보통 크기가 비슷한 부분들로 분할
- 재귀적 해결이 가능해야 함

정복(Conquer):
- 하위 문제들을 재귀적으로 해결
- 기저 사례(base case) 필요
- 독립적 해결 가능해야 함

결합(Combine):
- 하위 문제의 해답을 결합
- 전체 문제의 해답 생성
- 효율적인 결합 방법 필요

2. 시간 복잡도 분석
---------------
마스터 정리 사용:
T(n) = aT(n/b) + f(n)
- a: 하위 문제 개수
- b: 크기 감소 비율
- f(n): 분할과 결합 비용

일반적인 경우:
- 이진 분할: O(n log n)
- 삼진 분할: O(n log₃ n)
- 선형 결합: O(n)

3. 장단점
-------
장점:
- 병렬 처리 가능
- 캐시 효율적
- 문제 크기 감소
- 직관적인 설계

단점:
- 재귀 호출 오버헤드
- 스택 공간 사용
- 코드 복잡도 증가
- 기저 사례 주의

4. 최적화 기법
-----------
- 기저 사례 크기 조정
- 반복적 구현 전환
- 캐시 지역성 활용
- 병렬 처리 적용

5. 응용 분야
----------
- 정렬 알고리즘
- 수치 계산
- 기하 알고리즘
- 변환 알고리즘

이 구현은 분할 정복의 기본 개념을
실제 예제를 통해 보여주며, 다음
단계인 병합 정렬의 기초가 됩니다.
*/