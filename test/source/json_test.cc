#include "gtest/gtest.h"  

extern "C" {

#include "cJSON.h"
#include "stub.h"
#include "json_test.h"
#include "json.h"
}

void _tcfs_log_stub(E_tcfs_log_level level,
        const char *file, unsigned int line, const char *fmt, ...)
{
    return;         
}

void Tulip_Free_stub(char *pucBuf)
{
    if(pucBuf)
        free(pucBuf);
}

class dba_json: public testing::Test  
{  
    protected:    
        static void SetUpTestCase()    
        {        
            INSTALL_STUB(tulip_free,Tulip_Free_stub); 
            INSTALL_STUB(_tcfs_log,_tcfs_log_stub);         
        }      
        static void TearDownTestCase()    
        {        
            REMOVE_STUB(Tulip_Free_stub); 
            REMOVE_STUB(_tcfs_log_stub);            
        }
    protected:      
        virtual void SetUp() {}      
        virtual void TearDown() {}  
}; 

JSONStructItem_T Json_Type1[] = 
{
    /* Json类型 */       /* Json键名 */       /* 字段长度 */         /* 字段在结构体中的偏移 */      /*数组元素类型*/  /*数组元素个数*/   /* Json子结构地址 */    /* 子结构Item个数 */  /* 回调函数 */
    {cJSON_String,       "name",                 64,                  OFFSETOF(Json_type1_T,name),       cJSON_False,       0,             NULL,                0,                 NULL},
    {cJSON_String,       "addr",                 128,                 OFFSETOF(Json_type1_T,addr),       cJSON_False,       0,             NULL,                0,                 NULL},
    {cJSON_Number,       "age",                  sizeof(BYTE),        OFFSETOF(Json_type1_T,age),        cJSON_False,       0,             NULL,                0,                 NULL},
};

TEST_F(dba_json,decode_json_type_simple_1)
{
    FILE *f;
    DWORD len;
    char *data;
    Json_type1_T tJsonType1;
    memset(&tJsonType1,0x00,sizeof(Json_type1_T));

    f=fopen("../../../../test/ft_dba/data/Json_Type_Simple.json","rb");
    if(NULL == f)
    {
        fprintf(stderr,"open file failed");
    }
    
    fseek(f,0,SEEK_END);
    len=ftell(f);
    fseek(f,0,SEEK_SET);
    
    data=(char*)malloc(len+1);
    fread(data,1,len,f);
    data[len]='\0';
    fclose(f);

    com_decode_json_msg(data,Json_Type1,sizeof(Json_Type1)/sizeof(JSONStructItem_T),(BYTE *)&tJsonType1);

    EXPECT_EQ(0,strcmp(tJsonType1.name, "wangxiaoming"));
    EXPECT_EQ(0,strcmp(tJsonType1.addr, "huashendadao"));
    EXPECT_EQ(tJsonType1.age,30);

    free(data);

}

JSONStructItem_T Json_Type2[] = 
{
    /* Json类型 */       /* Json键名 */       /* 字段长度 */           /* 字段在结构体中的偏移 */            /*数组元素类型*/  /*数组元素个数*/    /* Json子结构地址 */   /* 子结构Item个数 */                               /* 回调函数 */
    {cJSON_String,       "name2",                 64,                    OFFSETOF(Json_type2_T,name2),        cJSON_False,       0,                  NULL,                 0,                                                 NULL},
    {cJSON_String,       "addr2",                 128,                   OFFSETOF(Json_type2_T,addr2),        cJSON_False,       0,                  NULL,                 0,                                                 NULL},
    {cJSON_Number,       "age2",                  sizeof(BYTE),          OFFSETOF(Json_type2_T,age2),         cJSON_False,       0,                  NULL,                 0,                                                 NULL},
    {cJSON_Object,       "jsontype1",             sizeof(Json_type1_T),  OFFSETOF(Json_type2_T,jsonType1),    cJSON_False,       0,                  Json_Type1,           sizeof(Json_Type1)/sizeof(JSONStructItem_T),       NULL},
};

TEST_F(dba_json,decode_json_type_object_2)
{
    FILE *f;
    DWORD len;
    char *data;
    Json_type2_T tJsonType2;
    memset(&tJsonType2,0x00,sizeof(Json_type2_T));

    f=fopen("../test/ft_dba/data/Json_Type_Object.json","rb");
    if(NULL == f)
    {
        fprintf(stderr,"open file failed");
    }
    
    fseek(f,0,SEEK_END);
    len=ftell(f);
    fseek(f,0,SEEK_SET);
    
    data=(char*)malloc(len+1);
    fread(data,1,len,f);
    data[len]='\0';
    fclose(f);

    com_decode_json_msg(data,Json_Type2,sizeof(Json_Type2)/sizeof(JSONStructItem_T),(BYTE *)&tJsonType2);

    EXPECT_EQ(0,strcmp(tJsonType2.name2, "wangxiaoming"));
    EXPECT_EQ(0,strcmp(tJsonType2.addr2, "huashendadao"));
    EXPECT_EQ(tJsonType2.age2,30);

    EXPECT_EQ(0,strcmp(tJsonType2.jsonType1.name, "aa"));
    EXPECT_EQ(0,strcmp(tJsonType2.jsonType1.addr, "nj"));
    EXPECT_EQ(tJsonType2.jsonType1.age, 32);

    free(data);
}

JSONStructItem_T Json_Operate_Type[] = 
{
    /* Json类型 */       /* Json键名 */       /* 字段长度 */          /* 字段在结构体中的偏移 */            /*数组元素类型*/ /*数组元素个数*/   /* Json子结构地址 */   /* 子结构Item个数 */       /* 回调函数 */
    {cJSON_String,       "value",                64,                  OFFSETOF(Json_operate_T,value),         cJSON_False,       0,               NULL,              0,                             NULL},
    {cJSON_Number,       "onclick",              sizeof(WORD),        OFFSETOF(Json_operate_T,onclick),       cJSON_False,       0,               NULL,              0,                             NULL},
};

JSONStructItem_T Json_Type_Array[] = 
{
    /* Json类型 */       /* Json键名 */       /* 字段长度 */                /* 字段在结构体中的偏移 */               /*数组元素类型*/  /*数组元素个数*/   /* Json子结构地址 */      /* 子结构Item个数 */                                     /* 回调函数 */
    {cJSON_Array,        "index",             sizeof(WORD),                 OFFSETOF(Json_type_Array_T,numarray),     cJSON_Number,       3,                NULL,                        0,                                                    NULL},
    {cJSON_Array,        "name",              128,                          OFFSETOF(Json_type_Array_T,strarray),     cJSON_String,       4,                NULL,                        0,                                                    NULL},
    {cJSON_Array,        "operate",           sizeof(Json_operate_T),       OFFSETOF(Json_type_Array_T,operate),      cJSON_Object,       3,                Json_Operate_Type,           sizeof(Json_Operate_Type)/sizeof(JSONStructItem_T),   NULL},
};

