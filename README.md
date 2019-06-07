# Simple JSON serialization example

Recursive conversion of especially prepared C data structure to JSON-formatted string.  
It can be used in embedded or IoT devices to send data with predefined hierarchy or schema.  
This implementation has some minor restrictions in comparison with standard JSON.  

## Tree

Structure tree can be predefined or dynamically built by client application.  
Elements in data structure tree are linked in descending direction only.

## Data

User data has to be allocated by application with appropriate lifetime.  
Object keys, values and user data can be predefined or dynamically assigned in runtime.  
Each element consists of `length` (0..N) pointers to `data` of type `type`.  
Root and interim nodes has pointer(s) to descendant nodes and finally to leafs.  
Leaf elements has pointer(s) to user data.  
Member with its pointer = NULL will be printed as `null`.  
If element has zero child members, it will be printed as `{}` or `[]`.

## Data Types

Root and all interim nodes must be of type `Object`.  
Only leaf elements can be of `Number`, `String` or `Boolean` type.  
Array type can be defined only for leaf elements by field `length`.

### Use predefined types to declare your data:

 - `json_number_t`     - single number (signed int32_t)
 - `json_boolean_t`    - single boolean
 - `json_string_t`     - a pointer to null-terminated string
 - `json_object_t`     - the root and all interim nodes

## Usage

User application has to have predefined data structure or build it in runtime.  
It has to allocate buffer for resulting JSON string and call `json_out_init()`.  
Then it can call `json_out_serialize()` and get JSON string in the buffer.  
JSON structure backtrace will be printed in case of output buffer overflow or too deep recursion level.  

## Files

`src/json_out.h` - JSON serialization header file.  
`src/json_out.c` - JSON serialization source file.  
`src/example_main.c` - An example of JSON serialization usage.  
`src/example_data.c` - An example of predefined data structure.  
`doc/example_result.txt` - Result of the example serialization.
