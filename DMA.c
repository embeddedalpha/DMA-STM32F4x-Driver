/**
 * @file DMA.c
 * @brief DMA Driver Implementation for STM32F407VGT6
 *
 * This file provides the implementation of the DMA driver for the STM32F407VGT6 microcontroller.
 * It includes functions to initialize DMA, handle DMA interrupts, configure DMA streams,
 * and manage memory-to-memory transfers.
 *
 * @version 1.0
 * @date 2024-08-21
 *
 * @author Your Name
 * @copyright Copyright (c) 2024
 */

#include "DMA.h"


/* Compare stream identities: CMSIS stream objects are not a C array. */
static int DMA_Stream_Index(const DMA_Config *config)
{
    if (!config) return -1;
    DMA_Stream_TypeDef *const stream_table[2][8] = {
        {DMA1_Stream0,DMA1_Stream1,DMA1_Stream2,DMA1_Stream3,DMA1_Stream4,DMA1_Stream5,DMA1_Stream6,DMA1_Stream7},
        {DMA2_Stream0,DMA2_Stream1,DMA2_Stream2,DMA2_Stream3,DMA2_Stream4,DMA2_Stream5,DMA2_Stream6,DMA2_Stream7}
    };
    int controller = config->Request.Controller == DMA1 ? 0 :
                     config->Request.Controller == DMA2 ? 1 : -1;
    if (controller < 0) return -1;
    for (int i=0;i<8;++i) if (config->Request.Stream == stream_table[controller][i]) return i;
    return -1;
}
static int DMA_Stop(DMA_Stream_TypeDef *stream)
{
    uint32_t timeout = 1000000U;
    stream->CR &= ~DMA_SxCR_EN;
    while (stream->CR & DMA_SxCR_EN) if (--timeout == 0U) return -1;
    return 1;
}

// DMA Flags for each stream of DMA1 and DMA2
DMA_Flags_Typedef DMA1_Stream0_Flag;
DMA_Flags_Typedef DMA1_Stream1_Flag;
DMA_Flags_Typedef DMA1_Stream2_Flag;
DMA_Flags_Typedef DMA1_Stream3_Flag;
DMA_Flags_Typedef DMA1_Stream4_Flag;
DMA_Flags_Typedef DMA1_Stream5_Flag;
DMA_Flags_Typedef DMA1_Stream6_Flag;
DMA_Flags_Typedef DMA1_Stream7_Flag;

DMA_Flags_Typedef DMA2_Stream0_Flag;
DMA_Flags_Typedef DMA2_Stream1_Flag;
DMA_Flags_Typedef DMA2_Stream2_Flag;
DMA_Flags_Typedef DMA2_Stream3_Flag;
DMA_Flags_Typedef DMA2_Stream4_Flag;
DMA_Flags_Typedef DMA2_Stream5_Flag;
DMA_Flags_Typedef DMA2_Stream6_Flag;
DMA_Flags_Typedef DMA2_Stream7_Flag;

/**
 * @brief DMA1 Stream 0 Interrupt Handler
 *
 * This function handles the interrupt for DMA1 Stream 0. It checks the
 * status flags for FIFO error, direct mode error, transfer error, half
 * transfer complete, and transfer complete, and clears the respective
 * interrupt flag after handling it.
 */
void DMA1_Stream0_IRQHandler(void)
{
	if(DMA1 -> LISR & DMA_LISR_FEIF0)
	{
		DMA1_Stream0_Flag.Fifo_Error_Flag = true;
		DMA1 -> LIFCR = DMA_LIFCR_CFEIF0;
	}
	/************************************************************************************************************/
	if(DMA1 -> LISR & DMA_LISR_DMEIF0)
	{
		DMA1_Stream0_Flag.Direct_Mode_Error_Flag = true;
		DMA1 -> LIFCR = DMA_LIFCR_CDMEIF0;
	}
	/************************************************************************************************************/
	if(DMA1 -> LISR & DMA_LISR_TEIF0)
	{
		DMA1_Stream0_Flag.Transfer_Error_Flag = true;
		DMA1 -> LIFCR = DMA_LIFCR_CTEIF0;
	}
	/************************************************************************************************************/
	if(DMA1 -> LISR & DMA_LISR_HTIF0)
	{
		DMA1_Stream0_Flag.Half_Transfer_Complete_Flag = true;
		DMA1 -> LIFCR = DMA_LIFCR_CHTIF0;
	}
	/************************************************************************************************************/
	if(DMA1 -> LISR & DMA_LISR_TCIF0)
	{
		DMA1_Stream0_Flag.Transfer_Complete_Flag = true;
		DMA1 -> LIFCR = DMA_LIFCR_CTCIF0;
	}
}

/**
 * @brief DMA1 Stream 1 Interrupt Handler
 *
 * This function handles the interrupt for DMA1 Stream 1. It checks the
 * status flags for different events such as FIFO error, direct mode error,
 * transfer error, half transfer complete, and transfer complete. For each
 * event, it sets the corresponding flag in the `DMA1_Stream1_Flag` structure
 * and clears the respective interrupt flag in the DMA interrupt flag clear register.
 */
