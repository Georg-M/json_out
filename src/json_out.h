#ifndef _JSON_OUT_
#define _JSON_OUT_

/**
 * Simple JSON serialization program.
 *
 * Recursive conversion of especially prepared C data structure(s) to
 * JSON-formatted string. It can be used for embedded or IoT devices
 * to send data with predefined rigid hierarchy or schema.
 *
 * **Tree**
 * Structure tree can be predefined or dynamically built in runtime.
 * Branches and elements of tree are linked in descending direction only.
 *
 * **Data**
 * User data has to be allocated by application with appropriate lifetime.
 * Each element consists of `length` (0..N) pointers to `data` of type `type`.
 * Root and interim nodes has pointer(s) to descendant nodes and finally to leafs.
 * Leaf elements has pointer(s) to user data.
 * Object keys can be defined in structure, or they can be assigned in runtime.
 * Member with its pointer = NULL will be printed as `null`.
 * If element hasn't child members, it will be printed as {} or [].
 *
 * **Data Types**
 * Root and all interim nodes must be of type Object.
 * Only leaf elements can be of Number, String or Boolean type.
 * Array type can be defined only for leaf elements by field `length`.
 *
 * Use predefined types to declare your data:
 *  - `json_number_t`     single number (signed int32_t)
 *  - `json_boolean_t`    single boolean
 *  - `json_string_t`     a pointer to null-terminated string
 *  - `json_object_t`     the root and all interim nodes
 *
 * **Usage**
 * User application has to have predefined data structure or build it in runtime.
 * It has to allocate buffer for resulting JSON string and call `json_out_init()`.
 * Then it can call `json_out_serialize()` and get JSON string in the buffer.
 *
 * **Example**
 * See example of predefined data structure and its serialization in
 * accompanying files `example_main.c` and `example_data.c`.
 */

#include "stdint.h"
#include "stdbool.h"
#include "stddef.h"

typedef enum {
    JSON_UNDEFINED,
    JSON_NULL,
    JSON_NUMBER,
    JSON_BOOLEAN,
    JSON_STRING,
    JSON_OBJECT,
} json_type_t;

typedef struct {
    char*         key;
    json_type_t   type;
    uint16_t      length;
    void*         data;
} json_object_t;

typedef int32_t     json_number_t;
typedef bool        json_boolean_t;
typedef const char* json_string_t;


void json_out_init(const char* buf, size_t buf_size);
int json_out_serialize(const json_object_t* root);

#endif /* _JSON_OUT_ */
