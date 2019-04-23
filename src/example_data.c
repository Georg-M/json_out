#include <stdio.h>
#include <stdlib.h>

#include "json_out.h"

/**
 * An example how to build data structure for `json_out`.
 *
 * See output of this example in file `../doc/example_result.txt`.
 *
 * Objects with 1 child member can be initialized during its declaration.
 * Objects with more children should be declared with C unions, and
 * this mean that we need to initialize them in runtime.
 */

/* Schema version is used to control firmware versions in multiple peer devices */
#define SCHEMA_VER 1
#define SCHEMA_VER_MAJ 1
#define SCHEMA_VER_MIN 0

/* Filler is used to map data beginning address in two members of C union */
#define FILLER  char placeholder[offsetof(json_object_t, data)]

/*************** An example of structure declaration and initialization **************************/
/**
 *  Root
 *  OBJECT with many OBJECTs
 *  { <object>, <object>, ... }
 */
static union {
    json_object_t meta;
    struct {
        FILLER;
        json_object_t* members[10];
    } data;
} m_root = {
    .meta.key       = "json",
    .meta.type      = JSON_OBJECT,
    .meta.length    = 10,
};

/**
 *  OBJECT with NUMBER
 *  "<key>": <number>
 */
static json_number_t schema_version = SCHEMA_VER;
static json_object_t m_schema_version_obj = {
    .key        = "schema-version",
    .type       = JSON_NUMBER,
    .length     = 1,
    .data       = &schema_version,
};


/**
 *  OBJECT with two NUMBERs
 *  "<key>": [ <number>, <number> ]
 */
static json_number_t schema_ver_maj = SCHEMA_VER_MAJ;
static json_number_t schema_ver_min = SCHEMA_VER_MIN;
union {
    json_object_t meta;
    struct {
        FILLER;
        json_number_t* members[2];
    } data;
} m_schema_ver_minmaj_obj = {
    .meta.key        = "maj-min-version",
    .meta.type       = JSON_NUMBER,
    .meta.length     = 2,
};

/**
 *  OBJECT with STRING
 *  "<key>": "<string>"
 */
static json_object_t m_example_string_obj = {
    .key        = "test",
    .type       = JSON_STRING,
    .length     = 1,
    .data       = "Hello JSON!",
};

/**
 *  OBJECT with two STRINGs
 *  "<key>": [ "<string>", "<string>" ]
 */
static union {
    json_object_t meta;
    struct {
        FILLER;
        json_string_t members[2];
    } data;
} m_example_two_strings_obj = {
    .meta.key        = "an-example",
    .meta.type       = JSON_STRING,
    .meta.length     = 2,
};

/**
 * A group of little objects with one number, boolean or string leaf data member per each.
 * Will be used as child members in examples below.
 */
static json_number_t num1 = 126;        static json_object_t m_num1_obj = {"number-1", JSON_NUMBER, 1, &num1};  // used in m_obj_obj
static json_number_t num2 = 0;          static json_object_t m_num2_obj = {"number-2", JSON_NUMBER, 1, &num2};  // used in m_obj_4objs
static json_number_t num3 = 7480;       static json_object_t m_num3_obj = {"number-3", JSON_NUMBER, 1, &num3};
static json_number_t num4 = 49;         static json_object_t m_num4_obj = {"number-4", JSON_NUMBER, 1, &num4};
static json_number_t num5 = 876857962;  static json_object_t m_num5_obj = {"number-5", JSON_NUMBER, 1, &num5};
static json_number_t num6 = -53;        static json_object_t m_num6_obj = {"number-6", JSON_NUMBER, 1, &num6};

                                        static json_object_t m_str1_obj = {"string-1", JSON_STRING, 1, "test2 OK"};  // used in m_obj_4objs
                                        static json_object_t m_str2_obj = {"string-2", JSON_STRING, 1, ""        };  // used in m_nested_5
                                        static json_object_t m_str3_obj = {"string-3", JSON_STRING, 1, "    "    };
                                        static json_object_t m_str4_obj = {"string-4", JSON_STRING, 1,  NULL     };
                                        static json_object_t m_str5_obj = {"string-5", JSON_STRING, 1, "vid1.mp3"};
                                        static json_object_t m_str6_obj = {"string-6", JSON_STRING, 1, "ERR (3)" };

static json_boolean_t bool1 = true;     static json_object_t m_bool1_obj = {"bool-1", JSON_BOOLEAN, 1, &bool1};  // used in m_obj_4objs
static json_boolean_t bool2 = false;    static json_object_t m_bool2_obj = {"bool-2", JSON_BOOLEAN, 1, &bool2};
static json_boolean_t bool3 = !5==6;    static json_object_t m_bool3_obj = {"bool-3", JSON_BOOLEAN, 1, &bool3};
static json_boolean_t bool4 = 0;        static json_object_t m_bool4_obj = {"bool-4", JSON_BOOLEAN, 1, &bool4};
static json_boolean_t bool5 = 2+2;      static json_object_t m_bool5_obj = {"bool-5", JSON_BOOLEAN, 1, &bool5};
static json_boolean_t bool6 = 1;        static json_object_t m_bool6_obj = {"bool-6", JSON_BOOLEAN, 1, &bool6};

                                        static json_object_t m_null1_obj =   {"null-000", JSON_BOOLEAN, 1, NULL};  // used in m_obj_dataless_null
                                        static json_object_t m_0_membs_obj = {"no-child", JSON_BOOLEAN, 0, NULL};  // used in m_obj_dataless_null

