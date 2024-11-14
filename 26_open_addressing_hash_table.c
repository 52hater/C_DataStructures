#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/*
개방 주소법 해시 테이블:
- 충돌 시 다른 버킷을 탐색
- 선형 조사법, 이차 조사법, 이중 해싱 구현
- 삭제 처리를 위한 표시(Tombstone) 사용
- 더 나은 캐시 지역성
*/

#define INITIAL_SIZE 7  // 소수 사용
#define MAX_LOAD_FACTOR 0.75
#define DELETED_NODE (void*)(~0)  // Tombstone 값

typedef enum {
    PROBE_LINEAR,      // 선형 조사법
    PROBE_QUADRATIC,   // 이차 조사법
    PROBE_DOUBLE_HASH  // 이중 해싱
} ProbeType;

typedef struct {
    char* key;    // 키 (NULL: 빈 버킷, DELETED_NODE: 삭제된 버킷)
    int value;    // 값
} Entry;

typedef struct {
    Entry* entries;     // 엔트리 배열
    size_t capacity;   // 테이블 크기
    size_t size;       // 저장된 요소 수
    size_t tombstones; // 삭제 표시 수
    ProbeType type;    // 조사 방식
} HashTable;

/* 문자열 해시 함수 (djb2) */
unsigned long hash_function(const char* str) {
    unsigned long hash = 5381;
    int c;
    while ((c = *str++)) {
        hash = ((hash << 5) + hash) + c;
    }
    return hash;
}

/* 보조 해시 함수 (이중 해싱용) */
unsigned long hash_function2(const char* str) {
    unsigned long hash = 0;
    int c;
    while ((c = *str++)) {
        hash = hash * 31 + c;
    }
    return hash | 1;  // 항상 홀수 반환
}

/* 해시 테이블 생성 */
HashTable* hash_table_create(size_t capacity, ProbeType type) {
    HashTable* table = (HashTable*)malloc(sizeof(HashTable));
    if (!table) return NULL;

    table->entries = (Entry*)calloc(capacity, sizeof(Entry));
    if (!table->entries) {
        free(table);
        return NULL;
    }

    table->capacity = capacity;
    table->size = 0;
    table->tombstones = 0;
    table->type = type;
    return table;
}

/* 현재 로드 팩터 계산 */
double get_load_factor(const HashTable* table) {
    return (double)(table->size + table->tombstones) / table->capacity;
}

/* 탐사 위치 계산 */
size_t get_probe_position(const HashTable* table, const char* key, size_t i) {
    unsigned long hash = hash_function(key);

    switch (table->type) {
    case PROBE_LINEAR:  // 선형 조사법
        return (hash + i) % table->capacity;

    case PROBE_QUADRATIC:  // 이차 조사법
        return (hash + i * i) % table->capacity;

    case PROBE_DOUBLE_HASH:  // 이중 해싱
        unsigned long hash2 = hash_function2(key);
        return (hash + i * hash2) % table->capacity;

    default:
        return hash % table->capacity;
    }
}

/* 해시 테이블 재해싱 */
bool hash_table_resize(HashTable* table) {
    size_t old_capacity = table->capacity;
    Entry* old_entries = table->entries;

    // 새로운 크기의 테이블 생성
    size_t new_capacity = old_capacity * 2;
    table->entries = (Entry*)calloc(new_capacity, sizeof(Entry));
    if (!table->entries) {
        table->entries = old_entries;
        return false;
    }

    table->capacity = new_capacity;
    table->size = 0;
    table->tombstones = 0;

    // 이전 엔트리들을 새 테이블로 복사
    for (size_t i = 0; i < old_capacity; i++) {
        if (old_entries[i].key && old_entries[i].key != DELETED_NODE) {
            hash_table_insert(table, old_entries[i].key, old_entries[i].value);
            free(old_entries[i].key);
        }
    }

    free(old_entries);
    return true;
}