void DMA1_Stream1_IRQHandler(void)
{
	if(DMA1 -> LISR & DMA_LISR_FEIF1)
	{
		DMA1_Stream1_Flag.Fifo_Error_Flag = true;
		DMA1 -> LIFCR = DMA_LIFCR_CFEIF1;
	}
	/************************************************************************************************************/
	if(DMA1 -> LISR & DMA_LISR_DMEIF1)
	{
		DMA1_Stream1_Flag.Direct_Mode_Error_Flag = true;
		DMA1 -> LIFCR = DMA_LIFCR_CDMEIF1;
	}
	/************************************************************************************************************/
	if(DMA1 -> LISR & DMA_LISR_TEIF1)
	{
		DMA1_Stream1_Flag.Transfer_Error_Flag = true;
		DMA1 -> LIFCR = DMA_LIFCR_CTEIF1;
	}
	/************************************************************************************************************/
	if(DMA1 -> LISR & DMA_LISR_HTIF1)
	{
		DMA1_Stream1_Flag.Half_Transfer_Complete_Flag = true;
		DMA1 -> LIFCR = DMA_LIFCR_CHTIF1;
	}
	/************************************************************************************************************/
	if(DMA1 -> LISR & DMA_LISR_TCIF1)
	{
		DMA1_Stream1_Flag.Transfer_Complete_Flag = true;
		DMA1 -> LIFCR = DMA_LIFCR_CTCIF1;
	}
}
/**
 * @brief DMA1 Stream 2 Interrupt Handler
 *
 * This function handles the interrupt for DMA1 Stream 2. It checks the
 * status flags for different events such as FIFO error, direct mode error,
 * transfer error, half transfer complete, and transfer complete. For each
 * event, it sets the corresponding flag in the `DMA1_Stream2_Flag` structure
 * and clears the respective interrupt flag in the DMA interrupt flag clear register.
 */
void DMA1_Stream2_IRQHandler(void)
{
	if(DMA1 -> LISR & DMA_LISR_FEIF2)
	{
		DMA1_Stream2_Flag.Fifo_Error_Flag = true;
		DMA1 -> LIFCR = DMA_LIFCR_CFEIF2;
	}
	/************************************************************************************************************/
	if(DMA1 -> LISR & DMA_LISR_DMEIF2)
	{
		DMA1_Stream2_Flag.Direct_Mode_Error_Flag = true;
		DMA1 -> LIFCR = DMA_LIFCR_CDMEIF2;
	}
	/************************************************************************************************************/
	if(DMA1 -> LISR & DMA_LISR_TEIF2)
	{
		DMA1_Stream2_Flag.Transfer_Error_Flag = true;
		DMA1 -> LIFCR = DMA_LIFCR_CTEIF2;
	}
	/************************************************************************************************************/
	if(DMA1 -> LISR & DMA_LISR_HTIF2)
	{
		DMA1_Stream2_Flag.Half_Transfer_Complete_Flag = true;
		DMA1 -> LIFCR = DMA_LIFCR_CHTIF2;
	}
	/************************************************************************************************************/
	if(DMA1 -> LISR & DMA_LISR_TCIF2)
	{
		DMA1_Stream2_Flag.Transfer_Complete_Flag = true;
		DMA1 -> LIFCR = DMA_LIFCR_CTCIF2;
	}
}

/**
 * @brief DMA1 Stream 3 Interrupt Handler
 *
 * This function handles the interrupt for DMA1 Stream 3. It checks the
 * status flags for different events such as FIFO error, direct mode error,
 * transfer error, half transfer complete, and transfer complete. For each
 * event, it sets the corresponding flag in the `DMA1_Stream3_Flag` structure
 * and clears the respective interrupt flag in the DMA interrupt flag clear register.
 */
void DMA1_Stream3_IRQHandler(void)
{
	if(DMA1 -> LISR & DMA_LISR_FEIF3)
	{
		DMA1_Stream3_Flag.Fifo_Error_Flag = true;
		DMA1 -> LIFCR = DMA_LIFCR_CFEIF3;
	}
	/************************************************************************************************************/
	if(DMA1 -> LISR & DMA_LISR_DMEIF3)
	{
		DMA1_Stream3_Flag.Direct_Mode_Error_Flag = true;
		DMA1 -> LIFCR = DMA_LIFCR_CDMEIF3;
	}
	/************************************************************************************************************/
	if(DMA1 -> LISR & DMA_LISR_TEIF3)
	{
		DMA1_Stream3_Flag.Transfer_Error_Flag = true;
		DMA1 -> LIFCR = DMA_LIFCR_CTEIF3;
	}
	/************************************************************************************************************/
	if(DMA1 -> LISR & DMA_LISR_HTIF3)
	{
		DMA1_Stream3_Flag.Half_Transfer_Complete_Flag = true;
		DMA1 -> LIFCR = DMA_LIFCR_CHTIF3;
	}
	/************************************************************************************************************/
	if(DMA1 -> LISR & DMA_LISR_TCIF3)
	{
		DMA1_Stream3_Flag.Transfer_Complete_Flag = true;
		DMA1 -> LIFCR = DMA_LIFCR_CTCIF3;
	}
}

/**
 * @brief DMA1 Stream 4 Interrupt Handler
 *
 * This function handles the interrupt for DMA1 Stream 4. It checks the
 * status flags for different events such as FIFO error, direct mode error,
 * transfer error, half transfer complete, and transfer complete. For each
 * event, it sets the corresponding flag in the `DMA1_Stream4_Flag` structure
 * and clears the respective interrupt flag in the DMA interrupt flag clear register.
 */
void DMA1_Stream4_IRQHandler(void)
{
	if(DMA1 -> HISR & DMA_HISR_FEIF4)
	{
		DMA1_Stream4_Flag.Fifo_Error_Flag = true;
		DMA1 -> HIFCR = DMA_HIFCR_CFEIF4;
	}
	/************************************************************************************************************/
	if(DMA1 -> HISR & DMA_HISR_DMEIF4)
	{
		DMA1_Stream4_Flag.Direct_Mode_Error_Flag = true;
		DMA1 -> HIFCR = DMA_HIFCR_CDMEIF4;
	}
	/************************************************************************************************************/
	if(DMA1 -> HISR & DMA_HISR_TEIF4)
	{
		DMA1_Stream4_Flag.Transfer_Error_Flag = true;
		DMA1 -> HIFCR = DMA_HIFCR_CTEIF4;
	}
	/************************************************************************************************************/
	if(DMA1 -> HISR & DMA_HISR_HTIF4)
	{
		DMA1_Stream4_Flag.Half_Transfer_Complete_Flag = true;
		DMA1 -> HIFCR = DMA_HIFCR_CHTIF4;
	}
	/************************************************************************************************************/
	if(DMA1 -> HISR & DMA_HISR_TCIF4)
	{
		DMA1_Stream4_Flag.Transfer_Complete_Flag = true;
		DMA1 -> HIFCR = DMA_HIFCR_CTCIF4;
	}
}

