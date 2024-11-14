#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

/*
�Ǻ���ġ ���� �پ��� ��� ���:

1. �⺻ ��� ��� (O(2^n))
   - ������������ �ſ� ��ȿ����
   - �ߺ� ����� ����

2. ���� ��ȹ�� (O(n))
   - �ߺ� ��� ����
   - �޸� O(n) �Ǵ� O(1)

3. ��� �ŵ����� ��� (O(log n))
   - ���� ������ �ٽ� ����
   - [[1 1], [1 0]]^n = [[F(n+1) F(n)], [F(n) F(n-1)]]

4. ������ ���� (O(1))
   - �������� ū ������ ���е� ����
   - Binet's Formula Ȱ��
*/

// 2x2 ��� ����ü
typedef struct {
    long long m[2][2];
} Matrix2x2;

/* ��� ����
 * - 2x2 ����� ���� ����
 */
Matrix2x2 matrix_multiply(Matrix2x2 a, Matrix2x2 b) {
    Matrix2x2 result;
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            result.m[i][j] = 0;
            for (int k = 0; k < 2; k++) {
                result.m[i][j] += a.m[i][k] * b.m[k][j];
            }
        }
    }
    return result;
}

/* �⺻ ��� ��� �Ǻ���ġ
 * - �ð����⵵: O(2^n)
 * - ������������ �ſ� ��ȿ����
 */
long long fib_recursive(int n, bool print_steps) {
    if (print_steps) {
        printf("Computing F(%d)\n", n);
    }

    if (n <= 1) return n;

    long long result = fib_recursive(n - 1, print_steps) +
        fib_recursive(n - 2, print_steps);

    if (print_steps) {
        printf("F(%d) = %lld\n", n, result);
    }

    return result;
}

/* ���� ��ȹ�� �Ǻ���ġ
 * - �ð����⵵: O(n)
 * - �������⵵: O(1)
 */
long long fib_dynamic(int n, bool print_steps) {
    if (n <= 1) return n;

    long long prev = 0, curr = 1;

    for (int i = 2; i <= n; i++) {
        long long next = prev + curr;
        if (print_steps) {
            printf("F(%d) = F(%d) + F(%d) = %lld + %lld = %lld\n",
                i, i - 2, i - 1, prev, curr, next);
        }
        prev = curr;
        curr = next;
    }

    return curr;
}

/* ��� �ŵ������� �̿��� �Ǻ���ġ
 * - �ð����⵵: O(log n)
 * - ���� ���� ���
 */
Matrix2x2 matrix_power(Matrix2x2 base, int exp, bool print_steps) {
    if (print_steps) {
        printf("Computing matrix power for n = %d\n", exp);
    }

    if (exp == 0) {
        Matrix2x2 identity = { {1, 0}, {0, 1} };
        return identity;
    }
    if (exp == 1) return base;

    Matrix2x2 half = matrix_power(base, exp / 2, print_steps);
    Matrix2x2 result = matrix_multiply(half, half);

    if (exp % 2 == 1) {
        result = matrix_multiply(result, base);
    }

    if (print_steps) {
        printf("Matrix power %d result:\n", exp);
        printf("[[%lld %lld]\n", result.m[0][0], result.m[0][1]);
        printf(" [%lld %lld]]\n", result.m[1][0], result.m[1][1]);
    }

    return result;
}

/* ��� ��� �Ǻ���ġ
 * - F(n)�� ��� �ŵ��������� ���
 */
long long fib_matrix(int n, bool print_steps) {
    if (n <= 1) return n;

    Matrix2x2 base = { {1, 1}, {1, 0} };
    if (print_steps) {
        printf("Base matrix:\n[[1 1]\n [1 0]]\n\n");
    }

    Matrix2x2 result = matrix_power(base, n - 1, print_steps);

    return result.m[0][0];
}

/* ������ ������ �̿��� �Ǻ���ġ (Binet's Formula)
 * - �ð����⵵: O(1)
 * - ū ������ ���е� ���� ����
 */
double fib_binet(int n) {
    const double phi = (1 + sqrt(5)) / 2;  // Ȳ�ݺ�
    return round((pow(phi, n) - pow(-phi, -n)) / sqrt(5));
}

/* ���� �ð� ���� �� �� */
void compare_methods(int n) {
    printf("\nComparing different methods for F(%d):\n", n);
    printf("----------------------------------------\n");

    // ���� ��ȹ��
    clock_t start = clock();
    long long result_dp = fib_dynamic(n, false);
    clock_t end = clock();
    printf("Dynamic Programming: %lld (%.6f seconds)\n",
        result_dp, ((double)(end - start)) / CLOCKS_PER_SEC);

    // ��� ���
    start = clock();
    long long result_matrix = fib_matrix(n, false);
    end = clock();
    printf("Matrix Method: %lld (%.6f seconds)\n",
        result_matrix, ((double)(end - start)) / CLOCKS_PER_SEC);

    // Binet ����
    start = clock();
    double result_binet = fib_binet(n);
    end = clock();
    printf("Binet's Formula: %.0f (%.6f seconds)\n",
        result_binet, ((double)(end - start)) / CLOCKS_PER_SEC);

    // �⺻ ��� (���� ����)
    if (n <= 40) {
        start = clock();
        long long result_rec = fib_recursive(n, false);
        end = clock();
        printf("Basic Recursive: %lld (%.6f seconds)\n",
            result_rec, ((double)(end - start)) / CLOCKS_PER_SEC);
    }
    else {
        printf("Basic Recursive: Too large to compute\n");
    }
}

