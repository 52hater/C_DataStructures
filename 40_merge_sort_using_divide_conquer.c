#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/*
병합 정렬(Merge Sort):
- 대표적인 분할 정복 정렬 알고리즘
- 안정 정렬(Stable Sort)
- 3단계: 분할, 정복, 병합
- 항상 O(n log n) 시간복잡도 보장

분할(Divide): 배열을 두 개의 부분 배열로 분할
정복(Conquer): 부분 배열들을 재귀적으로 정렬
병합(Merge): 정렬된 부분 배열들을 하나의 배열로 병합
*/

/* 두 정렬된 부분 배열을 병합하는 함수
 * - left ~ mid: 첫 번째 부분 배열
 * - mid+1 ~ right: 두 번째 부분 배열
 * - 시간복잡도: O(n), 공간복잡도: O(n)
 */
void merge(int arr[], int left, int mid, int right, bool print_steps) {
    int i, j, k;
    int n1 = mid - left + 1;  // 왼쪽 부분 배열의 크기
    int n2 = right - mid;     // 오른쪽 부분 배열의 크기

    // 임시 배열 동적 할당
    int* L = (int*)malloc(n1 * sizeof(int));
    int* R = (int*)malloc(n2 * sizeof(int));

    // 데이터를 임시 배열로 복사
    for (i = 0; i < n1; i++)
        L[i] = arr[left + i];
    for (j = 0; j < n2; j++)
        R[j] = arr[mid + 1 + j];

    if (print_steps) {
        printf("Merging subarrays:\n");
        printf("Left:  ");
        for (i = 0; i < n1; i++)
            printf("%d ", L[i]);
        printf("\nRight: ");
        for (j = 0; j < n2; j++)
            printf("%d ", R[j]);
        printf("\n");
    }

    // 두 부분 배열을 병합
    i = 0;  // 첫 번째 부분 배열의 인덱스
    j = 0;  // 두 번째 부분 배열의 인덱스
    k = left;  // 병합된 배열의 인덱스

    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            i++;
        }
        else {
            arr[k] = R[j];
            j++;
        }
        k++;

        if (print_steps) {
            printf("Current array: ");
            for (int x = left; x <= right; x++)
                printf("%d ", arr[x]);
            printf("\n");
        }
    }

    // 남은 원소들을 복사
    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }
    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }

    free(L);
    free(R);

    if (print_steps) {
        printf("Merged result: ");
        for (i = left; i <= right; i++)
            printf("%d ", arr[i]);
        printf("\n\n");
    }
}

/* 병합 정렬 구현
 * - 시간복잡도: O(n log n)
 * - 공간복잡도: O(n)
 */
void merge_sort(int arr[], int left, int right, bool print_steps) {
    if (left < right) {
        if (print_steps) {
            printf("Dividing array: ");
            for (int i = left; i <= right; i++)
                printf("%d ", arr[i]);
            printf("\n");
        }

        int mid = left + (right - left) / 2;  // 오버플로우 방지

        // 분할 및 정복
        merge_sort(arr, left, mid, print_steps);      // 왼쪽 부분 배열 정렬
        merge_sort(arr, mid + 1, right, print_steps); // 오른쪽 부분 배열 정렬

        // 병합
        merge(arr, left, mid, right, print_steps);
    }
}

/* 배열이 정렬되었는지 검증 */
bool is_sorted(const int arr[], int size) {
    for (int i = 0; i < size - 1; i++) {
        if (arr[i] > arr[i + 1]) {
            return false;
        }
    }
    return true;
}

/* 배열 출력 */
void print_array(const int arr[], int size) {
    printf("[");
    for (int i = 0; i < size; i++) {
        printf("%d", arr[i]);
        if (i < size - 1) printf(", ");
    }
    printf("]\n");
}