/**
 * @brief DMA1 Stream 5 Interrupt Handler
 *
 * This function handles the interrupt for DMA1 Stream 5. It checks the
 * status flags for different events such as FIFO error, direct mode error,
 * transfer error, half transfer complete, and transfer complete. For each
 * event, it sets the corresponding flag in the `DMA1_Stream5_Flag` structure
 * and clears the respective interrupt flag in the DMA interrupt flag clear register.
 */
void DMA1_Stream5_IRQHandler(void)
{
	if(DMA1 -> HISR & DMA_HISR_FEIF5)
	{
		DMA1_Stream5_Flag.Fifo_Error_Flag = true;
		DMA1 -> HIFCR = DMA_HIFCR_CFEIF5;
	}
	/************************************************************************************************************/
	if(DMA1 -> HISR & DMA_HISR_DMEIF5)
	{
		DMA1_Stream5_Flag.Direct_Mode_Error_Flag = true;
		DMA1 -> HIFCR = DMA_HIFCR_CDMEIF5;
	}
	/************************************************************************************************************/
	if(DMA1 -> HISR & DMA_HISR_TEIF5)
	{
		DMA1_Stream5_Flag.Transfer_Error_Flag = true;
		DMA1 -> HIFCR = DMA_HIFCR_CTEIF5;
	}
	/************************************************************************************************************/
	if(DMA1 -> HISR & DMA_HISR_HTIF5)
	{
		DMA1_Stream5_Flag.Half_Transfer_Complete_Flag = true;
		DMA1 -> HIFCR = DMA_HIFCR_CHTIF5;
	}
	/************************************************************************************************************/
	if(DMA1 -> HISR & DMA_HISR_TCIF5)
	{
		DMA1_Stream5_Flag.Transfer_Complete_Flag = true;
		DMA1 -> HIFCR = DMA_HIFCR_CTCIF5;
	}
}

/**
 * @brief DMA1 Stream 6 Interrupt Handler
 *
 * This function handles the interrupt for DMA1 Stream 6. It checks the
 * status flags for different events such as FIFO error, direct mode error,
 * transfer error, half transfer complete, and transfer complete. For each
 * event, it sets the corresponding flag in the `DMA1_Stream6_Flag` structure
 * and clears the respective interrupt flag in the DMA interrupt flag clear register.
 */
void DMA1_Stream6_IRQHandler(void)
{
	if(DMA1 -> HISR & DMA_HISR_FEIF6)
	{
		DMA1_Stream6_Flag.Fifo_Error_Flag = true;
		DMA1 -> HIFCR = DMA_HIFCR_CFEIF6;
	}
	/************************************************************************************************************/
	if(DMA1 -> HISR & DMA_HISR_DMEIF6)
	{
		DMA1_Stream6_Flag.Direct_Mode_Error_Flag = true;
		DMA1 -> HIFCR = DMA_HIFCR_CDMEIF6;
	}
	/************************************************************************************************************/
	if(DMA1 -> HISR & DMA_HISR_TEIF6)
	{
		DMA1_Stream6_Flag.Transfer_Error_Flag = true;
		DMA1 -> HIFCR = DMA_HIFCR_CTEIF6;
	}
	/************************************************************************************************************/
	if(DMA1 -> HISR & DMA_HISR_HTIF6)
	{
		DMA1_Stream6_Flag.Half_Transfer_Complete_Flag = true;
		DMA1 -> HIFCR = DMA_HIFCR_CHTIF6;
	}
	/************************************************************************************************************/
	if(DMA1 -> HISR & DMA_HISR_TCIF6)
	{
		DMA1_Stream6_Flag.Transfer_Complete_Flag = true;
		DMA1 -> HIFCR = DMA_HIFCR_CTCIF6;
	}
}

/**
 * @brief DMA1 Stream 7 Interrupt Handler
 *
 * This function handles the interrupt for DMA1 Stream 7. It checks the
 * status flags for different events such as FIFO error, direct mode error,
 * transfer error, half transfer complete, and transfer complete. For each
 * event, it sets the corresponding flag in the `DMA1_Stream7_Flag` structure
 * and clears the respective interrupt flag in the DMA interrupt flag clear register.
 */
void DMA1_Stream7_IRQHandler(void)
{
	if(DMA1 -> HISR & DMA_HISR_FEIF7)
	{
		DMA1_Stream7_Flag.Fifo_Error_Flag = true;
		DMA1 -> HIFCR = DMA_HIFCR_CFEIF7;
	}
	/************************************************************************************************************/
	if(DMA1 -> HISR & DMA_HISR_DMEIF7)
	{
		DMA1_Stream7_Flag.Direct_Mode_Error_Flag = true;
		DMA1 -> HIFCR = DMA_HIFCR_CDMEIF7;
	}
	/************************************************************************************************************/
	if(DMA1 -> HISR & DMA_HISR_TEIF7)
	{
		DMA1_Stream7_Flag.Transfer_Error_Flag = true;
		DMA1 -> HIFCR = DMA_HIFCR_CTEIF7;
	}
	/************************************************************************************************************/
	if(DMA1 -> HISR & DMA_HISR_HTIF7)
	{
		DMA1_Stream7_Flag.Half_Transfer_Complete_Flag = true;
		DMA1 -> HIFCR = DMA_HIFCR_CHTIF7;
	}
	/************************************************************************************************************/
	if(DMA1 -> HISR & DMA_HISR_TCIF7)
	{
		DMA1_Stream7_Flag.Transfer_Complete_Flag = true;
		DMA1 -> HIFCR = DMA_HIFCR_CTCIF7;
	}
}

