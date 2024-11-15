#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/*
�˰��� �з�: Ʈ�� �ڷᱸ��
���� �з�: ���ڿ� Ưȭ Ʈ��
���̵�: �߻�
���� �ڷᱸ��: �ؽ� ���̺�, ���� Ž�� Ʈ��

Ʈ���� Ʈ�� (Trie Tree / Prefix Tree):
- ���ڿ� ������ �����ϴ� ���ĵ� Ʈ��
- �� ���� ���� �ϳ��� ǥ��
- ���� ���ξ �����Ͽ� ���� ����
- ���ڿ� �˻�, �ڵ� �ϼ��� ȿ����
*/

#define ALPHABET_SIZE 26

typedef struct TrieNode {
    struct TrieNode* children[ALPHABET_SIZE];
    bool is_end_of_word;
    int word_count;       // �� ���ξ�� �����ϴ� �ܾ� ��
} TrieNode;

typedef struct {
    TrieNode* root;
    int total_words;
} TrieTree;

// �� ��� ����
TrieNode* create_node(void) {
    TrieNode* node = (TrieNode*)malloc(sizeof(TrieNode));
    node->is_end_of_word = false;
    node->word_count = 0;

    for (int i = 0; i < ALPHABET_SIZE; i++) {
        node->children[i] = NULL;
    }

    return node;
}

// Ʈ���� Ʈ�� ����
TrieTree* create_trie(void) {
    TrieTree* trie = (TrieTree*)malloc(sizeof(TrieTree));
    trie->root = create_node();
    trie->total_words = 0;
    return trie;
}

// �ܾ� ����
void insert(TrieTree* trie, const char* word) {
    TrieNode* current = trie->root;

    printf("\n�ܾ� '%s' ���� ����:\n", word);

    for (int i = 0; word[i]; i++) {
        int index = word[i] - 'a';
        if (index < 0 || index >= ALPHABET_SIZE) {
            printf("���: ���ĺ� �ҹ��ڸ� ����\n");
            return;
        }

        if (current->children[index] == NULL) {
            current->children[index] = create_node();
            printf("�� ��� ����: '%c'\n", word[i]);
        }
        current = current->children[index];
        current->word_count++;
    }

    if (!current->is_end_of_word) {
        current->is_end_of_word = true;
        trie->total_words++;
        printf("�ܾ� �� ǥ�� �߰�\n");
    }
}

// �ܾ� �˻�
bool search(TrieTree* trie, const char* word) {
    TrieNode* current = trie->root;

    printf("\n�ܾ� '%s' �˻� ����:\n", word);

    for (int i = 0; word[i]; i++) {
        int index = word[i] - 'a';
        if (!current->children[index]) {
            printf("���� '%c'���� �˻� ����\n", word[i]);
            return false;
        }
        current = current->children[index];
        printf("���� '%c' ã��\n", word[i]);
    }

    bool found = current->is_end_of_word;
    printf("�˻� ���: %s\n", found ? "ã��" : "����");
    return found;
}

// ���ξ�� �����ϴ� �ܾ� �� ��ȯ
int count_prefix(TrieTree* trie, const char* prefix) {
    TrieNode* current = trie->root;

    for (int i = 0; prefix[i]; i++) {
        int index = prefix[i] - 'a';
        if (!current->children[index]) {
            return 0;
        }
        current = current->children[index];
    }

    return current->word_count;
}

// �ڵ� �ϼ� ������ ���� ��� �Լ�
void find_words_with_prefix(TrieNode* node, char* prefix, int level) {
    if (node->is_end_of_word) {
        prefix[level] = '\0';
        printf("%s\n", prefix);
    }

    for (int i = 0; i < ALPHABET_SIZE; i++) {
        if (node->children[i]) {
            prefix[level] = i + 'a';
            find_words_with_prefix(node->children[i], prefix, level + 1);
        }
    }
}

