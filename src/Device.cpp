#include <Arduino.h>

#include "Device.h"
#include "KnxHelper.h"
#include "LED_Statusanzeige.h"

#include "Modbus.h"
#include "ModbusGateway.h"
#include "ModbusMaster.h"
#include "ModbusRead.h"

#include "HelperFunc.h"
#include "wiring_private.h" // pinPeripheral() function
#include <knx.h>

#ifdef BOARD_MASIFI_MODBUS_BREAKOUT
SerialPIO modbusSerial = SerialPIO(MODBUS_UART_TX_PIN, MODBUS_UART_RX_PIN, 64U);
#endif

uint32_t gStartupDelay = 0;
uint8_t usedModbusChannels = 0;
bool modbusReady = false;

uint16_t getPar(uint16_t PAR, uint8_t CH)
{
    return MOD_ParamBlockOffset + (CH * MOD_ParamBlockSize) + PAR;
}

uint16_t getCom(uint16_t COM, uint8_t CH)
{
    return MOD_KoOffset + (CH * MOD_KoBlockSize) + COM;
}

void ProcessReadRequests()
{
    // this method is called after startup delay and executes read requests, which should just happen once after startup
    static bool sCalledProcessReadRequests = false;
    if (!sCalledProcessReadRequests)
    {
        // put here own readRequest coding
        sCalledProcessReadRequests = true;
    }
}

// true solange der Start des gesamten Moduls verzögert werden soll
bool startupDelay()
{
    return !delayCheck(gStartupDelay, getDelayPattern(LOG_StartupDelayBase, true));
}

void ProcessDiagnoseCommand(GroupObject &iKo)
{
}

void processInputKo(GroupObject &iKo)
{
    // Compute modbus channel number
    int channel = (iKo.asap() - MOD_KoOffset - MOD_KoGO_BASE_) / MOD_KoBlockSize;
    if (channel >= 0 && channel < MOD_ChannelCount)
    {
        // SERIAL_DEBUG.print("KO: ");
        // SERIAL_DEBUG.println(channel + 1);
        //  Get slave number
        int slave = knx.paramByte(getPar(MOD_CHModbusSlaveSelection, channel));
        if (slave >= 1 && slave < MaxCountSlaves)
        {
            Slave[slave - 1].ReadyToSendModbus(channel);
        }
    }
}

void ProcessKoCallback(GroupObject &iKo)
{
    // check if we evaluate own KO
    if (iKo.asap() == LOG_KoDiagnose)
    {
        ProcessDiagnoseCommand(iKo);
    }
    else
    {
        processInputKo(iKo);
    }
}

/**
 * @return upper border of used channels
 */
uint8_t getUsedModbusChannels()
{
    uint8_t countChannels = 0;
    for (int channel = 0; channel < MOD_ChannelCount; channel++)
    {
        if (knx.paramByte(getPar(MOD_CHModbusSlaveSelection, channel)) != 0) // wenn ein Slave gewählt wurde
        {
            if (knx.paramByte(getPar(MOD_CHModBusDptSelection, channel)) != 0) // und zusätzlich auch ein DPT
            {
                countChannels = channel + 1;
            }
        }
    }
    return countChannels;
}

bool setupModbus()
{
    // HardwareSerial &serial = Serial; //does not work
    pinMode(MAX485_DIR, OUTPUT);
    // Init in receive mode
    digitalWrite(MAX485_DIR, 0);

#ifdef BOARD_MASIFI_MODBUS_V21
    modbusInitSerial(Serial2);
    modbusInitSlaves(Serial2);
#endif
#ifdef BOARD_MASIFI_MODBUS_BREAKOUT
    // modbusInitSerial(Serial1);
    // modbusInitSlaves(Serial1);
    modbusInitSerial(modbusSerial);
    modbusInitSlaves(modbusSerial);
#endif
    // SERIAL_DEBUG.println("Modbus Setup Done");
    return true;
}

void logicCallback(void *iInstance)
{
    gLogic.loop();
}

void appSetup()
{
    // Modbus
    modbusReady = setupModbus();
    // determine count of used channels
    usedModbusChannels = getUsedModbusChannels();

    // SERIAL_DEBUG.print("Channel Count:");
    // SERIAL_DEBUG.println(usedModbusChannels);

    if (knx.configured())
    {
        if (GroupObject::classCallback() == 0)
            GroupObject::classCallback(ProcessKoCallback);

        gStartupDelay = millis();
    }
}

void appLoop()
{
    if (!knx.configured())
        return;
    // handle KNX stuff
    if (startupDelay())
        return;

    ProcessReadRequests();
    ModbusRead(usedModbusChannels);
}