#include "I2C.h"
#include "fsl_debug_console.h"

void i2c_init_alt()
{
	PRINTF("i2c_init_alt\r\n");
	/*Clock setting for LPI2C*/
	CLOCK_SetMux(kCLOCK_Lpi2cMux, LPI2C_CLOCK_SOURCE_SELECT);
	CLOCK_SetDiv(kCLOCK_Lpi2cDiv, LPI2C_CLOCK_SOURCE_DIVIDER);
	LPI2C_MasterGetDefaultConfig(&masterConfig);
	masterConfig.baudRate_Hz = I2C_BAUDRATE;
	masterConfig.debugEnable = true;
	masterConfig.enableMaster = true;
	LPI2C_MasterInit(EXAMPLE_I2C_MASTER, &masterConfig, LPI2C_MASTER_CLOCK_FREQUENCY);
}

int i2c_scan_alt()
{
	int fd = 0;
	PRINTF("i2c_scan_alt\r\n");
	status_t reVal = kStatus_Fail;
	//reVal = LPI2C_MasterStop(EXAMPLE_I2C_MASTER);

    for(uint8_t deviceAddr = 1; deviceAddr < 127; deviceAddr++ )
    {
    	PRINTF(".");
    	if (kStatus_Success == LPI2C_MasterStart(EXAMPLE_I2C_MASTER, deviceAddr, kLPI2C_Write))
        {
            reVal = LPI2C_MasterStop(EXAMPLE_I2C_MASTER);
            if (reVal != kStatus_Success)
            {
                continue;
            }
            PRINTF("\r\nFound: 0x%X\n", deviceAddr);
            fd++;
        }else{

        }
    }
    return fd;
}

bool i2c_read_alt(uint8_t deviceAddr, uint8_t reg, uint8_t bytes, uint8_t *in)
{
	status_t reVal = kStatus_Fail;
	size_t txCount = 0xFFU;

    if (kStatus_Success == LPI2C_MasterStart(EXAMPLE_I2C_MASTER, deviceAddr, kLPI2C_Write))
    {
        /* Check master tx FIFO empty or not */
        LPI2C_MasterGetFifoCounts(EXAMPLE_I2C_MASTER, NULL, &txCount);
        while (txCount)
        {
            LPI2C_MasterGetFifoCounts(EXAMPLE_I2C_MASTER, NULL, &txCount);
        }
        /* Check communicate with slave successful or not */
        if (LPI2C_MasterGetStatusFlags(EXAMPLE_I2C_MASTER) & kLPI2C_MasterNackDetectFlag)
        {
            //return kStatus_LPI2C_Nak;
            return false;
        }

        reVal = LPI2C_MasterSend(EXAMPLE_I2C_MASTER, &reg, 1);
        if (reVal != kStatus_Success)
        {
            if (reVal == kStatus_LPI2C_Nak)
            {
                LPI2C_MasterStop(EXAMPLE_I2C_MASTER);
            }
            return false;
        }

        reVal = LPI2C_MasterRepeatedStart(EXAMPLE_I2C_MASTER, deviceAddr, kLPI2C_Read);
        if (reVal != kStatus_Success)
        {
            return false;
        }

        reVal = LPI2C_MasterReceive(EXAMPLE_I2C_MASTER, in,  bytes);
        if (reVal != kStatus_Success)
        {
            if (reVal == kStatus_LPI2C_Nak)
            {
                LPI2C_MasterStop(EXAMPLE_I2C_MASTER);
            }
            return false;
        }

        reVal = LPI2C_MasterStop(EXAMPLE_I2C_MASTER);
        if (reVal != kStatus_Success)
        {
            return false;
        }
    }

    return true;
}


