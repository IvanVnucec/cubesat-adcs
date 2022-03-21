/**
 ******************************************************************************
 * @file           : adcs_imu.c
 * @brief          : Source file.
 *                   This file contains files for the Inertial Measurement 
 *                   Unit.
 ******************************************************************************
 * @attention
 *
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "adcs_imu.h"

#include "adcs.h"
#include "mpu9250/mpu9250.h"
#include "mcu/i2c/mcu_i2c.h"
#include "stm32l4xx_hal.h"
#include "utils/error/error.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static MPU9250_Handle_s *ADCS_IMU_mpu9250Handle = NULL;

/* Private function prototypes -----------------------------------------------*/
static int ADCS_IMU_writeToMpu9250(uint8_t subAddress, uint8_t data);
static int ADCS_IMU_readFromMpu9250(uint8_t subAddress, uint8_t count, uint8_t *dest);

/* Private user code ---------------------------------------------------------*/
void ADCS_IMU_init(ADCS_Handle_T *ADCS_handle)
{
    ADCS_IMU_mpu9250Handle = &ADCS_handle->mpu9250Handle;

    int retval = MPU9250_begin(&ADCS_IMU_writeToMpu9250,
                               &ADCS_IMU_readFromMpu9250,
                               &ADCS_delayMs,
                               ADCS_IMU_mpu9250Handle);

    ERROR_assert(retval == 1);
}

void ADCS_IMU_getData(ADCS_ImuData_T *imu_data)
{
    ERROR_assert(MPU9250_readSensor(ADCS_IMU_mpu9250Handle) == 1);

    imu_data->acc[0] = MPU9250_getAccelX_mss(ADCS_IMU_mpu9250Handle);
    imu_data->acc[1] = MPU9250_getAccelY_mss(ADCS_IMU_mpu9250Handle);
    imu_data->acc[2] = MPU9250_getAccelZ_mss(ADCS_IMU_mpu9250Handle);

    imu_data->mag[0] = MPU9250_getMagX_uT(ADCS_IMU_mpu9250Handle);
    imu_data->mag[1] = MPU9250_getMagY_uT(ADCS_IMU_mpu9250Handle);
    imu_data->mag[2] = MPU9250_getMagZ_uT(ADCS_IMU_mpu9250Handle);

    imu_data->gyr[0] = MPU9250_getGyroX_rads(ADCS_IMU_mpu9250Handle);
    imu_data->gyr[1] = MPU9250_getGyroY_rads(ADCS_IMU_mpu9250Handle);
    imu_data->gyr[2] = MPU9250_getGyroZ_rads(ADCS_IMU_mpu9250Handle);
}

static int ADCS_IMU_writeToMpu9250(uint8_t subAddress, uint8_t data)
{
    HAL_StatusTypeDef retval;

    /*
	 DevAddress Target device address: The device 7 bits address value
	 in datasheet must be shifted to the left before calling the interface
	 */
    retval = HAL_I2C_Mem_Write(&hi2c3,
                               MPU9250_I2C_ADDRESS << 1,
                               subAddress,
                               I2C_MEMADD_SIZE_8BIT,
                               &data,
                               1,
                               10);
    if (retval != HAL_OK) {
        return -1;    // failure
    }

    return 1;    // success
}

static int ADCS_IMU_readFromMpu9250(uint8_t subAddress, uint8_t count, uint8_t *dest)
{
    HAL_StatusTypeDef retval;

    /*
	 DevAddress Target device address: The device 7 bits address value
	 in datasheet must be shifted to the left before calling the interface
	 */
    retval = HAL_I2C_Mem_Read(&hi2c3,
                              MPU9250_I2C_ADDRESS << 1,
                              subAddress,
                              I2C_MEMADD_SIZE_8BIT,
                              dest,
                              count,
                              10);

    if (retval != HAL_OK) {
        return -1;    // failure
    }

    return 1;    // success
}
