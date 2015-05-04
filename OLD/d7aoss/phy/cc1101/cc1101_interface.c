// *************************************************************************************************
// Radio core access functions. Taken from TI reference code for CC430.
// *************************************************************************************************

#include <stdbool.h>
#include <stdint.h>

#include "cc1101_constants.h"

#include "cc1101_interface.h"
#include "log.h"

// turn on/off the debug prints
#ifdef LOG_PHY_ENABLED
#define DPRINTF(...) log_printf(__VA_ARGS__)
#define DPRINT(str) log_print(str)
#else
#define DPRINTF(...)
#define DPRINT(str)
#endif

// functions to be defined which contain CC1101 or CC430 specific implementation
// implementation is in cc1101_interface_spi.c for cc1101 or cc1101_interface_cc430.c for cc430.
extern void _cc1101_interface_init();
extern void _c1101_interface_set_interrupts_enabled(bool);
extern uint8_t _c1101_interface_strobe(uint8_t);
extern void _c1101_interface_reset_radio_core();
extern uint8_t _c1101_interface_read_single_reg(uint8_t);
extern void _c1101_interface_write_single_reg(uint8_t, uint8_t);
extern void _c1101_interface_read_burst_reg(uint8_t, uint8_t*, uint8_t);
extern void _c1101_interface_write_burst_reg(uint8_t, uint8_t*, uint8_t);
extern void _c1101_interface_write_single_patable(uint8_t);
extern void _c1101_interface_write_burst_patable(uint8_t*, uint8_t);


void cc1101_interface_init()
{
    _cc1101_interface_init();
}

void cc1101_interface_set_interrupts_enabled(bool enabled)
{
    _c1101_interface_set_interrupts_enabled(enabled);
}

// *************************************************************************************************
// @fn          Strobe
// @brief       Send command to radio.
// @param       none
// @return      status byte
// *************************************************************************************************
uint8_t cc1101_interface_strobe(uint8_t strobe_command)
{
    uint8_t status = 0;
    uint8_t strobe_tmp = strobe_command & 0x7F;

    // Check for valid strobe command
    if ((strobe_tmp >= RF_SRES) && (strobe_tmp <= RF_SNOP))
    {
        status = _c1101_interface_strobe(strobe_command);
    }

    DPRINTF("STROBE 0x%02X STATUS: 0x%02X", strobe_command, status);

    return status;
}

// *****************************************************************************
// @fn          ResetRadioCore
// @brief       Reset the radio core using RF_SRES command
// @param       none
// @return      none
// ***********************************;******************************************
void cc1101_interface_reset_radio_core(void)
{
    DPRINT("RESET RADIO");
    _c1101_interface_reset_radio_core();
}

// *****************************************************************************
// @fn          WriteRfSettings
// @brief       Write the RF configuration register settings
// @param       RF_SETTINGS* rfsettings  Pointer to the structure that holds the rf settings
// @return      none
// *****************************************************************************
void cc1101_interface_write_rfsettings(RF_SETTINGS *rfsettings)
{
    cc1101_interface_write_burst_reg(IOCFG2, (unsigned char*) rfsettings, sizeof(RF_SETTINGS));
}


// *****************************************************************************
// @fn          ReadSingleReg
// @brief       Read a single byte from the radio register
// @param       unsigned char addr      Target radio register address
// @return      unsigned char data_out  Value of byte that was read
// *****************************************************************************
uint8_t cc1101_interface_read_single_reg(uint8_t addr)
{
    return _c1101_interface_read_single_reg(addr);
}

// *****************************************************************************
// @fn          WriteSingleReg
// @brief       Write a single byte to a radio register
// @param       unsigned char addr      Target radio register address
// @param       unsigned char value     Value to be written
// @return      none
// *****************************************************************************
void cc1101_interface_write_single_reg(uint8_t addr, uint8_t value)
{
    _c1101_interface_write_single_reg(addr, value);
    DPRINTF("WRITE SREG 0x%02X @0x%02X", value, addr);
}

// *****************************************************************************
// @fn          ReadBurstReg
// @brief       Read multiple bytes to the radio registers
// @param       unsigned char addr      Beginning address of burst read
// @param       unsigned char *buffer   Pointer to data table
// @param       unsigned char count     Number of bytes to be read
// @return      none
// *****************************************************************************
void cc1101_interface_read_burst_reg(uint8_t addr, uint8_t* buffer, uint8_t count)
{
    _c1101_interface_read_burst_reg(addr, buffer, count);

    DPRINTF("READ BREG %u Byte(s) @0x%02X", count, addr);
}

// *****************************************************************************
// @fn          WriteBurstReg
// @brief       Write multiple bytes to the radio registers
// @param       unsigned char addr      Beginning address of burst write
// @param       unsigned char *buffer   Pointer to data table
// @param       unsigned char count     Number of bytes to be written
// @return      none
// *****************************************************************************
void cc1101_interface_write_burst_reg(uint8_t addr, uint8_t* buffer, uint8_t count)
{
    _c1101_interface_write_burst_reg(addr, buffer, count);

    DPRINTF("WRITE BREG %u Byte(s) @0x%02X", count, addr);
}

// *****************************************************************************
// @fn          WritePATable
// @brief       Write data to power table
// @param       unsigned char value		Value to write
// @return      none
// *****************************************************************************
void cc1101_interface_write_single_patable(uint8_t value)
{
    _c1101_interface_write_single_patable(value);
}

// *****************************************************************************
// @fn          WritePATable
// @brief       Write to multiple locations in power table 
// @param       unsigned char *buffer	Pointer to the table of values to be written 
// @param       unsigned char count	Number of values to be written
// @return      none
// *****************************************************************************
void cc1101_interface_write_burst_patable(uint8_t* buffer, uint8_t count)
{
    _c1101_interface_write_burst_patable(buffer, count);
}