#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <string.h>
#include "lcd.h"
#include "i2c.h"

int main() {
	uint8_t status;
	uint8_t n;
	char *sp;

	i2c_init(BDIV);
	sci_init(MYUBRR);

	sci_outs("TEST");

	memset(rdata, 0, 32);
    status = rdprom(rdata, 32, DATA_ADDR);
    snprintf(ostr, OSTR_SIZE, "Read \"%s\" from EEPROM %d\r\n", rdata, DATA_ADDR);
    sci_outs(ostr);
    snprintf(ostr, OSTR_SIZE, "Status = 0x%2.2x\r\n", status);
    sci_outs(ostr);

    if ((PINC & 0x02) == 0) {   // If button down, write data
        // Figure out which string is there now and write a different one
        n = rdata[0] - '0' - 1;
        if (n < 3) {            // Should be 0, 1 or 2 for the strings above
            n++;
            if (n >= 3)
                n = 0;
            sp = strs[n];
        }
        else
            sp = strs[0];

        // Write data at the address
        snprintf(ostr, OSTR_SIZE, "Storing \"%s\" at EEPROM %d\r\n", sp, DATA_ADDR);
        sci_outs(ostr);
        n = strlen(sp) + 1;
        status = wrprom((uint8_t *) sp, n, DATA_ADDR);
        snprintf(ostr, OSTR_SIZE, "Status = 0x%2.2x\r\n", status);
        sci_outs(ostr);

        // Read back the data just written
        memset(rdata, 0, 32);
        status = rdprom(rdata, 32, DATA_ADDR);
        snprintf(ostr, OSTR_SIZE, "Read \"%s\" from EEPROM %d\r\n", rdata, DATA_ADDR);
        sci_outs(ostr);
        snprintf(ostr, OSTR_SIZE, "Status = 0x%2.2x\r\n", status);
        sci_outs(ostr);

    }
    else {
        snprintf(ostr, OSTR_SIZE, "Bypassing storing data\r\n");
        sci_outs(ostr);
    }

    while (1) {                 // Loop forever
    }
}
