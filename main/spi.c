//
// Created by fantl on 2020/3/7.
//
#include <stdint.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <asm/ioctl.h>
#include <linux/spi/spidev.h>

#include "spi.h"

const static char* spiDev0 = "/dev/spidev0.0";
const static char* spiDev1 = "/dev/spidev0.1";
static  uint8_t gSpiBPW = 8;
static  uint16_t gSpiDelay = 0;

static uint32_t gSpiSpeeds [2];
static int gSpiFds [2];

/**
 * @brief: Write and Read a block of data over the SPI bus.
 * @note: Note the data ia being read into the transmit buffer, so will overwrite it!
 * @note: This is also a full-duplex operation.
 */
/**
int spiReadWriteData(uint8_t channel, uint8_t* tx_data, uint8_t* rx_data, int len)
{
    struct spi_ioc_transfer trans = {
            .tx_buf        = (unsigned long)tx_data,
            .rx_buf        = (unsigned long)rx_data,
            .len           = len,
            .delay_usecs   = gSpiDelay,
            .speed_hz      = gSpiSpeeds[channel],
            .bits_per_word = gSpiBPW,
    };
    channel &= 1U;
    /// SPI_IOC_MESSAGE(1)的1表示spi_ioc_transfer的数量
    return ioctl(gSpiFds[channel], SPI_IOC_MESSAGE(1), &trans) ;
}
*/
int spiHalfDuplex(uint8_t channel, uint8_t* tx_data, uint16_t tx_len, uint8_t* rx_data, uint16_t rx_len)
{
    struct spi_ioc_transfer trans[2];
    memset(trans, 0, 2 * sizeof(struct spi_ioc_transfer));
    channel &= 1U;

    trans[0].tx_buf = (unsigned long) tx_data;
    trans[0].len = tx_len;
    trans[0].delay_usecs = gSpiDelay;
    trans[0].speed_hz = gSpiSpeeds[channel];
    trans[0].bits_per_word = gSpiBPW;

    trans[1].rx_buf = (unsigned long) rx_data;
    trans[1].len = rx_len;
    trans[1].delay_usecs = gSpiDelay;
    trans[1].speed_hz = gSpiSpeeds[channel];
    trans[1].bits_per_word = gSpiBPW;
    return ioctl(gSpiFds[channel], SPI_IOC_MESSAGE(2), trans);
}

/**
 * @brief: Open the SPI device, and set it up, with the mode, etc.
 */
int spiSetupMode(uint8_t channel, int speed, int mode)
{
    int fd;

    if((fd = open(channel==0 ? spiDev0:spiDev1, O_RDWR)) < 0)
    {
        printf("Unable to open SPI device: %s\n", strerror (errno));
        return -1;
    }

    gSpiSpeeds[channel] = speed;
    gSpiFds[channel] = fd;

    /**
     * @note: 设置spi的读写模式
     * Mode 0 CPOL=0, CPHA=0
     * Mode 1 CPOL=0, CPHA=1
     * Mode 2 CPOL=1, CPHA=0
     * Mode 3 CPOL=1, CPHA=1
     * 这里我们默认设置为模式0
     */
    if (ioctl (fd, SPI_IOC_WR_MODE, &mode) < 0)
    {
        printf("Can't set spi mode: %s\n", strerror (errno)) ;
        return -1;
    }

    if (ioctl (fd, SPI_IOC_RD_MODE, &mode) < 0)
    {
        printf("Can't get spi mode: %s\n", strerror (errno)) ;
        return -1;
    }

    /**
     * @note: spi的读写bit/word设置可写
     * 这里设置为8个位为一个字节
     */
    if (ioctl (fd, SPI_IOC_WR_BITS_PER_WORD, &gSpiBPW) < 0)
    {
        printf("Can't set bits per word: %s\n", strerror (errno))  ;
        return -1;
    }

    if (ioctl (fd, SPI_IOC_RD_BITS_PER_WORD, &gSpiBPW) < 0)
    {
        printf("Can't get bits per word: %s\n", strerror (errno))  ;
        return -1;
    }

    /**
     * @note: 设置spi读写速率
     */
    if (ioctl (fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed) < 0)
    {
        printf("Can't set max speed hz: %s\n", strerror (errno));
        return -1;
    }

    if (ioctl (fd, SPI_IOC_RD_MAX_SPEED_HZ, &speed) < 0)
    {
        printf("Can't get max speed hz: %s\n", strerror (errno));
        return -1;
    }

    return fd ;
}

/**
 * @brief: open the SPI device, and set it up, etc. in the default MODE 0
 */
int spiSetup(uint8_t channel, int speed)
{
    return spiSetupMode(channel, speed, SPI_MODE_0 | SPI_3WIRE);
}
