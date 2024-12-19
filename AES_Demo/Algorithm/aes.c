/**
  ******************************************************************************
  * @file       aes.c
  * @author     WenJie_Studio
  * @version    V 0.2.0
  * @date       2024-12-09
  * @brief      AES加密算法 实现文件
  ******************************************************************************
  * @attention
  *
  * 本文件主要分为四大区域：公共函数区、CBC函数区、CRT函数区、GCM函数区。
  *  
  * 公共区主要实现了AES核心算法。后三个区内容待补充。
  * 
  * V0.1.0版本已实现最基本的ECB模式，后续模式可以复用这些函数或稍作修改
  *
  * 版权所有 (c) [2024] [WenJie_Studio]
  ******************************************************************************
  */ 
  
#include "aes.h"  
#include "aes_tables.h"  


static void KeyExtension(AES_CTX *ctx, const uint8_t *key);

/*------------------------------------------------------------------------------
                         AES各模式复用(公共)函数区
------------------------------------------------------------------------------*/

/*******************************************************************************
 * @brief    GF(2^8)上的乘法 ----乘以2
 * @param    x  输入值
 * @return   结果
 ******************************************************************************/
static uint8_t xtime(uint8_t x) {  
    return ((x << 1) ^ (((x >> 7) & 1) * 0x1b));  
}  

/*******************************************************************************
 * @brief    GF(2^8)上的乘法 ----任意乘法
 * @param    
 * @return    
 ******************************************************************************/
static uint8_t mul(uint8_t x, uint8_t y) {  
    uint8_t result = 0;  
    
    for(int i = 0; i < 8; i++) {  
        if(y & 1) {  
            result ^= x;  
        }  
        x = xtime(x);  
        y >>= 1;  
    }  
    return result;  
}  


/*******************************************************************************
 * @brief    AES初始化函数  AES_Init
 * @param    ctx            AES上下文指针
 * @param    key            加密秘钥
 * @param    key_size       秘钥大小(16/24/32 bits)
 * @param    mode           加密模式
 * @return   AES_SUCCESS 或 错误代码
 ******************************************************************************/
int AES_Init(AES_CTX *ctx, const uint8_t *key, uint8_t key_size, AES_MODE mode) {
    if(!ctx || !key){
        return AES_INVALID_INPUT;
    }
    
    if(key_size != AES_KEY_SIZE_128 && key_size != AES_KEY_SIZE_192 && key_size != AES_KEY_SIZE_256){
        return AES_INVALID_KEY_SIZE;
    }
 
    //检查都正确，进行初始化
    ctx -> key_size = key_size;
    ctx -> mode = mode;
    
    //选择加密轮数
    switch(key_size){
        case AES_KEY_SIZE_128:ctx->rounds = AES_ROUNDS_128;break;
        case AES_KEY_SIZE_192:ctx->rounds = AES_ROUNDS_192;break;
        case AES_KEY_SIZE_256:ctx->rounds = AES_ROUNDS_256;break;
    }
    
    //初始密钥扩展
    KeyExtension(ctx, key);
    
    return AES_SUCCESS;
 }
 
 
/*******************************************************************************
 * @brief    秘钥扩展函数
 * @param    ctx    AES上下文指针
 * @param    key    加密秘钥
 * @return    
 ******************************************************************************/
 static void KeyExtension(AES_CTX *ctx, const uint8_t *key) {  
     uint32_t i, j;
     uint8_t temp[4];
     uint8_t *round_key = ctx->round_key;
     
     //复制密钥
     memcpy(round_key, key, ctx->key_size);
     
     //密钥扩展
     i = ctx->key_size;
     while(i < (ctx->rounds +1 ) * 16){
        for(j = 0;j < 4 ;j++){
            temp[j] = round_key[i -4 + j];
        }
        
        if(i % ctx->key_size == 0)
        {
            //字循环
            uint8_t k = temp[0];
            temp[0] = temp[1];  
            temp[1] = temp[2];  
            temp[2] = temp[3];  
            temp[3] = k;
            
            //Sbox替换
            temp[0] = Sbox[temp[0]];  
            temp[1] = Sbox[temp[1]];  
            temp[2] = Sbox[temp[2]];  
            temp[3] = Sbox[temp[3]];
            
            //轮常量异或
            temp[0] ^= RCON[i/ctx->key_size - 1]; 
        }
        
        //生成新密钥
        for (j = 0; j < 4;j++){
            round_key[i] = round_key[i -ctx->key_size] ^ temp[j];
            i++;
        }
     }
 }
 //暂时缺少对“密钥长度 24字节，加密轮次10次”、“密钥长度32字节，加密轮次14次”这两种情况的代码
 