TEST_F(dba_json,decode_json_type_array_3)
{
    FILE *f;
    DWORD len;
    char *data;
    Json_type_Array_T tJsonTypeArray;

    memset(&tJsonTypeArray,0x00,sizeof(Json_type_Array_T));

    f=fopen("../test/ft_dba/data/Json_Type_Array.json","rb");
    if(NULL == f)
    {
        fprintf(stderr,"open file failed");
    }
    
    fseek(f,0,SEEK_END);
    len=ftell(f);
    fseek(f,0,SEEK_SET);
    
    data=(char*)malloc(len+1);
    fread(data,1,len,f);
    data[len]='\0';
    fclose(f);

    com_decode_json_msg(data,Json_Type_Array,sizeof(Json_Type_Array)/sizeof(JSONStructItem_T),(BYTE *)&tJsonTypeArray);

    //test num array
    EXPECT_EQ(tJsonTypeArray.numarray[0], 11);
    EXPECT_EQ(tJsonTypeArray.numarray[2], 13);
    //test str array
    EXPECT_EQ(0,strcmp(tJsonTypeArray.strarray[0], "str1"));
    EXPECT_EQ(0,strcmp(tJsonTypeArray.strarray[1], "str2"));
    EXPECT_EQ(0,strcmp(tJsonTypeArray.strarray[3], "str4"));
    //test object array
    EXPECT_EQ(0,strcmp(tJsonTypeArray.operate[0].value, "Add"));
    EXPECT_EQ(0,strcmp(tJsonTypeArray.operate[1].value, "Mod"));
    EXPECT_EQ(0,strcmp(tJsonTypeArray.operate[2].value, "Del"));

    EXPECT_EQ(tJsonTypeArray.operate[0].onclick, 123);
    EXPECT_EQ(tJsonTypeArray.operate[1].onclick, 456);
    EXPECT_EQ(tJsonTypeArray.operate[2].onclick, 789);

    free(data);
}

JSONStructItem_T Json_Type_bb[] = 
{
    /* Json类型 */       /* Json键名 */   /* 字段长度 */      /* 字段在结构体中的偏移 */        /*数组元素类型*/ /*数组元素个数*/   /* Json子结构地址 */   /* 子结构Item个数 */       /* 回调函数 */
    {cJSON_String,       "bb1",            64,                OFFSETOF(Json_operate_bb_T,bb1),    cJSON_False,     0,                 NULL,                  0,                           NULL},
    {cJSON_Number,       "bb2",            sizeof(WORD),      OFFSETOF(Json_operate_bb_T,bb2),    cJSON_False,     0,                 NULL,                  0,                           NULL},
};


JSONStructItem_T Json_Type_aa[] = 
{
    /* Json类型 */       /* Json键名 */       /* 字段长度 */               /* 字段在结构体中的偏移 */            /*数组元素类型*/  /*数组元素个数*/    /* Json子结构地址 */     /* 子结构Item个数 */                                     /* 回调函数 */
    {cJSON_Array,        "name",             128,                          OFFSETOF(Json_operate_aa_T,name),      cJSON_String,         2,                NULL,                   0,                                                          NULL},
    {cJSON_Array,        "operate",          sizeof(Json_operate_T),       OFFSETOF(Json_operate_aa_T,operate),   cJSON_Object,         3,                Json_Operate_Type,      sizeof(Json_Operate_Type)/sizeof(JSONStructItem_T),         NULL},
    {cJSON_Object,       "bb",               sizeof(Json_operate_bb_T),    OFFSETOF(Json_operate_aa_T,bb),        cJSON_False,          0,                Json_Type_bb,           sizeof(Json_Type_bb)/sizeof(JSONStructItem_T),              NULL},
};

JSONStructItem_T Json_Type_Complicate[] = 
{
    /* Json类型 */       /* Json键名 */       /* 字段长度 */               /* 字段在结构体中的偏移 */                 /*数组元素类型*/  /*数组元素个数*/    /* Json子结构地址 */   /* 子结构Item个数 */                                  /* 回调函数 */
    {cJSON_Array,        "index",             sizeof(WORD),                 OFFSETOF(Json_type_Complicate_T,index),    cJSON_Number,        3,                 NULL,                0,                                                    NULL},
    {cJSON_Object,       "aa",                sizeof(Json_operate_aa_T),    OFFSETOF(Json_type_Complicate_T,aa),       cJSON_False,         0,                 Json_Type_aa,        sizeof(Json_Type_aa)/sizeof(JSONStructItem_T),        NULL},
};

TEST_F(dba_json,decode_json_type_complicate_4)
{
    FILE *f;
    DWORD len;
    char *data;
    Json_type_Complicate_T tJsonTypeCom;

    memset(&tJsonTypeCom,0x00,sizeof(Json_type_Complicate_T));

    f=fopen("../test/ft_dba/data/Json_Type_Complicate.json","rb");
    if(NULL == f)
    {
        fprintf(stderr,"open file failed");
    }
    
    fseek(f,0,SEEK_END);
    len=ftell(f);
    fseek(f,0,SEEK_SET);
    
    data=(char*)malloc(len+1);
    fread(data,1,len,f);
    data[len]='\0';
    fclose(f);

    com_decode_json_msg(data,Json_Type_Complicate,sizeof(Json_Type_Complicate)/sizeof(JSONStructItem_T),(BYTE *)&tJsonTypeCom);

    //test num array
    EXPECT_EQ(tJsonTypeCom.index[0], 11);
    EXPECT_EQ(tJsonTypeCom.index[2], 13);
    //test str array
    EXPECT_EQ(0,strcmp(tJsonTypeCom.aa.name[0], "str1"));
    EXPECT_EQ(0,strcmp(tJsonTypeCom.aa.name[1], "str2"));
    //test aa
    EXPECT_EQ(0,strcmp(tJsonTypeCom.aa.operate[0].value, "Add"));
    EXPECT_EQ(0,strcmp(tJsonTypeCom.aa.operate[1].value, "Mod"));
    EXPECT_EQ(0,strcmp(tJsonTypeCom.aa.operate[2].value, "Del"));

    EXPECT_EQ(tJsonTypeCom.aa.operate[0].onclick, 123);
    EXPECT_EQ(tJsonTypeCom.aa.operate[1].onclick, 456);
    EXPECT_EQ(tJsonTypeCom.aa.operate[2].onclick, 789);
    //bb
    EXPECT_EQ(0,strcmp(tJsonTypeCom.aa.bb.bb1, "Add"));
    EXPECT_EQ(tJsonTypeCom.aa.bb.bb2, 123); 

    free(data);
}

