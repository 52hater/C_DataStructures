#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

/*
�ŵ������� ���� ����:
- x^n = x^(n/2) * x^(n/2)     (n�� ¦���� ��)
- x^n = x^(n/2) * x^(n/2) * x (n�� Ȧ���� ��)

����: 2^10 ���
1. 2^10 = (2^5)^2
2. 2^5 = 2^4 * 2
3. 2^4 = (2^2)^2
4. 2^2 = (2^1)^2
5. 2^1 = 2

�Ϲ��� ���: O(n) -> ���� ����: O(log n)
*/

/* �Ϲ����� �ŵ����� ���
 * - �ð����⵵: O(n)
 */
long long power_naive(long long base, int exp, bool print_steps) {
    long long result = 1;

    for (int i = 0; i < exp; i++) {
        result *= base;
        if (print_steps) {
            printf("Step %d: %lld^%d = %lld\n",
                i + 1, base, i + 1, result);
        }
    }

    return result;
}

/* ���� ������ �̿��� �ŵ����� ���
 * - �ð����⵵: O(log n)
 */
long long power_divide_conquer(long long base, int exp, bool print_steps) {
    if (print_steps) {
        printf("Calculating %lld^%d\n", base, exp);
    }

    if (exp == 0) {
        if (print_steps) printf("Base case: %lld^0 = 1\n", base);
        return 1;
    }
    if (exp == 1) {
        if (print_steps) printf("Base case: %lld^1 = %lld\n", base, base);
        return base;
    }

    // ����: ������ �������� ����
    long long half = power_divide_conquer(base, exp / 2, print_steps);
    long long result;

    if (exp % 2 == 0) {
        // ¦��: half * half
        result = half * half;
        if (print_steps) {
            printf("Even exp: %lld^%d = %lld^%d * %lld^%d = %lld * %lld = %lld\n",
                base, exp, base, exp / 2, base, exp / 2, half, half, result);
        }
    }
    else {
        // Ȧ��: half * half * base
        result = half * half * base;
        if (print_steps) {
            printf("Odd exp: %lld^%d = %lld^%d * %lld^%d * %lld = %lld * %lld * %lld = %lld\n",
                base, exp, base, exp / 2, base, exp / 2, base, half, half, base, result);
        }
    }

    return result;
}

/* ��ⷯ ������ ������ �ŵ����� ���
 * - �ð����⵵: O(log n)
 * - ū ���� �ŵ������� �����ϰ� ���
 */
long long power_modular(long long base, int exp, long long mod, bool print_steps) {
    if (print_steps) {
        printf("Calculating %lld^%d mod %lld\n", base, exp, mod);
    }

    if (exp == 0) {
        if (print_steps) printf("Base case: %lld^0 mod %lld = 1\n", base, mod);
        return 1;
    }

    // ����: ������ �������� ����
    long long half = power_modular(base, exp / 2, mod, print_steps);
    long long result;

    if (exp % 2 == 0) {
        result = (half * half) % mod;
        if (print_steps) {
            printf("Even exp: %lld^%d mod %lld = (%lld * %lld) mod %lld = %lld\n",
                base, exp, mod, half, half, mod, result);
        }
    }
    else {
        result = ((half * half) % mod * base % mod) % mod;
        if (print_steps) {
            printf("Odd exp: %lld^%d mod %lld = (%lld * %lld * %lld) mod %lld = %lld\n",
                base, exp, mod, half, half, base, mod, result);
        }
    }

    return result;
}

/* ������ ǥ���� �̿��� �ŵ����� ���
 * - �ð����⵵: O(log n)
 * - ��Ʈ ���� Ȱ��
 */
long long power_binary(long long base, int exp, bool print_steps) {
    long long result = 1;

    if (print_steps) {
        printf("Binary representation of %d: ", exp);
        for (int i = 31; i >= 0; i--) {
            if (exp & (1 << i)) printf("1");
            else printf("0");
        }
        printf("\n");
    }

    while (exp > 0) {
        if (exp & 1) {  // ���� ��Ʈ�� 1�̸�
            if (print_steps) {
                printf("Current bit is 1: multiply result by %lld\n", base);
            }
            result *= base;
        }

        base *= base;  // ���� ��Ʈ�� ���� base�� ����
        exp >>= 1;     // ���� ��Ʈ�� �̵�

        if (print_steps && exp > 0) {
            printf("Square base: %lld, Next exp: %d\n", base, exp);
        }
    }

    return result;
}

