/* #include "mcp4725.h"
*
* Creada por: Ing. Abiezer Hernandez O.
* Fecha de creacion: 28/12/2020
* Electronica y Circuitos
*
*/

#include <xc.h>
#define _XTAL_FREQ 20000000

#include "i2c.h"

#define MCP4725_ADDRESS 0xC0

void DAC_Init(unsigned char add_device);
void DAC_Write_Value(unsigned char address, int data);