JSONStructItem_T Json_Type_cfb[] = 
{
    /* Json类型 */       /* Json键名 */       /* 字段长度 */         /* 字段在结构体中的偏移 */       /*数组元素类型*/  /*数组元素个数*/  /* Json子结构地址 */   /* 子结构Item个数 */  /* 回调函数 */
    {cJSON_Number,       "num2str",                 16,               OFFSETOF(Json_type_cfb_T,str),   cJSON_False,         0,                 NULL,                0,                 dba_json_int2str_cb},
    {cJSON_String,       "str2num",                 sizeof(DWORD),    OFFSETOF(Json_type_cfb_T,num),   cJSON_False,         0,                 NULL,                0,                 dba_json_str2int_cb},
};

TEST_F(dba_json,decode_json_type_cfb_5)
{
    FILE *f;
    DWORD len;
    char *data;
    Json_type_cfb_T tJsonTypeCfb;
    memset(&tJsonTypeCfb,0x00,sizeof(Json_type_cfb_T));

    f=fopen("../test/ft_dba/data/Json_Type_Cfb.json","rb");
    if(NULL == f)
    {
        fprintf(stderr,"open file failed");
    }
    
    fseek(f,0,SEEK_END);
    len=ftell(f);
    fseek(f,0,SEEK_SET);
    
    data=(char*)malloc(len+1);
    fread(data,1,len,f);
    data[len]='\0';
    fclose(f);

    com_decode_json_msg(data,Json_Type_cfb,sizeof(Json_Type_cfb)/sizeof(JSONStructItem_T),(BYTE *)&tJsonTypeCfb);

    EXPECT_EQ(0,strcmp(tJsonTypeCfb.str, "10160048"));
    EXPECT_EQ(tJsonTypeCfb.num,65535);

    free(data);
}

TEST_F(dba_json,code_json_type_simple_6)
{
    char text1[]="{\n\t\"name\":\t\"wangxiaoming\",\n\t\"addr\":\t\"huashendadao\",\n\t\"age\":\t129\n}";

    Json_type1_T tJsonType1;
    
    memset(&tJsonType1,0x00,sizeof(Json_type1_T));
    memcpy(tJsonType1.name,"wangxiaoming",strlen("wangxiaoming"));
    memcpy(tJsonType1.addr,"huashendadao",strlen("huashendadao"));
    tJsonType1.age = 129;

    char msg[2048] = {0};
    
    com_code_json_msg(Json_Type1,sizeof(Json_Type1)/sizeof(JSONStructItem_T),(BYTE *)&tJsonType1,msg);
    
    EXPECT_EQ(0,strcmp(msg, text1));
}

JSONStructItem_T Json_Type7[] = 
{
    /* Json类型 */       /* Json键名 */       /* 字段长度 */         /* 字段在结构体中的偏移 */      /*数组元素类型*/  /*数组元素个数*/   /* Json子结构地址 */    /* 子结构Item个数 */  /* 回调函数 */
    {cJSON_String,       "name",                 64,                  OFFSETOF(Json_type7_T,name),       cJSON_False,       0,             NULL,                0,                 NULL},
    {cJSON_String,       "addr",                 128,                 OFFSETOF(Json_type7_T,addr),       cJSON_False,       0,             NULL,                0,                 NULL},
    {cJSON_Number,       "age",                  sizeof(BYTE),        OFFSETOF(Json_type7_T,age),        cJSON_False,       0,             NULL,                0,                 NULL},
    {cJSON_Number,       "money",                sizeof(DWORD),       OFFSETOF(Json_type7_T,money),      cJSON_False,       0,             NULL,                0,                 NULL},
};

TEST_F(dba_json,code_json_type_simple_7)
{
    char text1[]="{\n\t\"name\":\t\"wangxiaoming\",\n\t\"addr\":\t\"huashendadao\",\n\t\"age\":\t129,\n\t\"money\":\t1024\n}";

    Json_type7_T tJsonType7;
    
    memset(&tJsonType7,0x00,sizeof(Json_type7_T));
    memcpy(tJsonType7.name,"wangxiaoming",strlen("wangxiaoming"));
    memcpy(tJsonType7.addr,"huashendadao",strlen("huashendadao"));
    tJsonType7.age = 129;
    tJsonType7.money = 1024;

    char msg[2048] = {0};
    
    com_code_json_msg(Json_Type7,sizeof(Json_Type7)/sizeof(JSONStructItem_T),(BYTE *)&tJsonType7,msg);
    
    EXPECT_EQ(0,strcmp(msg, text1));
}


TEST_F(dba_json,code_json_type_object_7)
{
    char text1[]="{\n\t\"name2\":\t\"wangxiaoming\",\n\t\"addr2\":\t\"huashendadao\",\n\t\"age2\":\t123,\n\t\"jsontype1\":\t{\n\t\t\"name\":\t\"aa\",\n\t\t\"addr\":\t\"nj\",\n\t\t\"age\":\t32\n\t}\n}";

    Json_type2_T tJsonType2;
    
    memset(&tJsonType2,0x00,sizeof(Json_type2_T));
    memcpy(tJsonType2.name2,"wangxiaoming",strlen("wangxiaoming"));
    memcpy(tJsonType2.addr2,"huashendadao",strlen("huashendadao"));
    tJsonType2.age2 = 123;
    memcpy(tJsonType2.jsonType1.name,"aa",strlen("aa"));
    memcpy(tJsonType2.jsonType1.addr,"nj",strlen("nj"));   
    tJsonType2.jsonType1.age = 32;
        

    char msg[2048] = {0};
    
    com_code_json_msg(Json_Type2,sizeof(Json_Type2)/sizeof(JSONStructItem_T),(BYTE *)&tJsonType2,msg);
    
    EXPECT_EQ(0,strcmp(msg, text1));
}

JSONStructItem_T Json_Type_Code_Array[] = 
{
    /* Json类型 */       /* Json键名 */     /* 字段长度 */        /* 字段在结构体中的偏移 */                 /*数组元素类型*/  /*数组元素个数*/   /* Json子结构地址 */      /* 子结构Item个数 */      /* 回调函数 */
    {cJSON_Array,        "index",           sizeof(WORD),         OFFSETOF(Json_type_Code_Arr_T,numarray),     cJSON_Number,       3,                 NULL,                        0,                      NULL},
    {cJSON_Array,        "name",            128,                  OFFSETOF(Json_type_Code_Arr_T,strarray),     cJSON_String,       4,                 NULL,                        0,                      NULL},
};

