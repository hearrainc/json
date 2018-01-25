/*******************************************************************************
 * 文件名称： json.c
 * 文件标识：
 * 内容摘要： 提供json格式的消息编解码
 *******************************************************************************/
#include "json.h"


BYTE *com_json_tulip_malloc(WORD32 size)
{
    return OSS_Malloc(size);
}

void com_json_tulip_free(BYTE *buf)
{
    OSS_Free(buf);
    return ;
}

/**********************************************************************
* 函数名称：com_get_json_object
* 功能描述：从JSON码流中获取子对象
* 修改日期     版本号     修改人      修改内容
*
************************************************************************/
DWORD com_get_json_object(cJSON *subitem,JSONStructItem_T *jsonitem,WORD itemnum,BYTE *out_struct)
{
    CHAR  *buf     = NULL;
    DWORD retcode  = 0;

    COM_NULLPOINTER_CHK_RTVALUE(subitem);
    COM_NULLPOINTER_CHK_RTVALUE(jsonitem);
    COM_NULLPOINTER_CHK_RTVALUE(out_struct);

    buf = cJSON_Print(subitem);
    if(NULL == buf)
    {
        printf("dba output json code failed(errinfo=%s)",cJSON_GetErrorPtr());
        return COM_RC_JSON_PRINT_ERR;
    }
    
    retcode = com_decode_json_msg(buf,jsonitem,itemnum,out_struct);
    if(COM_RC_OK != retcode)
    {
        printf("dba decode object failed(retcode=%d)",retcode);
        com_json_tulip_free((BYTE *)buf);
        return retcode;
    }

    com_json_tulip_free((BYTE *)buf);

    return COM_RC_OK;
}

/**********************************************************************
* 函数名称：com_get_json_array
* 功能描述：从JSON码流中获取数组
* 修改日期     版本号     修改人      修改内容
*
************************************************************************/
/*sslint disable*/
DWORD com_get_json_array(cJSON *subitem,
                             WORD arraynum,
                             JSONStructItem_T *jsonitem,
                             WORD itemnum,
                             WORD itemlen,
                             BYTE *out_struct)
{
    WORD i         = 0;
    INT arraysize  = 0;
    cJSON *pItem   = NULL;

    COM_NULLPOINTER_CHK_RTVALUE(subitem);
    COM_NULLPOINTER_CHK_RTVALUE(out_struct);

    arraysize = cJSON_GetArraySize(subitem);
    if(0 >= arraysize)
    {
        printf("com get invalid array size(sizeof=%d)",arraysize);
        return COM_RC_OK;
    }

    for(i = 0; (i < arraysize) && (i < arraynum) ;i++)
    {
        pItem = cJSON_GetArrayItem(subitem,i);
        if(NULL == pItem)
        {
            printf("com get array item failed(errinfo=%s)",cJSON_GetErrorPtr());
            return COM_RC_JSON_GET_ARRAY_ERR;
        }
        
        switch(pItem->type)
        {
            case cJSON_Number:
            {
                memcpy(out_struct+((long)(unsigned)(i*itemlen)),&pItem->valueint,itemlen);
            }
            break;

            case cJSON_String:
            {
                memcpy(out_struct+((long)(unsigned)(i*itemlen)),pItem->valuestring,strlen(pItem->valuestring));
            }
            break;

            case cJSON_Object:
            {
                COM_NOK_CHK_RTCODE(com_get_json_object(pItem,jsonitem,itemnum,out_struct+((long)(unsigned)(i*itemlen))));
            }
            break;

            default:
            {
                COM_PRN_ERROR("Inavlid JSON type(itemtype=%d,itemnum=%d)",pItem->type,i);
                return COM_RC_JSON_TYPE_INVALID;
            }
        }
    }

    return COM_RC_OK;
}