/**
 * @brief DMA2 Stream 0 Interrupt Handler
 *
 * This function handles the interrupt for DMA2 Stream 0. It checks the
 * status flags for different events such as FIFO error, direct mode error,
 * transfer error, half transfer complete, and transfer complete. For each
 * event, it sets the corresponding flag in the `DMA2_Stream0_Flag` structure
 * and clears the respective interrupt flag in the DMA interrupt flag clear register.
 */
void DMA2_Stream0_IRQHandler(void)
{
	if(DMA2 -> LISR & DMA_LISR_FEIF0)
	{
		DMA2_Stream0_Flag.Fifo_Error_Flag = true;
		DMA2 -> LIFCR = DMA_LIFCR_CFEIF0;
	}
	/************************************************************************************************************/
	if(DMA2 -> LISR & DMA_LISR_DMEIF0)
	{
		DMA2_Stream0_Flag.Direct_Mode_Error_Flag = true;
		DMA2 -> LIFCR = DMA_LIFCR_CDMEIF0;
	}
	/************************************************************************************************************/
	if(DMA2 -> LISR & DMA_LISR_TEIF0)
	{
		DMA2_Stream0_Flag.Transfer_Error_Flag = true;
		DMA2 -> LIFCR = DMA_LIFCR_CTEIF0;
	}
	/************************************************************************************************************/
	if(DMA2 -> LISR & DMA_LISR_HTIF0)
	{
		DMA2_Stream0_Flag.Half_Transfer_Complete_Flag = true;
		DMA2 -> LIFCR = DMA_LIFCR_CHTIF0;
	}
	/************************************************************************************************************/
	if(DMA2 -> LISR & DMA_LISR_TCIF0)
	{
		DMA2_Stream0_Flag.Transfer_Complete_Flag = true;
		DMA2 -> LIFCR = DMA_LIFCR_CTCIF0;
	}
}

/**
 * @brief DMA2 Stream 1 Interrupt Handler
 *
 * This function handles the interrupt for DMA2 Stream 1. It checks the
 * status flags for different events such as FIFO error, direct mode error,
 * transfer error, half transfer complete, and transfer complete. For each
 * event, it sets the corresponding flag in the `DMA2_Stream1_Flag` structure
 * and clears the respective interrupt flag in the DMA interrupt flag clear register.
 */
void DMA2_Stream1_IRQHandler(void)
{
	if(DMA2 -> LISR & DMA_LISR_FEIF1)
	{
		DMA2_Stream1_Flag.Fifo_Error_Flag = true;
		DMA2 -> LIFCR = DMA_LIFCR_CFEIF1;
	}
	/************************************************************************************************************/
	if(DMA2 -> LISR & DMA_LISR_DMEIF1)
	{
		DMA2_Stream1_Flag.Direct_Mode_Error_Flag = true;
		DMA2 -> LIFCR = DMA_LIFCR_CDMEIF1;
	}
	/************************************************************************************************************/
	if(DMA2 -> LISR & DMA_LISR_TEIF1)
	{
		DMA2_Stream1_Flag.Transfer_Error_Flag = true;
		DMA2 -> LIFCR = DMA_LIFCR_CTEIF1;
	}
	/************************************************************************************************************/
	if(DMA2 -> LISR & DMA_LISR_HTIF1)
	{
		DMA2_Stream1_Flag.Half_Transfer_Complete_Flag = true;
		DMA2 -> LIFCR = DMA_LIFCR_CHTIF1;
	}
	/************************************************************************************************************/
	if(DMA2 -> LISR & DMA_LISR_TCIF1)
	{
		DMA2_Stream1_Flag.Transfer_Complete_Flag = true;
		DMA2 -> LIFCR = DMA_LIFCR_CTCIF1;
	}
}

/**
 * @brief DMA2 Stream 2 Interrupt Handler
 *
 * This function handles the interrupt for DMA2 Stream 2. It checks the
 * status flags for different events such as FIFO error, direct mode error,
 * transfer error, half transfer complete, and transfer complete. For each
 * event, it sets the corresponding flag in the `DMA2_Stream2_Flag` structure
 * and clears the respective interrupt flag in the DMA interrupt flag clear register.
 */
void DMA2_Stream2_IRQHandler(void)
{
	if(DMA2 -> LISR & DMA_LISR_FEIF2)
	{
		DMA2_Stream2_Flag.Fifo_Error_Flag = true;
		DMA2 -> LIFCR = DMA_LIFCR_CFEIF2;
	}
	/************************************************************************************************************/
	if(DMA2 -> LISR & DMA_LISR_DMEIF2)
	{
		DMA2_Stream2_Flag.Direct_Mode_Error_Flag = true;
		DMA2 -> LIFCR = DMA_LIFCR_CDMEIF2;
	}
	/************************************************************************************************************/
	if(DMA2 -> LISR & DMA_LISR_TEIF2)
	{
		DMA2_Stream2_Flag.Transfer_Error_Flag = true;
		DMA2 -> LIFCR = DMA_LIFCR_CTEIF2;
	}
	/************************************************************************************************************/
	if(DMA2 -> LISR & DMA_LISR_HTIF2)
	{
		DMA2_Stream2_Flag.Half_Transfer_Complete_Flag = true;
		DMA2 -> LIFCR = DMA_LIFCR_CHTIF2;
	}
	/************************************************************************************************************/
	if(DMA2 -> LISR & DMA_LISR_TCIF2)
	{
		DMA2_Stream2_Flag.Transfer_Complete_Flag = true;
		DMA2 -> LIFCR = DMA_LIFCR_CTCIF2;
	}
}

