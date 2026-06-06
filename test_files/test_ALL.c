/*
  Compact assembler test: integers + floats + control flow
*/

int main() {

    // --------------------
    // Integer basics
    // --------------------
    int a, b, c, r;
    a = 4;
    b = 6;
    c = 3;

    r = a + b;      // 10
    r = r * c;      // 30
    r = r / 3;      // 10
    r = r - 2;      // 8
    printf("Int result expected 8: %d\n", r);

    // --------------------
    // If / else
    // --------------------
    if (a < b)
        r = 1;
    else
        r = 0;

    printf("If expected 1: %d\n", r);

    if (a > b)
        r = 100;
    else
        r = 200;

    printf("If-else expected 200: %d\n", r);

    // --------------------
    // Nested if
    // --------------------
    if (a < b)
        if (c < b)
            r = 7;
        else
            r = 9;
    else
        r = 11;

    printf("Nested if expected 7: %d\n", r);

    // --------------------
    // While loop
    // --------------------
    int i, sum;
    i = 0;
    sum = 0;

    while (i < 4) {
        sum = sum + i;
        i = i + 1;
    }

    printf("Loop expected 6: %d\n", sum);   // 0+1+2+3 = 6
    printf("Loop counter expected 4: %d\n", i);

    // --------------------
    // Floating point tests
    // --------------------
    float x, y, z;

    x = 2.5;
    y = 4.0;
    z = 1.5;

    float fr;

    fr = x + y;     // 6.5
    fr = fr - z;    // 5.0
    fr = fr * 2.0;  // 10.0
    fr = fr / 4.0;  // 2.5

    printf("Float expected 2.5: %f\n", fr);

    // --------------------
    // Float comparison + branch
    // --------------------
    if (fr > 2.0)
        fr = fr + 1.0;
    else
        fr = fr - 1.0;

    printf("Float branch expected 3.5: %f\n", fr);

    // --------------------
    // Mixed int → float
    // --------------------
    fr = a + 0.5;   // 4.5
    printf("Mixed expected 4.5: %f\n", fr);

    return 0;
}