TEST_F(dba_json,code_json_type_array_8)
{
    char text1[]="{\n\t\"index\":\t[123, 456, 789],\n\t\"name\":\t[\"str11\", \"str22\", \"str33\", \"str44\"]\n}";

    Json_type_Code_Arr_T tJsonTypeArray;
    
    memset(&tJsonTypeArray,0x00,sizeof(Json_type_Code_Arr_T));
    tJsonTypeArray.numarray[0] = 123;
    tJsonTypeArray.numarray[1] = 456;
    tJsonTypeArray.numarray[2] = 789;
    memcpy(tJsonTypeArray.strarray[0],"str11",strlen("str11"));
    memcpy(tJsonTypeArray.strarray[1],"str22",strlen("str22"));
    memcpy(tJsonTypeArray.strarray[2],"str33",strlen("str33"));
    memcpy(tJsonTypeArray.strarray[3],"str44",strlen("str44"));
        

    char msg[2048] = {0};
    
    com_code_json_msg(Json_Type_Code_Array,sizeof(Json_Type_Code_Array)/sizeof(JSONStructItem_T),(BYTE *)&tJsonTypeArray,msg);
    
    EXPECT_EQ(0,strcmp(msg, text1));
}

TEST_F(dba_json,code_json_type_array_object_9)
{
    char text1[]="{\n\t\"index\":\t[0, 99, 65535],\n\t\"name\":\t[\"str11\", \"str22\", \"str33\", \"str44\"],\n\t\"operate\":\t[{\n\t\t\t\"value\":\t\"add\",\n\t\t\t\"onclick\":\t256\n\t\t}, {\n\t\t\t\"value\":\t\"mod\",\n\t\t\t\"onclick\":\t1024\n\t\t}, {\n\t\t\t\"value\":\t\"del\",\n\t\t\t\"onclick\":\t35535\n\t\t}]\n}";

    Json_type_Array_T tJsonTypeArray;
    
    memset(&tJsonTypeArray,0x00,sizeof(Json_type_Code_Arr_T));
    tJsonTypeArray.numarray[0] = 0;
    tJsonTypeArray.numarray[1] = 99;
    tJsonTypeArray.numarray[2] = 65535;
    memcpy(tJsonTypeArray.strarray[0],"str11",strlen("str11"));
    memcpy(tJsonTypeArray.strarray[1],"str22",strlen("str22"));
    memcpy(tJsonTypeArray.strarray[2],"str33",strlen("str33"));
    memcpy(tJsonTypeArray.strarray[3],"str44",strlen("str44"));
    
    memcpy(tJsonTypeArray.operate[0].value,"add",strlen("add"));
    memcpy(tJsonTypeArray.operate[1].value,"mod",strlen("mod"));
    memcpy(tJsonTypeArray.operate[2].value,"del",strlen("del"));

    tJsonTypeArray.operate[0].onclick = 256;
    tJsonTypeArray.operate[1].onclick = 1024;
    tJsonTypeArray.operate[2].onclick = 35535;
        

    char msg[2048] = {0};
    
    com_code_json_msg(Json_Type_Array,sizeof(Json_Type_Array)/sizeof(JSONStructItem_T),(BYTE *)&tJsonTypeArray,msg);
    
    EXPECT_EQ(0,strcmp(msg, text1));
}

TEST_F(dba_json,code_json_type_array_object_10)
{
    
    char text1[]="{\n\t\"index\":\t[129, 1025, 34567],\n\t\"aa\":\t{\n\t\t\"name\":\t[\"zhumg\", \"chenlin\"],\n\t\t\"operate\":\t[{\n\t\t\t\t\"value\":\t\"Add Name\",\n\t\t\t\t\"onclick\":\t10\n\t\t\t}, {\n\t\t\t\t\"value\":\t\"Mod Sex\",\n\t\t\t\t\"onclick\":\t1\n\t\t\t}, {\n\t\t\t\t\"value\":\t\"Del Pepole\",\n\t\t\t\t\"onclick\":\t1239\n\t\t\t}],\n\t\t\"bb\":\t{\n\t\t\t\"bb1\":\t\"ZTE\",\n\t\t\t\"bb2\":\t2016\n\t\t}\n\t}\n}";
    

    Json_type_Complicate_T tJsonTypeComp;
    
    memset(&tJsonTypeComp,0x00,sizeof(Json_type_Complicate_T));
    tJsonTypeComp.index[0] = 129;
    tJsonTypeComp.index[1] = 1025;
    tJsonTypeComp.index[2] = 34567;
    memcpy(tJsonTypeComp.aa.name[0],"zhumg",strlen("zhumg"));
    memcpy(tJsonTypeComp.aa.name[1],"chenlin",strlen("chenlin"));
    
    memcpy(tJsonTypeComp.aa.operate[0].value,"Add Name",strlen("Add Name"));
    memcpy(tJsonTypeComp.aa.operate[1].value,"Mod Sex",strlen("Mod Sex"));
    memcpy(tJsonTypeComp.aa.operate[2].value,"Del Pepole",strlen("Del Pepole"));

    tJsonTypeComp.aa.operate[0].onclick = 10;
    tJsonTypeComp.aa.operate[1].onclick = 1;
    tJsonTypeComp.aa.operate[2].onclick = 1239;

    memcpy(tJsonTypeComp.aa.bb.bb1,"ZTE",strlen("zte"));
    tJsonTypeComp.aa.bb.bb2 = 2016;
        

    char msg[2048] = {0};
    
    com_code_json_msg(Json_Type_Complicate,sizeof(Json_Type_Complicate)/sizeof(JSONStructItem_T),(BYTE *)&tJsonTypeComp,msg);
    
    EXPECT_EQ(0,strcmp(msg, text1));
}

JSONStructItem_T Json_Type_Code_cfb[] = 
{
    /* Json类型 */       /* Json键名 */       /* 字段长度 */         /* 字段在结构体中的偏移 */       /*数组元素类型*/  /*数组元素个数*/  /* Json子结构地址 */   /* 子结构Item个数 */  /* 回调函数 */
    {cJSON_String,       "num2string",        sizeof(DWORD),     OFFSETOF(Json_type_Code_cb,num2string),   cJSON_False,         0,                 NULL,                0,                 dba_json_int2str_cb},
    {cJSON_Number,       "string2num",        5,                 OFFSETOF(Json_type_Code_cb,string2num),   cJSON_False,         0,                 NULL,                0,                 dba_json_str2int_cb},
};

TEST_F(dba_json,code_json_type_cfb)
{
    Json_type_Code_cb tJsonTypeCodeCB;
    char msg[1024] = {0};
    char text1[]="{\n\t\"num2string\":\t\"123\",\n\t\"string2num\":\t4567\n}";


    tJsonTypeCodeCB.num2string = 123;
    strncpy(tJsonTypeCodeCB.string2num,"4567",sizeof(tJsonTypeCodeCB.string2num));

    com_code_json_msg(Json_Type_Code_cfb,sizeof(Json_Type_Code_cfb)/sizeof(JSONStructItem_T),&tJsonTypeCodeCB,msg);

    EXPECT_EQ(0,strcmp(msg, text1));
}

cJSON *cJSON_CreateObject_stub(void)
{
    return NULL;
}