// �ڵ� �ϼ� ���
void autocomplete(TrieTree* trie, const char* prefix) {
    TrieNode* current = trie->root;
    char buffer[100];
    strcpy(buffer, prefix);

    printf("\n'%s'�� �����ϴ� �ܾ��:\n", prefix);

    // ���ξ���� �̵�
    for (int i = 0; prefix[i]; i++) {
        int index = prefix[i] - 'a';
        if (!current->children[index]) {
            printf("�ش� ���ξ�� �����ϴ� �ܾ �����ϴ�.\n");
            return;
        }
        current = current->children[index];
    }

    // ������ ��� �ܾ� ���
    find_words_with_prefix(current, buffer, strlen(prefix));
}

// Ʈ���� ��� ���� (���)
void free_node(TrieNode* node) {
    if (node == NULL) return;

    for (int i = 0; i < ALPHABET_SIZE; i++) {
        free_node(node->children[i]);
    }

    free(node);
}

// Ʈ���� Ʈ�� ����
void free_trie(TrieTree* trie) {
    free_node(trie->root);
    free(trie);
}

// ��� ���� ���
void print_stats(TrieTree* trie) {
    printf("\n=== Ʈ���� Ʈ�� ��� ===\n");
    printf("�� �ܾ� ��: %d\n", trie->total_words);
}

int main(void) {
    TrieTree* trie = create_trie();
    char word[100];
    int choice;

    printf("=== Ʈ���� Ʈ�� �׽�Ʈ ===\n");

    while (1) {
        printf("\n1. �ܾ� ����\n");
        printf("2. �ܾ� �˻�\n");
        printf("3. �ڵ� �ϼ�\n");
        printf("4. ���ξ� ���\n");
        printf("5. Ʈ�� ���\n");
        printf("0. ����\n");
        printf("����: ");

        scanf("%d", &choice);
        getchar(); // ���� ����

        switch (choice) {
        case 1:
            printf("������ �ܾ� (�ҹ���): ");
            scanf("%s", word);
            insert(trie, word);
            break;

        case 2:
            printf("�˻��� �ܾ�: ");
            scanf("%s", word);
            search(trie, word);
            break;

        case 3:
            printf("���ξ� �Է�: ");
            scanf("%s", word);
            autocomplete(trie, word);
            break;

        case 4:
            printf("���ξ� �Է�: ");
            scanf("%s", word);
            printf("'%s'�� �����ϴ� �ܾ� ��: %d\n",
                word, count_prefix(trie, word));
            break;

        case 5:
            print_stats(trie);
            break;

        case 0:
            free_trie(trie);
            return 0;

        default:
            printf("�߸��� ����\n");
        }
    }

    return 0;
}

/*
Ʈ���� Ʈ�� �м�
============

1. �ð� ���⵵
-----------
- ����: O(m), m�� ���ڿ� ����
- �˻�: O(m)
- ���ξ� �˻�: O(p + k)
  p: ���ξ� ����
  k: ��� �ܾ� ��

2. ���� ���⵵
-----------
- �־�: O(ALPHABET_SIZE * m * n)
  m: ��� ���ڿ� ����
  n: ���ڿ� ����
- �����δ� ���� ���ξ�� ���� �� ����

3. �����
-------
����:
- ���� �˻�/����
- ���ξ� ��� ���� ȿ����
- �ڵ� �ϼ��� �̻���

����:
- �޸� ��뷮 ����
- ���� ������ ����
- ������Ʈ ��� ����

4. ����ȭ ���
-----------
- ���� Ʈ����
- ��� Ʈ��
- �Ͻ�ƿ Ʈ��
- �޸� Ǯ��

5. ���� ����
---------
- �ڵ� �ϼ�
- ����� �˻�
- IP �����
- ��ȭ��ȣ��

�� ������ ���ڿ� ó����
Ưȭ�� Ʈ�� ������
�⺻ ������ �����ݴϴ�.
*/