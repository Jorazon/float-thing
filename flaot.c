#include <stdio.h>

typedef union {
    struct {
        unsigned int frac:23;
        unsigned int expt:8;
        unsigned int sign:1;
    };
    float value;
} float_union;

typedef struct {
    unsigned long long int integer;
    unsigned long long int decimal;
} float_llu;

float_llu parse(float value) {
    float_union bruh;
    bruh.value = value;

    int sign = bruh.sign?-1:1;
    int expt = bruh.expt - 127;
    unsigned long long int frac = bruh.frac | 1<<23;

    // denormalize numbers < 1
    if (expt < 0) frac >>= -expt;

    unsigned long long int sum = 0;
    unsigned long long int e = 10;

    for (int i = 0; i < 18; i++) {
        int bit = ((frac >> ((23 - 1) - i)) & 1);
        int two_pow_i = (2 << i);
        //printf("%llu %d %llu\n", sum, bit, e / two_pow_i);
        if (bit) sum += e / two_pow_i;
        sum *= 10;
        e *= 10;
    }

    return (float_llu){sign * frac * (2 << expt) / (2 << 23), sum};
}

void main(void) {
    float_llu a = parse(1.1);
    printf("%llu.%llu\n", a.integer, a.decimal);
}
