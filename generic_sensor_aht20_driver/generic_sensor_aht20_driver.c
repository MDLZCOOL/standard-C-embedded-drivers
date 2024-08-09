#include "generic_sensor_aht20_driver.h"

generic_err_t generic_sensor_aht20_init(generic_sensor_aht20_driver_interface_t *pfdev) {
    printf("init_start\r\n");
    printf("1\r\n");
    uint8_t readBuffer;
    printf("2\r\n");
    pfdev->pfsystem_interface->delay_ms(40);
    printf("3\r\n");
    pfdev->pfsystem_interface->receive(&readBuffer, 1);
    printf("4\r\n");
    if ((readBuffer & 0x08) == 0x00) {
        uint8_t sendBuffer[3] = {0xBE, 0x08, 0x00};
        pfdev->pfsystem_interface->send(sendBuffer, 3);
    }
    printf("init_end\r\n");
    return GENERIC_OK;
}

generic_err_t generic_sensor_aht20_measure(void *pfdev) {
    printf("measure_start\r\n");
    generic_sensor_aht20_driver_interface_t *ptemp = pfdev;
    uint8_t sendBuffer[3] = {0xAC, 0x33, 0x00};
    uint8_t readBuffer[6];
    ptemp->pfsystem_interface->send(sendBuffer, 3);
    ptemp->pfsystem_interface->delay_ms(75);
    ptemp->pfsystem_interface->receive(readBuffer, 6);

    if ((readBuffer[0] & 0x80) == 0x00) {
        float humi, temp;
#ifdef __CMSIS_GCC_H //只快一点点，但是看起来可能更直观一些：__REV()是硬实现的大小端序转换，在cmsis_gcc.h中定义
        humi = (__REV(*(uint32_t*)readBuffer) & 0x00fffff0) >> 4;
        temp = __REV(*(uint32_t*)(readBuffer+2)) & 0x000fffff;
#else
        humi = ((uint32_t) readBuffer[3] >> 4) + ((uint32_t) readBuffer[2] << 4) + ((uint32_t) readBuffer[1] << 12);
        temp = (((uint32_t) readBuffer[3] & 0x0F) << 16) + (((uint32_t) readBuffer[4]) << 8) + (uint32_t) readBuffer[5];
#endif
        ptemp->humidity = humi * 100 / (1 << 20);
        ptemp->temperature = temp * 200 / (1 << 20) - 50;
    }
    printf("measure_end\r\n");
    return GENERIC_OK;
}

generic_err_t generic_sensor_aht20_whoami(generic_sensor_aht20_driver_interface_t *pfdev,
                                      generic_err_t (*generic_send_func)(uint8_t *data, size_t len),
                                      generic_err_t (*generic_receive_func)(uint8_t *data, size_t len),
                                      generic_err_t (*delay_ms_func)(uint32_t nms),
                                      generic_err_t (*delay_us_func)(uint32_t nus)) {
    pfdev->pfsystem_interface = malloc(sizeof(system_interface_t));
    pfdev->pfsystem_interface->send = generic_send_func;
    pfdev->pfsystem_interface->receive = generic_receive_func;
    pfdev->pfsystem_interface->delay_ms = delay_ms_func;
    pfdev->pfsystem_interface->delay_us = delay_us_func;
    pfdev->pfinit = generic_sensor_aht20_init;
    pfdev->pfmeasure = generic_sensor_aht20_measure;

    return GENERIC_OK;
}

generic_err_t generic_sensor_aht20_del(generic_sensor_aht20_driver_interface_t *pfdev) {
    free(pfdev->pfsystem_interface);
    pfdev->pfsystem_interface = NULL;
    free(pfdev);
    pfdev = NULL;
    return GENERIC_OK;
}
