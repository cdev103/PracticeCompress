#include <stdio.h>
#include <stdint.h>

int write_bit(int value, uint8_t *bitstream_buffer, int *bit_count, uint32_t *total_bits);
int check_char(char ch, uint8_t *bitstream_buffer, int *bit_count, uint32_t *total_bits);
int check_full_byte(uint8_t *bitstream_buffer, int *bit_count, FILE *out);

int main()
{
    FILE *my_file;
    char input_filename[] = "filename.txt";
    my_file = fopen(input_filename, "rb");
    if (my_file == NULL)
    {
        printf("Error: %s does not exist!\n", input_filename);
        return -1;
    }

    FILE *out;
    char output_filename[] = "output.bin";
    out = fopen(output_filename, "wb");
    if (out == NULL)
    {
        printf("Error: Can't create output file!\n");
        fclose(my_file);
        return -1;
    }

    int header_length = 4;
    fseek(out, header_length, SEEK_SET);

    int ch;
    uint8_t bitstream_buffer = 0;
    uint32_t total_bits = 0;
    int bit_count = 0;

    while ((ch = fgetc(my_file)) != EOF)
    {
        check_char(ch, &bitstream_buffer, &bit_count, &total_bits);
        check_full_byte(&bitstream_buffer, &bit_count, out);
    }
    if(bit_count > 0)
    {
        int zeros_count = 8 - bit_count;
        bitstream_buffer <<= zeros_count;
        fputc(bitstream_buffer, out);
    }
    fseek(out, 0, SEEK_SET);
    fwrite(&total_bits, header_length, 1, out);
    printf("Compressed file output.bin has been created successfully!");
    fclose(my_file);
    fclose(out);
    return 0;
}
int write_bit(int value, uint8_t *bitstream_buffer, int *bit_count, uint32_t *total_bits)
{
    (*bitstream_buffer) = ((*bitstream_buffer) << 1) | value;
    (*bit_count)++;
    (*total_bits)++;
    return 0;
}
int check_char(char ch, uint8_t *bitstream_buffer, int *bit_count, uint32_t *total_bits)
{
    if (ch == 'a')
    {
        int value = 0;
        write_bit(value, bitstream_buffer, bit_count, total_bits);
        return 0;
    }
    else if (ch == 'b')
    {
        int value = 1;
        write_bit(value, bitstream_buffer, bit_count, total_bits);
        return 0;
    }
    else
    {
        printf("Error: File is invalid!\n");
        return -1;
    }
}
int check_full_byte(uint8_t *bitstream_buffer, int *bit_count, FILE *out)
{
    if (*bit_count == 8)
    {    
        fputc(*bitstream_buffer, out);     
        *bitstream_buffer = 0;
        *bit_count = 0;
    }
    return 0;
}