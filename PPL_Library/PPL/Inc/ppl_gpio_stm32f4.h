/**
  ******************************************************************************
  * @file       ppl_gpio_stm32f4.h
  * @author     WenJie_Studio
  * @version    V 0.2.1
  * @date       2024-12-15
  * @brief      STM32F4系列GPIO头文件定义
  ******************************************************************************
  * @attention
  *
  * 版权所有 (c) [2024] [WenJie_Studio]
  ******************************************************************************
  */ 

#ifndef __PPL_GPIO_STM32F4_H
#define __PPL_GPIO_STM32F4_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ppl_platform_config.h"
#include "ppl_gpio.h"

#ifdef PPL_PLATFORM_STM32F4

#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"

/*------------------------------- 宏定义区 -----------------------------------*/
// STM32F4 GPIO模式定义
#define STM32F4_MODE_INPUT               GPIO_Mode_IN
#define STM32F4_MODE_OUTPUT_PP           GPIO_Mode_OUT
#define STM32F4_MODE_OUTPUT_OD           GPIO_Mode_OUT
#define STM32F4_MODE_AF_PP               GPIO_Mode_AF
#define STM32F4_MODE_AF_OD               GPIO_Mode_AF
#define STM32F4_MODE_ANALOG              GPIO_Mode_AN

// STM32F4 GPIO速度定义
#define STM32F4_SPEED_2MHZ               GPIO_Low_Speed
#define STM32F4_SPEED_25MHZ              GPIO_Medium_Speed
#define STM32F4_SPEED_50MHZ              GPIO_Fast_Speed
#define STM32F4_SPEED_100MHZ             GPIO_High_Speed

// STM32F4 GPIO上下拉定义
#define STM32F4_PUPD_NOPULL              GPIO_PuPd_NOPULL
#define STM32F4_PUPD_UP                  GPIO_PuPd_UP
#define STM32F4_PUPD_DOWN                GPIO_PuPd_DOWN

#define PPL_IS_GPIO_MODE(MODE)           (((MODE) == STM32F4_MODE_INPUT) || \
                                         ((MODE) == STM32F4_MODE_OUTPUT_PP) || \
                                         ((MODE) == STM32F4_MODE_OUTPUT_OD) || \
                                         ((MODE) == STM32F4_MODE_AF_PP) || \
                                         ((MODE) == STM32F4_MODE_AF_OD) || \
                                         ((MODE) == STM32F4_MODE_ANALOG))
                            
#define PPL_IS_GPIO_SPEED(SPEED)         (((SPEED) == STM32F4_SPEED_2MHZ) || \
                                         ((SPEED) == STM32F4_SPEED_25MHZ) || \
                                         ((SPEED) == STM32F4_SPEED_50MHZ) || \
                                         ((SPEED) == STM32F4_SPEED_100MHZ))

#define PPL_IS_GPIO_PIN(PIN)             ((PIN) != 0x00)
#define PPL_IS_GPIO_PORT(PORT)           (((PORT) == GPIOA) || ((PORT) == GPIOB) || \
                                         ((PORT) == GPIOC) || ((PORT) == GPIOD) || \
                                         ((PORT) == GPIOE) || ((PORT) == GPIOF) || \
                                         ((PORT) == GPIOG) || ((PORT) == GPIOH))

/*------------------------------- 类型定义区 -----------------------------------*/
// STM32F4 GPIO配置结构体
typedef struct {
    GPIO_TypeDef* port;             // GPIO端口
    uint16_t pin;                   // GPIO引脚
    GPIOMode_TypeDef mode;          // GPIO模式
    GPIOOType_TypeDef otype;        // 输出类型
    GPIOSpeed_TypeDef speed;        // GPIO速度
    GPIOPuPd_TypeDef pupd;          // 上下拉配置
    uint8_t af_mode;                // 复用功能模式
} STM32F4_GPIO_Config;

// STM32F4 GPIO结构体
typedef struct {
    GPIO_Base base;                 // 基类
    GPIO_TypeDef* port;             // GPIO端口
    uint16_t pin;                   // GPIO引脚
    GPIOMode_TypeDef mode;          // GPIO模式
    GPIOOType_TypeDef otype;        // 输出类型
    GPIOSpeed_TypeDef speed;        // GPIO速度
    GPIOPuPd_TypeDef pupd;          // 上下拉配置
    uint8_t af_mode;                // 复用功能模式
    bool is_locked;                 // 锁定标志
    bool is_initialized;            // 初始化标志
} STM32F4_GPIO;

/*------------------------------- 函数声明区 -----------------------------------*/
// 公共函数
PPL_GPIO_Status STM32F4_GPIO_Create(STM32F4_GPIO* gpio, STM32F4_GPIO_Config* config);
PPL_GPIO_Status STM32F4_GPIO_SetAFMode(STM32F4_GPIO* gpio, uint8_t af_mode);
PPL_GPIO_Status STM32F4_GPIO_Lock(STM32F4_GPIO* gpio);

// 内部函数（供硬件抽象层使用）
PPL_GPIO_Status GPIO_Init_Internal(GPIO_Base* self);
PPL_GPIO_Status GPIO_Write_Internal(GPIO_Base* self, uint8_t state);
uint8_t GPIO_Read_Internal(GPIO_Base* self);
PPL_GPIO_Status GPIO_Toggle_Internal(GPIO_Base* self);

#endif // PPL_PLATFORM_STM32F4

#ifdef __cplusplus
}
#endif

#endif /* __PPL_GPIO_STM32F4_H */