class dba_json_code_fail1: public testing::Test  
{  
    protected:    
        static void SetUpTestCase()    
        {        
            INSTALL_STUB(cJSON_CreateObject,cJSON_CreateObject_stub);
            INSTALL_STUB(_tcfs_log,_tcfs_log_stub);
        }      
        static void TearDownTestCase()    
        {        
            REMOVE_STUB(cJSON_CreateObject_stub);
            REMOVE_STUB(_tcfs_log_stub);
        }
    protected:      
        virtual void SetUp() {}      
        virtual void TearDown() {}  
}; 

TEST_F(dba_json_code_fail1,dba_code_json_msg_fail)
{
    Json_type1_T tJsonType1;
    
    memset(&tJsonType1,0x00,sizeof(Json_type1_T));
    memcpy(tJsonType1.name,"wangxiaoming",strlen("wangxiaoming"));
    memcpy(tJsonType1.addr,"huashendadao",strlen("huashendadao"));
    tJsonType1.age = 129;

    char msg[2048] = {0};
    
    EXPECT_EQ(com_code_json_msg(Json_Type1,sizeof(Json_Type1)/sizeof(JSONStructItem_T),(BYTE *)&tJsonType1,msg),
        COM_RC_JSON_CREATE_OBJ_ERR);
}

char *cJSON_Print_stub(cJSON *item)
{
    return NULL;
}

class dba_json_code_fail2: public testing::Test  
{  
    protected:    
        static void SetUpTestCase()    
        {        
            INSTALL_STUB(cJSON_Print,cJSON_Print_stub);
            INSTALL_STUB(_tcfs_log,_tcfs_log_stub);
        }      
        static void TearDownTestCase()    
        {        
            REMOVE_STUB(cJSON_Print_stub);
            REMOVE_STUB(_tcfs_log_stub);
        }
    protected:      
        virtual void SetUp() {}      
        virtual void TearDown() {}  
}; 



TEST_F(dba_json_code_fail2,dba_code_json_msg_fail)
{

    Json_type1_T tJsonType1;
    
    memset(&tJsonType1,0x00,sizeof(Json_type1_T));
    memcpy(tJsonType1.name,"wangxiaoming",strlen("wangxiaoming"));
    memcpy(tJsonType1.addr,"huashendadao",strlen("huashendadao"));
    tJsonType1.age = 129;

    char msg[2048] = {0};
    

    EXPECT_EQ(com_code_json_msg(Json_Type1,sizeof(Json_Type1)/sizeof(JSONStructItem_T),(BYTE *)&tJsonType1,msg),
        COM_RC_JSON_PRINT_ERR);
}

class dba_json_code_fail3: public testing::Test  
{  
    protected:    
        static void SetUpTestCase()    
        {
            INSTALL_STUB(_tcfs_log,_tcfs_log_stub);
        }      
        static void TearDownTestCase()    
        {
            REMOVE_STUB(_tcfs_log_stub);
        }
    protected:      
        virtual void SetUp() {}      
        virtual void TearDown() {}  
};

JSONStructItem_T Json_Type_unknow[] = 
{
    /* Json类型 */       /* Json键名 */       /* 字段长度 */         /* 字段在结构体中的偏移 */      /*数组元素类型*/  /*数组元素个数*/   /* Json子结构地址 */    /* 子结构Item个数 */  /* 回调函数 */
    {cJSON_NULL,         "name",                 64,                  OFFSETOF(Json_type1_T,name),       cJSON_False,       0,             NULL,                0,                 NULL},
    {cJSON_False,        "addr",                 128,                 OFFSETOF(Json_type1_T,addr),       cJSON_False,       0,             NULL,                0,                 NULL},
    {cJSON_Number,       "age",                  sizeof(BYTE),        OFFSETOF(Json_type1_T,age),        cJSON_False,       0,             NULL,                0,                 NULL},
};

TEST_F(dba_json_code_fail3,dba_code_json_msg_fail1)
{
    Json_type1_T tJsonType1;
    
    memset(&tJsonType1,0x00,sizeof(Json_type1_T));
    memcpy(tJsonType1.name,"wangxiaoming",strlen("wangxiaoming"));
    memcpy(tJsonType1.addr,"huashendadao",strlen("huashendadao"));
    tJsonType1.age = 129;

    char msg[2048] = {0};
    

    EXPECT_EQ(com_code_json_msg(Json_Type_unknow,sizeof(Json_Type_unknow)/sizeof(JSONStructItem_T),(BYTE *)&tJsonType1,msg),
        COM_RC_JSON_TYPE_INVALID);
}

JSONStructItem_T Json_Type_aa1[] = 
{
    /* Json类型 */       /* Json键名 */       /* 字段长度 */               /* 字段在结构体中的偏移 */            /*数组元素类型*/  /*数组元素个数*/    /* Json子结构地址 */     /* 子结构Item个数 */                                     /* 回调函数 */
    {cJSON_False,        "name",             128,                          OFFSETOF(Json_operate_aa_T,name),      cJSON_String,         2,                NULL,                   0,                                                          NULL},
    {cJSON_Array,        "operate",          sizeof(Json_operate_T),       OFFSETOF(Json_operate_aa_T,operate),   cJSON_Object,         3,                Json_Operate_Type,      sizeof(Json_Operate_Type)/sizeof(JSONStructItem_T),         NULL},
    {cJSON_Object,       "bb",               sizeof(Json_operate_bb_T),    OFFSETOF(Json_operate_aa_T,bb),        cJSON_False,          0,                Json_Type_bb,           sizeof(Json_Type_bb)/sizeof(JSONStructItem_T),              NULL},
};

JSONStructItem_T Json_Type_Complicate1[] = 
{
    /* Json类型 */       /* Json键名 */       /* 字段长度 */               /* 字段在结构体中的偏移 */                 /*数组元素类型*/  /*数组元素个数*/    /* Json子结构地址 */   /* 子结构Item个数 */                                  /* 回调函数 */
    {cJSON_Array,        "index",             sizeof(WORD),                 OFFSETOF(Json_type_Complicate_T,index),    cJSON_Number,        3,                 NULL,                0,                                                    NULL},
    {cJSON_Object,       "aa",                sizeof(Json_operate_aa_T),    OFFSETOF(Json_type_Complicate_T,aa),       cJSON_False,         0,                 Json_Type_aa1,       sizeof(Json_Type_aa1)/sizeof(JSONStructItem_T),        NULL},
};


