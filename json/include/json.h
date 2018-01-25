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

/* DBA���ش����붨�� */
#define COM_RC_OK                                     0
#define COM_RC_ERR_CODE_BEGIN                         1

#define COM_RC_JSON_MSG_HEAD_FORMAT_ERR    COM_RC_ERR_CODE_BEGIN+1    /* JSON������Ϣͷ��ʽ���� */
#define COM_RC_JOSN_ENCODE_ERR             COM_RC_ERR_CODE_BEGIN+2    /* JSON����ʧ�� */
#define COM_RC_JSON_DECODE_ERR             COM_RC_ERR_CODE_BEGIN+3    /* JSON����ʧ�� */
#define COM_RC_JSON_CREATE_OBJ_ERR         COM_RC_ERR_CODE_BEGIN+4    /* ����JSON����ʧ�� */
#define COM_RC_JSON_PRINT_ERR              COM_RC_ERR_CODE_BEGIN+5    /* ���JSON����ʧ�� */
#define COM_RC_JSON_TYPE_INVALID           COM_RC_ERR_CODE_BEGIN+6    /* ��Ч��JSON���� */
#define COM_RC_JSON_GET_OBJECT_ERR         COM_RC_ERR_CODE_BEGIN+7    /* ��ȡJSON����ʧ�� */
#define COM_RC_JSON_CREATE_INTARR_ERR      COM_RC_ERR_CODE_BEGIN+8    /* ���������������ʧ�� */
#define COM_RC_JSON_CREATE_STRINGARR_ERR   COM_RC_ERR_CODE_BEGIN+9    /* �����ַ����������ʧ�� */
#define COM_RC_JSON_GET_ARRAY_ERR          COM_RC_ERR_CODE_BEGIN+10    /* ��ȡ�������ʧ�� */
#define COM_RC_NULL_POINTER                COM_RC_ERR_CODE_BEGIN+11    /* ��ָ�� */


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


