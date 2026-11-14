#include "DMA.h"
#include <assert.h>
DMA_TypeDef controllers[2];
DMA_Stream_TypeDef streams[2][8];
RCC_TypeDef rcc;
void DMA1_Stream0_IRQHandler(void);
int main(void) {
 DMA_Config c = {0}; c.Request=DMA_Configuration.Request.SPI1_RX;
 c.interrupts=DMA_Configuration.DMA_Interrupts.Fifo_Error | DMA_Configuration.DMA_Interrupts.Transfer_Complete;
 c.Request.Stream->CR=~0U;
 assert(DMA_Init(&c)==1);
 assert((c.Request.Stream->CR & DMA_SxCR_CIRC)==0);
 assert(c.Request.Stream->FCR & DMA_SxFCR_FEIE);
 assert(c.Request.Stream->CR & (1U<<4));
 c.Request.Stream->NDTR=2; DMA_Set_Trigger(&c);
 assert(DMA2->LIFCR==0x3D);
 c.Request.Controller=DMA1; assert(DMA_Init(&c)==-1);
 DMA1->LISR=DMA_LISR_HTIF0|DMA_LISR_TCIF0;
 DMA1_Stream0_IRQHandler();
 assert(DMA1_Stream0_Flag.Half_Transfer_Complete_Flag && DMA1_Stream0_Flag.Transfer_Complete_Flag);
 DMA_Reset_Flags(&DMA1_Stream0_Flag);
 assert(!DMA1_Stream0_Flag.Transfer_Complete_Flag);
 assert(DMA_Configuration.Request._DAC1.Stream==DMA1_Stream5);
 assert(DMA_Configuration.Request._DAC2.Stream==DMA1_Stream6);
 return 0;
}
