#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <fcntl.h>
#include <sys/ioctl.h>
#include <asm/ioctl.h>

#include "spi.h"
#include "hal.h"
#include "mac.h"
#include "halwrapper.h"


/// 470MHz, 10kbps (IFBW = 50KHz, Fdev = 18.75KHz), Crystal=12.8MHz
const uint16_t A7139Config[] =
{
    0x0823,     ///< SYSTEM CLOCK register,
    /// (0x0A24,0xB805)--470.001MHz, (0x0A26,0x4805)--490.001MHz, (0x0A27,0xD805)--510.001MHz
    0x0A24,     ///< PLL1 register,
    0xB805,     ///< PLL2 register,
    0x0000,     ///< PLL3 register,
    0x0E20,     ///< PLL4 register,
    0x0024,     ///< PLL5 register,
    0x0000,     ///< PLL6 register,
    0x0011,     ///< CRYSTAL register,
    0x0000,     ///< PAGEA,
    0x0000,     ///< PAGEB,
    0x18D0,     ///< RX1 register,     IFBW=50KHz
    0x7009,     ///< RX2 register,     by preamble
    0x4000,     ///< ADC register,
    0x0800,     ///< PIN CONTROL register,     Use Strobe CMD
    0x4C45,     ///< CALIBRATION register,
    0x20C0,     ///< MODE CONTROL register,    Use FIFO mode
    /// A7139Config_PageA[16]   //470MHz, 10kbps (IFBW = 50KHz, Fdev = 18.75KHz), Crystal=12.8MHz
    0xF606,     ///< TX1 register,     Fdev = 18.75kHz
    0x0000,     ///< WOR1 register,
    0xF800,     ///< WOR2 register,
    0x1907,     ///< RFI register,     Enable Tx Ramp up/down
    0x9B70,     ///< PM register,      CST=1
    0x0201,     ///< RTH register,
    0x400F,     ///< AGC1 register,
    0x2AC0,     ///< AGC2 register,
    0x0001,     ///< GIO register,     GIO2=hiZ, GIO1=WTR
    0xD181,     ///< CKO register
    0x0004,     ///< VCB register,
    0x0825,     ///< CHG1 register,    480MHz
    0x0127,     ///< CHG2 register,    500MHz
    0x003F,     ///< FIFO register,    FEP=63+1=64bytes
    0x1507,     ///< CODE register,    Preamble=4bytes, ID=4bytes
    0x0000,     ///< WCAL register,
    /// A7139Config_PageB[5]   //470MHz, 10kbps (IFBW = 50KHz, Fdev = 18.75KHz), Crystal=12.8MHz
    0x0337,     ///< TX2 register,
    0x8200,     ///< IF1 register,     Enable Auto-IF, IF=100KHz
    0x0000,     ///< IF2 register,
    0x0000,     ///< ACK register,
    0x0000      ///< ART register,
};

#define  FUNC_TX

#ifdef FUNC_TX
int main()
{
    a7139_dev_hal_t trm;

    trm.initHardware = spi0gpio24gpio27Init;
    trm.spiTransaction = spi0Trans;
    trm.readGIO1 = readGPIO24;
    trm.readGIO2 = readGPIO27;
    trm.delay10us = sleep10us;
    trm.delay1ms = sleep1ms;

    aPoweronInit(&trm, A7139Config);
    while(1) {
        if(aInitRF(&trm, A7139Config)) {
            sleep1ms(10);
        }
        else {
            break;
        }
    }

/*
    aAutoResendPrepare(&a7139[0], A7139Config);
    aAutoAckPrepare(&a7139[1], A7139Config);
    RESULT_TYPE res;

    while(1) {
        res = aAutoResend(&a7139[0], (const uint8_t*)"hello auto", 10);
        printf("auto resend result: %d\n", res);
        sleep1ms(1000);
    }
    uint8_t dat[64] = {0};
    while(1)
    {
        res = aAutoACK(&a7139[1], dat, 10);
        printf("auto ack result: %d\n", res);
        printf("received: %s\n", dat);
        sleep1ms(200);
    }
*/
    uint8_t i =0;
    uint8_t tx[64] = "hello direct";
    while(1)
    {
        tx[12] = i+'0';
        aWriteFIFO(&trm, tx, 13);
        aStrobeCmd(&trm, CMD_TX);
        if(++i > 20){
            i = 0;
        }
        sleep1ms(1000);
    }

    return 0;
}
#else
int main()
{
    a7139_dev_hal_t recv;

    recv.initHardware = spi1gpio25gpio22Init;
    recv.spiTransaction = spi1Trans;
    recv.readGIO1 = readGPIO25;
    recv.readGIO2 = readGPIO22;
    recv.delay10us = sleep10us;
    recv.delay1ms = sleep1ms;

    aPoweronInit(&recv, A7139Config);

    while(1) {
        if(aInitRF(&recv, A7139Config)) {
            sleep1ms(10);
        }
        else {
            break;
        }
    }

    uint8_t read[64];
    while(1)
    {
        aStrobeCmd(&recv, CMD_RX);
        while(recv.readGIO1());
        aReadFIFO(&recv, read, 64);
        printf("%s\n", read);
        //sleep1ms(500);
    }
    return 0;
}
#endif