/*******************************************************************************
 * @brief    字节代换
 * @param    state    状态矩阵
 * @return    
 ******************************************************************************/
static void SubBytes(uint8_t *state) {
    for (int i = 0; i < 16; i++) {  
        state[i] = Sbox[state[i]];  
    }  
}


/*******************************************************************************
 * @brief    逆字节代换
 * @param    state    状态矩阵
 * @return    
 ******************************************************************************/
static void InvSubBytes(uint8_t *state){
    for (int i = 0;i < 16;i++){
        state[i] = InvSbox[state[i]];
    }
}

/*******************************************************************************
 * @brief    行位移
 * @param    state    状态矩阵
 * @return    
 ******************************************************************************/
static void ShiftRows(uint8_t *state){
    uint8_t temp;
    
    //第二行左移一位
    temp = state[1];
    state[1] = state[5];
    state[5] = state[9];
    state[9] = state[13];
    state[13] = temp;
    
    //第三行左移两位
    temp = state[2];  
    state[2] = state[10];  
    state[10] = temp;  
    temp = state[6];  
    state[6] = state[14];  
    state[14] = temp;
    
    //第四行左移三位
    temp = state[15];  
    state[15] = state[11];  
    state[11] = state[7];  
    state[7] = state[3];  
    state[3] = temp;  
}


/*******************************************************************************
 * @brief    逆行位移
 * @param    state    状态矩阵
 * @return    
 ******************************************************************************/
static void InvShiftRows(uint8_t *state) {  
    uint8_t temp;  
    
    /* 第二行右移一位 */  
    temp = state[13];  
    state[13] = state[9];  
    state[9] = state[5];  
    state[5] = state[1];  
    state[1] = temp;  
    
    /* 第三行右移两位 */  
    temp = state[2];  
    state[2] = state[10];  
    state[10] = temp;  
    temp = state[6];  
    state[6] = state[14];  
    state[14] = temp;  
    
    /* 第四行右移三位 */  
    temp = state[3];  
    state[3] = state[7];  
    state[7] = state[11];  
    state[11] = state[15];  
    state[15] = temp;  
}  


/*******************************************************************************
 * @brief    列混淆
 * @param    state    状态矩阵
 * @return    
 ******************************************************************************/
static void MixColumns(uint8_t *state) {  
    uint8_t temp[4];  
    for (int i = 0; i < 4; i++) {  
        temp[0] = state[i*4];  
        temp[1] = state[i*4+1];  
        temp[2] = state[i*4+2];  
        temp[3] = state[i*4+3];  
        
        state[i*4]   = xtime(temp[0]) ^ xtime(temp[1]) ^ temp[1] ^ temp[2] ^ temp[3];  
        state[i*4+1] = temp[0] ^ xtime(temp[1]) ^ xtime(temp[2]) ^ temp[2] ^ temp[3];  
        state[i*4+2] = temp[0] ^ temp[1] ^ xtime(temp[2]) ^ xtime(temp[3]) ^ temp[3];  
        state[i*4+3] = xtime(temp[0]) ^ temp[0] ^ temp[1] ^ temp[2] ^ xtime(temp[3]);  
    }  
}  


/*******************************************************************************
 * @brief    逆列混淆
 * @param    state    状态矩阵
 * @return    
 ******************************************************************************/