/**
 * @brief DMA2 Stream 3 Interrupt Handler
 *
 * This function handles the interrupt for DMA2 Stream 3. It checks the
 * status flags for different events such as FIFO error, direct mode error,
 * transfer error, half transfer complete, and transfer complete. For each
 * event, it sets the corresponding flag in the `DMA2_Stream3_Flag` structure
 * and clears the respective interrupt flag in the DMA interrupt flag clear register.
 */
void DMA2_Stream3_IRQHandler(void)
{
	if(DMA2 -> LISR & DMA_LISR_FEIF3)
	{
		DMA2_Stream3_Flag.Fifo_Error_Flag = true;
		DMA2 -> LIFCR = DMA_LIFCR_CFEIF3;
	}
	/************************************************************************************************************/
	if(DMA2 -> LISR & DMA_LISR_DMEIF3)
	{
		DMA2_Stream3_Flag.Direct_Mode_Error_Flag = true;
		DMA2 -> LIFCR = DMA_LIFCR_CDMEIF3;
	}
	/************************************************************************************************************/
	if(DMA2 -> LISR & DMA_LISR_TEIF3)
	{
		DMA2_Stream3_Flag.Transfer_Error_Flag = true;
		DMA2 -> LIFCR = DMA_LIFCR_CTEIF3;
	}
	/************************************************************************************************************/
	if(DMA2 -> LISR & DMA_LISR_HTIF3)
	{
		DMA2_Stream3_Flag.Half_Transfer_Complete_Flag = true;
		DMA2 -> LIFCR = DMA_LIFCR_CHTIF3;
	}
	/************************************************************************************************************/
	if(DMA2 -> LISR & DMA_LISR_TCIF3)
	{
		DMA2_Stream3_Flag.Transfer_Complete_Flag = true;
		DMA2 -> LIFCR = DMA_LIFCR_CTCIF3;
	}
}

/**
 * @brief DMA2 Stream 4 Interrupt Handler
 *
 * This function handles the interrupt for DMA2 Stream 4. It checks the
 * status flags for different events such as FIFO error, direct mode error,
 * transfer error, half transfer complete, and transfer complete. For each
 * event, it sets the corresponding flag in the `DMA2_Stream4_Flag` structure
 * and clears the respective interrupt flag in the DMA interrupt flag clear register.
 */
void DMA2_Stream4_IRQHandler(void)
{
	if(DMA2 -> HISR & DMA_HISR_FEIF4)
	{
		DMA2_Stream4_Flag.Fifo_Error_Flag = true;
		DMA2 -> HIFCR = DMA_HIFCR_CFEIF4;
	}
	/************************************************************************************************************/
	if(DMA2 -> HISR & DMA_HISR_DMEIF4)
	{
		DMA2_Stream4_Flag.Direct_Mode_Error_Flag = true;
		DMA2 -> HIFCR = DMA_HIFCR_CDMEIF4;
	}
	/************************************************************************************************************/
	if(DMA2 -> HISR & DMA_HISR_TEIF4)
	{
		DMA2_Stream4_Flag.Transfer_Error_Flag = true;
		DMA2 -> HIFCR = DMA_HIFCR_CTEIF4;
	}
	/************************************************************************************************************/
	if(DMA2 -> HISR & DMA_HISR_HTIF4)
	{
		DMA2_Stream4_Flag.Half_Transfer_Complete_Flag = true;
		DMA2 -> HIFCR = DMA_HIFCR_CHTIF4;
	}
	/************************************************************************************************************/
	if(DMA2 -> HISR & DMA_HISR_TCIF4)
	{
		DMA2_Stream4_Flag.Transfer_Complete_Flag = true;
		DMA2 -> HIFCR = DMA_HIFCR_CTCIF4;
	}
}

/**
 * @brief DMA2 Stream 5 Interrupt Handler
 *
 * This function handles the interrupt for DMA2 Stream 5. It checks the
 * status flags for different events such as FIFO error, direct mode error,
 * transfer error, half transfer complete, and transfer complete. For each
 * event, it sets the corresponding flag in the `DMA2_Stream5_Flag` structure
 * and clears the respective interrupt flag in the DMA interrupt flag clear register.
 */
void DMA2_Stream5_IRQHandler(void)
{
	if(DMA2 -> HISR & DMA_HISR_FEIF5)
	{
		DMA2_Stream5_Flag.Fifo_Error_Flag = true;
		DMA2 -> HIFCR = DMA_HIFCR_CFEIF5;
	}
	/************************************************************************************************************/
	if(DMA2 -> HISR & DMA_HISR_DMEIF5)
	{
		DMA2_Stream5_Flag.Direct_Mode_Error_Flag = true;
		DMA2 -> HIFCR = DMA_HIFCR_CDMEIF5;
	}
	/************************************************************************************************************/
	if(DMA2 -> HISR & DMA_HISR_TEIF5)
	{
		DMA2_Stream5_Flag.Transfer_Error_Flag = true;
		DMA2 -> HIFCR = DMA_HIFCR_CTEIF5;
	}
	/************************************************************************************************************/
	if(DMA2 -> HISR & DMA_HISR_HTIF5)
	{
		DMA2_Stream5_Flag.Half_Transfer_Complete_Flag = true;
		DMA2 -> HIFCR = DMA_HIFCR_CHTIF5;
	}
	/************************************************************************************************************/
	if(DMA2 -> HISR & DMA_HISR_TCIF5)
	{
		DMA2_Stream5_Flag.Transfer_Complete_Flag = true;
		DMA2 -> HIFCR = DMA_HIFCR_CTCIF5;
	}
}

/**
 * @brief DMA2 Stream 6 Interrupt Handler
 *
 * This function handles the interrupt for DMA2 Stream 6. It checks the
 * status flags for different events such as FIFO error, direct mode error,
 * transfer error, half transfer complete, and transfer complete. For each
 * event, it sets the corresponding flag in the `DMA2_Stream6_Flag` structure
 * and clears the respective interrupt flag in the DMA interrupt flag clear register.
 */
