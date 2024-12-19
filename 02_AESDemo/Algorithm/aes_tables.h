/**
  ******************************************************************************
  * @file       aes_tables.h
  * @author     WenJie_Studio
  * @version    V 0.1.0
  * @date       2024-12-09
  * @brief      AES加密算法常量表头文件
  ******************************************************************************
  * @attention
  *
  * 若定义或自定义了其它表，在这里进行外部引用声明。
  *
  * 版权所有 (c) [2024] [WenJie_Studio]
  ******************************************************************************
  */ 

#ifndef __AES_TABLES_H  
#define __AES_TABLES_H  

#include <stdint.h>  

extern const uint8_t Sbox[256];  
extern const uint8_t InvSbox[256];  
extern const uint8_t RCON[10]; 

#endif /* __AES_TABLES_H */
