#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/**
 * 개방 주소법 해시 테이블 구현
 * - 충돌 발생시 다른 버킷을 탐색하는 방식 사용
 * - 세 가지 충돌 해결 방식 지원 (선형, 이차, 이중 해싱)
 */

// 기본 설정값
#define INITIAL_SIZE 7           // 초기 테이블 크기 (소수 사용)
#define MAX_LOAD_FACTOR 0.75     // 최대 적재율
#define DELETED_NODE (void*)(~0) // 삭제된 노드 표시값

// 탐사 방식 열거형
typedef enum {
    PROBE_LINEAR,      // 선형 조사법: 다음 버킷으로 순차적 이동
    PROBE_QUADRATIC,   // 이차 조사법: 제곱수만큼 이동
    PROBE_DOUBLE_HASH  // 이중 해싱: 두 번째 해시 함수 사용
} ProbeType;

// 키-값 쌍을 저장하는 구조체
typedef struct {
    char* key;   // 키 (NULL: 빈 버킷, DELETED_NODE: 삭제된 버킷)
    int value;   // 값
} Entry;

// 해시 테이블 구조체
typedef struct {
    Entry* entries;    // 엔트리 배열
    size_t capacity;   // 테이블 크기
    size_t size;       // 저장된 요소 수
    size_t tombstones; // 삭제 표시 수
    ProbeType type;    // 사용 중인 탐사 방식
} HashTable;

// 함수 선언부
HashTable* hash_table_create(size_t capacity, ProbeType type);
bool hash_table_insert(HashTable* table, const char* key, int value);
bool hash_table_get(const HashTable* table, const char* key, int* value);
bool hash_table_remove(HashTable* table, const char* key);
void hash_table_destroy(HashTable* table);
void hash_table_print(const HashTable* table);
const char* probe_type_to_string(ProbeType type);
static double get_load_factor(const HashTable* table);
static size_t get_probe_position(const HashTable* table, const char* key, size_t i);
static bool hash_table_resize(HashTable* table);

/**
 * 문자열 해시 함수 (djb2 알고리즘)
 * 충돌이 적고 빠른 계산이 가능한 문자열 해시 함수
 */
unsigned long hash_function(const char* str) {
    unsigned long hash = 5381;
    int c;
    while ((c = *str++)) {
        hash = ((hash << 5) + hash) + c;
    }
    return hash;
}

/**
 * 보조 해시 함수 (이중 해싱에서 사용)
 * 항상 홀수를 반환하여 모든 버킷 방문을 보장
 */
unsigned long hash_function2(const char* str) {
    unsigned long hash = 0;
    int c;
    while ((c = *str++)) {
        hash = hash * 31 + c;
    }
    return hash | 1;  // 마지막 비트를 1로 설정 (홀수 보장)
}

/**
 * 현재 해시 테이블의 적재율 계산
 * (실제 저장된 요소 수 + 삭제 표시 수) / 테이블 크기
 */
static double get_load_factor(const HashTable* table) {
    return (double)(table->size + table->tombstones) / table->capacity;
}

/**
 * 탐사 위치 계산
 * 각 탐사 방식에 따라 다음 위치를 계산
 *
 * @param table 해시 테이블
 * @param key 검색할 키
 * @param i 탐사 단계 (몇 번째 시도인지)
 * @return 다음 탐사할 버킷의 인덱스
 */
static size_t get_probe_position(const HashTable* table, const char* key, size_t i) {
    unsigned long hash = hash_function(key);
    unsigned long hash2;

    switch (table->type) {
        case PROBE_LINEAR:      // 선형 조사: hash + i
            return (hash + i) % table->capacity;

        case PROBE_QUADRATIC:   // 이차 조사: hash + i^2
            return (hash + i * i) % table->capacity;

        case PROBE_DOUBLE_HASH: // 이중 해싱: hash + i * hash2
            hash2 = hash_function2(key);
            return (hash + i * hash2) % table->capacity;

        default:
            return hash % table->capacity;
    }
}

/**
 * 해시 테이블 생성
 * 지정된 크기와 탐사 방식으로 새 해시 테이블 생성
 *
 * @param capacity 초기 테이블 크기
 * @param type 사용할 탐사 방식
 * @return 생성된 해시 테이블 포인터, 실패시 NULL
 */
