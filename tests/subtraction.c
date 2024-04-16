int printf(string fmt, ...);

float sub(float a, int b)
{
    return a - b;
}

int main()
{
    printf("%f\n", sub(7.0, 3));
    return 0;
}