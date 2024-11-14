#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

/*
���� ��ȹ�� (Dynamic Programming)
============================

�ٽ� ����:
1. ���� �κ� ���� (Optimal Substructure)
   - ū ������ �����ذ� ���� �������� �����ظ� ����
   - F(n) = F(n-1) + F(n-2) ���·� ǥ�� ����
   - �����/����� ���� ��� ����

2. �ߺ��Ǵ� �κ� ���� (Overlapping Subproblems)
   - ������ ���� �������� ���� �� ����
   - �޸𸮿� �����Ͽ� ���� (�޸������̼�)
   - �ߺ� ��� ���ŷ� ���� ���

3. ���� ���
   a) Top-down (�����)
      - ����� ����
      - �ʿ��� �κи� ���
      - �޸������̼� Ȱ��

   b) Bottom-up (�����)
      - �ݺ��� ����
      - ���� �������� ������ �ذ�
      - ���̺� ���
*/

// === 1. �⺻ ��� ��� ===
long long fib_recursive(int n, bool print_steps) {
    static int depth = 0;  // ��� ���� ����
    depth++;

    // �鿩����� ��� ���� ǥ��
    if (print_steps) {
        for (int i = 1; i < depth; i++) printf("  ");
        printf("Calculating F(%d)\n", n);
    }

    // ���� ����
    if (n <= 1) {
        if (print_steps) {
            for (int i = 1; i < depth; i++) printf("  ");
            printf("Base case: F(%d) = %d\n", n, n);
        }
        depth--;
        return n;
    }

    // ����� ���
    long long result = fib_recursive(n - 1, print_steps) +
        fib_recursive(n - 2, print_steps);

    if (print_steps) {
        for (int i = 1; i < depth; i++) printf("  ");
        printf("F(%d) = F(%d) + F(%d) = %lld\n", n, n - 1, n - 2, result);
    }

    depth--;
    return result;
}

// === 2. �޸������̼� (Top-down) ===
long long* memo;
int* calculation_count;  // �� n�� ���� ��� Ƚ�� ����

void init_memoization(int size) {
    memo = (long long*)malloc(size * sizeof(long long));
    calculation_count = (int*)calloc(size, sizeof(int));

    for (int i = 0; i < size; i++) {
        memo[i] = -1;  // �̰�� ���� ǥ��
    }
}

void cleanup_memoization(void) {
    free(memo);
    free(calculation_count);
}

long long fib_memoization(int n, bool print_steps) {
    static int depth = 0;
    depth++;

    // ���� ��Ȳ ���
    if (print_steps) {
        for (int i = 1; i < depth; i++) printf("  ");
        printf("Checking F(%d)", n);
        if (memo[n] != -1) {
            printf(" [Cached value: %lld]\n", memo[n]);
        }
        else {
            printf(" [Not calculated yet]\n");
        }
    }

    // ���� ����
    if (n <= 1) {
        calculation_count[n]++;
        if (print_steps) {
            for (int i = 1; i < depth; i++) printf("  ");
            printf("Base case: F(%d) = %d\n", n, n);
        }
        depth--;
        return n;
    }

    // �̹� ���� ���̸� ����
    if (memo[n] != -1) {
        if (print_steps) {
            for (int i = 1; i < depth; i++) printf("  ");
            printf("Reusing F(%d) = %lld\n", n, memo[n]);
        }
        depth--;
        return memo[n];
    }

    // ���ο� �� ���
    calculation_count[n]++;
    memo[n] = fib_memoization(n - 1, print_steps) +
        fib_memoization(n - 2, print_steps);

    if (print_steps) {
        for (int i = 1; i < depth; i++) printf("  ");
        printf("Calculated F(%d) = %lld\n", n, memo[n]);
    }

    depth--;
    return memo[n];
}

// === 3. Ÿ�淹�̼� (Bottom-up) ===
typedef struct {
    long long* table;
    int size;
} FibTable;

FibTable create_fib_table(int size) {
    FibTable table;
    table.table = (long long*)malloc(size * sizeof(long long));
    table.size = size;

    // ���� ���� �ʱ�ȭ
    if (size > 0) table.table[0] = 0;
    if (size > 1) table.table[1] = 1;

    return table;
}

void cleanup_fib_table(FibTable* table) {
    free(table->table);
}

