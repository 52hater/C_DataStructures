#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ������ Ÿ�� �߻�ȭ�� ���� type definition (�ʿ�� int�κ��� ��ü)
typedef int DataType;

// ��� ����ü ����
typedef struct Node {
	DataType data;
	struct Node* next;
} Node;

// ���Ḯ��Ʈ ����ü ����
typedef struct {
	Node* head;
	size_t size;
} LinkedList;

/*  ����Ʈ �ʱ�ȭ �Լ�
	- ���ο� �� ���� ����Ʈ�� �����ϰ� �ʱ�ȭ
	- ��ȯ�� : �ʱ�ȭ�� LinkedList ����ü */
LinkedList* list_create(void) {
	LinkedList* list = (LinkedList*)malloc(sizeof(LinkedList));
	if (list == NULL) { // �̰� ������ �������� ������ �Ͼ (���� ����ü�� list->head = NULL; �̷��� �Ϸ��ϴϱ� ũ���ó�)
		puts("����Ʈ ���� ����");
		return NULL; // �����͸� ��ȯ�ؾ��ϴ� 1 �ȵ�(Ÿ�Ժ���ġ)
	}
	list->head = NULL;
	list->size = 0;
	return list; // ���� ������ ������ ��ȯ
}

/*  ���ο� ��� ���� �Լ�
	- �����͸� ���� ���ο� ��带 ���� �Ҵ�(������)
	- �Ű����� : data >> ������ ������ ��
	- ��ȯ�� : ������ �� ����� ������ */
Node* node_create(DataType data) {
	Node* new_node = (Node*)malloc(sizeof(Node));
	if (new_node == NULL) {
		return NULL;
	}
	new_node->data = data;
	new_node->next = NULL;
	return new_node;
}

/*  ��� ���� �Լ�
	- ���� ����� "�޸� ��ü" �� ����
	- �Ű� ���� : node - ������ ����� ������ */
void node_destroy(Node* node) {
	free(node);
}

/*  ����Ʈ�� �� �տ� ��� ����
	- �� ��带 �����Ͽ� ����Ʈ�� ���� ����
	- �Ű����� : list - ��� ����Ʈ, data - ������ ������
	- ��ȯ�� : ���� �� 1, ���� �� 0 */
int list_push_front(LinkedList* list, DataType data) {
	Node* new_node = node_create(data);
	if (new_node == NULL) {
		return 0;
	}
	new_node->next = list->head;
	list->head = new_node;
	list->size++;
	return 1;
}