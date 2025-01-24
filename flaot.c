#include <stdio.h>
#include <stdint.h>

typedef union {
    struct {
        uint32_t frac:23;
        uint32_t expt:8;
        uint32_t sign:1;
    };
    float value;
} float_union;

typedef struct {
    uint64_t integer;
    uint64_t decimal;
} float_llu;

float_llu parse(float value) {
    float_union unified;
    unified.value = value;

    int sign = unified.sign?-1:1;
    int expt = unified.expt - 127;
    int frac = unified.frac | 1<<23;
    
    printf("%llu %lld %llu\n", sign, expt, frac);
    printf("%llu %lld %llu\n", sign, expt, frac);

    // denormalize numbers < 1
    if (expt < 0) frac >>= -expt;

    uint64_t sum = 0;
    uint64_t e = 10;

    // >18 causes integer overflow, results in imprecision
    for (int i = 0; i < 18; i++) {
        if ((frac << i) == 0) break;
        char bit = ((frac >> ((23 - 1) - i)) & 1);
        uint32_t two_pow_i = (2 << i);
        printf("%llu %d %llu\n", sum, bit, e / two_pow_i);
        if (bit) sum += e / two_pow_i;
        sum *= 10;
        e *= 10;
    }

    return (float_llu){sign * frac * (2 << expt) / (2 << 23), sum};
}

void main(void) {
    int x = 0b00111110101010101010101010000000;
    float_llu a = parse(*(float*)&x);
    printf("%llu.%llu\n", a.integer, a.decimal);
    //0.333332061767578125
}
