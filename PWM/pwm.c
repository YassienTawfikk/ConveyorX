//
// Created by Malak on 6/5/2025.
//

#include <Rcc.h>
#include "pwm.h"
#include "pwm_private.h"
#include "GPIO.h"


/*******************************************************************
 *                      Function Definition                        *
 *******************************************************************/

void PWM_Init(void) {
    GPIO_Init(GPIO_A, 8, GPIO_AF, GPIO_PUSH_PULL);

    /* set AF1 for PA8 (TIM1_CH1) */
    GPIO_SetAlternateFunction(GPIO_A, 8, 1); // TIM1_CH1 uses AF1

    /* enable clock for the PWM */
    Rcc_Enable(RCC_TIM1);

    /* set prescaler to 79 to make timer clock 1 MHz ---> 80 MHz / (79 + 1) = 1 MHz  */
    TIM1->TIMx_PSC = 79;
    /* set auto reload to 1000 so PWM frequency will be 1 Khz ---> 1Mhz / 1000 = 1 Khz */
    TIM1->TIMx_ARR = 1023;
    /* initial duty cycle 0% */
    TIM1->TIMx_CCR1 = 0;

    /* clear CC1P bit */
    TIM1->TIMx_CCER &= ~(1 << 1);
    /* force update event */
    TIM1->TIMx_EGR |= (1 << 0);

    /* to configure Channel 1 */
    /* clear OC1M bits */
    TIM1->TIMx_CCMR1 &= ~(0x7 << 4);
    /* set PWM mode 1 */
    TIM1->TIMx_CCMR1 |= (0x6 << 4);
    /* enable preload */
    TIM1->TIMx_CCMR1 |= (1 << 3);


    /* edge aligned mode */
    TIM1->TIMx_CR1 &= ~(0x3 << 5);
    /* enable auto-reload */
    TIM1->TIMx_CR1 |= (1 << 7);


    /* enable channel 1 output */
    TIM1->TIMx_CCER |= (1 << 0);
    /* enable main output */
    TIM1->TIMx_BDTR |= (1 << 15);
    /* enable the counter */
    TIM1->TIMx_CR1 |= (1 << 0);
}

void PWM_SetDutyCycle(uint16 duty) {
    if (duty > 1023) duty = 1023;
    TIM1->TIMx_CCR1 = duty;
}

void PWM_Stop(void) {
    /* set the duty cycle to 0 */
    TIM1->TIMx_CCR1 = 0;
}
