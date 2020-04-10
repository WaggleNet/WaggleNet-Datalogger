// Constant flags
// Data type hinting
#define CFLAG_TYPE_MASK     0x07
#define CFLAG_TYPE_LOC      0
#define CFLAG_SIGNED_LOC    0
#define CFLAG_REAL_LOC      1
#define CFLAG_STR_LOC       2
// Field is writable
#define CFLAG_RW_LOC        6
// Variable length
#define CFLAG_VAR_LOC       7

#define VFLAG_CHANGED_LOC   0
#define VFLAG_ERROR_LOC     1

// Convenience variables for data type
#define DATA_TYPE_UINT      0x00
#define DATA_TYPE_INT       0x01
#define DATA_TYPE_BOOL      0x02
#define DATA_TYPE_FLOAT     0x03
#define DATA_TYPE_STRING    0x06
#define DATA_TYPE_BINARY    0x04
#define DATA_TYPE_OTHER     0x04