void unused_vars_anti_warning(void)
{
    (void)m_num1_obj;  (void)m_num2_obj;  (void)m_num3_obj;  (void)m_num4_obj;  (void)m_num5_obj;  (void)m_num6_obj;
    (void)m_str1_obj;  (void)m_str2_obj;  (void)m_str3_obj;  (void)m_str4_obj;  (void)m_str5_obj;  (void)m_str6_obj;
    (void)m_bool1_obj; (void)m_bool2_obj; (void)m_bool3_obj; (void)m_bool4_obj; (void)m_bool5_obj; (void)m_bool6_obj;
    (void)m_null1_obj; (void)m_0_membs_obj;
}

/**
 *  OBJECT with OBJECT
 *  "<key>": { "<key>": "<value>" }
 */
static json_object_t m_obj_obj = {"obj-with-obj", JSON_OBJECT, 1};  // field `data` is initialized below


/**
 *  OBJECT with four OBJECTs
 *  "<key>": { "<key>": <number>, "<key>": <boolean>, "<key>": null, "<key>": "<string>" }
 *  Note that types of grandchildren are different.
 */
static union {json_object_t meta; struct {FILLER; void* members[4];} data;} m_obj_4objs = {
    .meta.key        = "obj-with-4-objs",
    .meta.type       = JSON_OBJECT,
    .meta.length     = 4,
};

/**
 *  OBJECT without children/value
 *  "<key>": {}
 */
static json_object_t m_obj_childless = {"obj-childless", JSON_OBJECT, 0};  // no child objects to initialize as members

/**
 *  OBJECT with OBJECT without data children and OBJECT with `null` child member
 *  "<key>": { "<key>": [], "<key>": null }
 */
static union {json_object_t meta; struct {FILLER; void* members[2];} data;} m_obj_dataless_null = {
    .meta.key        = "obj-with-dataless-and-null-objs",
    .meta.type       = JSON_OBJECT,
    .meta.length     = 2,
};

/**
 *  OBJECT with OBJECT with OBJECT with OBJECT with OBJECT with null-string
 *  "<key>": { "<key>": { "<key>": { "<key>": { "<key>": <value> }}}}
 */
static json_object_t m_nested_4 = {"nested-4", JSON_OBJECT, 1, &m_str2_obj};
static json_object_t m_nested_3 = {"nested-3", JSON_OBJECT, 1, &m_nested_4};
static json_object_t m_nested_2 = {"nested-2", JSON_OBJECT, 1, &m_nested_3};
static json_object_t m_nested_1 = {"nested-1", JSON_OBJECT, 1, &m_nested_2};

/**
 *  OBJECT with all remaining example OBJECTs
 *  "<key>": { "<key>": <value>,... }
 */
static union {json_object_t meta; struct {FILLER; void* members[13];} data;} m_all_remained = {
    .meta.key        = "obj-with-all-remained-objs",
    .meta.type       = JSON_OBJECT,
    .meta.length     = 13,
};


void example_struct_init(void)
{
    /* Root */
    m_root.data.members[0]  = &m_schema_version_obj;            /* 1 */  /* Members count and length of data array must */
    m_root.data.members[1]  = &m_schema_ver_minmaj_obj.meta;    /* 2 */  /* be appropriately updated in `m_root` object. */
    m_root.data.members[2]  = &m_example_string_obj;            /* 3 */  /* Members' index also must be updated after */
    m_root.data.members[3]  = &m_example_two_strings_obj.meta;  /* 4 */  /* in case of copy-pasting (see to the left). */
    m_root.data.members[4]  = &m_obj_obj;                       /* 5 */
    m_root.data.members[5]  = &m_obj_4objs.meta;                /* 6 */
    m_root.data.members[6]  = &m_obj_childless;                 /* 7 */
    m_root.data.members[7]  = &m_obj_dataless_null.meta;        /* 8 */
    m_root.data.members[8]  = &m_nested_1;                      /* 9 */
    m_root.data.members[9]  = &m_all_remained.meta;             /* 10 */

    /* Object with key "maj-min-version" and 2 numbers in value */
    m_schema_ver_minmaj_obj.data.members[0] = &schema_ver_maj;
    m_schema_ver_minmaj_obj.data.members[1] = &schema_ver_min;

    /* Object with key "an-example" and 2 strings in value */
    m_example_two_strings_obj.data.members[0]   = (json_string_t)"example-STR1";
    m_example_two_strings_obj.data.members[1]   = (json_string_t)"example-STR2";

    m_obj_obj.data = &m_num1_obj;

    m_obj_4objs.data.members[0] = &m_num2_obj;
    m_obj_4objs.data.members[1] = &m_bool1_obj;
    m_obj_4objs.data.members[2] = NULL;
    m_obj_4objs.data.members[3] = &m_str1_obj;

    m_obj_dataless_null.data.members[0] = &m_0_membs_obj;
    m_obj_dataless_null.data.members[1] = &m_null1_obj;

    m_all_remained.data.members[0] = &m_num3_obj;
    m_all_remained.data.members[1] = &m_num4_obj;
    m_all_remained.data.members[2] = &m_num5_obj;
    m_all_remained.data.members[3] = &m_num6_obj;
    m_all_remained.data.members[4] = &m_str3_obj;
    m_all_remained.data.members[5] = &m_str4_obj;
    m_all_remained.data.members[6] = &m_str5_obj;
    m_all_remained.data.members[7] = &m_str6_obj;
    m_all_remained.data.members[8] = &m_bool2_obj;
    m_all_remained.data.members[9] = &m_bool3_obj;
    m_all_remained.data.members[10] = &m_bool4_obj;
    m_all_remained.data.members[11] = &m_bool5_obj;
    m_all_remained.data.members[12] = &m_bool6_obj;
}

const json_object_t* example_struct_get(void)
{
    return &m_root.meta;
}