/**********************************************************************
* 函数名称：dba_decode_json_proc
* 功能描述：json格式解码

* 修改日期     版本号     修改人      修改内容
*
************************************************************************/
DWORD com_decode_json_proc(cJSON *root,JSONStructItem_T *jsonitem,WORD itemnum,BYTE *out_struct)
{
    WORD  i       = 0;
    cJSON *pItem  = NULL;
    
    COM_NULLPOINTER_CHK_RTVALUE(root);
    COM_NULLPOINTER_CHK_RTVALUE(jsonitem);
    COM_NULLPOINTER_CHK_RTVALUE(out_struct);

    for(i = 0; i < itemnum;i++)
    {
        pItem = cJSON_GetObjectItem(root,jsonitem[i].itemkey);
        if(NULL == pItem)
        {
            printf("get json object failed(errinfo=%s)",cJSON_GetErrorPtr());
            return COM_RC_JSON_GET_OBJECT_ERR;
        }

        switch(pItem->type)
        {
            case cJSON_String:
            {
                if(NULL != jsonitem[i].callback)
                {
                    jsonitem[i].callback(out_struct+jsonitem[i].offset,pItem->valuestring);
                }
                else
                {
                    memcpy(out_struct+jsonitem[i].offset,pItem->valuestring,strlen(pItem->valuestring));
                }
            }
            break;

            case cJSON_Number:
            {
                if(NULL != jsonitem[i].callback)
                {
                    jsonitem[i].callback(out_struct+jsonitem[i].offset,&pItem->valueint);
                }
                else
                {
                    memcpy(out_struct+jsonitem[i].offset,&pItem->valueint,jsonitem[i].length);
                }
            }
            break;

            case cJSON_Array:
            {
                COM_NOK_CHK_RTCODE(com_get_json_array(pItem,jsonitem[i].arrayNum,jsonitem[i].pSubJSONStructItem,jsonitem[i].subItemNum,jsonitem[i].length,out_struct+jsonitem[i].offset));
            }
            break;

            case cJSON_Object:
            {
                COM_NOK_CHK_RTCODE(com_get_json_object(pItem,jsonitem[i].pSubJSONStructItem,jsonitem[i].subItemNum,out_struct+jsonitem[i].offset));
            }
            break;

            default:
            {
                printf("Inavlid JSON type(itemtype=%d,itemnum=%d)",pItem->type,i);
                return COM_RC_JSON_TYPE_INVALID;
            }

        }

    }

    return COM_RC_OK;

}
/*sslint enable*/
/**********************************************************************
* 函数名称：com_decode_json_msg
* 功能描述：json格式解码
* 修改日期     版本号     修改人      修改内容
*
************************************************************************/
DWORD com_decode_json_msg(VOID *msg_body,JSONStructItem_T *jsonitem,WORD itemnum,BYTE *out_struct)
{
    cJSON *pRoot  = NULL;
    DWORD retcode = 0;

    COM_NULLPOINTER_CHK_RTVALUE(msg_body);
    COM_NULLPOINTER_CHK_RTVALUE(jsonitem);
    COM_NULLPOINTER_CHK_RTVALUE(out_struct);

    /* 获取根节点 */
    pRoot = cJSON_Parse(msg_body);
    if(NULL == pRoot)
    {
        printf("parse json code failed(errinfo=%s)",cJSON_GetErrorPtr());
        return COM_RC_JSON_DECODE_ERR;
    }

    retcode = com_decode_json_proc(pRoot,jsonitem,itemnum,out_struct);
    if(COM_RC_OK != retcode)
    {
        cJSON_Delete(pRoot);
        return retcode;
    }

    cJSON_Delete(pRoot);

    return COM_RC_OK;

}

/**********************************************************************
* 函数名称：com_code_json_msg
* 功能描述：json格式编码
* 修改日期     版本号     修改人      修改内容
*
************************************************************************/
DWORD com_code_json_num_array_proc(cJSON *root,JSONStructItem_T jsonitem,BYTE *in_struct)
{
    WORD  j = 0;

    COM_NULLPOINTER_CHK_RTVALUE(root);
    COM_NULLPOINTER_CHK_RTVALUE(in_struct);

    int numarr[100] = {0}; // can not modify INT type //double type not realize
    for(j=0; j < jsonitem.arrayNum; j++)
    {
        numarr[j] = *(WORD*)(in_struct+(long)(unsigned)(jsonitem.offset+j*jsonitem.length));
    }
    cJSON *num = cJSON_CreateIntArray(numarr,jsonitem.arrayNum);
    if(NULL == num)
    {
        printf("dba create int array failed(errinfo=%s)",cJSON_GetErrorPtr());
        return COM_RC_JSON_CREATE_INTARR_ERR;
    }
    
    cJSON_AddItemToObject(root,jsonitem.itemkey,num);

    return COM_RC_OK;
}

