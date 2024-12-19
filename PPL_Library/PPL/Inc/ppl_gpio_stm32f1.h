/**
  ******************************************************************************
  * @file       ppl_gpio_stm32f1.h
  * @author     WenJie_Studio
  * @version    V 0.2.1
  * @date       2024-12-15
  * @brief      STM32F1系列GPIO头文件定义
  ******************************************************************************
  * @attention
  *
  * 版权所有 (c) [2024] [WenJie_Studio]
  ******************************************************************************
  */ 

#ifndef __PPL_GPIO_STM32F1_H
#define __PPL_GPIO_STM32F1_H

#ifdef __cplusplus
extern "C" {
#endif

/*------------------------------- 包含头文件区 -------------------------------*/
#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "ppl_gpio.h"

/*------------------------------- 宏定义区 -----------------------------------*/
// STM32F1 GPIO模式定义
#define STM32F1_MODE_INPUT               GPIO_Mode_IN_FLOATING  // 浮空输入
#define STM32F1_MODE_OUTPUT_PP           GPIO_Mode_Out_PP       // 推挽输出
#define STM32F1_MODE_OUTPUT_OD           GPIO_Mode_Out_OD       // 开漏输出
#define STM32F1_MODE_AF_PP               GPIO_Mode_AF_PP        // 复用推挽
#define STM32F1_MODE_AF_OD               GPIO_Mode_AF_OD        // 复用开漏
#define STM32F1_MODE_ANALOG              GPIO_Mode_AIN          // 模拟输入

// STM32F1 GPIO速度定义
#define STM32F1_SPEED_10MHZ              GPIO_Speed_10MHz       // 10MHz输出速度
#define STM32F1_SPEED_2MHZ               GPIO_Speed_2MHz        // 2MHz输出速度
#define STM32F1_SPEED_50MHZ              GPIO_Speed_50MHz       // 50MHz输出速度

#define PPL_IS_GPIO_MODE(MODE)           (((MODE) == STM32F1_MODE_INPUT) || \
                                         ((MODE) == STM32F1_MODE_OUTPUT_PP) || \
                                         ((MODE) == STM32F1_MODE_OUTPUT_OD) || \
                                         ((MODE) == STM32F1_MODE_AF_PP) || \
                                         ((MODE) == STM32F1_MODE_AF_OD) || \
                                         ((MODE) == STM32F1_MODE_ANALOG))
                            
#define PPL_IS_GPIO_SPEED(SPEED)         (((SPEED) == STM32F1_SPEED_10MHZ) || \
                                         ((SPEED) == STM32F1_SPEED_2MHZ) || \
                                         ((SPEED) == STM32F1_SPEED_50MHZ))

#define PPL_IS_GPIO_PIN(PIN)             ((PIN) != 0x00)
#define PPL_IS_GPIO_PORT(PORT)           (((PORT) == GPIOA) || ((PORT) == GPIOB) || \
                                         ((PORT) == GPIOC) || ((PORT) == GPIOD) || \
                                         ((PORT) == GPIOE) || ((PORT) == GPIOF) || \
                                         ((PORT) == GPIOG))

/*------------------------------- 类型定义区 -----------------------------------*/
// STM32F1 GPIO配置结构体
typedef struct {
    GPIO_TypeDef* port;             // GPIO端口
    uint16_t pin;                   // GPIO引脚
    GPIOMode_TypeDef mode;          // GPIO模式
    GPIOSpeed_TypeDef speed;        // GPIO速度
    FunctionalState remap;          // 是否重映射
    uint32_t remap_func;            // 重映射功能
} STM32F1_GPIO_Config;

// STM32F1 GPIO结构体
typedef struct {
    GPIO_Base base;                 // 基类
    GPIO_TypeDef* port;             // GPIO端口
    uint16_t pin;                   // GPIO引脚
    GPIOMode_TypeDef mode;          // GPIO模式
    GPIOSpeed_TypeDef speed;        // GPIO速度
    bool is_locked;                 // 锁定标志
    bool is_initialized;            // 初始化标志
} STM32F1_GPIO;

/*------------------------------- 函数声明区 -----------------------------------*/
// 公共函数
PPL_GPIO_Status STM32F1_GPIO_Create(STM32F1_GPIO* gpio, STM32F1_GPIO_Config* config);
PPL_GPIO_Status STM32F1_GPIO_Remap(STM32F1_GPIO* gpio, uint32_t remap_func);
PPL_GPIO_Status STM32F1_GPIO_Lock(STM32F1_GPIO* gpio);

// 内部函数（供硬件抽象层使用）
PPL_GPIO_Status GPIO_Init_Internal(GPIO_Base* self);
PPL_GPIO_Status GPIO_Write_Internal(GPIO_Base* self, uint8_t state);
uint8_t GPIO_Read_Internal(GPIO_Base* self);
PPL_GPIO_Status GPIO_Toggle_Internal(GPIO_Base* self);

#ifdef __cplusplus
}
#endif

#endif /* __PPL_GPIO_STM32F1_H */