static void InvMixColumns(uint8_t *state) {  
    uint8_t temp[4];  
    
    for(int i = 0; i < 4; i++) {  
        // 保存当前列的值  
        for(int j = 0; j < 4; j++) {  
            temp[j] = state[i*4 + j];  
        }  
        
        // 计算新的列值  
        state[i*4]   = mul(temp[0], 0x0E) ^ mul(temp[1], 0x0B) ^   
                       mul(temp[2], 0x0D) ^ mul(temp[3], 0x09);  
        state[i*4+1] = mul(temp[0], 0x09) ^ mul(temp[1], 0x0E) ^   
                       mul(temp[2], 0x0B) ^ mul(temp[3], 0x0D);  
        state[i*4+2] = mul(temp[0], 0x0D) ^ mul(temp[1], 0x09) ^   
                       mul(temp[2], 0x0E) ^ mul(temp[3], 0x0B);  
        state[i*4+3] = mul(temp[0], 0x0B) ^ mul(temp[1], 0x0D) ^   
                       mul(temp[2], 0x09) ^ mul(temp[3], 0x0E);  
    }  
}  


/*******************************************************************************
 * @brief    添加轮密钥
 * @param    state        状态矩阵
 * @param    round_key    轮密钥
 * @return    
 ******************************************************************************/
static void AddRoundKey(uint8_t *state, const uint8_t *round_key) {  
    for (int i = 0; i < 16; i++) {  
        state[i] ^= round_key[i];  
    }  
}  


/*******************************************************************************  
 * @brief    加密单个块(修改后支持填充)  
 * @param    ctx       AES指针  
 * @param    input     输入数据  
 * @param    output    输出数据  
 * @param    length    输入数据长度  
 * @return   成功或错误标志  
 ******************************************************************************/  
int AES_EncryptBlock(AES_CTX *ctx, const uint8_t *input, uint8_t *output){
    if( !ctx || !input || !output ){
        return AES_INVALID_INPUT;
    }
    
    if(ctx->key_size == 0 || ctx->rounds == 0) {  
        return AES_NOT_INITIALIZED;  
    }  

    //复制输入数据到输出缓冲区
    memcpy(output, input, 16);
    
    //初始轮密钥加
    AddRoundKey(output, ctx->round_key);
    
    //主轮密钥加
    for(int round = 1;round < ctx->rounds;round++) {
        SubBytes(output);
        ShiftRows(output);
        MixColumns(output);
        AddRoundKey(output, ctx->round_key + round * 16);
    }
    
    //最后一轮
    SubBytes(output);
    ShiftRows(output);
    AddRoundKey(output, ctx->round_key + ctx->rounds * 16);
    
    return AES_SUCCESS;
}

/*******************************************************************************  
 * @brief    解密单个块(修改后支持去填充)  
 * @param    ctx       AES指针  
 * @param    input     输入数据  
 * @param    output    输出数据  
 * @param    length    输入数据长度  
 * @return   成功或错误标志  
 ******************************************************************************/  
int AES_DecryptBlock(AES_CTX *ctx, const uint8_t *input, uint8_t *output) {  
    if (!ctx || !input || !output) {  
        return AES_INVALID_INPUT;  
    }  
    
    /* 复制输入数据到输出缓冲区 */  
    memcpy(output, input, 16);  
    
    /* 初始轮密钥加 */  
    AddRoundKey(output, ctx->round_key + ctx->rounds * 16);  
    
    /* 主解密轮 */  
    for (int round = ctx->rounds - 1; round > 0; round--) {  
        InvShiftRows(output);  
        InvSubBytes(output);  
        AddRoundKey(output, ctx->round_key + round * 16);  
        InvMixColumns(output);  
    }  
    
    /* 最后一轮 */  
    InvShiftRows(output);  
    InvSubBytes(output);  
    AddRoundKey(output, ctx->round_key);  
    
    return AES_SUCCESS;  
}  


/*******************************************************************************
 * @brief    内存清理函数 ---- 简单版
 * @param    
 * @return    
 ******************************************************************************/
void AES_Clear(AES_CTX *ctx) {  
    if(ctx) {  
        // 清除密钥相关内存  
        memset(ctx->round_key, 0, sizeof(ctx->round_key));  
        ctx->key_size = 0;  
        ctx->rounds = 0;  
    }  
}

/*******************************************************************************
 * @brief    内存清理函数 ---- 完整版
 * @param    
 * @return    
 ******************************************************************************/
void AES_Clear_Pro(AES_CTX *ctx){
    
}

