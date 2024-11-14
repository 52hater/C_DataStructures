#include <stdio.h>
#include <stdlib.h>

/*
Ž�� �˰��� - �Ž����� ����
==========================

Ư¡:
1. ���� ū �������� ����
2. �� ������ ���� ���ÿ� ����
3. �ѱ�/�̱� ȭ��ó�� ū ������ ���� ������ ����� �� ������ ����
4. ������ ȭ�� ���������� ������ �������� ����
*/

typedef struct {
    int* coins;         // ���� ����
    int* counts;        // �� ������ ��� ����
    int num_types;      // ���� ������ ��
    int total_coins;    // �� ���� ���� ��
} ChangeResult;

/* �Ž����� ��� �Լ� */
ChangeResult* calculate_change(int amount, int* denominations, int num_denoms) {
    ChangeResult* result = (ChangeResult*)malloc(sizeof(ChangeResult));
    result->coins = (int*)malloc(num_denoms * sizeof(int));
    result->counts = (int*)calloc(num_denoms, sizeof(int));
    result->num_types = num_denoms;
    result->total_coins = 0;

    printf("\n=== �Ž����� ��� ���� ===\n");
    printf("��ǥ �ݾ�: %d\n\n", amount);
    int remaining = amount;

    // ū �������� ó��
    for (int i = 0; i < num_denoms; i++) {
        result->coins[i] = denominations[i];
        if (remaining >= denominations[i]) {
            result->counts[i] = remaining / denominations[i];
            remaining = remaining % denominations[i];
            result->total_coins += result->counts[i];

            printf("���� %d�� ���: %d��\n",
                denominations[i], result->counts[i]);
            printf("���� �ݾ�: %d��\n\n", remaining);
        }
    }

    if (remaining > 0) {
        printf("���: �Ž������� ������ ���� �� �����ϴ�!\n");
        printf("������ �ݾ�: %d��\n", remaining);
    }

    return result;
}

/* ��� ��� �Լ� */
void print_change_result(ChangeResult* result) {
    printf("\n=== ���� �Ž����� ���� ===\n");
    for (int i = 0; i < result->num_types; i++) {
        if (result->counts[i] > 0) {
            printf("%d��: %d�� (�� %d��)\n",
                result->coins[i],
                result->counts[i],
                result->coins[i] * result->counts[i]);
        }
    }
    printf("\n�� ���� ���� ����: %d��\n", result->total_coins);
}

/* �ݷ� ���̽� �˻� �Լ� */
void check_counterexample(void) {
    printf("\n=== �ݷ� ���̽� �˻� ===\n");
    int amount = 6;
    int denoms[] = { 4, 3, 1 };  // ������ ���̽�
    int num_denoms = sizeof(denoms) / sizeof(denoms[0]);

    printf("�ݾ�: %d\n", amount);
    printf("ȭ�� ����: ");
    for (int i = 0; i < num_denoms; i++) {
        printf("%d ", denoms[i]);
    }
    printf("\n\n");

    printf("Ž���� ��� ���:\n");
    ChangeResult* greedy = calculate_change(amount, denoms, num_denoms);
    print_change_result(greedy);

    printf("\n������: 3�� 2�� = 2��\n");
    printf("Ž���� ��: 4�� 1�� + 1�� 2�� = 3��\n");
    printf("\n�� ��� Ž���� ����� �����ظ� ã�� ����\n");

    free(greedy->coins);
    free(greedy->counts);
    free(greedy);
}

/* �޸� ���� �Լ� */
void cleanup_change_result(ChangeResult* result) {
    free(result->coins);
    free(result->counts);
    free(result);
}

int main(void) {
    int default_denoms[] = { 500, 100, 50, 10, 1 };  // �ѱ� ȭ�� ����
    int num_default_denoms = sizeof(default_denoms) / sizeof(default_denoms[0]);

    printf("=== �Ž����� ��� ���α׷� ===\n");
    printf("1. �⺻ ȭ�� ���� ��� (�ѱ� ��ȭ)\n");
    printf("2. ����� ���� ȭ�� ����\n");
    printf("3. �ݷ� ���̽� Ȯ��\n");
    printf("����: ");

    int choice;
    scanf("%d", &choice);

    if (choice == 1 || choice == 2) {
        int amount;
        printf("�ݾ� �Է�: ");
        scanf("%d", &amount);

        ChangeResult* result;
        if (choice == 1) {
            result = calculate_change(amount, default_denoms, num_default_denoms);
        }
        else {
            int num_denoms;
            printf("ȭ�� ���� ���� �Է�: ");
            scanf("%d", &num_denoms);

            int* denoms = (int*)malloc(num_denoms * sizeof(int));
            printf("ȭ�� ���� �Է� (ū ��������): ");
            for (int i = 0; i < num_denoms; i++) {
                scanf("%d", &denoms[i]);
            }

            result = calculate_change(amount, denoms, num_denoms);
            free(denoms);
        }

        print_change_result(result);
        cleanup_change_result(result);
    }
    else if (choice == 3) {
        check_counterexample();
    }

    return 0;
}

/*
�Ž����� ���� �м�
==============

1. Ž���� ������ Ư��
-----------------
- �� �ܰ迡�� ���� ū ���� ����
- ���� ������ �������� ����
- ������ ������ �߱�

2. ������ ���� ����
---------------
- ū ������ ���� ������ ���
- ���ӵ� ���� �� ������ ��� ����
- �ּ� ������ 1�� ���

3. �ð� ���⵵
-----------
- O(N): N�� ȭ�� ������ ����
- �� ������ ���� �� ������ ���
- �ſ� ȿ������ �˰���

4. ���� �� Ȯ��
-----------
- ���Ǳ� �Ž����� ���
- ATM ��� �ݾ� ���
- ȭ�� ���� ����ȭ
- ���� �� �ּ�ȭ

5. �Ѱ���
-------
- ������ ȭ�� �������� ������ �̺���
- Ư�� ���̽����� ��ȿ���� �� ����
- ���� ���� ���� �� ���� �����

�� ������ �Ž����� ������ ����
Ž�� �˰����� �⺻ �����
�Ѱ����� ��Ȯ�� �����ݴϴ�.
*/