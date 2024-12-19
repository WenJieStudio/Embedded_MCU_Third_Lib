/**
  ******************************************************************************
  * @file       ppl_gpio.h
  * @author     WenJie_Studio
  * @version    V 0.2.0
  * @date       2024-12-15
  * @brief      通用GPIO接口定义
  ******************************************************************************
  * @attention
  *
  * 版权所有 (c) [2024] [WenJie_Studio]
  ******************************************************************************
  */ 

#ifndef __PPL_GPIO_H
#define __PPL_GPIO_H

#ifdef __cplusplus
extern "C" {
#endif

/*------------------------------- 包含头文件区 -------------------------------*/
#include <stdint.h>
#include <stdbool.h>
/*------------------------------- 枚举定义区 -----------------------------------*/
typedef enum {
    PPL_GPIO_OK = 0,                // 操作成功
    PPL_GPIO_ERROR,                 // 通用错误
    PPL_GPIO_INVALID_PARAM,         // 无效参数
    PPL_GPIO_TIMEOUT,                // 操作超时
    PPL_GPIO_INVALID_PORT,
    PPL_GPIO_INVALID_PIN,
    PPL_GPIO_INVALID_MODE,
    PPL_GPIO_INVALID_SPEED,
    PPL_GPIO_NOT_INITIALIZED,
    PPL_GPIO_ALREADY_INITIALIZED,
    PPL_GPIO_LOCKED,
    PPL_GPIO_CLOCK_ERROR,
    PPL_GPIO_RESOURCE_ERROR
} PPL_GPIO_Status;



/*------------------------------- 前置声明区 -----------------------------------*/
struct GPIO_Base;
struct GPIO_Operations;



/*------------------------------- 类型定义区 -----------------------------------*/
// GPIO操作函数指针定义
typedef PPL_GPIO_Status (*GPIO_InitFunc)        (struct GPIO_Base* self);
typedef PPL_GPIO_Status (*GPIO_WriteFunc)       (struct GPIO_Base* self, uint8_t state);
typedef uint8_t         (*GPIO_ReadFunc)        (struct GPIO_Base* self);
typedef PPL_GPIO_Status (*GPIO_ToggleFunc)      (struct GPIO_Base* self);
typedef PPL_GPIO_Status (*GPIO_SetModeFunc)     (struct GPIO_Base* self, uint32_t mode);
typedef PPL_GPIO_Status (*GPIO_SetSpeedFunc)    (struct GPIO_Base* self, uint32_t speed);

// GPIO操作函数表结构体
typedef struct GPIO_Operations {
    GPIO_InitFunc init;
    GPIO_WriteFunc write;
    GPIO_ReadFunc read;
    GPIO_ToggleFunc toggle;
    GPIO_SetModeFunc set_mode;
    GPIO_SetSpeedFunc set_speed;
} GPIO_Operations;

// GPIO基类结构体
typedef struct GPIO_Base {
    void* hw_instance;              // 硬件实例指针
    GPIO_Operations* ops;           // 操作函数表指针
} GPIO_Base;

#ifdef __cplusplus
}
#endif

#endif /* __PPL_GPIO_H */
