/* *****************************************************************************************
*******************************************************************************************/

#ifndef  __JSON_TEST_H
#define  __JSON_TEST_H

/******************************************************************************************/
#ifndef DWORD
#define DWORD unsigned int
#endif

#ifndef WORD
#define WORD unsigned short
#endif

#ifndef BYTE
#define BYTE unsigned char
#endif

typedef struct
{
    char name[64];
    char addr[128];
    BYTE age;
} Json_type1_T;


typedef struct
{
    char name[64];
    char addr[128];
    BYTE age;
    DWORD money;
} Json_type7_T;

typedef struct
{
    char str[16];
    DWORD num;
} Json_type_cfb_T;


typedef struct
{
    char name2[64];
    char addr2[128];
    BYTE age2;
    Json_type1_T jsonType1;
} Json_type2_T;

typedef struct
{
    char value[64];
    WORD onclick;
} Json_operate_T;

typedef struct
{
    WORD numarray[3];
    char strarray[4][128];
    Json_operate_T operate[3];
} Json_type_Array_T;



typedef struct
{
    char bb1[64];
    WORD bb2;
} Json_operate_bb_T;


typedef struct
{
    char name[2][128];
    Json_operate_T operate[3];
    Json_operate_bb_T bb;
} Json_operate_aa_T;

typedef struct
{
    WORD index[3];
    Json_operate_aa_T aa;
} Json_type_Complicate_T;

typedef struct
{
    WORD numarray[3];
    char strarray[4][128];
} Json_type_Code_Arr_T;


typedef struct
{
    DWORD num2string;
    char  string2num[5];
} Json_type_Code_cb;


#endif
