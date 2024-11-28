#include "HardwareDevices.h"
#include <Logic.h>
#include <OpenKNX.h>
// #include "hardware.h"
#include "Device.h"
#include "Wire.h"

void setup()
{
#ifdef ARDUINO_ARCH_RP2040
#ifdef BOARD_MASIFI_MODBUS_BREAKOUT
    Serial2.setRX(KNX_UART_RX_PIN);
    Serial2.setTX(KNX_UART_TX_PIN);
#endif
#ifdef BOARD_MASIFI_MODBUS_V21
    Serial1.setRX(KNX_UART_RX_PIN);
    Serial1.setTX(KNX_UART_TX_PIN);
#endif
#endif

    delay(DEBUG_DELAY); // Seems this is needed to get the serial output working ?

#ifdef ARDUINO_ARCH_RP2040
#ifdef BOARD_MASIFI_MODBUS_V21
    Serial2.setRX(MODBUS_UART_RX_PIN);
    Serial2.setTX(MODBUS_UART_TX_PIN);
#endif
#endif
    //SERIAL_DEBUG.println("Startup called...");
    //ArduinoPlatform::SerialDebug = &SERIAL_DEBUG;

    appSetup();
}

void loop()
{
    // don't delay here to much. Otherwise you might lose packages or mess up the timing with ETS
    knx.loop();

    // only run the application code if the device was configured with ETS
    if (knx.configured())
    {
        appLoop();
    }
}