/* �޸� ��뷮 �м� */
void analyze_memory_usage(void) {
    printf("\nMemory Usage Analysis:\n");
    printf("---------------------\n");
    printf("Basic Recursive: O(n) stack space\n");
    printf("Dynamic Programming: O(1) auxiliary space\n");
    printf("Matrix Method: O(1) auxiliary space\n");
    printf("Binet's Formula: O(1) auxiliary space\n\n");

    printf("Detailed explanation:\n");
    printf("1. Basic Recursive:\n");
    printf("   - Requires stack space for each recursive call\n");
    printf("   - Maximum stack depth = n\n");
    printf("   - Can cause stack overflow for large n\n\n");

    printf("2. Dynamic Programming:\n");
    printf("   - Only stores previous two values\n");
    printf("   - Constant extra space\n");
    printf("   - Very memory efficient\n\n");

    printf("3. Matrix Method:\n");
    printf("   - Uses 2x2 matrix operations\n");
    printf("   - Constant extra space\n");
    printf("   - Logarithmic time complexity\n\n");

    printf("4. Binet's Formula:\n");
    printf("   - No extra space needed\n");
    printf("   - Limited by floating-point precision\n");
    printf("   - Best for small numbers\n");
}

/* �޴� ��� */
void print_menu(void) {
    printf("\n=== Fibonacci Calculation Menu ===\n");
    printf("1. Basic recursive method\n");
    printf("2. Dynamic programming method\n");
    printf("3. Matrix power method\n");
    printf("4. Compare all methods\n");
    printf("5. Analyze memory usage\n");
    printf("0. Exit\n");
    printf("Choice: ");
}

int main(void) {
    int choice;
    do {
        print_menu();
        scanf("%d", &choice);

        switch (choice) {
        case 1:  // Basic recursive
        case 2:  // Dynamic programming
        case 3: {  // Matrix power
            int n;
            printf("Enter n: ");
            scanf("%d", &n);

            if (n < 0) {
                printf("Please enter a non-negative number.\n");
                break;
            }

            if (choice == 1 && n > 40) {
                printf("Warning: n > 40 will take very long with recursive method.\n");
                printf("Do you want to continue? (1/0): ");
                int confirm;
                scanf("%d", &confirm);
                if (!confirm) break;
            }

            long long result;
            switch (choice) {
            case 1:
                result = fib_recursive(n, true);
                break;
            case 2:
                result = fib_dynamic(n, true);
                break;
            case 3:
                result = fib_matrix(n, true);
                break;
            }

            printf("\nF(%d) = %lld\n", n, result);
            break;
        }

        case 4: {  // Compare methods
            int n;
            printf("Enter n: ");
            scanf("%d", &n);
            if (n < 0) {
                printf("Please enter a non-negative number.\n");
                break;
            }
            compare_methods(n);
            break;
        }

        case 5:  // Analyze memory
            analyze_memory_usage();
            break;

        case 0:  // Exit
            printf("Exiting program\n");
            break;

        default:
            printf("Invalid choice\n");
        }
    } while (choice != 0);

    return 0;
}

/*
==========================================
�� ���� �� �ֿ� ����
==========================================

1. �پ��� ���� ���
---------------
�⺻ ���:
- ���� �������� ����
- F(n) = F(n-1) + F(n-2)
- ���� �ð� ���⵵
- ���������� ����

���� ��ȹ��:
- �ߺ� ��� ����
- ���� �ð� ���⵵
- �޸� ȿ����
- �ǿ����� ���

��� �ŵ�����:
- ���� ������ �ٽ�
- �α� �ð� ���⵵
- ū ���� ȿ����
- ��� ���� ����

2. ��� ����� ����
---------------
�⺻ ����:
[[1 1], [1 0]]^n = [[F(n+1) F(n)], [F(n) F(n-1)]]

������ ����:
1) n = 1�� �� ����
2) n -> n+1���� �ͳ��� ����
3) ��� ������ ���� Ȱ��

����:
- ���� ��� �ӵ�
- ū �� ó�� ����
- ������ �����

3. ���� �м�
----------
�ð� ���⵵:
- ���: O(2^n)
- ���� ��ȹ��: O(n)
- ���: O(log n)
- Binet: O(1)

���� ���⵵:
- ���: O(n)
- ���� ��ȹ��: O(1)
- ���: O(1)
- Binet: O(1)

4. ������ ����
-----------
�Ǻ���ġ ���� ����:
- Ȳ�ݺ���� ����
- ��� ǥ���� �ǹ�
- ������ ����
- ������ ����

���� �о�:
- �ڿ����� �Ǻ���ġ ����
- ��ȣ��
- �˰��� ����
- ������ �𵨸�

5. ����ȭ ���
-----------
- ��� ���� ����ȭ
- ĳ�� Ȱ��
- ��ⷯ ����
- ����ȭ ���ɼ�

�� ������ �Ǻ���ġ �� �����
�پ��� ����� �����ָ�, �� �����
������� ������ ���� �� �ְ�
�մϴ�.
*/