void DMA2_Stream6_IRQHandler(void)
{
	if(DMA2 -> HISR & DMA_HISR_FEIF6)
	{
		DMA2_Stream6_Flag.Fifo_Error_Flag = true;
		DMA2 -> HIFCR = DMA_HIFCR_CFEIF6;
	}
	/************************************************************************************************************/
	if(DMA2 -> HISR & DMA_HISR_DMEIF6)
	{
		DMA2_Stream6_Flag.Direct_Mode_Error_Flag = true;
		DMA2 -> HIFCR = DMA_HIFCR_CDMEIF6;
	}
	/************************************************************************************************************/
	if(DMA2 -> HISR & DMA_HISR_TEIF6)
	{
		DMA2_Stream6_Flag.Transfer_Error_Flag = true;
		DMA2 -> HIFCR = DMA_HIFCR_CTEIF6;
	}
	/************************************************************************************************************/
	if(DMA2 -> HISR & DMA_HISR_HTIF6)
	{
		DMA2_Stream6_Flag.Half_Transfer_Complete_Flag = true;
		DMA2 -> HIFCR = DMA_HIFCR_CHTIF6;
	}
	/************************************************************************************************************/
	if(DMA2 -> HISR & DMA_HISR_TCIF6)
	{
		DMA2_Stream6_Flag.Transfer_Complete_Flag = true;
		DMA2 -> HIFCR = DMA_HIFCR_CTCIF6;
	}
}

/**
 * @brief DMA2 Stream 7 Interrupt Handler
 *
 * This function handles the interrupt for DMA2 Stream 7. It checks the
 * status flags for different events such as FIFO error, direct mode error,
 * transfer error, half transfer complete, and transfer complete. For each
 * event, it sets the corresponding flag in the `DMA2_Stream7_Flag` structure
 * and clears the respective interrupt flag in the DMA interrupt flag clear register.
 */
void DMA2_Stream7_IRQHandler(void)
{
	if(DMA2 -> HISR & DMA_HISR_FEIF7)
	{
		DMA2_Stream7_Flag.Fifo_Error_Flag = true;
		DMA2 -> HIFCR = DMA_HIFCR_CFEIF7;
	}
	/************************************************************************************************************/
	if(DMA2 -> HISR & DMA_HISR_DMEIF7)
	{
		DMA2_Stream7_Flag.Direct_Mode_Error_Flag = true;
		DMA2 -> HIFCR = DMA_HIFCR_CDMEIF7;
	}
	/************************************************************************************************************/
	if(DMA2 -> HISR & DMA_HISR_TEIF7)
	{
		DMA2_Stream7_Flag.Transfer_Error_Flag = true;
		DMA2 -> HIFCR = DMA_HIFCR_CTEIF7;
	}
	/************************************************************************************************************/
	if(DMA2 -> HISR & DMA_HISR_HTIF7)
	{
		DMA2_Stream7_Flag.Half_Transfer_Complete_Flag = true;
		DMA2 -> HIFCR = DMA_HIFCR_CHTIF7;
	}
	/************************************************************************************************************/
	if(DMA2 -> HISR & DMA_HISR_TCIF7)
	{
		DMA2_Stream7_Flag.Transfer_Complete_Flag = true;
		DMA2 -> HIFCR = DMA_HIFCR_CTCIF7;
	}
}

/**
 * @brief Resets all DMA flags in the provided DMA_Flags_Typedef structure.
 *
 * This function sets all the flags in the provided `DMA_Flags_Typedef` structure
 * to `false`, effectively resetting the state of the flags that monitor DMA events
 * such as direct mode error, FIFO error, half transfer complete, transfer complete,
 * and transfer error.
 *
 * @param flag The `DMA_Flags_Typedef` structure whose flags are to be reset.
 */
void DMA_Reset_Flags(volatile DMA_Flags_Typedef *flag)
{
	if (!flag) return;
	flag->Direct_Mode_Error_Flag = false;
	flag->Fifo_Error_Flag = false;
	flag->Half_Transfer_Complete_Flag = false;
	flag->Transfer_Complete_Flag = false;
	flag->Transfer_Error_Flag = false;
}

/**
 * @brief Enables the clock for the specified DMA controller.
 *
 * This function enables the clock for the DMA controller specified in the
 * `DMA_Config` structure. It checks whether the controller is `DMA1` or `DMA2`
 * and then enables the corresponding clock by setting the appropriate bit in
 * the RCC AHB1 peripheral clock enable register.
 *
 * @param[in] config Pointer to the `DMA_Config` structure that contains the DMA controller configuration.
 */
void DMA_Clock_Enable(DMA_Config *config)
{
	if(config -> Request.Controller == DMA1) RCC -> AHB1ENR |= RCC_AHB1ENR_DMA1EN;
	if(config -> Request.Controller == DMA2) RCC -> AHB1ENR |= RCC_AHB1ENR_DMA2EN;
}
//

/**
 * @brief Disables the clock for the specified DMA controller.
 *
 * This function disables the clock for the DMA controller specified in the
 * `DMA_Config` structure. It checks whether the controller is `DMA1` or `DMA2`
 * and then disables the corresponding clock by clearing the appropriate bit in
 * the RCC AHB1 peripheral clock enable register.
 *
 * @param[in] config Pointer to the `DMA_Config` structure that contains the DMA controller configuration.
 */
void DMA_Clock_Disable(DMA_Config *config)
{

	if(config -> Request.Controller == DMA1) RCC -> AHB1ENR &= ~RCC_AHB1ENR_DMA1EN;
	if(config -> Request.Controller == DMA2) RCC -> AHB1ENR &= ~RCC_AHB1ENR_DMA2EN;
}
//

