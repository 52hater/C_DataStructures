#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ������ Ÿ�� �߻�ȭ�� ���� type definition (�ʿ�� int�κ��� ��ü)
typedef int DataType;

// ��� ����ü ����
typedef struct Node {
	DataType data; // ��尡 ������ ������
	struct Node* next; // ���� ��带 ����Ű�� ������
} Node;

// ���Ḯ��Ʈ ����ü ����
typedef struct LinkedList {
	Node* head; // ù��° ���(���)�� ����Ű�� ������
	size_t size; // ����Ʈ�� ũ��(��� ����)
} LinkedList;

/*  ����Ʈ ���� �� �ʱ�ȭ �Լ�
	- ���ο� �� ���� ����Ʈ�� �����ϰ� �ʱ�ȭ
	- ��ȯ�� : �ʱ�ȭ�� LinkedList ����ü */
LinkedList* list_create(void) {
	LinkedList* list = (LinkedList*)malloc(sizeof(LinkedList)); //������Լ��� void* Ÿ���̹Ƿ� ����ȯ ����.
	if (list == NULL) { // �̰� ������ �������� ������ �Ͼ (���� ����ü�� list->head = NULL; �̷��� �Ϸ��ϴϱ� ũ���ó�)
		puts("����Ʈ ���� ����");
		return NULL; // �����͸� ��ȯ�ؾ��ϴ� 1 �ȵ�(Ÿ�Ժ���ġ)
	}
	list->head = NULL; // = (*list).head �� �Ϻ��ϰ� ��ġ��(->�����ڴ� �������� ��� ������ ���ÿ� ����)
	list->size = 0;
	return list; // ���� ������ ������ ��ȯ
}

/*  ���ο� ��� ���� �Լ�
	- �����͸� ���� ���ο� ��带 ���� �Ҵ�(������)
	- �Ű����� : data >> ������ ������ ��
	- ��ȯ�� : ������ �� ����� ������ */

// node_create(5); �̷��� ȣ���ϸ� �Ķ���Ϳ� 5�� �ְ� ����� data�ʵ忡 �����ؼ� ������5�� ������ ���ο� ��尡 �����ǰ���.
Node* node_create(DataType data) { // ���ο� ��尡 ������ �� ����� data �ʵ忡 ����� ���� �޾ƿ��� ��.
	Node* new_node = (Node*)malloc(sizeof(Node));
	if (new_node == NULL) {
		return NULL;
	}
	new_node->data = data;
	new_node->next = NULL;
	return new_node;
}


// ����Ʈ�� ����ٰ� �Ǵ°� �ƴϰ� ������ ���� �� ���� �޸������� �Ǿ������� ������ �����ְ� ���� �� ����Ʈ�� ������ ��.
void list_destroy(LinkedList* list) {
	// �������忡�� ����� �޸��������� �����
	// �� �Լ������� �������带 �ݺ������� ȣ���ؼ� ��带 �� ���ִ� �ڵ带 ������ְ�
	// �������� free(list);
}

/*  ����Ʈ�� �� �տ� ��� ����
	- �� ��带 �����Ͽ� ����Ʈ�� ���� ����
	- �Ű����� : list - ��� ����Ʈ, data - ������ ������
	- ��ȯ�� : ���� �� 1, ���� �� 0 */
int list_push_front(LinkedList* list, DataType data) {
	Node* new_node = node_create(data); // ���ο� ��带 �����ϰ�(Node�� ����ü�� �޸� �ּ� ������ �� : new_node) ���빰�� �޾ƿ�
	if (new_node == NULL) { // ����� ���� ���н�
		return 0; // ������ ���н� 0��ȯ
	}
	new_node->next = list->head; // ���ο� ����� next �����Ͱ� ���� ����Ʈ�� ��带 ����Ű���� ����
	list->head = new_node; // ���� ���ο��尡 ���Ḯ��Ʈ'��' ��尡 �Ǿ���(->�� ������ �ڵ�)
	list->size++; // �������(����� ������) �ϳ� �þ��
	return 1; // �Ǿտ� ������ ������ 1��ȯ
}
/*  
	Node* new_node = node_create(data);
	�� �ڵ�� ���ο� ��带 �����ϰ�, ������ ����� �ּҸ� new_node �����Ϳ� �����մϴ�.
	��, node_create �Լ��� Node* Ÿ���� ���� ��ȯ�մϴ�. �� Node* Ÿ���� ���� ���� ������ Node ����ü�� �޸� �ּ��Դϴ�.
	Node*�� ��ȯ�Ѵٴ� ���� ������ Node�� ����ü�� �ּҸ� ��ȯ�ϴ� ���Դϴ�.
	Node ����ü ��ü�� �ƴ϶�, �� ����ü�� �޸� �ּҸ� ��ȯ�մϴ�.
	���������� ��ȯ�Ѵٴ� ���� �޸� �ּҸ� ��ȯ�Ѵٴ� ��
*/

