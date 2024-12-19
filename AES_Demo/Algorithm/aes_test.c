/**
  ******************************************************************************
  * @file       aes_test.c
  * @author     WenJie_Studio
  * @version    V 0.1.0
  * @date       2024-12-09
  * @brief      AES加密算法 验证文件
  ******************************************************************************
  * @attention
  *
  * test_aes()函数可作为后续CBC/CRT/GCM模式测试的模板，也可以直接在此函数上修改
  * 如：test_aes_cbc();  test_aes_crt();  test_aes_gcm();
  *
  * 版权所有 (c) [2024] [WenJie_Studio]
  ******************************************************************************
  */ 

#include "aes.h"  
#include <stdio.h>  

void test_aes(void) {  
    AES_CTX ctx;  
    int ret;  

    // Plaintext and key  
    const char plaintext[] = "Hello,!";  // 16 字节  
    const char key[] = "MySecretKey12345";        // 16 字节  

    uint8_t *padded_text = NULL;  
    size_t padded_len = 0;  

    uint8_t input_key[16] = {0};     // 输入密钥缓冲区  
    uint8_t output_text[256] = {0};  // 输出缓冲区，假设最大长度为 256 字节  
    uint8_t output_key[16] = {0};    // 输出密钥缓冲区  

    uint8_t *decrypted_text = NULL;  

    memcpy(input_key, key, strlen(key));  

    // 打印加密前  
    printf("Before encryption:\n");  
    printf("Plaintext: %s\n", plaintext);  
    printf("Key: %s\n", key);  
    printf("---------------------------------------------------\n");  

    // 填充明文  
    ret = pkcs7_pad((const uint8_t*)plaintext, strlen(plaintext), &padded_text, &padded_len);  
    if(ret != AES_SUCCESS) {  
        printf("Padding failed, error code: %d\n", ret);  
        return;  
    }  

    // 初始化 AES  
    ret = AES_Init(&ctx, (const uint8_t*)key, AES_KEY_SIZE_128, AES_MODE_ECB);  
    if(ret != AES_SUCCESS) {  
        printf("AES initialization failed, error code: %d\n", ret);  
        free(padded_text);  
        return;  
    }  

    // 分块加密  
    for(size_t i = 0; i < padded_len; i += AES_BLOCK_SIZE) {  
        ret = AES_EncryptBlock(&ctx, padded_text + i, output_text + i);  
        if(ret != AES_SUCCESS) {  
            printf("Plaintext encryption failed at block %zu, error code: %d\n", i/AES_BLOCK_SIZE, ret);  
            free(padded_text);  
            AES_Clear(&ctx);  
            return;  
        }  
    }  

    // 加密密钥（通常不需要加密密钥，这里仅为示例）  
    ret = AES_EncryptBlock(&ctx, input_key, output_key);  
    if(ret != AES_SUCCESS) {  
        printf("Key encryption failed, error code: %d\n", ret);  
        free(padded_text);  
        AES_Clear(&ctx);  
        return;  
    }  

    // 打印加密后  
    printf("After encryption:\n");  
    printf("Ciphertext: ");  
    for(size_t i = 0; i < padded_len; i++) {  
        printf("%02X ", output_text[i]);  
    }  
    printf("\n");  
    printf("Encrypted Key: ");  
    for(int i = 0; i < 16; i++) {  
        printf("%02X ", output_key[i]);  
    }  
    printf("\n");  
    printf("---------------------------------------------------\n");  

    // 分配解密后缓冲区  
    decrypted_text = (uint8_t *)malloc(padded_len + 1); // +1 为 NULL 终止符  
    if(!decrypted_text) {  
        printf("Memory allocation failed for decrypted text.\n");  
        free(padded_text);  
        AES_Clear(&ctx);  
        return;  
    }  
    decrypted_text[padded_len] = '\0'; // 初始化为 NULL  

    // 分块解密  
    for(size_t i = 0; i < padded_len; i += AES_BLOCK_SIZE) {  
        ret = AES_DecryptBlock(&ctx, output_text + i, decrypted_text + i);  
        if(ret != AES_SUCCESS) {  
            printf("Plaintext decryption failed at block %zu, error code: %d\n", i/AES_BLOCK_SIZE, ret);  
            free(padded_text);  
            free(decrypted_text);  
            AES_Clear(&ctx);  
            return;  
        }  
    }  

    // 去除填充  
    uint8_t *unpadded_text = NULL;  
    size_t unpadded_len = 0;  
    ret = pkcs7_unpad(decrypted_text, padded_len, &unpadded_text, &unpadded_len);  
    if(ret != AES_SUCCESS) {  
        printf("Unpadding failed, error code: %d\n", ret);  
        free(padded_text);  
        free(decrypted_text);  
        AES_Clear(&ctx);  
        return;  
    }  

    // 打印解密后  
    printf("After decryption:\n");  
    printf("Plaintext: %s\n", unpadded_text);  
    printf("Encrypted Key: %s\n", "Not displayed"); // 加密密钥不进行解密显示  
    printf("---------------------------------------------------\n");  

    // 清理  
    free(padded_text);  
    free(decrypted_text);  
    free(unpadded_text);  
    AES_Clear(&ctx);  
}  
