/* #include "mcp4725.h"
*
* Creada por: Ing. Abiezer Hernandez O.
* Fecha de creacion: 28/12/2020
* Electronica y Circuitos
*
*/

#include "mcp4725.h"

void DAC_Init(unsigned char add_device)
{
    I2C_Start();
    I2C_Write(add_device);              // Direccion (GND-> 0xC0 , VCC-> 0xC2)
    I2C_Write(0x40);                    // Direccion interna
    I2C_Write((0x00 & 0xFF0) >> 4);     // MSB
    I2C_Write((0x00 & 0xF) << 4);       // LSB
    I2C_Stop();
}

void DAC_Write_Value(unsigned char address, int data)
{
    I2C_Start();
    I2C_Write(address);                 // Direccion (GND-> 0xC0 , VCC-> 0xC2)
    I2C_Write(0x40);                    // Direccion interna
    I2C_Write((data & 0xFF0) >> 4);     // MSB
    I2C_Write((data & 0xF) << 4);       // LSB
    I2C_Stop();
}