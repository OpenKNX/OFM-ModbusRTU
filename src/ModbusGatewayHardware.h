#pragma once

//Gehäuse Anzeige
#define MODBUS_STATUS 5
#define MODBUS_ERROR  4
#define MBUS_STATUS   3
#define MBUS_ERROR    2

//Gehäuse Anzeige
#define  i2cAddr_LED   0x24

// Modbus 
#define MaxCountSlaves 10

// Virtuelle Zähler
#define maxMeters 4

//HW-ID
#ifdef ARDUINO_ARCH_RP2040
#ifdef BOARD_MASIFI_MODBUS_BREAKOUT
  #define ID1 2
  #define ID2 3
  #define ID3 15

  // #define Diag_LED 26

  /*!
    We're using a MAX485-compatible RS485 Transceiver.
    The Data Enable and Receiver Enable pins are hooked up as follows:
  */
  #define MAX485_DIR 10
  
  // move to hardware autoconfig
  #define KNX_UART_RX_PIN 9
  #define KNX_UART_TX_PIN 8
  #define MODBUS_UART_RX_PIN 21
  #define MODBUS_UART_TX_PIN 20
#endif
#ifdef BOARD_MASIFI_MODBUS_V21
  #define ID1 2
  #define ID2 3
  #define ID3 15

  // #define Diag_LED 26

  /*!
    We're using a MAX485-compatible RS485 Transceiver.
    The Data Enable and Receiver Enable pins are hooked up as follows:
  */
  #define MAX485_DIR 24

  // move to hardware autoconfig
  #define KNX_UART_RX_PIN 1
  #define KNX_UART_TX_PIN 0
  #define MODBUS_UART_RX_PIN 5
  #define MODBUS_UART_TX_PIN 4
#endif
#else
  #define ID1 23
  #define ID2 24
  #define ID3 22

  //DIAG LED
  #define Diag_LED 9
  /*!
    We're using a MAX485-compatible RS485 Transceiver.
    The Data Enable and Receiver Enable pins are hooked up as follows:
  */
  #define MAX485_DIR 12


#endif


