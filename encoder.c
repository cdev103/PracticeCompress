#include <stdio.h>
#include <stdint.h>

int main()
{
    FILE *my_file;
    my_file = fopen("filename.txt", "rb");
    if (my_file == NULL)
    {
        printf("Error: filename.txt does not exist!\n");
        return 1;
    }
    FILE *out;
    out = fopen("output.bin", "wb");
    if (out == NULL)
    {
        printf("Error: Can't create output file!\n");
        fclose(my_file);
        return 1;
    }
    fseek(out, 4, SEEK_SET);

    int ch;
    uint32_t bitstream_buffer = 0;
    uint32_t total_bits = 0;
    int bit_count = 0;

    while ((ch = fgetc(my_file)) != EOF)
    {
        if (ch == 'a')
        {
            bitstream_buffer = (bitstream_buffer << 1) | 0;
            bit_count += 1;
            total_bits += 1;
        }
        else if (ch == 'b')
        {
            bitstream_buffer = (bitstream_buffer << 1) | 1;
            bit_count += 1;
            total_bits += 1;
        }
        else
        {
            printf("Error: File is invalid!\n");
            fclose(my_file);
            fclose(out);
            return -1;
        }
        if (bit_count == 8)
        {
            fputc(bitstream_buffer, out);
            bit_count = 0;
            bitstream_buffer = 0;
        }
    }
    if(bit_count > 0)
    {
        int zeros_count = 8 - bit_count;
        bitstream_buffer <<= zeros_count;
        fputc(bitstream_buffer, out);
    }
    fseek(out, 0, SEEK_SET);
    fwrite(&total_bits, 4, 1, out);
    printf("Compressed file output.bin has been created successfully!");
    fclose(my_file);
    fclose(out);
    return 0;
}