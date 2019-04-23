#include <stdio.h>
#include <stdlib.h>

#include "string.h"
#include "json_out.h"

#define JSON_BUF_SIZE 1024
static char m_json_buf[JSON_BUF_SIZE];

void example_struct_init(void);
const json_object_t* example_struct_get(void);


void print_ruller(size_t length, size_t indent)
{
    while (indent-- > 0)
        printf(" ");

    uint32_t pos = 0;
    while (length-- > 0)
    {
        if (pos%100 == 0 && pos != 0 && length > 1)
        {
            printf("%u", pos%1000);
            pos += 2;
            length -= 2;
        }
        else if (pos%10 == 0)
        {
            printf("%u", pos%100/10);
        }
        else
        {
            printf(".");
        }
        pos++;
    }
    printf("\n");
}

int main()
{
    example_struct_init();
    json_out_init(m_json_buf, JSON_BUF_SIZE);

    const json_object_t* my_struct = example_struct_get();
    json_out_serialize(my_struct);

    printf("JSON: (%d bytes)\n", (int)strlen(m_json_buf));
    print_ruller(strlen(m_json_buf), 1);
    printf("'%s'\n", m_json_buf);

    return EXIT_SUCCESS;
}
