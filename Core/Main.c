#include "stm32f4xx.h"
#include <stdio.h>

volatile uint16_t adc_buffer;
char uart_buf[32];

void uart_send(char *s)
{
    while (*s)
    {
        while (!(USART2->SR & USART_SR_TXE));
        USART2->DR = *s++;
    }
}

void DMA2_Stream0_IRQHandler(void)
{
    if (DMA2->LISR & DMA_LISR_TCIF0)
    {
        DMA2->LIFCR |= DMA_LIFCR_CTCIF0;

        sprintf(uart_buf, "ADC = %d\r\n", adc_buffer);
        uart_send(uart_buf);
    }
}

void uart2_init(void)
{
    RCC->APB1ENR |= RCC_APB1ENR_USART2EN;
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

    GPIOA->MODER |= (2 << GPIO_MODER_MODER2_Pos);
    GPIOA->AFR[0] |= (7 << GPIO_AFRL_AFSEL2_Pos);

    USART2->BRR = 84000000 / 115200;
    USART2->CR1 = USART_CR1_TE | USART_CR1_UE;
}

void timer2_init(void)
{
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;

    TIM2->PSC = 8399;
    TIM2->ARR = 99;

    TIM2->CR2 |= TIM_CR2_MMS_1; // TRGO on update
    TIM2->CR1 |= TIM_CR1_CEN;
}

void adc_dma_init(void)
{
    RCC->AHB1ENR |= RCC_AHB1ENR_DMA2EN;
    RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

    GPIOA->MODER |= GPIO_MODER_MODER0;

    DMA2_Stream0->PAR = (uint32_t)&ADC1->DR;
    DMA2_Stream0->M0AR = (uint32_t)&adc_buffer;
    DMA2_Stream0->NDTR = 1;

    DMA2_Stream0->CR =
        DMA_SxCR_PSIZE_0 |
        DMA_SxCR_MSIZE_0 |
        DMA_SxCR_TCIE |
        DMA_SxCR_CIRC;

    NVIC_EnableIRQ(DMA2_Stream0_IRQn);
    DMA2_Stream0->CR |= DMA_SxCR_EN;

    ADC1->SQR3 = 0;
    ADC1->CR2 =
        ADC_CR2_DMA |
        ADC_CR2_DDS |
        ADC_CR2_EXTEN_0 |
        (0b010 << ADC_CR2_EXTSEL_Pos);

    ADC1->CR2 |= ADC_CR2_ADON;
}

int main(void)
{
    uart2_init();
    adc_dma_init();
    timer2_init();

    while (1)
    {
        __WFI();
    }
}
