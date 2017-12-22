/*
Copyright (c) 2017, CenturyArks
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met: 

1. Redistributions of source code must retain the above copyright notice,
   this list of conditions and the following disclaimer. 
2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution. 

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

The views and conclusions contained in the software and documentation are those
of the authors and should not be interpreted as representing official policies, 
either expressed or implied, of the FreeBSD Project.
*/

#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <termios.h>
#include "communication.h"
#include "debug_util.h"
#include "types_util.h"

#ifdef __cplusplus
extern "C" {
#endif

/*------------------------------------------------------------------------------
 * Device define for I2C for Jetson TX2
 -----------------------------------------------------------------------------*/

/* I2C fie descriptor for Jetson TX2 */
static s32 g_i2c_fd[I2C_DEV_MAX] = {0};
static char *g_i2c_dev_name_table[] = {
        "/dev/i2c-30",
        "/dev/i2c-31",
        "/dev/i2c-32",
        "/dev/i2c-33",
        "/dev/i2c-34",
        "/dev/i2c-35",
    };

/*******************************************************************************
 * @brief   Open I2C
 *
 * @param   i2c_dev     I2C device name
 *
 * @return  Success COMMUNICATION_SUCCESS, Failure COMMUNICATION_ERROR
 ******************************************************************************/
s32 i2c_open(i2c_dev_t i2c_dev)
{
    if (g_i2c_fd[i2c_dev] == 0)
    {
        /* Open I2C device */
        if ((g_i2c_fd[i2c_dev] = open(g_i2c_dev_name_table[i2c_dev],O_RDWR)) < 0)
        {
            DEBUG_ERROR("Unable open I2C Device with error : errno = %d, errmsg = %s\n",errno,strerror(errno));
            return COMMUNICATION_ERROR;
        }
    }

    return COMMUNICATION_SUCCESS;
}

/*******************************************************************************
 * @brief   Close I2C
 *
 * @param   i2c_dev     I2C device name
 *
 * @return  Success COMMUNICATION_SUCCESS
 ******************************************************************************/
s32 i2c_close(i2c_dev_t i2c_dev)
{
    if (g_i2c_fd[i2c_dev] > 0)
    {
        close(g_i2c_fd[i2c_dev]);
        g_i2c_fd[i2c_dev] = 0;
    }

    return COMMUNICATION_SUCCESS;
}

/*******************************************************************************
 * @brief   Set force slave address
 *
 * @param   i2c_dev     I2C device
 * @param   slaveAddr   Slave address
 *
 * @return  Success COMMUNICATION_SUCCESS
 ******************************************************************************/
s32 i2c_set_slave_force(i2c_dev_t i2c_dev, u16 slaveAddr)
{
    if (g_i2c_fd[i2c_dev] > 0)
    {
        if (ioctl(g_i2c_fd[i2c_dev],I2C_SLAVE_FORCE,slaveAddr) < 0)
        {
            return COMMUNICATION_ERROR;
        }
    }

    return COMMUNICATION_SUCCESS;
}

/*******************************************************************************
 * @brief   Read I2C
 *
 * @param   i2c_dev     I2C device name
 * @param   devAddr     Slave address
 * @param   writeData   Read data
 * @param   dataLen     Read length
 *
 * @return  Success COMMUNICATION_SUCCESS, Failure COMMUNICATION_ERROR
 ******************************************************************************/
s32 i2c_read(i2c_dev_t i2c_dev,u16 devAddr,u8 * readData,u16 dataLen)
{
    struct i2c_msg i2c_msg_data = {0};
    struct i2c_rdwr_ioctl_data i2c_ioctl_data = {0};

    if (g_i2c_fd[i2c_dev] > 0)
    {
        i2c_msg_data.addr = devAddr;
        i2c_msg_data.flags = I2C_M_RD;
        i2c_msg_data.len = dataLen;
        i2c_msg_data.buf = readData;

        i2c_ioctl_data.msgs = &i2c_msg_data;
        i2c_ioctl_data.nmsgs = 1;

        if (ioctl(g_i2c_fd[i2c_dev],I2C_RDWR,&i2c_ioctl_data) < 0)
        {
            DEBUG_ERROR("Unable read data from I2C device with error : errno = %d, errmsg = %s\n",errno,strerror(errno));
            return COMMUNICATION_ERROR;
        }

        return COMMUNICATION_SUCCESS;
    }
    else
    {
        DEBUG_ERROR("I2C has not yet opened!\n");
    }

    return COMMUNICATION_ERROR;
}

/*******************************************************************************
 * @brief   Write I2C
 *
 * @param   i2c_dev     I2C device name
 * @param   devAddr     Slave address
 * @param   writeData   Write data
 * @param   dataLen     Write length
 *
 * @return  Success COMMUNICATION_SUCCESS, Failure COMMUNICATION_ERROR
 ******************************************************************************/
s32 i2c_write(i2c_dev_t i2c_dev, u16 devAddr,u8 * writeData, u16 dataLen)
{
    struct i2c_msg i2c_msg_data = {0};
    struct i2c_rdwr_ioctl_data i2c_ioctl_data = {0};

    if (g_i2c_fd[i2c_dev] > 0)
    {
        i2c_msg_data.addr = devAddr;
        i2c_msg_data.len = dataLen;
        i2c_msg_data.buf = writeData;

        i2c_ioctl_data.msgs = &i2c_msg_data;
        i2c_ioctl_data.nmsgs = 1;

        if (ioctl(g_i2c_fd[i2c_dev],I2C_RDWR,&i2c_ioctl_data) < 0)
        {
            DEBUG_ERROR("Unable write data from I2C device with error : errno = %d, errmsg = %s\n",errno,strerror(errno));
            return COMMUNICATION_ERROR;
        }

        return COMMUNICATION_SUCCESS;
    }

    DEBUG_ERROR("I2C has not ye opened!\n");
    return COMMUNICATION_ERROR;
}

/*******************************************************************************
 * @brief   Delay millisecond
 *
 * @param   ms  Millisecond
 *
 * @return  void
 ******************************************************************************/
void delay_msec(u32 ms)
{
    usleep(ms*1000);
}

void delay_usec(u32 us)
{
    usleep(us);
}

#ifdef __cplusplus
} /*extern "C" {*/
#endif