/*******************************************************************************  
 * @brief    PKCS7填充函数  
 * @param    
 * @param      
 * @param     
 * @return    
 ******************************************************************************/  
int pkcs7_pad(const uint8_t *input, size_t input_len, uint8_t **output, size_t *output_len) {  
    if (!input || !output || !output_len) {  
        return AES_INVALID_INPUT;  
    }  
    
    size_t pad_len = AES_BLOCK_SIZE - (input_len % AES_BLOCK_SIZE);  
    *output_len = input_len + pad_len;  
    *output = (uint8_t *)malloc(*output_len);  
    if (!*output) {  
        return AES_MEMORY_ERROR;  
    }  
    
    memcpy(*output, input, input_len);  
    memset(*output + input_len, (uint8_t)pad_len, pad_len);  
    
    return AES_SUCCESS;  
}  

/*******************************************************************************  
 * @brief    PKCS7去填充函数  
 * @param     
 * @param     
 * @return     
 ******************************************************************************/  
int pkcs7_unpad(const uint8_t *input, size_t input_len, uint8_t **output, size_t *output_len) {  
    if (!input || !output || !output_len || input_len == 0 || input_len % AES_BLOCK_SIZE != 0) {  
        return AES_INVALID_INPUT;  
    }  
    
    uint8_t pad_len = input[input_len - 1];  
    if (pad_len == 0 || pad_len > AES_BLOCK_SIZE) {  
        return AES_INVALID_INPUT;  
    }  
    
    // 验证所有填充字节是否合法  
    for (size_t i = 0; i < pad_len; i++) {  
        if (input[input_len - 1 - i] != pad_len) {  
            return AES_INVALID_INPUT;  
        }  
    }  
    
    *output_len = input_len - pad_len;  
    *output = (uint8_t *)malloc(*output_len + 1); // +1 为了添加 NULL 终止符  
    if (!*output) {  
        return AES_MEMORY_ERROR;  
    }  
    
    memcpy(*output, input, *output_len);  
    (*output)[*output_len] = '\0'; // 适用于字符串  
    
    return AES_SUCCESS;  
}  


/*------------------------------------------------------------------------------
                         AES算法CBC模式函数区
------------------------------------------------------------------------------*/


/*******************************************************************************
 * @brief    CBC初始化函数
 * @param    
 * @return    
 ******************************************************************************/

/*******************************************************************************
 * @brief    CBC加密函数 AES_CBC_Encrypt
 * @param    
 * @return    
 ******************************************************************************/

/*******************************************************************************
 * @brief    CBC解密函数 AES_CBC_Decrypt
 * @param    
 * @return    
 ******************************************************************************/



/*------------------------------------------------------------------------------
                           AES算法CRT模式函数区
------------------------------------------------------------------------------*/

/*******************************************************************************
 * @brief    AES_CRT_Init
 * @param    
 * @return    
 ******************************************************************************/


/*******************************************************************************
 * @brief    加密/解密 AES_CTR_Process
 * @param    
 * @return    
 ******************************************************************************/

/*******************************************************************************
 * @brief    计数器递增函数 void CTR_IncCounter
 * @param    
 * @return    
 ******************************************************************************/

/*------------------------------------------------------------------------------
                           AES算法GCM模式函数区
------------------------------------------------------------------------------*/

/*******************************************************************************
 * @brief    AES_GCM_Init
 * @param    
 * @return    
 ******************************************************************************/


/*******************************************************************************
 * @brief    GCM加密函数 AES_GCM_Encrypt
 * @param    
 * @return    
 ******************************************************************************/

/*******************************************************************************
 * @brief    GCM解密函数 AES_GCM_Decrypt
 * @param    
 * @return    
 ******************************************************************************/

/*******************************************************************************
 * @brief    额外认证 AES_GCM_UpdateAAD
 * @param    
 * @return    
 ******************************************************************************/


/*******************************************************************************
 * @brief    生成/验证标签 AES_GCM_Final
 * @param    
 * @return    
 ******************************************************************************/

/*******************************************************************************
 * @brief    伽罗华域乘法
 * @param    
 * @return    
 ******************************************************************************/

/*******************************************************************************
 * @brief    哈希函数
 * @param    
 * @return    
 ******************************************************************************/
