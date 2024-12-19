/**
  ******************************************************************************
  * @file       aes.h
  * @author     WenJie_Studio
  * @version    V 0.1.0
  * @date       2024-12-09
  * @brief      AES加密算法头文件，定义
  ******************************************************************************
  * @attention
  *
  * [特别说明或注意事项]
  *
  * 版权所有 (c) [2024] [WenJie_Studio]
  ******************************************************************************
  */

#ifndef __AES_H
#define __AES_H


/*-------------------------------包含头文件区 --------------------------------*/
#include <stdint.h>  
#include <string.h>  
#include <stdio.h>
#include <stdlib.h>

/*---------------------------------宏定义区 ----------------------------------*/
/* AES字节块大小 */
#define AES_BLOCK_SIZE      16          // 128 位

/* AES密钥长度 */
#define AES_KEY_SIZE_128    16          // 128 位  16字节
#define AES_KEY_SIZE_192    24          // 192 位  24字节
#define AES_KEY_SIZE_256    32          // 256 位  32字节

/* 加密轮次 */
#define AES_ROUNDS_128      10          // 128位密钥 10轮  
#define AES_ROUNDS_192      12          // 192位密钥 12轮  
#define AES_ROUNDS_256      14          // 256位密钥 14轮  

/* 错误代码 */
#define AES_SUCCESS            0       // 操作成功 
#define AES_INVALID_KEY_SIZE  -1       // 密钥大小无效  
#define AES_INVALID_INPUT     -2       // 输入参数无效  
#define AES_MEMORY_ERROR      -3       // 内存分配错误  
#define AES_NOT_INITIALIZED   -4       // 操作未初始化
#define AES_INVALID_MODE      -5       // 加密模式错误
#define AES_UNPADDED_ERROR    -6       // 去填充错误
#define AES_INVALID_INPUT_LENGTH -7    // 输入长度错误

/*---------------------------------类型定义区---------------------------------*/
/* AES加密模式选择 */
typedef enum {  
    AES_MODE_ECB = 0,         // Electronic Codebook  
    AES_MODE_CBC = 1,         // Cipher Block Chaining  
    AES_MODE_CTR = 2,         // Counter  
    AES_MODE_GCM = 3          // Galois/Counter Mode  
} AES_MODE;  

/* AES算法结构 */
typedef struct {  
    uint8_t round_key[240];     // 存储扩展密钥  
    uint8_t key_size;           // 密钥长度(以字节为单位)  
    uint8_t rounds;             // 加密轮数  
    AES_MODE mode;              // 加密模式  
    
    //CBC模式向量
    uint8_t iv[16];
    
    // CTR模式需要的字段  
    uint8_t counter[16];        // 计数器  
    uint8_t nonce[12];          // CTR模式的nonce  
    
    // GCM模式需要的字段  
    uint8_t hash_subkey[16];    // GHASH的子密钥  
    uint8_t ghash_state[16];    // GHASH状态  
    size_t aad_length;          // AAD长度  
    size_t data_length;         // 数据长度  
    uint8_t final_block[16];    // 最后一个块的缓存  
    uint8_t tag[16];            // 认证标签  
} AES_CTX;


/* -------------------------------函数声明区 ----------------------------------*/
/* 共有函数 */
int AES_Init(AES_CTX *ctx, const uint8_t *key, uint8_t key_size, AES_MODE mode);
int AES_EncryptBlock(AES_CTX *ctx, const uint8_t *input, uint8_t *output);
int AES_DecryptBlock(AES_CTX *ctx, const uint8_t *input, uint8_t *output);
void AES_Clear(AES_CTX *ctx);
void AES_Clear_Pro(AES_CTX *ctx);

/* 填充与去填充函数 */  
int pkcs7_pad(const uint8_t *input, size_t input_len, uint8_t **output, size_t *output_len);  
int pkcs7_unpad(const uint8_t *input, size_t input_len, uint8_t **output, size_t *output_len);  

/* CBC模式属函数 */


/* CRT模式属函数 */


/* GCM模式属函数 */

#endif /* __AES_H */
