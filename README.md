# STM32F10X-GCC-Encoder
STM32F103: How to Connect a rotary volume encoder

Rotary volume encoders are widely used. This example shows how they can be connected to the STM32F10x.

Encoder is connected to port GPIOA

PinA: PA0 Pulse A

PinB: PA1 Pulse B

PinC: PA3 Click press

You can connect to other pins on port A, simply modify the #defines at the top of encoder.h. You can also connect to other GPIO pins, by doing a bit more surgery on the GPIO setup function: init_ENC_GPIO().

The encoder is read purely by software, no counter hardware is used. The signals are debounced using a simple DSP lowpass filter implemented in the function "check_keypress()". 1kHz sampling rate is used. Very clean signals, no glitches. It is fast enough to respond to normal user interaction.

What is does: It transmits a '1' over the serial port when the encoder is rotated clockwise, and '0' when it is rotated counterclokwise. This is a "guarded encoder", meaning it only responds if you *press* *and* turn. One value is sent at each encoder detent.

How to run it: Modify the Makefile and make. Program the processor. See that the encoder pins are connected correctly and modify "encoder.h" as necessary. Connect USART1 pins to your PC, and run a terminal program (kermit, miniterm, hyperterminal etc.) at 115200bps. You can connect a USB-Serial converter also. PA10 is USART1-RX you must connect it to your PC TTL-TX, PA09 is USART1-TX you must connect it to your PC TTL-RX.

For serial communications, it relies on the xprintf and xuart functions written by Elm ChaN (see elm-chan.org).

Important functions:

  int ReadEnc_Guarded(void)

  Reads encoder state using a software algorithm.

  Returns:

        UP when click+ turn clockwise.

        DOWN when click+ turn counterclockwise.

        NOCHANGE else.

  return values are #defined in encoder.h

  void init_ENC_GPIO (void)

       Initializes the GPIO related with encoder.

  void init_LED_GPIO (void)

       Initializes the GPIO related with LED

  void init_USART1 (void)

       Enables USART1 RCC clock.

The functions have been divided into separate files for different hardware and functionality for ease of implementation.
