#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char *digits;
    int size;
} BigNum;

void bignum_init(BigNum *n, const char *str) {
    n->size = strlen(str);
    n->digits = malloc(n->size * sizeof(char));
    for (int i = 0; i < n->size; i++) {
        // Store digits in reverse. Convert from ASCII.
        n->digits[i] = str[n->size - 1 - i] - '0'; 
    }
}

void bignum_free(BigNum *n) {
    free(n->digits);
    n->digits = NULL;
    n->size = 0;
}

void bignum_print(BigNum *n) {
    printf("BigNum: ");
    for (int i = n->size - 1; i >= 0; i--) {
        printf("%d", n->digits[i]);
    }
    printf("\n");
}

int bignum_compare(const BigNum *a, const BigNum *b) {
    // Returns 1 if a is greater than b, -1 if less than, and 0 if equal to.
    if (a->size != b->size) {
        return a->size > b->size ? 1 : -1;
    }
    for (int i = a->size - 1; i >= 0; i--) {
        if (a->digits[i] != b->digits[i]) {
            return a->digits[i] > b->digits[i] ? 1 : -1;
        }
    }
    return 0;
}

void bignum_add(BigNum *result, const BigNum *a, const BigNum *b) {
    int max_size = a->size > b->size ? a->size : b->size;
    result->digits = malloc((max_size + 1) * sizeof(char));
    int carry = 0;
    int i;

    for (i = 0; i < max_size || carry; i++) {
        int sum = carry + (i < a->size ? a->digits[i] : 0) + (i < b->size ? b->digits[i] : 0);
        result->digits[i] = sum % 10; // Store the last digit of the sum.
        carry = sum / 10; // Carry any overflow.
    }
    result->size = i;
}

void bignum_multiply(BigNum *result, const BigNum *a, const BigNum *b) {
    result->digits = calloc(a->size + b->size, sizeof(char));
    result->size = a->size + b->size; // Max size we will need.

    for (int i = 0; i < a->size; i++) {
        for (int j = 0; j < b->size; j++) {
            int index = i + j;
            result->digits[index] += a->digits[i] * b->digits[j];
            result->digits[index + 1] += result->digits[index] / 10;
            result->digits[index] %= 10;
        }
    }
    
    // Trim any leading zeros.
    while (result->size > 1 && result->digits[result->size - 1] == 0) {
        result->size--;
    }
}

int main() {
    BigNum a, b, sum, product;

    bignum_init(&a, "12345678901234567890");
    bignum_init(&b, "98765432109876543210");

    bignum_print(&a);
    bignum_print(&b);

    bignum_add(&sum, &a, &b);
    bignum_print(&sum);
    bignum_free(&sum);

    bignum_multiply(&product, &a, &b);
    bignum_print(&product);
    bignum_free(&product);

    int cmp_result = bignum_compare(&a, &b);
    if (cmp_result > 0) {
        printf("a is greater than b\n");
    } else if (cmp_result < 0) {
        printf("a is less than b\n");
    } else {
        printf("a is equal to b\n");
    }

    bignum_free(&a);
    bignum_free(&b);

    return 0;
}
