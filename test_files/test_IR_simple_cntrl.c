/*
  Example for while with nested if
*/
int main() {
    int a,b,c;

    a=1;
    b=3;
    c=0;

    while (a<b) {
        if (b>c) {
            a=a+1; c=c+3;
        }
        else
            b=b-1;
    }
    /* gcc: warning implicit declaration, executable generated and correct */
    printf("Expected 2: %d\n", b); /* Expected: 2 */
}