bool i2c_read_alt2(uint8_t deviceAddr,  uint8_t bytes, uint8_t *in)
{
	status_t reVal = kStatus_Fail;
	size_t txCount = 0xFFU;

    if (kStatus_Success == LPI2C_MasterStart(EXAMPLE_I2C_MASTER, deviceAddr, kLPI2C_Read))
    {
        /* Check master tx FIFO empty or not */
        LPI2C_MasterGetFifoCounts(EXAMPLE_I2C_MASTER, NULL, &txCount);
        while (txCount)
        {
            LPI2C_MasterGetFifoCounts(EXAMPLE_I2C_MASTER, NULL, &txCount);
        }
        /* Check communicate with slave successful or not */
        if (LPI2C_MasterGetStatusFlags(EXAMPLE_I2C_MASTER) & kLPI2C_MasterNackDetectFlag)
        {
            //return kStatus_LPI2C_Nak;
            return false;
        }


        reVal = LPI2C_MasterReceive(EXAMPLE_I2C_MASTER, in,  bytes);
        if (reVal != kStatus_Success)
        {
            if (reVal == kStatus_LPI2C_Nak)
            {
                LPI2C_MasterStop(EXAMPLE_I2C_MASTER);
            }
            return false;
        }

        reVal = LPI2C_MasterStop(EXAMPLE_I2C_MASTER);
        if (reVal != kStatus_Success)
        {
            return false;
        }
    } else {
    	return false;
    }

    return true;
}


bool i2c_write_alt(uint8_t deviceAddr, uint8_t reg, uint8_t bytes, uint8_t *out)
{
	status_t reVal = kStatus_Fail;
	size_t txCount = 0xFFU;

    if (kStatus_Success == LPI2C_MasterStart(EXAMPLE_I2C_MASTER, deviceAddr, kLPI2C_Write))
    {
        /* Check master tx FIFO empty or not */
        LPI2C_MasterGetFifoCounts(EXAMPLE_I2C_MASTER, NULL, &txCount);
        while (txCount)
        {
            LPI2C_MasterGetFifoCounts(EXAMPLE_I2C_MASTER, NULL, &txCount);
        }
        /* Check communicate with slave successful or not */
        if (LPI2C_MasterGetStatusFlags(EXAMPLE_I2C_MASTER) & kLPI2C_MasterNackDetectFlag)
        {
            //return kStatus_LPI2C_Nak;
        	return false;
        }

		reVal = LPI2C_MasterSend(EXAMPLE_I2C_MASTER, &reg, 1);

		if (reVal != kStatus_Success)
		{
			if (reVal == kStatus_LPI2C_Nak)
			{
				LPI2C_MasterStop(EXAMPLE_I2C_MASTER);
			}
			return false;
		}

        reVal = LPI2C_MasterSend(EXAMPLE_I2C_MASTER, out, bytes);
        if (reVal != kStatus_Success)
        {
            if (reVal == kStatus_LPI2C_Nak)
            {
                LPI2C_MasterStop(EXAMPLE_I2C_MASTER);
            }
            return false;
        }

        reVal = LPI2C_MasterStop(EXAMPLE_I2C_MASTER);
        if (reVal != kStatus_Success)
        {
            return false;
        }
    }

    return true;
}

bool i2c_write_alt2(uint8_t deviceAddr, uint8_t bytes, uint8_t *out)
{
	status_t reVal = kStatus_Fail;
	size_t txCount = 0xFFU;

    if (kStatus_Success == LPI2C_MasterStart(EXAMPLE_I2C_MASTER, deviceAddr, kLPI2C_Write))
    {
        /* Check master tx FIFO empty or not */
        LPI2C_MasterGetFifoCounts(EXAMPLE_I2C_MASTER, NULL, &txCount);
        while (txCount)
        {
            LPI2C_MasterGetFifoCounts(EXAMPLE_I2C_MASTER, NULL, &txCount);
        }
        /* Check communicate with slave successful or not */
        if (LPI2C_MasterGetStatusFlags(EXAMPLE_I2C_MASTER) & kLPI2C_MasterNackDetectFlag)
        {
            //return kStatus_LPI2C_Nak;
        	return false;
        }


        reVal = LPI2C_MasterSend(EXAMPLE_I2C_MASTER, out, bytes);
        if (reVal != kStatus_Success)
        {
            if (reVal == kStatus_LPI2C_Nak)
            {
                LPI2C_MasterStop(EXAMPLE_I2C_MASTER);
            }
            return false;
        }

        reVal = LPI2C_MasterStop(EXAMPLE_I2C_MASTER);
        if (reVal != kStatus_Success)
        {
            return false;
        }
    }

    return true;
}
