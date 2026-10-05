#include <stdio.h>
#define PI 3.14
#define SQUARE(x) ((x)*(x))

int main(void)
{
    // This comment should be removed
    int number = 5;
    double area = PI * SQUARE(number); /* block comment */
    printf("Number = %d\n", number);
    printf("Area = %.2f\n", area);
    return 0;
}
