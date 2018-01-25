#ifndef  __JSON_H
#define  __JSON_H

#include "cJSON.h"

#ifndef DWORD
#define DWORD unsigned int
#endif

#ifndef WORD
#define WORD unsigned short
#endif

#ifndef BYTE
#define BYTE unsigned char
#endif

typedef void (*FCB)(void * pDest, void *pSrc);
typedef struct
{
    BYTE itemtype;
    char itemkey[32];
    WORD length;
    WORD offset;
    BYTE arrayType;
    WORD arrayNum;
    void *pSubJSONStructItem;
    WORD subItemNum;
    FCB  callback;
} JSONStructItem_T,*pJSONStructItem_T;

/* DBA返回错误码定义 */
#define COM_RC_OK                                     0
#define COM_RC_ERR_CODE_BEGIN                         1

#define COM_RC_JSON_MSG_HEAD_FORMAT_ERR    COM_RC_ERR_CODE_BEGIN+1    /* JSON码流消息头格式错误 */
#define COM_RC_JOSN_ENCODE_ERR             COM_RC_ERR_CODE_BEGIN+2    /* JSON编码失败 */
#define COM_RC_JSON_DECODE_ERR             COM_RC_ERR_CODE_BEGIN+3    /* JSON解码失败 */
#define COM_RC_JSON_CREATE_OBJ_ERR         COM_RC_ERR_CODE_BEGIN+4    /* 创建JSON对象失败 */
#define COM_RC_JSON_PRINT_ERR              COM_RC_ERR_CODE_BEGIN+5    /* 输出JSON码流失败 */
#define COM_RC_JSON_TYPE_INVALID           COM_RC_ERR_CODE_BEGIN+6    /* 无效的JSON类型 */
#define COM_RC_JSON_GET_OBJECT_ERR         COM_RC_ERR_CODE_BEGIN+7    /* 获取JSON对象失败 */
#define COM_RC_JSON_CREATE_INTARR_ERR      COM_RC_ERR_CODE_BEGIN+8    /* 创建整型数组对象失败 */
#define COM_RC_JSON_CREATE_STRINGARR_ERR   COM_RC_ERR_CODE_BEGIN+9    /* 创建字符串数组对象失败 */
#define COM_RC_JSON_GET_ARRAY_ERR          COM_RC_ERR_CODE_BEGIN+10    /* 获取数组对象失败 */
#define COM_RC_NULL_POINTER                COM_RC_ERR_CODE_BEGIN+11    /* 空指针 */


#define COM_NULLPOINTER_CHK_RTVALUE(pointer)\
{\
    if (NULL == (pointer))\
    {\
        printf("Pointer Is NULL!!!");\
        return COM_RC_NULL_POINTER;\
    }\
}
#define COM_NOK_CHK_RTCODE(retcode)\
{\
    DWORD mac_ret = retcode;\
    if (COM_RC_OK != mac_ret)\
    {\
        return mac_ret;\
    }\
}

/* public */
DWORD com_decode_json_msg(void *msg_body,JSONStructItem_T *jsonitem,WORD itemnum,BYTE *out_struct);
DWORD com_code_json_msg(JSONStructItem_T *jsonitem,WORD itemnum,void *in_struct,void *msg_body);
/* private */
DWORD com_code_json_proc(cJSON *root,JSONStructItem_T *jsonitem,WORD itemnum,BYTE *in_struct);
DWORD com_code_json_num_proc(cJSON *root,JSONStructItem_T jsonitem,BYTE *in_struct);
void com_json_tulip_free(BYTE *buf);
void com_set_json_hooks();
#endif