/* ����Ʈ�� �� �ڿ� ��� �߰�
  - �� ��带 �����Ͽ� ����Ʈ�� �������� �߰�
  - �Ű�����: list - ��� ����Ʈ, data - ������ ������
  - ��ȯ��: ���� �� 1, ���� �� 0
*/

int list_push_back(LinkedList* list, DataType data) {
	Node* new_node = node_create(data); // ��屸��ü������Ÿ�� ������ ��常��� �Լ��� data�� �־ ������
	if (new_node == NULL) {
		return 0; // ������������ 0��ȯ
	}

	if (list->head == NULL) { // ����Ʈ�� ��尡 ������ (���������)
		list->head == new_node; // ���⼭ ���� ����尡 ��尡 �ǰ���
	}
	else { // ����Ʈ�� ������� ������
		Node* current = list->head; // ����Ʈ�� ���� �ִ� ��带 �����带 ����Ű�� �����ͷ� �����ϰ�
		while (current->next != NULL) { // �������ּ��� ���� ����ּҸ� ����Ű�� �ؽ�Ʈ �����Ͱ� ����������(���� �ڷ� �� ������)
			current = current->next; // ���ǹ� �ѹ� �ݺ��ɶ����� �̰� ����Ǵ°��� > ��ĭ�� �̵�
		}
		current->next = new_node; // ���� ����� �ؽ�Ʈ�� ���� ������ ��带 ����Ŵ(push_back�� �� ��)
		//current->next == NULL�� �Ǹ� while�� ���������� current->next = new_node; ����
	}
	list->size++; // ��� �߰��Ǿ����� ��� ���� �þ��
	return 1; // ����߰� ������ 1��ȯ
}

/* Ư�� ��ġ �ڿ� ��� ����
  - ������ ��� �ڿ� �� ��带 ����
  - �Ű�����: node - ���� ��ġ ���, data - ������ ������
  - ��ȯ��: ���� �� 1, ���� �� 0
 */
int list_insert_after(Node* node, DataType data) {
	
	if (node == NULL) { // ������ ��ġ(node) ��ȿ���˻����
		return 0;
	}

	Node* new_node = node_create(data);
	if (new_node == NULL) { // ������ ��ȿ���˻�
		return 0;
	}

	new_node->next = node->next; // ���� ����� �ؽ�Ʈ�ּҿ� ������� �ؽ�Ʈ�ּҸ� ���� (�ڿ� ��������)
	node->next = new_node; // �׷��� �ڿ����� �� ��
	
	return 1;
}

/* Ư�� ��ġ�� ��� Ž��
  - �ε����� ��带 ã�Ƽ� ��ȯ
  - �Ű�����: list - ��� ����Ʈ, index - ã�� ��ġ
  - ��ȯ��: ã�� ����� ������ �Ǵ� NULL
*/
Node* list_get_node(LinkedList* list, size_t index) { // search
	if (index >= list->size) { // index == list�� ��ü size�� 5�������� index 0,1,2,3,4 �̹Ƿ� ��ȿ���� ���� ��ġ������
		return NULL;
	}

	Node* current = list->head; // ������ �� �����ڰ�
	for (size_t i = 0; i < index; i++) { // ������ index(������� 3�̸�) 3ũ���� 012 ������ 2�� ã���� 3��° �ε����� ã�� ��)
		current = current->next;
		/* �ּ�ó�� ����Ʈ ũ�� 5�̰� 0, 1, 2, 3, 4 �� �ִٰ� ���� ��
		 Node* current = list->head; ���⼭ current�� 0�� ����Ŵ
		 index�� 3�� �޾ƿ��� 0 > 1 ���� ã��(i < index; �̴ϱ� 2��°���� ��ȸ�Ѱ���)
		 1��� ������ 2��带 ����Ű�� �ִ�(current = current->next;)
		 �׷��� �ᱹ ã�ƾ� �ϴ� 2��尡 ���� ��. (�޾ƿ� ��3�� �ᱹ 3��° ���(2���)�ϱ�)
		*/
	}
	return current;
}

/* ��� ����
 * - ����Ʈ���� Ư�� ��带 �����ϰ� �޸� ����
 * - �Ű�����: list - ��� ����Ʈ, target - ������ ���
 * - ��ȯ��: ���� �� 1, ���� �� 0
 */