TEST_F(dba_json_code_fail3,dba_code_json_msg_fail2)
{
    
    Json_type_Complicate_T tJsonTypeComp;
    
    memset(&tJsonTypeComp,0x00,sizeof(Json_type_Complicate_T));
    tJsonTypeComp.index[0] = 129;
    tJsonTypeComp.index[1] = 1025;
    tJsonTypeComp.index[2] = 34567;
    memcpy(tJsonTypeComp.aa.name[0],"zhumg",strlen("zhumg"));
    memcpy(tJsonTypeComp.aa.name[1],"chenlin",strlen("chenlin"));
    
    memcpy(tJsonTypeComp.aa.operate[0].value,"Add Name",strlen("Add Name"));
    memcpy(tJsonTypeComp.aa.operate[1].value,"Mod Sex",strlen("Mod Sex"));
    memcpy(tJsonTypeComp.aa.operate[2].value,"Del Pepole",strlen("Del Pepole"));

    tJsonTypeComp.aa.operate[0].onclick = 10;
    tJsonTypeComp.aa.operate[1].onclick = 1;
    tJsonTypeComp.aa.operate[2].onclick = 1239;

    memcpy(tJsonTypeComp.aa.bb.bb1,"ZTE",strlen("zte"));
    tJsonTypeComp.aa.bb.bb2 = 2016;
        

    char msg[2048] = {0};
    
    EXPECT_EQ(com_code_json_msg(Json_Type_Complicate1,sizeof(Json_Type_Complicate1)/sizeof(JSONStructItem_T),(BYTE *)&tJsonTypeComp,msg),
        COM_RC_JSON_TYPE_INVALID);
    
}

JSONStructItem_T Json_Operate_Type2[] = 
{
    /* Json类型 */       /* Json键名 */       /* 字段长度 */          /* 字段在结构体中的偏移 */            /*数组元素类型*/ /*数组元素个数*/   /* Json子结构地址 */   /* 子结构Item个数 */       /* 回调函数 */
    {cJSON_False,       "value",                64,                  OFFSETOF(Json_operate_T,value),         cJSON_False,       0,               NULL,              0,                             NULL},
    {cJSON_False,       "onclick",              sizeof(WORD),        OFFSETOF(Json_operate_T,onclick),       cJSON_False,       0,               NULL,              0,                             NULL},
};

JSONStructItem_T Json_Type_aa2[] = 
{
    /* Json类型 */       /* Json键名 */       /* 字段长度 */               /* 字段在结构体中的偏移 */            /*数组元素类型*/  /*数组元素个数*/    /* Json子结构地址 */     /* 子结构Item个数 */                                     /* 回调函数 */
    {cJSON_Array,        "name",             128,                          OFFSETOF(Json_operate_aa_T,name),      cJSON_String,         2,                NULL,                   0,                                                          NULL},
    {cJSON_Array,        "operate",          sizeof(Json_operate_T),       OFFSETOF(Json_operate_aa_T,operate),   cJSON_Object,         3,                Json_Operate_Type2,      sizeof(Json_Operate_Type2)/sizeof(JSONStructItem_T),         NULL},
    {cJSON_Object,       "bb",               sizeof(Json_operate_bb_T),    OFFSETOF(Json_operate_aa_T,bb),        cJSON_False,          0,                Json_Type_bb,           sizeof(Json_Type_bb)/sizeof(JSONStructItem_T),              NULL},
};

JSONStructItem_T Json_Type_Complicate2[] = 
{
    /* Json类型 */       /* Json键名 */       /* 字段长度 */               /* 字段在结构体中的偏移 */                 /*数组元素类型*/  /*数组元素个数*/    /* Json子结构地址 */   /* 子结构Item个数 */                                  /* 回调函数 */
    {cJSON_Array,        "index",             sizeof(WORD),                 OFFSETOF(Json_type_Complicate_T,index),    cJSON_Number,        3,                 NULL,                0,                                                    NULL},
    {cJSON_Object,       "aa",                sizeof(Json_operate_aa_T),    OFFSETOF(Json_type_Complicate_T,aa),       cJSON_False,         0,                 Json_Type_aa2,       sizeof(Json_Type_aa2)/sizeof(JSONStructItem_T),       NULL},
};

TEST_F(dba_json_code_fail3,dba_code_json_msg_fail3)
{
    
    Json_type_Complicate_T tJsonTypeComp;
    
    memset(&tJsonTypeComp,0x00,sizeof(Json_type_Complicate_T));
    tJsonTypeComp.index[0] = 129;
    tJsonTypeComp.index[1] = 1025;
    tJsonTypeComp.index[2] = 34567;
    memcpy(tJsonTypeComp.aa.name[0],"zhumg",strlen("zhumg"));
    memcpy(tJsonTypeComp.aa.name[1],"chenlin",strlen("chenlin"));
    
    memcpy(tJsonTypeComp.aa.operate[0].value,"Add Name",strlen("Add Name"));
    memcpy(tJsonTypeComp.aa.operate[1].value,"Mod Sex",strlen("Mod Sex"));
    memcpy(tJsonTypeComp.aa.operate[2].value,"Del Pepole",strlen("Del Pepole"));

    tJsonTypeComp.aa.operate[0].onclick = 10;
    tJsonTypeComp.aa.operate[1].onclick = 1;
    tJsonTypeComp.aa.operate[2].onclick = 1239;

    memcpy(tJsonTypeComp.aa.bb.bb1,"ZTE",strlen("zte"));
    tJsonTypeComp.aa.bb.bb2 = 2016;
        

    char msg[2048] = {0};
    
    EXPECT_EQ(com_code_json_msg(Json_Type_Complicate2,sizeof(Json_Type_Complicate2)/sizeof(JSONStructItem_T),(BYTE *)&tJsonTypeComp,msg),
        COM_RC_JSON_TYPE_INVALID);
    
}


cJSON *cJSON_CreateIntArray_stub(const int *numbers,int count)
{
    return NULL;
}

class dba_json_code_fail4: public testing::Test  
{  
    protected:    
        static void SetUpTestCase()    
        {
            INSTALL_STUB(cJSON_CreateIntArray,cJSON_CreateIntArray_stub);
            INSTALL_STUB(_tcfs_log,_tcfs_log_stub);
        }      
        static void TearDownTestCase()    
        {
            REMOVE_STUB(cJSON_CreateIntArray_stub);
            REMOVE_STUB(_tcfs_log_stub);
        }
    protected:      
        virtual void SetUp() {}      
        virtual void TearDown() {}  
}; 


TEST_F(dba_json_code_fail4,dba_code_json_msg_fail)
{
    
    Json_type_Complicate_T tJsonTypeComp;
    
    memset(&tJsonTypeComp,0x00,sizeof(Json_type_Complicate_T));
    tJsonTypeComp.index[0] = 129;
    tJsonTypeComp.index[1] = 1025;
    tJsonTypeComp.index[2] = 34567;
    memcpy(tJsonTypeComp.aa.name[0],"zhumg",strlen("zhumg"));
    memcpy(tJsonTypeComp.aa.name[1],"chenlin",strlen("chenlin"));
    
    memcpy(tJsonTypeComp.aa.operate[0].value,"Add Name",strlen("Add Name"));
    memcpy(tJsonTypeComp.aa.operate[1].value,"Mod Sex",strlen("Mod Sex"));
    memcpy(tJsonTypeComp.aa.operate[2].value,"Del Pepole",strlen("Del Pepole"));

    tJsonTypeComp.aa.operate[0].onclick = 10;
    tJsonTypeComp.aa.operate[1].onclick = 1;
    tJsonTypeComp.aa.operate[2].onclick = 1239;

    memcpy(tJsonTypeComp.aa.bb.bb1,"ZTE",strlen("zte"));
    tJsonTypeComp.aa.bb.bb2 = 2016;
        

    char msg[2048] = {0};
    
    EXPECT_EQ(com_code_json_msg(Json_Type_Complicate,sizeof(Json_Type_Complicate)/sizeof(JSONStructItem_T),(BYTE *)&tJsonTypeComp,msg),
        COM_RC_JSON_CREATE_INTARR_ERR);
    
}


