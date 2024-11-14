#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/*
체이닝 방식 해시 테이블:
- 충돌 발생시 연결 리스트로 처리
- 동적 크기 조정으로 성능 유지
- 다양한 해시 함수 지원
- 실제 응용에 가장 널리 사용
*/

#define INITIAL_SIZE 7  // 소수를 사용하여 더 좋은 분포
#define MAX_LOAD_FACTOR 0.75

typedef struct {
    char* key;
    int value;
} KeyValuePair;

typedef struct Node {
    KeyValuePair data;
    struct Node* next;
} Node;

typedef struct {
    Node** buckets;       // 버킷 배열
    size_t size;         // 현재 저장된 원소 수
    size_t capacity;     // 해시 테이블 크기
} HashTable;

/* 문자열 해시 함수 (djb2)
 * - 실제 프로젝트에서 많이 사용되는 해시 함수
 */
unsigned long hash_function(const char* str) {
    unsigned long hash = 5381;
    int c;

    while ((c = *str++)) {
        hash = ((hash << 5) + hash) + c; // hash * 33 + c
    }

    return hash;
}

/* 해시 테이블 생성 */
HashTable* hash_table_create(size_t initial_capacity) {
    HashTable* table = (HashTable*)malloc(sizeof(HashTable));
    if (!table) return NULL;

    table->buckets = (Node**)calloc(initial_capacity, sizeof(Node*));
    if (!table->buckets) {
        free(table);
        return NULL;
    }

    table->size = 0;
    table->capacity = initial_capacity;
    return table;
}

/* 노드 생성 */
Node* create_node(const char* key, int value) {
    Node* node = (Node*)malloc(sizeof(Node));
    if (!node) return NULL;

    node->data.key = strdup(key);
    if (!node->data.key) {
        free(node);
        return NULL;
    }

    node->data.value = value;
    node->next = NULL;
    return node;
}

/* 노드 삭제 */
void free_node(Node* node) {
    free(node->data.key);
    free(node);
}

/* 현재 로드 팩터 계산 */
double get_load_factor(const HashTable* table) {
    return (double)table->size / table->capacity;
}

/* 해시 테이블 재해싱
 * - 테이블 크기를 두 배로 늘리고 모든 원소 재삽입
 */
bool hash_table_resize(HashTable* table) {
    size_t new_capacity = table->capacity * 2;
    Node** new_buckets = (Node**)calloc(new_capacity, sizeof(Node*));
    if (!new_buckets) return false;

    // 모든 원소를 새 버킷으로 재배치
    for (size_t i = 0; i < table->capacity; i++) {
        Node* current = table->buckets[i];
        while (current) {
            Node* next = current->next;

            // 새 위치 계산
            unsigned long hash = hash_function(current->data.key);
            size_t new_index = hash % new_capacity;

            // 새 버킷의 앞에 삽입
            current->next = new_buckets[new_index];
            new_buckets[new_index] = current;

            current = next;
        }
    }

    // 이전 버킷 배열 해제
    free(table->buckets);

    // 새 버킷으로 교체
    table->buckets = new_buckets;
    table->capacity = new_capacity;

    return true;
}

/* 삽입 연산
 * - 시간복잡도: 평균 O(1), 최악 O(n)
 */
bool hash_table_insert(HashTable* table, const char* key, int value) {
    // 로드 팩터 검사
    if (get_load_factor(table) >= MAX_LOAD_FACTOR) {
        if (!hash_table_resize(table)) {
            return false;
        }
    }

    unsigned long hash = hash_function(key);
    size_t index = hash % table->capacity;

    // 키가 이미 존재하는지 검사
    Node* current = table->buckets[index];
    while (current) {
        if (strcmp(current->data.key, key) == 0) {
            current->data.value = value;  // 값 갱신
            return true;
        }
        current = current->next;
    }

    // 새 노드 생성
    Node* new_node = create_node(key, value);
    if (!new_node) return false;

    // 버킷의 앞에 삽입
    new_node->next = table->buckets[index];
    table->buckets[index] = new_node;
    table->size++;

    return true;
}

/* 검색 연산
 * - 시간복잡도: 평균 O(1), 최악 O(n)
 */
bool hash_table_get(const HashTable* table, const char* key, int* value) {
    unsigned long hash = hash_function(key);
    size_t index = hash % table->capacity;

    Node* current = table->buckets[index];
    while (current) {
        if (strcmp(current->data.key, key) == 0) {
            *value = current->data.value;
            return true;
        }
        current = current->next;
    }

    return false;
}

/* 삭제 연산
 * - 시간복잡도: 평균 O(1), 최악 O(n)
 */
bool hash_table_remove(HashTable* table, const char* key) {
    unsigned long hash = hash_function(key);
    size_t index = hash % table->capacity;

    Node* current = table->buckets[index];
    Node* prev = NULL;

    while (current) {
        if (strcmp(current->data.key, key) == 0) {
            if (prev) {
                prev->next = current->next;
            }
            else {
                table->buckets[index] = current->next;
            }

            free_node(current);
            table->size--;
            return true;
        }
        prev = current;
        current = current->next;
    }

    return false;
}

/* 해시 테이블 메모리 해제 */
void hash_table_destroy(HashTable* table) {
    if (!table) return;

    for (size_t i = 0; i < table->capacity; i++) {
        Node* current = table->buckets[i];
        while (current) {
            Node* next = current->next;
            free_node(current);
            current = next;
        }
    }

    free(table->buckets);
    free(table);
}

