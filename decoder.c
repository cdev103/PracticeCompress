#include <stdio.h>
#include <stdint.h>

int main()
{
    FILE *compressed_file;
    compressed_file = fopen("output.bin", "rb");
    if (compressed_file == NULL)
    {
        printf("Error: File does not exist!\n");
        return 1;
    }
    FILE *out;
    out = fopen("output.txt", "wb");
    if (out == NULL)
    {
        printf("Error: Can't create output file!\n");
        fclose(compressed_file);
        return 1;
    }
    int len = 0;
    fread(&len, 4, 1, compressed_file); // read the length header to know when the decoding is done
    int len_counter = 0;
    int ch = 0;
    while ((ch = fgetc(compressed_file)) != EOF)
    {
        unsigned char byte = (unsigned char)ch;
        for(int i = 7; i >= 0; i--) // i is the var used to shift the byte so that the first, second etc bits are at the last bit position each cycle of the loop
        {
            if (((byte >> i) & 1) == 0) //compare the last bit of the shifted byte against 1 to see if its 1 or 0
            {
                fputc('a', out);
                len_counter += 1;
            }
            else
            {
                fputc('b', out);
                len_counter += 1;
            }

            if (len_counter == len) // if length is hit, stop working and give user info that job is done
            {
                printf("extraction done!");
                fclose(compressed_file);
            fclose(out);
            return 0;
            }
        }
    }
    fclose(compressed_file);
    fclose(out);
    return 0;
}