/**
 * @brief Resets the specified DMA controller.
 *
 * This function resets the DMA controller specified in the `DMA_Config` structure.
 * It checks whether the controller is `DMA1` or `DMA2` and then triggers a reset
 * by setting the appropriate bit in the RCC AHB1 peripheral reset register.
 *
 * @param[in] config Pointer to the `DMA_Config` structure that contains the DMA controller configuration.
 */
void DMA_Reset(DMA_Config *config)
{
	if(config -> Request.Controller == DMA1) RCC -> AHB1RSTR |= RCC_AHB1RSTR_DMA1RST;
	if(config -> Request.Controller == DMA1) RCC->AHB1RSTR &= ~RCC_AHB1RSTR_DMA1RST;
	if(config -> Request.Controller == DMA2) RCC -> AHB1RSTR |= RCC_AHB1RSTR_DMA2RST;
	if(config -> Request.Controller == DMA2) RCC->AHB1RSTR &= ~RCC_AHB1RSTR_DMA2RST;
}

/**
 * @brief Initializes the DMA with the specified configuration.
 *
 * This function configures the DMA stream with the settings provided in the
 * `DMA_Config` structure. It enables the clock for the specified DMA controller,
 * sets up the DMA channel, circular mode, flow control, priority level, data sizes,
 * transfer direction, and interrupts. If interrupts are enabled, it also configures
 * the NVIC for the corresponding DMA stream.
 *
 * @param[in] config Pointer to the `DMA_Config` structure containing the configuration parameters.
 *
 * @return int8_t Returns 1 on successful initialization, or -1 if an error occurs.
 */
int8_t DMA_Init(DMA_Config *config)
{
    if (DMA_Stream_Index(config) < 0 || config->Request.channel > 7U ||
        (config->transfer_direction != 0U && config->transfer_direction != (1U<<6) && config->transfer_direction != (2U<<6)) ||
        (config->circular_mode & ~DMA_SxCR_CIRC) ||
        (config->flow_control & ~DMA_SxCR_PFCTRL) ||
        (config->priority_level & ~DMA_SxCR_PL) ||
        (config->memory_pointer_increment & ~DMA_SxCR_MINC) ||
        (config->peripheral_pointer_increment & ~DMA_SxCR_PINC) ||
        (config->memory_data_size != 0U && config->memory_data_size != (1U<<13) && config->memory_data_size != (2U<<13)) ||
        (config->peripheral_data_size != 0U && config->peripheral_data_size != (1U<<11) && config->peripheral_data_size != (2U<<11)) ||
        (config->interrupts & ~0x9EU)) return -1;
    if (config->transfer_direction == (2U<<6) &&
        (config->Request.Controller != DMA2 || config->circular_mode || config->flow_control)) return -1;
    DMA_Clock_Enable(config);
    (void)RCC->AHB1ENR;
    if (DMA_Stop(config->Request.Stream) < 0) return -1;
    config->Request.Stream->CR = (config->Request.channel << DMA_SxCR_CHSEL_Pos) |
        config->circular_mode | config->flow_control | config->priority_level |
        config->memory_data_size | config->peripheral_data_size | config->transfer_direction;
    config->Request.Stream->FCR = (config->interrupts & DMA_SxFCR_FEIE) |
        ((config->transfer_direction == (2U<<6) || config->memory_data_size >> 13 != config->peripheral_data_size >> 11)
         ? DMA_SxFCR_DMDIS : 0U);
    config->Request.Stream->CR |= config->interrupts & 0x1EU;
    if(config->interrupts)
    {
        // Enable the corresponding NVIC interrupt for the DMA stream
        if(config->Request.Controller == DMA1)
        {
            if(config->Request.Stream == DMA1_Stream0)        NVIC_EnableIRQ(DMA1_Stream0_IRQn);
            else if(config->Request.Stream == DMA1_Stream1)   NVIC_EnableIRQ(DMA1_Stream1_IRQn);
            else if(config->Request.Stream == DMA1_Stream2)   NVIC_EnableIRQ(DMA1_Stream2_IRQn);
            else if(config->Request.Stream == DMA1_Stream3)   NVIC_EnableIRQ(DMA1_Stream3_IRQn);
            else if(config->Request.Stream == DMA1_Stream4)   NVIC_EnableIRQ(DMA1_Stream4_IRQn);
            else if(config->Request.Stream == DMA1_Stream5)   NVIC_EnableIRQ(DMA1_Stream5_IRQn);
            else if(config->Request.Stream == DMA1_Stream6)   NVIC_EnableIRQ(DMA1_Stream6_IRQn);
            else if(config->Request.Stream == DMA1_Stream7)   NVIC_EnableIRQ(DMA1_Stream7_IRQn);
        }
        else if(config->Request.Controller == DMA2)
        {
            if(config->Request.Stream == DMA2_Stream0)        NVIC_EnableIRQ(DMA2_Stream0_IRQn);
            else if(config->Request.Stream == DMA2_Stream1)   NVIC_EnableIRQ(DMA2_Stream1_IRQn);
            else if(config->Request.Stream == DMA2_Stream2)   NVIC_EnableIRQ(DMA2_Stream2_IRQn);
            else if(config->Request.Stream == DMA2_Stream3)   NVIC_EnableIRQ(DMA2_Stream3_IRQn);
            else if(config->Request.Stream == DMA2_Stream4)   NVIC_EnableIRQ(DMA2_Stream4_IRQn);
            else if(config->Request.Stream == DMA2_Stream5)   NVIC_EnableIRQ(DMA2_Stream5_IRQn);
            else if(config->Request.Stream == DMA2_Stream6)   NVIC_EnableIRQ(DMA2_Stream6_IRQn);
            else if(config->Request.Stream == DMA2_Stream7)   NVIC_EnableIRQ(DMA2_Stream7_IRQn);
        }
    }

    // Configure memory and peripheral pointer increments
    config->Request.Stream->CR |= config->memory_pointer_increment;
    config->Request.Stream->CR |= config->peripheral_pointer_increment;

    // Configure circular mode
    if(config->circular_mode == DMA_Configuration.Circular_Mode.Enable)
    {
        config->Request.Stream->CR |= DMA_SxCR_CIRC;
    }
    else if(config->circular_mode == DMA_Configuration.Circular_Mode.Disable)
    {
        config->Request.Stream->CR &= ~DMA_SxCR_CIRC;
    }
    else
    {
        return -1;  // Return -1 if circular mode configuration is invalid
    }

    return 1;  // Return 1 on successful initialization
}