long long fib_tabulation(int n, bool print_steps) {
    if (n < 0) return -1;

    FibTable table = create_fib_table(n + 1);

    if (print_steps) {
        printf("\n=== Bottom-up Calculation Steps ===\n");
        if (n >= 0) printf("Initialize F(0) = 0\n");
        if (n >= 1) printf("Initialize F(1) = 1\n");
    }

    // Bottom-up���� �� ���
    for (int i = 2; i <= n; i++) {
        table.table[i] = table.table[i - 1] + table.table[i - 2];

        if (print_steps) {
            printf("F(%d) = F(%d) + F(%d) = %lld + %lld = %lld\n",
                i, i - 1, i - 2, table.table[i - 1], table.table[i - 2], table.table[i]);
        }
    }

    long long result = table.table[n];
    cleanup_fib_table(&table);
    return result;
}

// === 4. ���� ����ȭ ���� ===
long long fib_optimized(int n, bool print_steps) {
    if (n <= 1) return n;

    long long prev = 0, curr = 1;

    if (print_steps) {
        printf("\n=== Space-Optimized Calculation Steps ===\n");
        printf("Initialize: prev = 0, curr = 1\n");
    }

    for (int i = 2; i <= n; i++) {
        long long next = prev + curr;

        if (print_steps) {
            printf("Step %d: %lld + %lld = %lld\n", i, prev, curr, next);
        }

        prev = curr;
        curr = next;
    }

    return curr;
}

// === ���� �м� �� �� ===
void analyze_performance(int n) {
    clock_t start, end;
    double cpu_time;

    printf("\n=== Performance Analysis for F(%d) ===\n", n);

    // 1. �⺻ ���
    if (n <= 40) {
        start = clock();
        long long result1 = fib_recursive(n, false);
        end = clock();
        cpu_time = ((double)(end - start)) / CLOCKS_PER_SEC;
        printf("\n1. Basic Recursive:\n");
        printf("   Result: %lld\n", result1);
        printf("   Time: %.6f seconds\n", cpu_time);
        printf("   Space: O(n) - ��� ����\n");
    }
    else {
        printf("\n1. Basic Recursive: n > 40 - �ð� �ʰ� ����\n");
    }

    // 2. �޸������̼�
    start = clock();
    init_memoization(n + 1);
    long long result2 = fib_memoization(n, false);
    end = clock();
    cpu_time = ((double)(end - start)) / CLOCKS_PER_SEC;

    printf("\n2. Memoization:\n");
    printf("   Result: %lld\n", result2);
    printf("   Time: %.6f seconds\n", cpu_time);
    printf("   Calculation counts:\n");
    for (int i = 0; i <= n; i++) {
        if (calculation_count[i] > 0) {
            printf("   F(%d): %d times\n", i, calculation_count[i]);
        }
    }
    cleanup_memoization();

    // 3. Ÿ�淹�̼�
    start = clock();
    long long result3 = fib_tabulation(n, false);
    end = clock();
    cpu_time = ((double)(end - start)) / CLOCKS_PER_SEC;

    printf("\n3. Tabulation:\n");
    printf("   Result: %lld\n", result3);
    printf("   Time: %.6f seconds\n", cpu_time);
    printf("   Space: O(n) - ���̺�\n");

    // 4. ���� ����ȭ
    start = clock();
    long long result4 = fib_optimized(n, false);
    end = clock();
    cpu_time = ((double)(end - start)) / CLOCKS_PER_SEC;

    printf("\n4. Space Optimized:\n");
    printf("   Result: %lld\n", result4);
    printf("   Time: %.6f seconds\n", cpu_time);
    printf("   Space: O(1) - ��� ����\n");
}

// === ���� �Լ� ===
int main(void) {
    printf("=== ���� ��ȹ�� �н�: �Ǻ���ġ ���� ===\n");
    printf("�� ���α׷��� �� ���� �ٸ� ���� ����� ���մϴ�:\n");
    printf("1. �⺻ ��� (educational)\n");
    printf("2. �޸������̼� (top-down)\n");
    printf("3. Ÿ�淹�̼� (bottom-up)\n");
    printf("4. ���� ����ȭ ����\n\n");

    int choice, n;
    bool running = true;

    while (running) {
        printf("\n=== �޴� ===\n");
        printf("1. �⺻ ��� ���� (�ܰ� ǥ��)\n");
        printf("2. �޸������̼� ���� (�ܰ� ǥ��)\n");
        printf("3. Ÿ�淹�̼� ���� (�ܰ� ǥ��)\n");
        printf("4. ���� ����ȭ ���� (�ܰ� ǥ��)\n");
        printf("5. ��� ��� ���� ��\n");
        printf("0. ����\n");
        printf("����: ");
        scanf("%d", &choice);

        if (choice == 0) {
            running = false;
            continue;
        }

        printf("����� �Ǻ���ġ �� �Է� (n): ");
        scanf("%d", &n);

        if (n < 0) {
            printf("������ ����� �� �����ϴ�.\n");
            continue;
        }

        switch (choice) {
        case 1:
            if (n > 40) {
                printf("n�� �ʹ� Ů�ϴ�. 40 ���ϸ� �Է��ϼ���.\n");
                break;
            }
            printf("\n���: %lld\n", fib_recursive(n, true));
            break;

        case 2:
            init_memoization(n + 1);
            printf("\n���: %lld\n", fib_memoization(n, true));
            cleanup_memoization();
            break;

        case 3:
            printf("\n���: %lld\n", fib_tabulation(n, true));
            break;

        case 4:
            printf("\n���: %lld\n", fib_optimized(n, true));
            break;

        case 5:
            analyze_performance(n);
            break;

        default:
            printf("�߸��� �����Դϴ�.\n");
        }
    }

    return 0;
}

