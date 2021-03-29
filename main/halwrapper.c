//
// Created by fantl on 2020/4/13.
//
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <asm/ioctl.h>

#include "spi.h"

static int gGioFds[2][2];

void spi0gpio24gpio27Init(void)
{
    int spi_fd;
    spi_fd = spiSetup(0, 10*1000000); ///> 初始化SPI通道0
    if(spi_fd == -1)
    {
        perror("Init spi0 failed!\n");
    }

    /// GIO1 -- GPIO.5 <24>
    int export_fd = open("/sys/class/gpio/export", O_WRONLY);
    if(export_fd == -1)
    {
        printf("open gpio24 export file error\n");
        return;
    }
    if(write(export_fd, "24", 2) == -1)
    {
        printf("write gpio24 export failed\n");
    }
    close(export_fd);
    int direction_fd = open("/sys/class/gpio/gpio24/direction", O_WRONLY);
    if(direction_fd == -1)
    {
        printf("open gpio24 direction file error\n");
        return;
    }
    if(write(direction_fd, "in", sizeof("in")) == -1)
    {
        printf("write gpio24 direction failed\n");
    }
    close(direction_fd);
    gGioFds[0][0] = open("/sys/class/gpio/gpio24/value", O_RDONLY);
    if(gGioFds[0][0] == -1)
    {
        printf("open gpio24 value file error\n");
        return;
    }

    /// GIO2 -- GPIO.2 <27>
    export_fd = open("/sys/class/gpio/export", O_WRONLY);
    if(export_fd == -1)
    {
        printf("open gpio27 export file error\n");
        return;
    }
    if(write(export_fd, "27", 2) == -1)
    {
        printf("write gpio27 export failed\n");
    }
    close(export_fd);
    direction_fd = open("/sys/class/gpio/gpio27/direction", O_WRONLY);
    if(direction_fd == -1)
    {
        printf("open gpio27 direction file error\n");
        return;
    }
    if(write(direction_fd, "in", sizeof("in")) == -1)
    {
        printf("write gpio27 direction failed\n");
    }
    close(direction_fd);
    gGioFds[0][1] = open("/sys/class/gpio/gpio27/value", O_RDONLY);
    if(gGioFds[0][1] == -1)
    {
        printf("open gpio27 value file error\n");
        return;
    }
}

void spi0Trans(uint8_t* txbuff, uint16_t txlen, uint8_t* rxbuff, uint16_t rxlen)
{
    spiHalfDuplex(0, txbuff, txlen, rxbuff, rxlen);
}

uint8_t readGPIO24(void)
{
    uint8_t value;
    if(read(gGioFds[0][0] , &value, 1) == -1)
    {
        printf("read gpio24 value failed\n");
    }
    lseek(gGioFds[0][0], 0, SEEK_SET);
    value -= '0';
    return value;
}

uint8_t readGPIO27(void)
{
    uint8_t value;
    if(read(gGioFds[0][1] , &value, 1) == -1)
    {
        printf("read gpio27 value failed\n");
    }
    lseek(gGioFds[0][1], 0, SEEK_SET);
    value -= '0';
    return value;
}

void spi1gpio25gpio22Init(void)
{
    int spi_fd;
    spi_fd = spiSetup(1, 10*1000000); ///> 初始化SPI通道1
    if(spi_fd == -1)
    {
        printf("Init spi1 failed!\n");
    }

    /// GIO1 -- GPIO.6 <25>
    int export_fd = open("/sys/class/gpio/export", O_WRONLY);
    if(export_fd == -1)
    {
        printf("open gpio24 export file error\n");
        return;
    }
    if(write(export_fd, "25", 2) == -1)
    {
        printf("write gpio25 export failed\n");
    }
    close(export_fd);
    int direction_fd = open("/sys/class/gpio/gpio25/direction", O_WRONLY);
    if(direction_fd == -1)
    {
        printf("open gpio25 direction file error\n");
        return;
    }
    if(write(direction_fd, "in", sizeof("in")) == -1)
    {
        printf("write gpio25 direction failed\n");
    }
    close(direction_fd);
    gGioFds[1][0] = open("/sys/class/gpio/gpio25/value", O_RDONLY);
    if(gGioFds[1][0] == -1)
    {
        printf("open gpio25 value file error\n");
        return;
    }

    /// GIO2 -- GPIO.3 <22>
    export_fd = open("/sys/class/gpio/export", O_WRONLY);
    if(export_fd == -1)
    {
        printf("open gpio22 export file error\n");
        return;
    }
    if(write(export_fd, "22", 2) == -1)
    {
        printf("write gpio22 export failed\n");
    }
    close(export_fd);
    direction_fd = open("/sys/class/gpio/gpio22/direction", O_WRONLY);
    if(direction_fd == -1)
    {
        printf("open gpio22 direction file error\n");
        return;
    }
    if(write(direction_fd, "in", sizeof("in")) == -1)
    {
        printf("write gpio22 direction failed\n");
    }
    close(direction_fd);
    gGioFds[1][1] = open("/sys/class/gpio/gpio22/value", O_RDONLY);
    if(gGioFds[1][1] == -1)
    {
        printf("open gpio22 value file error\n");
        return;
    }
}

void spi1Trans(uint8_t* txbuff, uint16_t txlen, uint8_t* rxbuff, uint16_t rxlen)
{
    spiHalfDuplex(1, txbuff, txlen, rxbuff, rxlen);
}

uint8_t readGPIO25(void)
{
    uint8_t value;
    if(read(gGioFds[1][0] , &value, 1) == -1)
    {
        printf("read gpio25 value failed\n");
    }
    lseek(gGioFds[1][0], 0, SEEK_SET);
    value -= '0';
    return value;
}

uint8_t readGPIO22(void)
{
    uint8_t value;
    if(read(gGioFds[1][1] , &value, 1) == -1)
    {
        printf("read gpio25 value failed\n");
    }
    lseek(gGioFds[1][1], 0, SEEK_SET);
    value -= '0';
    return value;
}

void sleep1ms(uint16_t cnt)
{
    usleep(1000*cnt);
}

void sleep10us(uint16_t cnt)
{
    usleep(10*cnt);
}