/* 삽입 연산
 * - 시간복잡도: 평균 O(1), 최악 O(n)
 */
bool hash_table_insert(HashTable* table, const char* key, int value) {
    if (get_load_factor(table) >= MAX_LOAD_FACTOR) {
        if (!hash_table_resize(table)) {
            return false;
        }
    }

    size_t i = 0;
    size_t index;

    do {
        index = get_probe_position(table, key, i);

        // 빈 버킷이나 삭제된 버킷을 찾음
        if (!table->entries[index].key ||
            table->entries[index].key == DELETED_NODE) {
            // 이미 있던 키인지 확인
            size_t j = 0;
            size_t check_index;
            do {
                check_index = get_probe_position(table, key, j);
                if (table->entries[check_index].key &&
                    table->entries[check_index].key != DELETED_NODE &&
                    strcmp(table->entries[check_index].key, key) == 0) {
                    table->entries[check_index].value = value;
                    return true;
                }
                j++;
            } while (table->entries[check_index].key &&
                j < table->capacity);

            // 새로운 키 삽입
            if (table->entries[index].key == DELETED_NODE) {
                table->tombstones--;
            }

            table->entries[index].key = strdup(key);
            if (!table->entries[index].key) {
                return false;
            }

            table->entries[index].value = value;
            table->size++;
            return true;
        }

        // 이미 있는 키면 값 갱신
        if (strcmp(table->entries[index].key, key) == 0) {
            table->entries[index].value = value;
            return true;
        }

        i++;
    } while (i < table->capacity);

    return false;  // 테이블이 가득 찬 경우
}

/* 검색 연산
 * - 시간복잡도: 평균 O(1), 최악 O(n)
 */
bool hash_table_get(const HashTable* table, const char* key, int* value) {
    size_t i = 0;
    size_t index;

    do {
        index = get_probe_position(table, key, i);

        if (!table->entries[index].key) {
            return false;  // 빈 버킷을 만나면 종료
        }

        if (table->entries[index].key != DELETED_NODE &&
            strcmp(table->entries[index].key, key) == 0) {
            *value = table->entries[index].value;
            return true;
        }

        i++;
    } while (i < table->capacity);

    return false;
}

/* 삭제 연산
 * - 시간복잡도: 평균 O(1), 최악 O(n)
 */
bool hash_table_remove(HashTable* table, const char* key) {
    size_t i = 0;
    size_t index;

    do {
        index = get_probe_position(table, key, i);

        if (!table->entries[index].key) {
            return false;  // 빈 버킷을 만나면 종료
        }

        if (table->entries[index].key != DELETED_NODE &&
            strcmp(table->entries[index].key, key) == 0) {
            free(table->entries[index].key);
            table->entries[index].key = DELETED_NODE;
            table->size--;
            table->tombstones++;
            return true;
        }

        i++;
    } while (i < table->capacity);

    return false;
}

/* 해시 테이블 메모리 해제 */
void hash_table_destroy(HashTable* table) {
    if (!table) return;

    for (size_t i = 0; i < table->capacity; i++) {
        if (table->entries[i].key && table->entries[i].key != DELETED_NODE) {
            free(table->entries[i].key);
        }
    }

    free(table->entries);
    free(table);
}

/* 해시 테이블 상태 출력 */
void hash_table_print(const HashTable* table) {
    printf("\nHash Table Status:\n");
    printf("Size: %zu\n", table->size);
    printf("Capacity: %zu\n", table->capacity);
    printf("Tombstones: %zu\n", table->tombstones);
    printf("Load factor: %.2f\n", get_load_factor(table));

    printf("\nTable contents:\n");
    for (size_t i = 0; i < table->capacity; i++) {
        printf("[%zu] ", i);
        if (!table->entries[i].key) {
            printf("Empty\n");
        }
        else if (table->entries[i].key == DELETED_NODE) {
            printf("Deleted\n");
        }
        else {
            printf("%s: %d\n", table->entries[i].key, table->entries[i].value);
        }
    }
}