int list_remove_target_node(LinkedList* list, Node* target) {
	
	if (target >= list->size) { // Ÿ�� ��ȿ�� �˻� outOfRange�� return 0;
		return 0;
	}

	// Ÿ�� ã��
	Node* current = list->head; // ������ ������ �ϳ� ��忡 �������ְ�
	Node* prev = NULL; // ������� ������ �ϳ� �������ְ� (previous)
	for (size_t i = 0; i < target; i++) { // target -1 ũ�⸸ŭ ��ȸ > �ε��� ã��
		prev = current; // ��ȸ�ϴµ��� �굵 ��ĭ�� �̵�
		current = current->next; // ��ȸ�ϴµ��� ������ ��ĭ�� �̵� (Ÿ���ּҴ� current�����Ϳ� ����)
	} // ã������ �ݺ��� ����������

	// ������� ��ũ�������(��� ���� ���� ����)
	if (prev == NULL) { // ù��° ��带 ������ ��
		list->head = current->next; // ������ Ÿ�� ���� ��尡 ��尡 �ǰ�
	}
	else { // �߰��̳� ������ ��带 ������ ��(Ÿ�� �տ� ��尡 �ִ�)
		prev->next = current->next; // ������ Ÿ���� �������� �ճ���� next ����
	}

	free(current); // Ÿ�� ��� �޸� ����
	list->size--; // �ϳ� �����ϸ� ����Ʈ�� ũ��--;
	return 1; // ���� ����
}

int list_remove_head(LinkedList* list) {

	

}

int list_remove_tail(LinkedList* list, Node* index) {



}

int main(void) {
	
	// ����Ʈ ����
	LinkedList* list = list_create();

	// �ڿ� ��� 5�� �߰� (0~4)
	for (int i = 0; i < 5; i++) {
		list_push_back(list, i);
	}

	// �տ� ��� 2�� �߰� (-2, -1)
	list_push_front(list, -2);
	list_push_front(list, -1);

	// ����Ʈ ���
	printf("����Ʈ1 :\n");
	for (size_t i = 0; i < list_size(list); i++) {
		Node* current = list_get_node(list, i);
		printf("list[%zu]: %d\n", i, current->data);
	}

	// �� ��° ��� �ڿ� �� ��� ����
	printf("\n����° ��� �ڿ� 3000����\n");
	Node* third_node = list_get_node(list, 2);
	list_insert_after(third_node, 3000);

	// ������ ����Ʈ ���
	printf("\n������ ����Ʈ :\n");
	for (size_t i = 0; i < list_size(list); i++) {
		Node* current = list_get_node(list, i);
		printf("list[%zu]: %d\n", i, current->data);
	}

	// ����Ʈ �޸� ����
	printf("\n����Ʈ �޸� ����\n");
	list_destroy(list);

	return 0;

}

/*
==========================================
�� ���� �� �ֿ� ����
==========================================

1. ����ü ����
-------------
- Node: ���� ��带 ǥ���ϴ� ����ü
  * data: ���� ������ ������
  * next: ���� ��带 ����Ű�� ������

- LinkedList: ��ü ����Ʈ�� �����ϴ� ����ü
  * head: ù ��° ��带 ����Ű�� ������
  * size: ���� ����Ʈ�� ũ��

2. �Լ� ��� ��Ģ
----------------
�������� ���� ���Ǵ� ��� ��Ģ�� ����:
- list_*: ����Ʈ ��ü�� ���� �۾�
- node_*: ���� ��忡 ���� �۾�
- ����_������ ���·� �Լ� �̸� ���� (��: list_push_front)

3. �޸� ����
-------------
- malloc()�� ���� ���� �Ҵ�
- �� �Ҵ� �� NULL üũ�� �޸� ���� ��Ȳ ó��
- free()�� ���� ������ �޸� ����
- �޸� ���� ������ ���� ü������ ���� ����

4. ���� ó��
-----------
- �Լ����� �۾� ����/���и� ��Ÿ���� ��ȯ�� ����
- NULL ������ �˻�
- �߸��� �ε��� ���� ����
- �޸� �Ҵ� ���� ó��

5. �ֿ� �۾���
-------------
- ����: list_create(), node_create()
- ����: list_push_front(), list_push_back(), list_insert_after()
- Ž��: list_get_node()
- ����: list_remove_node(), node_destroy()
- ����: list_destroy()

6. ����ȭ ����Ʈ
---------------
- size �ʵ带 ���� O(1) ũ�� ����
- ������ ������ ���� ȿ������ ��� ����
- ���ʿ��� ��ȸ �ּ�ȭ

7. ������ �������
----------------
- ��� ������ ���� �� NULL üũ
- ��� ����(empty list, single node) ó��
- �޸� ���� ������ ���� ü������ ����

8. Ȯ�强
--------
- DataType typedef�� ���� ������ Ÿ�� �߻�ȭ
- ���ȭ�� ����� ��� �߰� ����
- �ϰ��� �������̽� ����

9. �ڵ� ��Ÿ��
-------------
- �ϰ��� �鿩����
- ��Ȯ�� �Լ� �ּ�
- �ǹ� �ִ� ������
- ������ �Լ� ����ȭ

10. ��� ����
------------
- �⺻���� CRUD �۾� ����
- ����Ʈ ��ȸ �� ������ ����
- �޸� ���� ����
- ���� ó�� ����

�� ������ ���������ε� �����ϸ�, ���� ������Ʈ������
Ȱ���� �� �ִ� ������ �������� Ȯ�强�� ���߰� �ֽ��ϴ�.
*/