/* 해시 테이블 상태 출력 */
void hash_table_print(const HashTable* table) {
    printf("\nHash Table Status:\n");
    printf("Size: %zu\n", table->size);
    printf("Capacity: %zu\n", table->capacity);
    printf("Load factor: %.2f\n", get_load_factor(table));

    for (size_t i = 0; i < table->capacity; i++) {
        printf("\nBucket %zu: ", i);
        Node* current = table->buckets[i];

        if (!current) {
            printf("Empty");
        }
        else {
            while (current) {
                printf("[%s: %d] -> ", current->data.key, current->data.value);
                current = current->next;
            }
            printf("NULL");
        }
    }
    printf("\n");
}

/* 충돌 통계 출력 */
void print_collision_stats(const HashTable* table) {
    size_t empty_buckets = 0;
    size_t max_chain = 0;
    size_t total_chain = 0;
    size_t chains = 0;

    for (size_t i = 0; i < table->capacity; i++) {
        size_t chain_length = 0;
        Node* current = table->buckets[i];

        if (!current) {
            empty_buckets++;
        }
        else {
            while (current) {
                chain_length++;
                current = current->next;
            }
            if (chain_length > 1) chains++;
            if (chain_length > max_chain) max_chain = chain_length;
            total_chain += chain_length;
        }
    }

    printf("\nCollision Statistics:\n");
    printf("Empty buckets: %zu (%.1f%%)\n",
        empty_buckets, (float)empty_buckets / table->capacity * 100);
    printf("Buckets with collisions: %zu\n", chains);
    printf("Maximum chain length: %zu\n", max_chain);
    printf("Average chain length: %.2f\n",
        (float)total_chain / (table->capacity - empty_buckets));
}

/* 메뉴 출력 */
void print_menu(void) {
    printf("\n=== Hash Table Menu ===\n");
    printf("1. Insert key-value pair\n");
    printf("2. Get value by key\n");
    printf("3. Remove key-value pair\n");
    printf("4. Print hash table\n");
    printf("5. Print collision statistics\n");
    printf("0. Exit\n");
    printf("Choice: ");
}

int main(void) {
    HashTable* table = hash_table_create(INITIAL_SIZE);
    if (!table) {
        printf("Failed to create hash table\n");
        return 1;
    }

    int choice;
    char key[256];
    int value;

    do {
        print_menu();
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input\n");
            while (getchar() != '\n');
            continue;
        }
        while (getchar() != '\n');  // 버퍼 비우기

        switch (choice) {
        case 1:  // Insert
            printf("Enter key: ");
            if (!fgets(key, sizeof(key), stdin)) continue;
            key[strcspn(key, "\n")] = 0;  // 개행 제거

            printf("Enter value: ");
            if (scanf("%d", &value) != 1) {
                printf("Invalid value\n");
                continue;
            }

            if (hash_table_insert(table, key, value)) {
                printf("Successfully inserted (%s: %d)\n", key, value);
            }
            else {
                printf("Failed to insert\n");
            }
            break;

        case 2:  // Get
            printf("Enter key: ");
            if (!fgets(key, sizeof(key), stdin)) continue;
            key[strcspn(key, "\n")] = 0;

            if (hash_table_get(table, key, &value)) {
                printf("Value for key '%s': %d\n", key, value);
            }
            else {
                printf("Key not found\n");
            }
            break;

        case 3:  // Remove
            printf("Enter key: ");
            if (!fgets(key, sizeof(key), stdin)) continue;
            key[strcspn(key, "\n")] = 0;

            if (hash_table_remove(table, key)) {
                printf("Successfully removed key '%s'\n", key);
            }
            else {
                printf("Key not found\n");
            }
            break;

        case 4:  // Print
            hash_table_print(table);
            break;

        case 5:  // Statistics
            print_collision_stats(table);
            break;

        case 0:  // Exit
            printf("Exiting program\n");
            break;

        default:
            printf("Invalid choice\n");
        }
    } while (choice != 0);

    hash_table_destroy(table);
    return 0;
}

/*
==========================================
상세 설명 및 주요 개념
==========================================

1. 체이닝의 특징
------------
- 충돌 처리가 간단
- 메모리 오버헤드 발생
- 캐시 지역성 낮음
- 구현이 직관적

2. 시간 복잡도
-----------
평균 케이스:
- 삽입: O(1)
- 검색: O(1)
- 삭제: O(1)

최악 케이스:
- 모든 연산 O(n)
- 해시 충돌 시

3. 공간 복잡도
-----------
O(n)
- 추가 연결 리스트 공간
- 동적 크기 조정
- 로드 팩터 관리

4. 주요 구성 요소
-------------
해시 함수:
- 균등 분포
- 계산 효율성
- 충돌 최소화

충돌 처리:
- 연결 리스트 사용
- 동적 메모리 관리
- 효율적인 탐색

5. 최적화 기법
-----------
- 로드 팩터 관리
- 동적 크기 조정
- 효율적 해시 함수
- 메모리 관리

6. 장단점
-------
장점:
- 구현 용이
- 삭제 간단
- 성능 안정적
- 확장 용이

단점:
- 추가 메모리
- 캐시 성능
- 포인터 오버헤드
- 클러스터링

7. 응용 분야
----------
- 심볼 테이블
- 캐시 구현
- 데이터베이스 인덱싱
- 메모리 풀

8. 구현 특징
----------
- 동적 크기 조정
- 충돌 통계
- 안전한 메모리 관리
- 사용자 인터페이스

이 구현은 실제 프로젝트에서
사용할 수 있는 수준의 해시 테이블을
제공하며, 다음 단계인 개방 주소법
구현의 비교 기준이 됩니다.
*/