cJSON *cJSON_CreateStringArrayEx_stub(const char *strings,int row,int col)
{
    return NULL;
}


class dba_json_code_fail5: public testing::Test  
{  
    protected:    
        static void SetUpTestCase()    
        {
            INSTALL_STUB(cJSON_CreateStringArrayEx,cJSON_CreateStringArrayEx_stub);
            INSTALL_STUB(_tcfs_log,_tcfs_log_stub);
        }      
        static void TearDownTestCase()    
        {
            REMOVE_STUB(cJSON_CreateStringArrayEx_stub);
            REMOVE_STUB(_tcfs_log_stub);
        }
    protected:      
        virtual void SetUp() {}      
        virtual void TearDown() {}  
};

TEST_F(dba_json_code_fail5,dba_code_json_msg_fail)
{
    
    Json_type_Complicate_T tJsonTypeComp;
    
    memset(&tJsonTypeComp,0x00,sizeof(Json_type_Complicate_T));
    tJsonTypeComp.index[0] = 129;
    tJsonTypeComp.index[1] = 1025;
    tJsonTypeComp.index[2] = 34567;
    memcpy(tJsonTypeComp.aa.name[0],"zhumg",strlen("zhumg"));
    memcpy(tJsonTypeComp.aa.name[1],"chenlin",strlen("chenlin"));
    
    memcpy(tJsonTypeComp.aa.operate[0].value,"Add Name",strlen("Add Name"));
    memcpy(tJsonTypeComp.aa.operate[1].value,"Mod Sex",strlen("Mod Sex"));
    memcpy(tJsonTypeComp.aa.operate[2].value,"Del Pepole",strlen("Del Pepole"));

    tJsonTypeComp.aa.operate[0].onclick = 10;
    tJsonTypeComp.aa.operate[1].onclick = 1;
    tJsonTypeComp.aa.operate[2].onclick = 1239;

    memcpy(tJsonTypeComp.aa.bb.bb1,"ZTE",strlen("zte"));
    tJsonTypeComp.aa.bb.bb2 = 2016;
        

    char msg[2048] = {0};
    
    EXPECT_EQ(com_code_json_msg(Json_Type_Complicate,sizeof(Json_Type_Complicate)/sizeof(JSONStructItem_T),(BYTE *)&tJsonTypeComp,msg),
        COM_RC_JSON_CREATE_STRINGARR_ERR);
    
}


cJSON *cJSON_Parse_stub(const char *value)
{
    return NULL;
}



class dba_json_decode_fail1: public testing::Test  
{  
    protected:    
        static void SetUpTestCase()    
        {        
            INSTALL_STUB(cJSON_Parse,cJSON_Parse_stub);
            INSTALL_STUB(_tcfs_log,_tcfs_log_stub);         
        }      
        static void TearDownTestCase()    
        {        
            REMOVE_STUB(cJSON_Parse_stub);
            REMOVE_STUB(_tcfs_log_stub);
        }
    protected:      
        virtual void SetUp() {}      
        virtual void TearDown() {}  
}; 


TEST_F(dba_json_decode_fail1,dba_decode_json_msg_fail)
{
    FILE *f;
    DWORD len;
    char *data;
    Json_type1_T tJsonType1;
    memset(&tJsonType1,0x00,sizeof(Json_type1_T));

    f=fopen("../test/ft_dba/data/Json_Type_Simple.json","rb");
    if(NULL == f)
    {
        fprintf(stderr,"open file failed");
    }
    
    fseek(f,0,SEEK_END);
    len=ftell(f);
    fseek(f,0,SEEK_SET);
    
    data=(char*)malloc(len+1);
    fread(data,1,len,f);
    data[len]='\0';
    fclose(f);

    EXPECT_EQ(com_decode_json_msg(data,Json_Type1,sizeof(Json_Type1)/sizeof(JSONStructItem_T),(BYTE *)&tJsonType1),COM_RC_JSON_DECODE_ERR);

    free(data);
}

class dba_json_decode_fail2: public testing::Test  
{  
    protected:    
        static void SetUpTestCase()    
        {
            INSTALL_STUB(tulip_free,Tulip_Free_stub);
            INSTALL_STUB(_tcfs_log,_tcfs_log_stub);
        }      
        static void TearDownTestCase()    
        {
            REMOVE_STUB(Tulip_Free_stub);
            REMOVE_STUB(_tcfs_log_stub);
        }
    protected:      
        virtual void SetUp() {}      
        virtual void TearDown() {}  
}; 


TEST_F(dba_json_decode_fail2,dba_decode_json_msg_fail1)
{
    FILE *f;
    DWORD len;
    char *data;
    Json_type1_T tJsonType1;
    memset(&tJsonType1,0x00,sizeof(Json_type1_T));

    f=fopen("../test/ft_dba/data/Json_Type_UnknowType.json","rb");
    if(NULL == f)
    {
        fprintf(stderr,"open file failed");
    }
    
    fseek(f,0,SEEK_END);
    len=ftell(f);
    fseek(f,0,SEEK_SET);
    
    data=(char*)malloc(len+1);
    fread(data,1,len,f);
    data[len]='\0';
    fclose(f);

    EXPECT_EQ(com_decode_json_msg(data,Json_Type1,sizeof(Json_Type1)/sizeof(JSONStructItem_T),(BYTE *)&tJsonType1),
        COM_RC_JSON_TYPE_INVALID);

    free(data);
}


TEST_F(dba_json_decode_fail2,dba_decode_json_msg_fail2)
{
    FILE *f;
    DWORD len;
    char *data;
    Json_type_Array_T tJsonTypeArray;

    memset(&tJsonTypeArray,0x00,sizeof(Json_type_Array_T));

    f=fopen("../test/ft_dba/data/Json_Type_Array_UnknowType.json","rb");
    if(NULL == f)
    {
        fprintf(stderr,"open file failed");
    }
    
    fseek(f,0,SEEK_END);
    len=ftell(f);
    fseek(f,0,SEEK_SET);
    
    data=(char*)malloc(len+1);
    fread(data,1,len,f);
    data[len]='\0';
    fclose(f);

    EXPECT_EQ(com_decode_json_msg(data,Json_Type_Array,sizeof(Json_Type_Array)/sizeof(JSONStructItem_T),(BYTE *)&tJsonTypeArray),COM_RC_JSON_TYPE_INVALID);

    free(data);
}


