#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
������ �ڵ��� Ž���� Ư��
====================
1. ���� ���� �󵵸� ���� �� ��� ����
2. ���õ� ��带 �����Ͽ� ���ο� ��� ����
3. ���� ������ �������� ����
*/

// ������ Ʈ�� ���
typedef struct Node {
    char data;             // ����
    unsigned frequency;    // �󵵼�
    struct Node* left, * right;
} Node;

// �ּ� �� ���
typedef struct {
    Node** array;
    int size;
    int capacity;
} MinHeap;

// ��� ����
Node* create_node(char data, unsigned frequency) {
    Node* node = (Node*)malloc(sizeof(Node));
    node->data = data;
    node->frequency = frequency;
    node->left = node->right = NULL;
    return node;
}

// �ּ� �� ����
MinHeap* create_min_heap(unsigned capacity) {
    MinHeap* heap = (MinHeap*)malloc(sizeof(MinHeap));
    heap->size = 0;
    heap->capacity = capacity;
    heap->array = (Node**)malloc(capacity * sizeof(Node*));
    return heap;
}

// �� ����
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

// ��� ����/����
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

// �󵵼� ���
void calculate_frequency(const char* text, int* freq) {
    for (int i = 0; text[i]; i++) {
        freq[(unsigned char)text[i]]++;
    }

    printf("\n���ں� �󵵼�:\n");
    for (int i = 0; i < 256; i++) {
        if (freq[i])
            printf("'%c': %d\n", i, freq[i]);
    }
}

// ������ Ʈ�� ����
Node* build_huffman_tree(char* data, int* freq, int size) {
    printf("\n=== ������ Ʈ�� ���� ���� ===\n");

    MinHeap* heap = create_min_heap(size);

    // �ʱ� ��� ����
    for (int i = 0; i < size; i++) {
        if (freq[i] > 0) {
            heap->array[heap->size] = create_node(data[i], freq[i]);
            heap->size++;
            printf("��� ����: '%c' (�󵵼�: %d)\n", data[i], freq[i]);
        }
    }

    // �� ����
    for (int i = (heap->size - 1) / 2; i >= 0; i--)
        min_heapify(heap, i);

    // Ʈ�� ����
    while (heap->size > 1) {
        Node* left = extract_min(heap);
        Node* right = extract_min(heap);

        Node* parent = create_node('$', left->frequency + right->frequency);
        parent->left = left;
        parent->right = right;

        printf("\n��� ����: %d + %d = %d\n",
            left->frequency, right->frequency, parent->frequency);

        insert_min_heap(heap, parent);
    }

    Node* root = extract_min(heap);
    free(heap->array);
    free(heap);
    return root;
}

// ������ �ڵ� ����
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

// Ʈ�� �޸� ����
void free_tree(Node* root) {
    if (root) {
        free_tree(root->left);
        free_tree(root->right);
        free(root);
    }
}

// ����� ���
void calculate_compression_ratio(const char* text, Node* root) {
    int original_size = strlen(text) * 8;  // 8 bits per character
    int compressed_size = 0;
    int* code = (int*)malloc(100 * sizeof(int));

    // �� ������ ������ �ڵ� ���� ���
    void count_bits(Node * node, int depth) {
        if (!node) return;
        if (!node->left && !node->right) {
            // ���ں� ���� �� * �ڵ� ����
            compressed_size += strlen(text) * depth;
        }
        count_bits(node->left, depth + 1);
        count_bits(node->right, depth + 1);
    }

    count_bits(root, 0);

    printf("\n=== ���� �м� ===\n");
    printf("���� ũ��: %d bits\n", original_size);
    printf("���� �� ũ��: %d bits\n", compressed_size);
    printf("�����: %.2f%%\n",
        (1 - (float)compressed_size / original_size) * 100);

    free(code);
}

int main(void) {
    char text[1000];
    int freq[256] = { 0 };

    printf("�ؽ�Ʈ �Է�: ");
    fgets(text, sizeof(text), stdin);
    text[strcspn(text, "\n")] = 0;

    calculate_frequency(text, freq);

    // ������ Ʈ�� ����
    Node* root = build_huffman_tree(text, freq, 256);

    // ������ �ڵ� ���
    printf("\n=== ������ ������ �ڵ� ===\n");
    int code[100];
    print_codes(root, code, 0);

    // ����� ���
    calculate_compression_ratio(text, root);

    // �޸� ����
    free_tree(root);

    return 0;
}

/*
������ �ڵ��� �м�
==============

1. Ž���� ������ ���缺
------------------
- ���� ���� ���� ���� ����
- ���� ������ ����
- �ڵ� ���ξ� ���� ����

2. �ð� ���⵵
-----------
- �󵵼� ���: O(n)
- �� ����: O(n)
- Ʈ�� ����: O(n log n)
- ��ü: O(n log n)

3. ���� ���⵵
-----------
- �󵵼� ���̺�: O(1)
- ������ Ʈ��: O(n)
- �ڵ� ���̺�: O(n)

4. ���� ȿ����
-----------
- ���� �� ����: ª�� �ڵ�
- ���� �� ����: �� �ڵ�
- ���� ���ξ� �ڵ� ����

�� ������ ������ �ڵ���
Ž���� ���� ��İ� �� ȿ������
��Ȯ�ϰ� �����ݴϴ�.
*/