/**********************************************************************
* 函数名称：com_code_json_object_array_proc
* 修改日期     版本号     修改人      修改内容
*
************************************************************************/
/*sslint disable*/
DWORD com_code_json_object_array_proc(cJSON *root,JSONStructItem_T jsonitem,BYTE *in_struct)
{
    WORD  j = 0;

    COM_NULLPOINTER_CHK_RTVALUE(root);
    COM_NULLPOINTER_CHK_RTVALUE(in_struct);

    cJSON *objectarr = cJSON_CreateArray();

    for(j=0; j < jsonitem.arrayNum; j++)
    {
        cJSON * subitem = cJSON_CreateObject();
        if(NULL == subitem)
        {
            printf("com create json object failed(errinfo=%s)",cJSON_GetErrorPtr());
            return COM_RC_JSON_CREATE_OBJ_ERR;
        }
        cJSON_AddItemToArray(objectarr,subitem);
        
        COM_NOK_CHK_RTCODE(com_code_json_proc(subitem,jsonitem.pSubJSONStructItem,jsonitem.subItemNum,(BYTE*)(in_struct+(long)(unsigned)(jsonitem.offset+j*jsonitem.length))));
    }

    cJSON_AddItemToObject(root,jsonitem.itemkey,objectarr);

    return COM_RC_OK;
}

/**********************************************************************
* 函数名称：dba_code_json_array_proc
* 修改日期     版本号     修改人      修改内容
*
************************************************************************/
DWORD com_code_json_array_proc(cJSON *root,JSONStructItem_T jsonitem,BYTE *in_struct)
{
    DWORD retcode = 0;
    
    COM_NULLPOINTER_CHK_RTVALUE(root);
    COM_NULLPOINTER_CHK_RTVALUE(in_struct);

    //num array
    if(cJSON_Number == jsonitem.arrayType)
    {
        retcode = com_code_json_num_array_proc(root,jsonitem,in_struct);
        if(COM_RC_OK != retcode)
        {
            printf("dba code num array failed(retcode=%d)",retcode);
            return retcode;
        }
    }
    //str array
    if(cJSON_String == jsonitem.arrayType)
    {
        cJSON *str = cJSON_CreateStringArrayEx((CHAR*)in_struct+jsonitem.offset,jsonitem.arrayNum,jsonitem.length);
        if(NULL == str)
        {
            printf("dba create string array failed(errinfo=%s)",cJSON_GetErrorPtr());
            return COM_RC_JSON_CREATE_STRINGARR_ERR;
        }
        cJSON_AddItemToObject(root,jsonitem.itemkey,str);
    }
    //object array
    if(cJSON_Object == jsonitem.arrayType)
    {
        retcode = com_code_json_object_array_proc(root,jsonitem,in_struct);
        if(COM_RC_OK != retcode)
        {
            printf("dba code object array failed(retcode=%d)",retcode);
            return retcode;
        }
    }

    return COM_RC_OK;
}

/*sslint enable*/
/**********************************************************************
* 函数名称：com_code_json_num_proc
* 修改日期     版本号     修改人      修改内容
*
************************************************************************/
DWORD com_code_json_num_proc(cJSON *root,JSONStructItem_T jsonitem,BYTE *in_struct)
{
    DWORD num = 0;

    COM_NULLPOINTER_CHK_RTVALUE(root);
    COM_NULLPOINTER_CHK_RTVALUE(in_struct);

    /* 支持回调 */
    if(NULL != jsonitem.callback)
    {
        jsonitem.callback(&num,in_struct+jsonitem.offset);
    }
    else
    {
        if(sizeof(BYTE) == jsonitem.length)
        {
            num = *(BYTE*)(in_struct+jsonitem.offset);
        }
        if(sizeof(WORD) == jsonitem.length)
        {
            num = *(WORD*)(in_struct+jsonitem.offset);
        }
        if(sizeof(DWORD) == jsonitem.length)
        {
            num = *(DWORD*)(in_struct+jsonitem.offset);
        }
    }

    cJSON_AddNumberToObject(root,jsonitem.itemkey,num);

    return COM_RC_OK;
}



