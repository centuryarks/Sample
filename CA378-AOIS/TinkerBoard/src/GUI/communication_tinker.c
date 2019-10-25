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
 * Device define for spi,i2c in Tinker OS
 -----------------------------------------------------------------------------*/
#define TINKER_OK					(0)
#define TINKER_ERROR				(1)

#define TINKER_TRUE					(1)
#define TINKER_FALSE				(0)

/* i2c fie descriptor in Tinker OS */
static s32 g_i2c_fd[I2C_DEV_MAX] = {0};
static char *g_i2c_dev_name_table[] = {
        "/dev/i2c-0",
        "/dev/i2c-1",
        "/dev/i2c-2",
        "/dev/i2c-3",
        "/dev/i2c-4",
        "/dev/i2c-5",
        "/dev/i2c-6",
        "/dev/i2c-7",
        "/dev/i2c-8",
        "/dev/i2c-9",
    };

/*******************************************************************************
 * @brief
 *
 * @param
 *
 * @return
 ******************************************************************************/
s32 i2c_open(i2c_dev_t i2c_dev) {
	if(g_i2c_fd[i2c_dev] == 0) {
		/*Open I2C device*/
		if( (g_i2c_fd[i2c_dev] = open(g_i2c_dev_name_table[i2c_dev],O_RDWR)) < 0 ) {
			DEBUG_ERROR("Unable open I2C Device with error : errno = %d, errmsg = %s\n",errno,strerror(errno));
			return TINKER_ERROR;
		}
	}
	return TINKER_OK;
}

/*******************************************************************************
 * @brief
 *
 * @param
 *
 * @return
 ******************************************************************************/
s32 i2c_read(i2c_dev_t i2c_dev,u16 devAddr,u8 * readData,u16 dataLen)
{
	struct i2c_msg i2c_msg_data = {0};
	struct i2c_rdwr_ioctl_data i2c_ioctl_data = {0};

	if(g_i2c_fd[i2c_dev] > 0)
	{
		i2c_msg_data.addr = devAddr;
		i2c_msg_data.flags = I2C_M_RD;
		i2c_msg_data.len = dataLen;
		i2c_msg_data.buf = readData;

		i2c_ioctl_data.msgs = &i2c_msg_data;
		i2c_ioctl_data.nmsgs = 1;

		if( ioctl(g_i2c_fd[i2c_dev],I2C_RDWR,&i2c_ioctl_data) < 0 ) {
			DEBUG_ERROR("Unable read data from I2C device with error : errno = %d, errmsg = %s\n",errno,strerror(errno));
			return TINKER_ERROR;
		}
		return TINKER_OK;
	} else {
		DEBUG_ERROR("I2C has not yet opened!\n");
	}
	return TINKER_ERROR;
}

/*******************************************************************************
 * @brief
 *
 * @param
 *
 * @return
 ******************************************************************************/
s32 i2c_write(i2c_dev_t i2c_dev,u16 devAddr,u8 * writeData,u16 dataLen)
{
	struct i2c_msg i2c_msg_data = {0};
	struct i2c_rdwr_ioctl_data i2c_ioctl_data = {0};

	if(g_i2c_fd[i2c_dev] > 0) {
		i2c_msg_data.addr = devAddr;
		i2c_msg_data.len = dataLen;
		i2c_msg_data.buf = writeData;

		i2c_ioctl_data.msgs = &i2c_msg_data;
		i2c_ioctl_data.nmsgs = 1;

		if( ioctl(g_i2c_fd[i2c_dev],I2C_RDWR,&i2c_ioctl_data) < 0 ) {
			DEBUG_ERROR("Unable write data from I2C device with error : errno = %d, errmsg = %s\n",errno,strerror(errno));
			return TINKER_ERROR;
		}
		return TINKER_OK;
	}
	DEBUG_ERROR("I2C has not ye opened!\n");
	return TINKER_ERROR;
}

/*******************************************************************************
 * @brief
 *
 * @param
 *
 * @return
 ******************************************************************************/
s32 i2c_close(i2c_dev_t i2c_dev)
{
	if(g_i2c_fd[i2c_dev] > 0) {
		close(g_i2c_fd[i2c_dev]);
		g_i2c_fd[i2c_dev] = 0;
	}
	return TINKER_OK;
}

/*******************************************************************************
 * @brief
 *
 * @param
 *
 * @return
 ******************************************************************************/
s32 i2c_set_slave_force(i2c_dev_t i2c_dev, u16 slaveAddr){
	if(g_i2c_fd[i2c_dev] > 0) {
		if(ioctl(g_i2c_fd[i2c_dev],I2C_SLAVE_FORCE,slaveAddr) < 0){
			return TINKER_ERROR;
		}
	}
	return TINKER_OK;
}

/*******************************************************************************
 * @brief
 *
 * @param
 *
 * @return
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