/* 탐사 방식을 문자열로 변환 */
const char* probe_type_to_string(ProbeType type) {
    switch (type) {
    case PROBE_LINEAR:
        return "Linear Probing";
    case PROBE_QUADRATIC:
        return "Quadratic Probing";
    case PROBE_DOUBLE_HASH:
        return "Double Hashing";
    default:
        return "Unknown";
    }
}

/* 메뉴 출력 */
void print_menu(void) {
    printf("\n=== Hash Table Menu ===\n");
    printf("1. Insert key-value pair\n");
    printf("2. Get value by key\n");
    printf("3. Remove key-value pair\n");
    printf("4. Print hash table\n");
    printf("5. Change probe type\n");
    printf("0. Exit\n");
    printf("Choice: ");
}

int main(void) {
    HashTable* table = hash_table_create(INITIAL_SIZE, PROBE_LINEAR);
    if (!table) {
        printf("Failed to create hash table\n");
        return 1;
    }

    printf("Initial probe type: %s\n", probe_type_to_string(table->type));

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
        while (getchar() != '\n');

        switch (choice) {
        case 1:  // Insert
            printf("Enter key: ");
            if (!fgets(key, sizeof(key), stdin)) continue;
            key[strcspn(key, "\n")] = 0;

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

        case 5:  // Change probe type
            printf("Select probe type:\n");
            printf("1. Linear Probing\n");
            printf("2. Quadratic Probing\n");
            printf("3. Double Hashing\n");
            printf("Choice: ");

            int probe_choice;
            if (scanf("%d", &probe_choice) == 1 &&
                probe_choice >= 1 && probe_choice <= 3) {
                table->type = (ProbeType)(probe_choice - 1);
                printf("Changed to %s\n",
                    probe_type_to_string(table->type));
            }
            else {
                printf("Invalid choice\n");
            }
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

1. 개방 주소법의 특징
-----------------
- 모든 원소가 주 테이블에 저장
- 더 나은 캐시 지역성
- 포인터 오버헤드 없음
- 클러스터링 현상 발생

2. 탐사 방식
----------
선형 조사법:
- 가장 단순한 방식
- 기본 클러스터링 발생
- 캐시 효율성 우수

이차 조사법:
- 2차 클러스터링 발생
- 더 나은 분산
- 전체 탐색 보장 어려움

이중 해싱:
- 가장 좋은 분산
- 추가 해시 함수 필요
- 클러스터링 최소

3. 시간 복잡도
-----------
평균 케이스:
- 삽입: O(1)
- 검색: O(1)
- 삭제: O(1)

최악 케이스:
- 모든 연산 O(n)
- 클러스터링 발생 시

4. 공간 복잡도
-----------
O(n)
- 추가 포인터 없음
- Tombstone 공간
- 낮은 로드 팩터 필요
- 메모리 효율적

5. Tombstone 처리
--------------
- 삭제 시 특수 마커 사용
- 검색 시 건너뛰기 가능
- 삽입 시 재사용 가능
- 주기적 재해싱 필요

6. 장단점
-------
장점:
- 캐시 지역성
- 포인터 없음
- 메모리 효율
- 구현 단순

단점:
- 클러스터링
- 로드 팩터 제한
- 재해싱 필요
- 삭제 처리 복잡

7. 최적화 기법
-----------
- 효율적 해시 함수
- 적절한 크기 관리
- 재해싱 전략
- 탐사 방식 선택

8. 응용 분야
----------
- 캐시 구현
- 메모리 관리
- 작은 데이터셋
- 실시간 처리

이 구현은 세 가지 탐사 방식을
모두 지원하며, 실제 환경에서
사용할 수 있는 수준의 개방 주소법
해시 테이블을 제공합니다.
*/