/* ���� �ð� ���� �Լ� */
double measure_time(long long (*func)(long long, int, bool),
    long long base, int exp, bool print_steps) {
    clock_t start = clock();
    long long result = func(base, exp, print_steps);
    clock_t end = clock();

    printf("Result: %lld\n", result);
    return ((double)(end - start)) / CLOCKS_PER_SEC;
}

/* ���� �� �м� */
void compare_performance(void) {
    printf("\nPerformance Comparison:\n");
    printf("--------------------\n");

    int exponents[] = { 10, 100, 1000, 10000 };
    int num_tests = sizeof(exponents) / sizeof(exponents[0]);

    for (int i = 0; i < num_tests; i++) {
        int exp = exponents[i];
        printf("\nTesting with base = 2, exponent = %d\n", exp);

        printf("Naive approach: ");
        double time_naive = measure_time(power_naive, 2, exp, false);
        printf("Time: %f seconds\n", time_naive);

        printf("Divide and conquer: ");
        double time_dc = measure_time(power_divide_conquer, 2, exp, false);
        printf("Time: %f seconds\n", time_dc);

        printf("Binary method: ");
        double time_bin = measure_time(power_binary, 2, exp, false);
        printf("Time: %f seconds\n", time_bin);
    }
}

/* �޴� ��� */
void print_menu(void) {
    printf("\n=== Fast Power Calculation Menu ===\n");
    printf("1. Calculate power (naive)\n");
    printf("2. Calculate power (divide and conquer)\n");
    printf("3. Calculate power with modulo\n");
    printf("4. Calculate power (binary method)\n");
    printf("5. Compare performance\n");
    printf("0. Exit\n");
    printf("Choice: ");
}

int main(void) {
    int choice;
    do {
        print_menu();
        scanf("%d", &choice);

        switch (choice) {
        case 1:  // Naive
        case 2:  // Divide and conquer
        case 4: { // Binary method
            long long base;
            int exp;
            printf("Enter base: ");
            scanf("%lld", &base);
            printf("Enter exponent: ");
            scanf("%d", &exp);

            long long result;
            if (choice == 1) {
                result = power_naive(base, exp, true);
            }
            else if (choice == 2) {
                result = power_divide_conquer(base, exp, true);
            }
            else {
                result = power_binary(base, exp, true);
            }

            printf("Result: %lld\n", result);
            break;
        }

        case 3: {  // Modular exponentiation
            long long base, mod;
            int exp;
            printf("Enter base: ");
            scanf("%lld", &base);
            printf("Enter exponent: ");
            scanf("%d", &exp);
            printf("Enter modulus: ");
            scanf("%lld", &mod);

            long long result = power_modular(base, exp, mod, true);
            printf("Result: %lld\n", result);
            break;
        }

        case 5:  // Compare performance
            compare_performance();
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

1. ���� ���� ���ٹ�
---------------
�⺻ ���̵��:
- ������ �������� ����
- �κ� ����� ����
- Ȧ��/¦�� ��� ����

����:
- ���� Ƚ�� ����
- ���� ��� �ӵ�
- �޸� ȿ����

2. �ð� ���⵵ �м�
---------------
�Ϲ��� ���: O(n)
- n���� ���� �ʿ�
- ���� �ð� ���⵵

���� ����: O(log n)
- ������ ���ݾ� ����
- �α� �ð� ���⵵
- ��� ȣ�� Ʈ��

3. ���� ���
----------
����� ����:
- ������ ���� ����
- �ڵ� ����
- ���� ���

������ ���:
- ��Ʈ ���� Ȱ��
- �ݺ��� ����
- �޸� ȿ����

4. ���� �о�
----------
- ��ȣȭ �˰���
- ��ⷯ ����
- ū �� ���
- ��� ���� ���

5. ����ȭ ���
-----------
- ��ⷯ ���� Ȱ��
- ��Ʈ ���� ����ȭ
- ��� ����
- �߰� ��� ����

�� ������ �ŵ����� ����� �پ���
����� �����ָ�, ���� ���� ���̸�
���� �� �ֽ��ϴ�. ���� �ܰ���
�Ǻ���ġ �� ����� ���ʰ� �˴ϴ�.
*/