/**********************************************************************
* 函数名称：com_code_json_proc
* 修改日期     版本号     修改人      修改内容
*
************************************************************************/
/*sslint disable*/
DWORD com_code_json_proc(cJSON *root,JSONStructItem_T *jsonitem,WORD itemnum,BYTE *in_struct)
{
    WORD  i       = 0;
    CHAR  ch[15]  = {0};
    cJSON *object = NULL;

    COM_NULLPOINTER_CHK_RTVALUE(jsonitem);
    COM_NULLPOINTER_CHK_RTVALUE(in_struct);

    for(i = 0; i < itemnum; i++)
    {
        switch(jsonitem[i].itemtype)
        {
            case cJSON_Number:
            {
                COM_NOK_CHK_RTCODE(com_code_json_num_proc(root,jsonitem[i],in_struct));
            }
            break;

            case cJSON_String:
            {
                if(NULL != jsonitem[i].callback)
                {
                    jsonitem[i].callback(ch,in_struct+(jsonitem[i].offset));
                    cJSON_AddStringToObject(root,jsonitem[i].itemkey,ch);
                }
                else
                {
                    cJSON_AddStringToObject(root,jsonitem[i].itemkey,(CHAR*)in_struct+(jsonitem[i].offset));
                }
            }
            break;

            case cJSON_Array:
            {
                COM_NOK_CHK_RTCODE(com_code_json_array_proc(root,jsonitem[i],in_struct));
            }
            break;

            case cJSON_Object:
            {
                object = cJSON_CreateObject();
                if(NULL == object)
                {
                    printf("dba create sub object failed(errinfo=%s)",cJSON_GetErrorPtr());
                    return COM_RC_JSON_CREATE_OBJ_ERR;
                }
                
                cJSON_AddItemToObject(root, jsonitem[i].itemkey, object);

                /* 递归调用创建子对象 */
                COM_NOK_CHK_RTCODE(com_code_json_proc(object,jsonitem[i].pSubJSONStructItem,jsonitem[i].subItemNum,in_struct+jsonitem[i].offset));
            }
            break;

            default:
            {
                printf("Inavlid JSON type(itemtype=%d,itemnum=%d)",jsonitem[i].itemtype,i);
                return COM_RC_JSON_TYPE_INVALID;
            }
        }
    }

    return COM_RC_OK;
}
/*sslint enable*/
/**********************************************************************
* 函数名称：com_code_json_msg
* 修改日期     版本号     修改人      修改内容
*
************************************************************************/
DWORD com_code_json_msg(JSONStructItem_T *jsonitem,WORD itemnum,void *in_struct,void *msg_body)
{
    cJSON *root   = NULL;
    char  *out    = NULL;    
    DWORD retcode = 0;

    COM_NULLPOINTER_CHK_RTVALUE(jsonitem);
    COM_NULLPOINTER_CHK_RTVALUE(in_struct);
    COM_NULLPOINTER_CHK_RTVALUE(msg_body);

    root = cJSON_CreateObject();
    if(NULL == root)
    {
        printf("com create root object failed(errinfo=%s)",cJSON_GetErrorPtr());
        return COM_RC_JSON_CREATE_OBJ_ERR;
    }

    retcode = com_code_json_proc(root,jsonitem,itemnum,in_struct);
    if(COM_RC_OK != retcode)
    {
        cJSON_Delete(root);
        return retcode;
    }

    out = cJSON_Print(root);
    if(NULL == out)
    {
        printf("com output json code failed(errinfo=%s)",cJSON_GetErrorPtr());
        cJSON_Delete(root);
        return COM_RC_JSON_PRINT_ERR;
    }
    
    cJSON_Delete(root);

    snprintf(msg_body,1024,"%s",out);
    //memcpy(msg_body, out, strlen(out));
    
    com_json_tulip_free((BYTE *)out);

    return COM_RC_OK;
}

/**********************************************************************
* com_set_json_hooks
* 功能描述：设置JSON内存分配钩子
* 修改日期     版本号     修改人      修改内容
* 
************************************************************************/
void com_set_json_hooks()
{
    cJSON_Hooks tJsonHooks;
    memset(&tJsonHooks,0x00,sizeof(cJSON_Hooks));
    
    tJsonHooks.malloc_fn = (void* (*)(size_t))com_json_tulip_malloc;
    tJsonHooks.free_fn = (void (*)(void *))com_json_tulip_free;

    cJSON_InitHooks(&tJsonHooks);
}