/*
���� �м� �� ����
==============

1. �ð� ���⵵ �м�
----------------
�⺻ ���:
- O(2^n)
- �� ȣ�⸶�� 2���� ��� ȣ��
- �ߺ� ����� �ſ� ����

�޸������̼�:
- O(n)
- �� �κ� ������ �� ���� ���
- ĳ�� �������� ���� ����

Ÿ�淹�̼�:
- O(n)
- ������ ������� ������� �ּ�ȭ
- ��� ���� �� ������ ���

���� ����ȭ:
- O(n) �ð�
- O(1) ����
- ���� ȿ������ ����

2. ���� ���⵵ �м�
----------------
�⺻ ���:
- O(n) ���� ����
- ��� ȣ�⸶�� ���� ������ ���
- ���� �����÷ο� ����

�޸������̼�:
- O(n) �޸�
- ��� �κ� ������ ��� ����
- ���� ���� �߰� �ʿ�

Ÿ�淹�̼�:
- O(n) �޸�
- ���̺� ��� �� ����
- ���� ��� ����

���� ����ȭ:
- O(1) �޸�
- �ʿ��� �ּ����� ���� ����
- �߰� ���� ����

3. ����
------
�ߺ� ����� ����:
- �⺻ ��Ϳ��� ������ ����
- �޸������̼����� ������ ���� ���
- ���� ��� �ݺ��� ���� ������ �ֿ���

�޸�-�ð� Ʈ���̵����:
- ������ ����� �ð� ����
- �޸� ��뷮�� ���� �ӵ��� ����
- ��Ȳ�� ���� ������ ����

���� ����� ����:
- ���� ũ��� ���� ���� ���
- �������� ���������� ����
- ���� ȯ�濡���� ���뼺 ����

4. ����ȭ ���ɼ�
-------------
��� �ŵ�����:
- O(log n) �ð� ���⵵
- ���� ���� ��� Ȱ��
- ��Ը� ���� ȿ����

��ⷯ ����:
- �����÷ο� ����
- ��ȣ���� ����
- �ſ� ū �� ó��

����ȭ:
- ������ �κ� ���� ���� ó��
- �ϵ���� Ȱ�� ����ȭ
- ��Ը� ��꿡 ����

5. ���� ���� �о�
--------------
�˰��� ����:
- ���� ��ȹ���� ���� ����
- ���� �������� ��
- ����ȭ ��� �н�

�ý��� ����:
- ĳ�� �ý��� ����
- �޸� ���� ����
- ���� ����ȭ ���

������ ����:
- ������ ����
- Ȳ�ݺ���� ����
- �ڿ��� ���� �𵨸�

6. ���� �� �������
---------------
�����÷ο� ó��:
- ū �� ó�� ���
- �ڷ��� ����
- ���� ó��

�޸� ����:
- ���� �Ҵ� �� ����
- �޸� ���� ����
- ȿ���� �ڿ� ���

����� ���̼�:
- �ܰ躰 ���� ��Ȳ ���
- �߰� ��� Ȯ��
- ���� ���� ����

7. ���� ��� ����
--------------
ĳ�� ȿ����:
- �޸� ���� ���� ����ȭ
- ������ ���� Ȱ��
- ĳ�� �̽� �ּ�ȭ

�޸� ����:
- ���ʿ��� �Ҵ� ����
- ���� ��Ȱ��
- �ӽ� ��ü �ּ�ȭ

�˰��� ����:
- �Է� ũ�⺰ ���� ���
- �ϵ���� Ư�� ���
- ���� ȯ�� ����

�� ������ ���� ��ȹ���� �⺻��
�Ǻ���ġ ������ ���� �����ϸ�,
�پ��� ����ȭ ����� ���� ����
���ɼ��� �����ݴϴ�.
*/