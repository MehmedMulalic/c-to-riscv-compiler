int main() {
    int a = 2;
    int b = 4;
    int x = 10;
    int y = 20;
    int z;

    // arithmetic + assignment
    z = x + y * a - b / 2;

    // unary operations
    x++;
    --y;

    // if / else
    if (z > 50) {
        z = z - 10;
    } else {
        z = z + 10;
    }

    // while loop
    while (x < y) {
        x = x + 1;
    }

    // do-while loop
    do {
        y = y - 1;
    } while (y > 0);

    // for loop (basic)
    for (x = 0; x < 5; x = x + 1) {
        z = z + x;
    }

    // function calls
    printf("z = %d\n", z);
    printf("no args call");

    // nested expressions + function call inside
    z = printf("value: %d\n", x + y);

    // logical operators
    if (x > 0 && y > 0 || z == 0) {
        z = 1;
    }

    return z;
}