TEST_F(dba_json_decode_fail2,dba_decode_json_msg_fail3)
{
    FILE *f;
    DWORD len;
    char *data;
    Json_type2_T tJsonType2;
    memset(&tJsonType2,0x00,sizeof(Json_type2_T));

    f=fopen("../test/ft_dba/data/Json_Type_Object_UnknowType.json","rb");
    if(NULL == f)
    {
        fprintf(stderr,"open file failed");
    }
    
    fseek(f,0,SEEK_END);
    len=ftell(f);
    fseek(f,0,SEEK_SET);
    
    data=(char*)malloc(len+1);
    fread(data,1,len,f);
    data[len]='\0';
    fclose(f);

    EXPECT_EQ(com_decode_json_msg(data,Json_Type2,sizeof(Json_Type2)/sizeof(JSONStructItem_T),(BYTE *)&tJsonType2),COM_RC_JSON_TYPE_INVALID);

    free(data);
}


class dba_json_decode_fail3: public testing::Test  
{  
    protected:    
        static void SetUpTestCase()    
        {        
            INSTALL_STUB(cJSON_Print,cJSON_Print_stub);
            INSTALL_STUB(_tcfs_log,_tcfs_log_stub);
        }      
        static void TearDownTestCase()    
        {        
            REMOVE_STUB(cJSON_Print_stub);
            REMOVE_STUB(_tcfs_log_stub);
        }
    protected:      
        virtual void SetUp() {}      
        virtual void TearDown() {}  
}; 


TEST_F(dba_json_decode_fail3,dba_decode_json_msg_fail)
{
    FILE *f;
    DWORD len;
    char *data;
    Json_type2_T tJsonType2;
    memset(&tJsonType2,0x00,sizeof(Json_type2_T));

    f=fopen("../test/ft_dba/data/Json_Type_Object.json","rb");
    if(NULL == f)
    {
        fprintf(stderr,"open file failed");
    }
    
    fseek(f,0,SEEK_END);
    len=ftell(f);
    fseek(f,0,SEEK_SET);
    
    data=(char*)malloc(len+1);
    fread(data,1,len,f);
    data[len]='\0';
    fclose(f);

    EXPECT_EQ(com_decode_json_msg(data,Json_Type2,sizeof(Json_Type2)/sizeof(JSONStructItem_T),(BYTE *)&tJsonType2),COM_RC_JSON_PRINT_ERR);

    free(data);
}

class dba_json_decode_fail4: public testing::Test  
{  
    protected:    
        static void SetUpTestCase()    
        {        
            INSTALL_STUB(tulip_free,Tulip_Free_stub);
            INSTALL_STUB(_tcfs_log,_tcfs_log_stub);
        }      
        static void TearDownTestCase()    
        {        
            REMOVE_STUB(Tulip_Free_stub);
            REMOVE_STUB(_tcfs_log_stub);
        }
    protected:      
        virtual void SetUp() {}      
        virtual void TearDown() {}  
}; 


TEST_F(dba_json_decode_fail4,dba_decode_json_msg_fail)
{
    FILE *f;
    DWORD len;
    char *data;
    Json_type_Array_T tJsonTypeArray;

    memset(&tJsonTypeArray,0x00,sizeof(Json_type_Array_T));

    f=fopen("../test/ft_dba/data/Json_Type_Array_Size_Zero.json","rb");
    if(NULL == f)
    {
        fprintf(stderr,"open file failed");
    }
    
    fseek(f,0,SEEK_END);
    len=ftell(f);
    fseek(f,0,SEEK_SET);
    
    data=(char*)malloc(len+1);
    fread(data,1,len,f);
    data[len]='\0';
    fclose(f);

    EXPECT_EQ(com_decode_json_msg(data,Json_Type_Array,sizeof(Json_Type_Array)/sizeof(JSONStructItem_T),(BYTE *)&tJsonTypeArray),DBA_RC_OK);

    //test num array
    EXPECT_EQ(tJsonTypeArray.numarray[0], 0);
    EXPECT_EQ(tJsonTypeArray.numarray[2], 0);
    //test str array
    EXPECT_EQ(0,strcmp(tJsonTypeArray.strarray[0], "str1"));
    EXPECT_EQ(0,strcmp(tJsonTypeArray.strarray[1], "str2"));
    EXPECT_EQ(0,strcmp(tJsonTypeArray.strarray[3], "str4"));
    //test object array
    EXPECT_EQ(0,strcmp(tJsonTypeArray.operate[0].value, "Add"));
    EXPECT_EQ(0,strcmp(tJsonTypeArray.operate[1].value, "Mod"));
    EXPECT_EQ(0,strcmp(tJsonTypeArray.operate[2].value, "Del"));

    EXPECT_EQ(tJsonTypeArray.operate[0].onclick, 123);
    EXPECT_EQ(tJsonTypeArray.operate[1].onclick, 456);
    EXPECT_EQ(tJsonTypeArray.operate[2].onclick, 789);

    free(data);
}

cJSON *cJSON_GetArrayItem_stub(cJSON *array,int item)
{
    return NULL;
}

class dba_json_decode_fail5: public testing::Test  
{  
    protected:    
        static void SetUpTestCase()    
        {        
            INSTALL_STUB(cJSON_GetArrayItem,cJSON_GetArrayItem_stub);
            INSTALL_STUB(_tcfs_log,_tcfs_log_stub);
        }      
        static void TearDownTestCase()    
        {        
            REMOVE_STUB(cJSON_GetArrayItem_stub);
            REMOVE_STUB(_tcfs_log_stub);
        }
    protected:      
        virtual void SetUp() {}      
        virtual void TearDown() {}  
}; 


TEST_F(dba_json_decode_fail5,dba_decode_json_msg_fail)
{
    FILE *f;
    DWORD len;
    char *data;
    Json_type_Array_T tJsonTypeArray;

    memset(&tJsonTypeArray,0x00,sizeof(Json_type_Array_T));

    f=fopen("../test/ft_dba/data/Json_Type_Array_Size_Zero.json","rb");
    if(NULL == f)
    {
        fprintf(stderr,"open file failed");
    }
    
    fseek(f,0,SEEK_END);
    len=ftell(f);
    fseek(f,0,SEEK_SET);
    
    data=(char*)malloc(len+1);
    fread(data,1,len,f);
    data[len]='\0';
    fclose(f);

    EXPECT_EQ(com_decode_json_msg(data,Json_Type_Array,sizeof(Json_Type_Array)/sizeof(JSONStructItem_T),(BYTE *)&tJsonTypeArray),COM_RC_JSON_GET_ARRAY_ERR);

    free(data);
}