HashTable* hash_table_create(size_t capacity, ProbeType type) {
    // 테이블 구조체 할당
    HashTable* table = (HashTable*)malloc(sizeof(HashTable));
    if (!table) return NULL;

    // 엔트리 배열 할당
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

/**
 * 해시 테이블 재해싱
 * 테이블 크기를 2배로 증가시키고 모든 요소를 재배치
 *
 * @param table 재해싱할 테이블
 * @return 성공 여부
 */
static bool hash_table_resize(HashTable* table) {
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

/**
 * 키-값 쌍 삽입
 * 적재율이 임계값을 초과하면 재해싱 수행
 *
 * @param table 해시 테이블
 * @param key 키 문자열
 * @param value 저장할 값
 * @return 성공 여부
 */
bool hash_table_insert(HashTable* table, const char* key, int value) {
    // 적재율 체크 및 재해싱
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

            // 이미 있는 키인지 확인
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

/**
 * 키로 값 검색
 * 키에 해당하는 값을 찾아서 반환
 *
 * @param table 해시 테이블
 * @param key 검색할 키
 * @param value 찾은 값을 저장할 포인터
 * @return 검색 성공 여부
 */
bool hash_table_get(const HashTable* table, const char* key, int* value) {
    size_t i = 0;
    size_t index;

    do {
        index = get_probe_position(table, key, i);

        // 빈 버킷을 만나면 검색 종료
        if (!table->entries[index].key) {
            return false;
        }

        // 키를 찾으면 값 반환
        if (table->entries[index].key != DELETED_NODE &&
            strcmp(table->entries[index].key, key) == 0) {
            *value = table->entries[index].value;
            return true;
        }

        i++;
    } while (i < table->capacity);

    return false;  // 찾지 못한 경우
}

/**
 * 키-값 쌍 삭제
 * 키를 찾아서 삭제 표시(Tombstone) 처리
 *
 * @param table 해시 테이블
 * @param key 삭제할 키
 * @return 삭제 성공 여부
 */
bool hash_table_remove(HashTable* table, const char* key) {
    size_t i = 0;
    size_t index;

    do {
        index = get_probe_position(table, key, i);

        // 빈 버킷을 만나면 검색 종료
        if (!table->entries[index].key) {
            return false;
        }

        // 키를 찾으면 삭제 처리
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

    return false;  // 찾지 못한 경우
}

/**
 * 해시 테이블 메모리 해제
 * 모든 동적 할당된 메모리를 해제
 *
 * @param table 해제할 테이블
 */
void hash_table_destroy(HashTable* table) {
    if (!table) return;

    // 각 엔트리의 키 문자열 해제
    for (size_t i = 0; i < table->capacity; i++) {
        if (table->entries[i].key && table->entries[i].key != DELETED_NODE) {
            free(table->entries[i].key);
        }
    }

    // 엔트리 배열과 테이블 구조체 해제
    free(table->entries);
    free(table);
}

/**
 * 해시 테이블 상태 출력
 * 테이블의 현재 상태와 모든 저장된 데이터를 출력
 *
 * @param table 출력할 테이블
 */
void hash_table_print(const HashTable* table) {
    printf("\n=== Hash Table Status ===\n");
    printf("Size: %zu\n", table->size);
    printf("Capacity: %zu\n", table->capacity);
    printf("Tombstones: %zu\n", table->tombstones);
    printf("Load factor: %.2f\n", get_load_factor(table));

    printf("\n=== Table Contents ===\n");
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

/**
 * 탐사 방식을 문자열로 변환
 *
 * @param type 탐사 방식
 * @return 탐사 방식 설명 문자열
 */
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

/**
 * 메뉴 출력
 */
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
    // 초기 해시 테이블 생성
    HashTable* table = hash_table_create(INITIAL_SIZE, PROBE_LINEAR);
    if (!table) {
        printf("Failed to create hash table\n");
        return 1;
    }

    printf("Initial probe type: %s\n", probe_type_to_string(table->type));

    int choice;
    char key[256];
    int value;

    // 메인 루프
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

    // 메모리 정리
    hash_table_destroy(table);
    return 0;
}