/**
 * @brief Configures the target memory and peripheral for DMA transfers.
 *
 * This function sets up the DMA stream to transfer data between the specified
 * memory and peripheral addresses. It configures the memory and peripheral data
 * sizes, the number of data items to transfer, and the memory pointer increment
 * mode. The function also clears the previous configurations for data size and
 * memory increment before applying the new settings.
 *
 * @param[in] config Pointer to the `DMA_Config` structure containing the target configuration.
 */
void DMA_Set_Target(DMA_Config *config)
{
    if (DMA_Stream_Index(config) < 0 || DMA_Stop(config->Request.Stream) < 0) return;
    // Clear previous data size and memory increment settings
    config -> Request.Stream -> CR &= ~(DMA_SxCR_MSIZE | DMA_SxCR_PSIZE | DMA_SxCR_MINC);

    // Set the peripheral data size
    config -> Request.Stream -> CR |= config -> peripheral_data_size;

    // Set the memory data size
    config -> Request.Stream -> CR |= config -> memory_data_size;

    // Set the number of data items to be transferred
    config -> Request.Stream -> NDTR = config -> buffer_length;

    // Set memory pointer increment mode
    config -> Request.Stream -> CR |= config -> memory_pointer_increment;

    // Set the memory address
    config -> Request.Stream -> M0AR = (uint32_t)config->memory_address;

    // Set the peripheral address
    config -> Request.Stream -> PAR = (uint32_t)config->peripheral_address;
}



/**
 * @brief Sets up and enables the DMA stream for data transfer.
 *
 * This function configures the DMA stream trigger by clearing any pending interrupt
 * flags for the specified stream and then enables the stream for data transfer. The
 * function determines which register to modify (LIFCR or HIFCR) based on the stream
 * number and the DMA controller (DMA1 or DMA2).
 *
 * @param[in] config Pointer to the `DMA_Config` structure that contains the configuration settings.
 */
void DMA_Set_Trigger(DMA_Config *config)
{
    int index = DMA_Stream_Index(config);
    static const uint8_t shifts[4] = {0,6,16,22};
    if (index < 0 || (config->Request.Stream->CR & DMA_SxCR_EN) ||
        config->Request.Stream->NDTR == 0U) return;
    uint32_t mask = 0x3DU << shifts[index % 4];
    if (index < 4) config->Request.Controller->LIFCR = mask;
    else config->Request.Controller->HIFCR = mask;
    config->Request.Stream->CR |= DMA_SxCR_EN;
}




/**
 * @brief Performs a memory-to-memory data transfer using DMA.
 *
 * This function configures and initiates a DMA transfer from a source memory
 * location to a destination memory location. It sets up the data size,
 * increment modes, and the length of the transfer. The function enables the
 * DMA stream, waits for the transfer to complete, and then disables the stream.
 *
 * @param[in] source Pointer to the source memory location.
 * @param[in] source_data_size Size of the data at the source (8, 16, or 32 bits).
 * @param[in] dest_data_size Size of the data at the destination (8, 16, or 32 bits).
 * @param[in] destination Pointer to the destination memory location.
 * @param[in] source_increment If true, the source address will be incremented after each transfer.
 * @param[in] destination_increment If true, the destination address will be incremented after each transfer.
 * @param[in] length Number of data items to transfer.
 */
void DMA_Memory_To_Memory_Transfer(uint32_t *source,
                          uint8_t source_data_size, uint8_t dest_data_size,
                          uint32_t *destination, bool source_increment,
                          bool destination_increment, uint16_t length)
{
    if (!source || !destination || !length ||
        (source_data_size != 8 && source_data_size != 16 && source_data_size != 32) ||
        (dest_data_size != 8 && dest_data_size != 16 && dest_data_size != 32) ||
        ((uintptr_t)source % (source_data_size/8)) ||
        ((uintptr_t)destination % (dest_data_size/8))) return;
    RCC->AHB1ENR |= RCC_AHB1ENR_DMA2EN;
    (void)RCC->AHB1ENR;
    /* Stream 0 is reserved by this blocking helper; never abort its current owner. */
    if (DMA2_Stream0->CR & DMA_SxCR_EN) return;
    DMA2_Stream0->CR = (2U<<6) | DMA_SxCR_PL |
        (source_data_size == 32 ? (2U<<11) : source_data_size == 16 ? (1U<<11) : 0U) |
        (dest_data_size == 32 ? (2U<<13) : dest_data_size == 16 ? (1U<<13) : 0U) |
        (source_increment ? DMA_SxCR_PINC : 0U) |
        (destination_increment ? DMA_SxCR_MINC : 0U);
    DMA2_Stream0->FCR = DMA_SxFCR_DMDIS;
    DMA2_Stream0->PAR = (uint32_t)(uintptr_t)source;
    DMA2_Stream0->M0AR = (uint32_t)(uintptr_t)destination;
    DMA2_Stream0->NDTR = length;
    DMA2->LIFCR = 0x3DU;
    DMA2_Stream0->CR |= DMA_SxCR_EN;
    uint32_t timeout = 1000000U;
    while (!(DMA2->LISR & (DMA_LISR_TCIF0 | DMA_LISR_TEIF0 | DMA_LISR_DMEIF0 | DMA_LISR_FEIF0)))
        if (--timeout == 0U) break;
    if (DMA_Stop(DMA2_Stream0) > 0) DMA2->LIFCR = 0x3DU;
}
