#include <stdio.h>

int main()
{
    FILE *my_file;
    my_file = fopen("filename.txt", "rb");
    if (my_file == NULL)
    {
        printf("Error: File is empty!\n");
        return -1;
    }

    int counts[26] = {0};
    int ch;

    while ((ch = fgetc(my_file)) != -1)
    {
        counts[ch - 97] += 1;
    };
    int ct = 0;
    while (ct != 25)
    {
        if (counts[ct] != 0)
        {
            printf("Count of letter %c is %d\n", 97 + ct, counts[ct]);
        };
        ct += 1;
    }

    fclose(my_file);
    return 0;
}