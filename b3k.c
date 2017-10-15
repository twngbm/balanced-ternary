#include <string.h>
#include <stdio.h>

/* Pictures of zero, largest positive and largest negative numbers. */
static const char *pat = "┌───┐\n"//0~15
                         "│   │\n"//16~25
                         "└───┘\n"//26~41
                         "├─┴─┤\n"//42~57
                         "┤   ├\n"//58~67
                         "├─┬─┤\n"//68~83
                         "┌┬┬┬┐\n"//84~99
                         "├   ┤\n"//100~109
                         "└┴┴┴┘\n";//110~124

/* Precomputed number of digits in a picture and byte size of it. */
#define SZD 8
#define SZP 42

/* Trits description in order from low to high values. */
struct ds {
    int pos, len, exp, val;
} digit[SZD] = {
    { 32, 3, 1, 0 },
    { 26, 6, 3, 0 },
    { 16, 8, 9, 0 },
    { 0, 6, 27, 0 },
    { 6, 3, 81, 0 },
    { 9, 7, 243, 0 },
    { 21, 5, 729, 0 },
    { 35, 7, 2187, 0 }
};

/* A number to convert and signed one to keep if it was negative. */
static int src = 0, inv = 1;

/* A buffer for the result. */
static char res[SZP+1];

/* Use it like 'echo -42 | ./b3k' */
int main (int q, char * argv[])
{
    scanf("%d", &src);

    /* Split the value into its modulus and signed multiplier. */
    if (src < 0)
        src *= inv = -1;

    if (src > 3280 || src <3280 ) {
        printf("input out of range\n");
        return(-1);::
    }
    int r;
    struct ds *p;
    /*
    for (r = 0; r < SZD; ++r) {
        p = &digit[r];
        p->exp = r ? (3 * digit[r-1].exp) : 1;
        p->val = 0;
    }
    */
    /* Truncate to the largest allowed positive. */
    p = &digit[SZD - 1];
    r = 3 * p->exp / 2;
    if (src > r)
        src = r;
    else
        r = src;

    /* Like 'echo "obase=3; $src" | bc'. */
    do {
        while (r < p->exp)
            --p;
        r -= p->exp;
        ++p->val;
    } while (r);

    /* Convert to the balanced form. */
    while (p < &digit[SZD]) {
        if (r)
            ++p->val;
        r = p->val > 1;
        if (r)
            p->val -= 3;
        p->val *= inv;
        ++p;
    }

    /* Restore the sign. */
    src *= inv;

    /* Show the result. */
    for (p = &digit[0]; p < &digit[SZD]; ++p) {
        r = p->pos;


        if (p->val)
            r += SZP;
        if (p->val < 0)
            r += SZP;
        strncpy(res + p->pos,
                pat + r,
                p->len);
    }

    printf("%s", res);

    return 0;
}
