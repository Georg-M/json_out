#include <stdio.h>
#include <stdlib.h>

#include "string.h"
#include "setjmp.h"
#include "json_out.h"

#define JSON_LEVELS_MAX 10  /* limit for recursion depth */

#define ENOBUFS       105  /* see: man errno */
#define ETOOMANYREFS  109

#define ARRAY_SIZE(arr) (sizeof(arr)/sizeof((arr)[0]))

/* static data */
static const char* m_buf_start;
static const char* m_buf_end;
static char* m_buf_ptr;
size_t m_buf_size;
static jmp_buf m_jumper;

static int debug_trace_level;
static json_type_t debug_trace_types[JSON_LEVELS_MAX];
static size_t debug_trace_lengths[JSON_LEVELS_MAX];
static const char* debug_trace_keys[JSON_LEVELS_MAX];
static int debug_trace_indexes[JSON_LEVELS_MAX];


void json_out_init(const char* buf, size_t buf_size)
{
    m_buf_start = buf;
    m_buf_end = buf + buf_size;
    m_buf_ptr = (char*)buf;
    m_buf_size = buf_size;

    for (int i=0; i<JSON_LEVELS_MAX; i++)
    {
        debug_trace_types[i] = JSON_UNDEFINED;
        debug_trace_lengths[i] = -1;
        debug_trace_indexes[i] = -1;
        debug_trace_keys[i] = NULL;
    }
}

static void json_trace_print(void)
{
    printf("JSON tree backtrace:\n");
    for (int i=0; i<=debug_trace_level; i++)
    {
        if (debug_trace_indexes[i] == -1)
            break;
        printf("Level: %d, ", i);
        printf("Index: %d, ", debug_trace_indexes[i]);
        if (debug_trace_types[i] == JSON_OBJECT || (i < JSON_LEVELS_MAX - 1 && debug_trace_indexes[i + 1] != -1))
            printf("Key: \"%s\"", (debug_trace_keys[i] == NULL) ? "---" : debug_trace_keys[i]);
        else
        {
            const char* type;
            switch (debug_trace_types[i])
            {
            case JSON_NUMBER:   type = "Number";    break;
            case JSON_BOOLEAN:  type = "Boolean";   break;
            case JSON_STRING:   type = "String";    break;
            case JSON_OBJECT:   type = "Object";    break;
            case JSON_UNDEFINED:
            case JSON_NULL:
            default:            type = "???";       break;
            }
            /* Note: length and type can be printed for previous leaf, not for current one */
            printf("Out of %d members of type: %s", (int)debug_trace_lengths[i], type);
        }
        printf("\n");
    }
    printf("\n");
}

static void print_token_chr(char chr)
{
    if (m_buf_ptr < m_buf_end - sizeof(char))
    {
        snprintf(m_buf_ptr++, sizeof(char) + 1, "%c", chr);
    } else {
        longjmp(m_jumper, ENOBUFS);
    }
}

static void print_token_str(const char* str, const char* left, const char* right)
{
    size_t buf_size = m_buf_end - m_buf_ptr;
    int would_be_written = snprintf(m_buf_ptr, buf_size, "%s%s%s", left, str, right);

    if (would_be_written < buf_size)
    {
        m_buf_ptr += would_be_written;
    } else {
        m_buf_ptr += buf_size - 1;
        longjmp(m_jumper, ENOBUFS);
    }
}

/* recursive function */
static void json_out(const json_object_t* obj, int level)
{
    json_type_t type    = obj->type;
    size_t length       = obj->length;
    void** data         = &((json_object_t*)obj)->data;

    debug_trace_level = level;
    debug_trace_types[level] = type;
    debug_trace_lengths[level] = length;

    if (type == JSON_OBJECT)
        print_token_chr('{');
    else if (length != 1)
        print_token_chr('[');

    for (int i=0; i<length; i++)
    {
        static char tmp_buf[40];
        debug_trace_indexes[level] = i;
        const json_object_t* member = *((json_object_t**)data + i);

        switch ((member != NULL) ? type : JSON_NULL)
        {
        case JSON_OBJECT:
            debug_trace_keys[level] = member->key;
            print_token_str(member->key, "\"", "\"");
            print_token_chr(':');
            /* recursion */
            if (level < JSON_LEVELS_MAX - 1)
                json_out(member, level + 1);
            else
                longjmp(m_jumper, ETOOMANYREFS);
            debug_trace_level = level;
            break;

        case JSON_STRING:
            print_token_str((const json_string_t)member, "\"", "\"");
            break;

        case JSON_BOOLEAN:
            print_token_str((*((json_boolean_t*)member)) ? "true" : "false", "", "");
            break;

        case JSON_NUMBER:
            snprintf(tmp_buf, ARRAY_SIZE(tmp_buf), "%d", *((json_number_t*)member));
            print_token_str((const char*)tmp_buf, "", "");
            break;

        case JSON_NULL:
            print_token_str("null", "", "");
            break;

        case JSON_UNDEFINED:
            print_token_str("ERROR: Undefined JSON type", "<", ">");
            break;

        default:
            snprintf(tmp_buf, ARRAY_SIZE(tmp_buf), "ERROR: Unknown JSON type: %d", type);
            print_token_str((const char*)tmp_buf, "<", ">");
            break;
        }

        if (i < length - 1)
            print_token_chr(',');
    }

    if (type == JSON_OBJECT)
        print_token_chr('}');
    else if (length != 1)
        print_token_chr(']');
}

int json_out_serialize(const json_object_t* root)
{
    int rc = EXIT_SUCCESS;

    switch (setjmp(m_jumper))
    {
    case 0: /* try */
        json_out(root, 0);
        break;

    case ENOBUFS: /* catch(ENOBUFS) */
        rc = ENOBUFS;
        printf("Exception ENOBUFS: Too long JSON string. Buffer is limited by %d bytes.\n", (int)m_buf_size);
        break;

    case ETOOMANYREFS: /* catch(ETOOMANYREFS) */
        rc = ETOOMANYREFS;
        printf("Exception ETOOMANYREFS: Too many recursion levels. Limit is %d.\n", JSON_LEVELS_MAX);
        break;

    default: /* catch(*) */
        rc = EXIT_FAILURE;
        printf("Exception ???: JSON serialization failed.\n");
        break;
    }
    /* finally */

    if (rc != EXIT_SUCCESS)
        json_trace_print();

    return rc;
}
