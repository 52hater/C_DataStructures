#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
허프만 코딩의 탐욕적 특성
====================
1. 가장 낮은 빈도를 가진 두 노드 선택
2. 선택된 노드를 결합하여 새로운 노드 생성
3. 이전 선택을 번복하지 않음
*/

// 허프만 트리 노드
typedef struct Node {
    char data;             // 문자
    unsigned frequency;    // 빈도수
    struct Node* left, * right;
} Node;

// 최소 힙 노드
typedef struct {
    Node** array;
    int size;
    int capacity;
} MinHeap;

// 노드 생성
Node* create_node(char data, unsigned frequency) {
    Node* node = (Node*)malloc(sizeof(Node));
    node->data = data;
    node->frequency = frequency;
    node->left = node->right = NULL;
    return node;
}

// 최소 힙 생성
MinHeap* create_min_heap(unsigned capacity) {
    MinHeap* heap = (MinHeap*)malloc(sizeof(MinHeap));
    heap->size = 0;
    heap->capacity = capacity;
    heap->array = (Node**)malloc(capacity * sizeof(Node*));
    return heap;
}

// 힙 연산
void swap_nodes(Node** a, Node** b) {
    Node* temp = *a;
    *a = *b;
    *b = temp;
}

void min_heapify(MinHeap* heap, int idx) {
    int smallest = idx;
    int left = 2 * idx + 1;
    int right = 2 * idx + 2;

    if (left < heap->size &&
        heap->array[left]->frequency < heap->array[smallest]->frequency)
        smallest = left;

    if (right < heap->size &&
        heap->array[right]->frequency < heap->array[smallest]->frequency)
        smallest = right;

    if (smallest != idx) {
        swap_nodes(&heap->array[idx], &heap->array[smallest]);
        min_heapify(heap, smallest);
    }
}

// 노드 삽입/추출
void insert_min_heap(MinHeap* heap, Node* node) {
    heap->size++;
    int i = heap->size - 1;

    while (i && node->frequency < heap->array[(i - 1) / 2]->frequency) {
        heap->array[i] = heap->array[(i - 1) / 2];
        i = (i - 1) / 2;
    }

    heap->array[i] = node;
}

Node* extract_min(MinHeap* heap) {
    if (heap->size <= 0) return NULL;

    Node* temp = heap->array[0];
    heap->array[0] = heap->array[heap->size - 1];
    heap->size--;

    min_heapify(heap, 0);
    return temp;
}

// 빈도수 계산
void calculate_frequency(const char* text, int* freq) {
    for (int i = 0; text[i]; i++) {
        freq[(unsigned char)text[i]]++;
    }

    printf("\n문자별 빈도수:\n");
    for (int i = 0; i < 256; i++) {
        if (freq[i])
            printf("'%c': %d\n", i, freq[i]);
    }
}

// 허프만 트리 구축
Node* build_huffman_tree(char* data, int* freq, int size) {
    printf("\n=== 허프만 트리 구축 과정 ===\n");

    MinHeap* heap = create_min_heap(size);

    // 초기 노드 생성
    for (int i = 0; i < size; i++) {
        if (freq[i] > 0) {
            heap->array[heap->size] = create_node(data[i], freq[i]);
            heap->size++;
            printf("노드 생성: '%c' (빈도수: %d)\n", data[i], freq[i]);
        }
    }

    // 힙 구성
    for (int i = (heap->size - 1) / 2; i >= 0; i--)
        min_heapify(heap, i);

    // 트리 구축
    while (heap->size > 1) {
        Node* left = extract_min(heap);
        Node* right = extract_min(heap);

        Node* parent = create_node('$', left->frequency + right->frequency);
        parent->left = left;
        parent->right = right;

        printf("\n노드 결합: %d + %d = %d\n",
            left->frequency, right->frequency, parent->frequency);

        insert_min_heap(heap, parent);
    }

    Node* root = extract_min(heap);
    free(heap->array);
    free(heap);
    return root;
}

// 허프만 코드 생성
void print_codes(Node* root, int* code, int top) {
    if (root->left) {
        code[top] = 0;
        print_codes(root->left, code, top + 1);
    }

    if (root->right) {
        code[top] = 1;
        print_codes(root->right, code, top + 1);
    }

    if (!root->left && !root->right) {
        printf("'%c': ", root->data);
        for (int i = 0; i < top; i++)
            printf("%d", code[i]);
        printf("\n");
    }
}

// 트리 메모리 해제
void free_tree(Node* root) {
    if (root) {
        free_tree(root->left);
        free_tree(root->right);
        free(root);
    }
}

// 압축률 계산
void calculate_compression_ratio(const char* text, Node* root) {
    int original_size = strlen(text) * 8;  // 8 bits per character
    int compressed_size = 0;
    int* code = (int*)malloc(100 * sizeof(int));

    // 각 문자의 허프만 코드 길이 계산
    void count_bits(Node * node, int depth) {
        if (!node) return;
        if (!node->left && !node->right) {
            // 문자별 출현 빈도 * 코드 길이
            compressed_size += strlen(text) * depth;
        }
        count_bits(node->left, depth + 1);
        count_bits(node->right, depth + 1);
    }

    count_bits(root, 0);

    printf("\n=== 압축 분석 ===\n");
    printf("원본 크기: %d bits\n", original_size);
    printf("압축 후 크기: %d bits\n", compressed_size);
    printf("압축률: %.2f%%\n",
        (1 - (float)compressed_size / original_size) * 100);

    free(code);
}

int main(void) {
    char text[1000];
    int freq[256] = { 0 };

    printf("텍스트 입력: ");
    fgets(text, sizeof(text), stdin);
    text[strcspn(text, "\n")] = 0;

    calculate_frequency(text, freq);

    // 허프만 트리 구축
    Node* root = build_huffman_tree(text, freq, 256);

    // 허프만 코드 출력
    printf("\n=== 생성된 허프만 코드 ===\n");
    int code[100];
    print_codes(root, code, 0);

    // 압축률 계산
    calculate_compression_ratio(text, root);

    // 메모리 해제
    free_tree(root);

    return 0;
}

/*
허프만 코딩의 분석
==============

1. 탐욕적 선택의 정당성
------------------
- 가장 낮은 빈도의 문자 병합
- 전역 최적해 보장
- 코드 접두어 성질 유지

2. 시간 복잡도
-----------
- 빈도수 계산: O(n)
- 힙 구성: O(n)
- 트리 구축: O(n log n)
- 전체: O(n log n)

3. 공간 복잡도
-----------
- 빈도수 테이블: O(1)
- 허프만 트리: O(n)
- 코드 테이블: O(n)

4. 압축 효율성
-----------
- 높은 빈도 문자: 짧은 코드
- 낮은 빈도 문자: 긴 코드
- 최적 접두어 코드 보장

이 구현은 허프만 코딩의
탐욕적 접근 방식과 그 효율성을
명확하게 보여줍니다.
*/