/* 성능 분석을 위한 함수 */
void analyze_performance(void) {
    printf("\nPerformance Analysis:\n");
    printf("-------------------\n");

    int sizes[] = { 100, 1000, 10000, 100000 };
    int num_sizes = sizeof(sizes) / sizeof(sizes[0]);

    for (int i = 0; i < num_sizes; i++) {
        int size = sizes[i];
        int* arr = (int*)malloc(size * sizeof(int));

        // 랜덤 배열 생성
        for (int j = 0; j < size; j++) {
            arr[j] = rand() % 1000;
        }

        // 시간 측정
        clock_t start = clock();
        merge_sort(arr, 0, size - 1, false);
        clock_t end = clock();

        double time = ((double)(end - start)) / CLOCKS_PER_SEC;
        printf("Size: %d, Time: %f seconds\n", size, time);

        free(arr);
    }
}

/* 메뉴 출력 */
void print_menu(void) {
    printf("\n=== Merge Sort Menu ===\n");
    printf("1. Sort new array\n");
    printf("2. Sort with steps\n");
    printf("3. Analyze performance\n");
    printf("4. Sort custom array\n");
    printf("0. Exit\n");
    printf("Choice: ");
}

int main(void) {
    srand(time(NULL));
    int size;
    int* arr = NULL;

    int choice;
    do {
        print_menu();
        scanf("%d", &choice);

        switch (choice) {
        case 1:  // Sort new array
        case 2: {  // Sort with steps
            printf("Enter array size: ");
            scanf("%d", &size);

            arr = (int*)realloc(arr, size * sizeof(int));
            if (!arr) {
                printf("Memory allocation failed\n");
                return 1;
            }

            for (int i = 0; i < size; i++) {
                arr[i] = rand() % 100;
            }

            printf("\nOriginal array: ");
            print_array(arr, size);

            merge_sort(arr, 0, size - 1, choice == 2);

            printf("Sorted array: ");
            print_array(arr, size);

            printf("Verification: %s\n",
                is_sorted(arr, size) ? "PASSED" : "FAILED");
            break;
        }

        case 3:  // Analyze performance
            analyze_performance();
            break;

        case 4: {  // Sort custom array
            printf("Enter array size: ");
            scanf("%d", &size);

            arr = (int*)realloc(arr, size * sizeof(int));
            if (!arr) {
                printf("Memory allocation failed\n");
                return 1;
            }

            printf("Enter %d integers: ", size);
            for (int i = 0; i < size; i++) {
                scanf("%d", &arr[i]);
            }

            printf("\nOriginal array: ");
            print_array(arr, size);

            merge_sort(arr, 0, size - 1, false);

            printf("Sorted array: ");
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

1. 병합 정렬의 원리
---------------
분할:
- 배열을 두 개의 부분 배열로 분할
- 중간 지점을 기준으로 분할
- 크기가 1이 될 때까지 반복

정복:
- 부분 배열들을 재귀적으로 정렬
- 기저 사례: 크기가 1인 배열
- 독립적으로 처리 가능

병합:
- 정렬된 두 배열을 하나로 병합
- 추가 공간 필요
- 안정성 보장

2. 시간 복잡도
-----------
분할: O(1)
정복: 2T(n/2)
병합: O(n)

전체: O(n log n)
- 모든 경우 동일
- 입력에 독립적
- 재귀 트리의 각 레벨: O(n)
- 트리의 높이: O(log n)

3. 공간 복잡도
-----------
O(n)
- 임시 배열 필요
- 재귀 스택: O(log n)
- 제자리 정렬 아님

4. 장단점
-------
장점:
- 안정 정렬
- 성능 예측 가능
- 연결 리스트에 적합
- 대용량 데이터에 효율적

단점:
- 추가 메모리 필요
- 작은 배열에 오버헤드
- 캐시 지역성 낮음
- 구현 복잡

5. 최적화 기법
-----------
- 작은 배열은 삽입 정렬
- 제자리 병합
- 캐시 최적화
- 병렬화

6. 응용 분야
----------
- 외부 정렬
- 안정성 필요시
- 연결 리스트 정렬
- 병렬 정렬

이 구현은 병합 정렬의 모든 단계를
명확히 보여주며, 실제 성능 분석도
포함하고 있습니다. 다음 단계인
거듭제곱 계산의 기초가 됩니다.
*/