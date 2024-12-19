/**
  ******************************************************************************
  * @file       ppl_gpio.c
  * @author     WenJie_Studio
  * @version    V 0.3.0
  * @date       2024-12-15
  * @brief      通用GPIO接口实现，支持多态和继承
  ******************************************************************************
  * @attention
  *
  * 版权所有 (c) [2024] [WenJie_Studio]
  ******************************************************************************
  */ 

/*------------------------------- 包含头文件区 -------------------------------*/
#include "ppl_gpio.h"
#include "ppl_gpio_private.h"
#include "ppl_gpio_stm32f1.h"

/*------------------------------- 函数实现区 -----------------------------------*/

/**
 * @brief 初始化GPIO
 * @param gpio GPIO基础结构指针
 * @return PPL_GPIO_Status 初始化状态
 */
PPL_GPIO_Status PPL_GPIO_Init(GPIO_Base* gpio) {
    if (gpio == NULL || gpio->ops == NULL || gpio->ops->init == NULL|| gpio->hw_instance == NULL) {
        return PPL_GPIO_INVALID_PARAM;
    }
    
    STM32F1_GPIO* stm_gpio = (STM32F1_GPIO*)gpio->hw_instance;
    if (stm_gpio->is_initialized) {
        return PPL_GPIO_ALREADY_INITIALIZED;
    }
    
    return gpio->ops->init(gpio);
}

/**
 * @brief 写GPIO状态
 * @param gpio GPIO基础结构指针
 * @param state 要写入的状态
 * @return PPL_GPIO_Status 写入状态
 */
PPL_GPIO_Status PPL_GPIO_Write(GPIO_Base* gpio, uint8_t state) {
    if (gpio == NULL || gpio->ops == NULL || gpio->ops->write == NULL || gpio->hw_instance == NULL) {
        return PPL_GPIO_INVALID_PARAM;
    }
    
    STM32F1_GPIO* stm_gpio = (STM32F1_GPIO*)gpio->hw_instance;
    if (!stm_gpio->is_initialized) {
        return PPL_GPIO_NOT_INITIALIZED;
    }

    // 如果GPIO被锁定，则不允许写入
    if (stm_gpio->is_locked) {
        return PPL_GPIO_LOCKED;
    }

    // 状态值只能为0或者1
    if (state > 1) {
        return PPL_GPIO_INVALID_PARAM;
    }
    
    return gpio->ops->write(gpio, state);
}

/**
 * @brief 读GPIO状态
 * @param gpio GPIO基础结构指针
 * @return uint8_t GPIO状态
 */
uint8_t PPL_GPIO_Read(GPIO_Base* gpio) {
    // 增加参数验证
    if (gpio == NULL || gpio->ops == NULL || gpio->ops->read == NULL) {
        return 0;   //默认返回低电平
    }

    return gpio->ops->read(gpio);
}

/**
 * @brief 切换GPIO状态
 * @param gpio GPIO基础结构指针
 * @return PPL_GPIO_Status 切换状态
 */
PPL_GPIO_Status PPL_GPIO_Toggle(GPIO_Base* gpio) {
    if (gpio == NULL || gpio->ops == NULL || gpio->ops->toggle == NULL|| gpio->hw_instance == NULL) {
        return PPL_GPIO_INVALID_PARAM;
    }

    // 检查硬件实例是否有效
    STM32F1_GPIO* stm_gpio = (STM32F1_GPIO*)gpio->hw_instance;
    
    // 检查是否已初始化
    if (!stm_gpio->is_initialized) {
        return PPL_GPIO_NOT_INITIALIZED;
    }
    
    // 检查是否被锁定
    if (stm_gpio->is_locked) {
        return PPL_GPIO_LOCKED;
    }

    return gpio->ops->toggle(gpio);
}

/**
 * @brief 设置GPIO模式
 * @param gpio GPIO基础结构指针
 * @param mode 要设置的模式
 * @return PPL_GPIO_Status 设置状态
 */
PPL_GPIO_Status PPL_GPIO_SetMode(GPIO_Base* gpio, uint32_t mode) {
    if (gpio == NULL || gpio->ops == NULL || gpio->ops->set_mode == NULL) {
        return PPL_GPIO_INVALID_PARAM;
    }
    return gpio->ops->set_mode(gpio, mode);
}

/**
 * @brief 设置GPIO速度
 * @param gpio GPIO基础结构指针
 * @param speed 要设置的速度
 * @return PPL_GPIO_Status 设置状态
 */
PPL_GPIO_Status PPL_GPIO_SetSpeed(GPIO_Base* gpio, uint32_t speed) {
    if (gpio == NULL || gpio->ops == NULL || gpio->ops->set_speed == NULL) {
        return PPL_GPIO_INVALID_PARAM;
    }
    return gpio->ops->set_speed(gpio, speed);
}

/**
 * @brief 创建GPIO实例的通用接口
 * @param gpio GPIO基础结构指针
 * @param hw_instance 硬件实例
 * @return PPL_GPIO_Status 创建状态
 */
PPL_GPIO_Status PPL_GPIO_Create(GPIO_Base* gpio, void* hw_instance) {
    if (gpio == NULL || hw_instance == NULL) {
        return PPL_GPIO_INVALID_PARAM;
    }
    
    // 在这里，我们只需要设置基类的hw_instance
    // 具体的初始化由各个派生类完成
    gpio->hw_instance = hw_instance;
    
    return PPL_GPIO_OK;
}
