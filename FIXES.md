# DMA fixes and validation

DMA_Reset_Flags now takes a pointer: DMA_Reset_Flags(&DMA1_Stream0_Flag).
Flags shared with interrupt handlers are volatile. Combined interrupt masks are supported.
FIFO error enable belongs only to FCR. Initialization stops the stream before replacing configuration.
Memory-to-memory uses DMA2 Stream0 and FIFO mode, with correct 32-bit width encoding,
stale flag clearing, busy protection, error detection and bounded polling.

The legacy void memory-copy API cannot report failure: callers requiring reliable completion/error
reporting should use a status-returning API in a future change. The loop limit is an iteration
budget, not a calibrated wall-clock timeout. Reserve DMA2 Stream0 for the blocking helper.
Keep DMA buffers out of CCM RAM and keep their lifetime valid until completion.

Host regression checks use a minimal register mock, not a peripheral simulator:
gcc -std=c11 -Wall -Wextra -Werror -Itests -I. DMA.c tests/test_dma.c -o test_dma
./test_dma

Checks cover combined FIFO/TC interrupt masks, channel/stream validation, simultaneous HT/TC flags,
flag reset by pointer, and DAC presets. Board validation and an ARM firmware build remain required.
main.h and the device CMSIS headers come from the consuming firmware project.
Generated Doxygen files are not regenerated. Peripheral request presets beyond DAC were not
exhaustively audited; entries for UART7/8 target other F4 devices, not STM32F407.

Register behavior reference: https://www.st.com/resource/en/reference_manual/rm0090-stm32f407-advanced-armbased-32bit-mcus-stmicroelectronics.pdf
