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

#include "FreeRTOS.h"
#include "task.h"
#include "mcu/i2c/mcu_i2c.h"
#include "utils/error/error.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static MPU9250_Handle_s *ADCS_IMU_mpu9250Handle = NULL;

// clang-format off
const static float A[3][3] = {
    {1.0f, 0.0f, 0.0f}, 
    {0.0f, 1.0f, 0.0f}, 
    {0.0f, 0.0f, 1.0f}
};
const static float b[3] = {12.6506f, -17.8589f, -56.9918f};
// clang-format on

/* Private function prototypes -----------------------------------------------*/
static void ADCS_IMU_delayMs(unsigned ms);
static void ADCS_IMU_calibrateMagnetometerMeasurements(float mag[3]);
static int ADCS_IMU_writeToMpu9250(uint8_t subAddress, uint8_t data);
static int ADCS_IMU_readFromMpu9250(uint8_t subAddress, uint8_t count, uint8_t *dest);

/* Private user code ---------------------------------------------------------*/
void ADCS_IMU_init(MPU9250_Handle_s *mpu9250Handle)
{
    ADCS_IMU_mpu9250Handle = mpu9250Handle;

    int retval = MPU9250_begin(&ADCS_IMU_writeToMpu9250,
                               &ADCS_IMU_readFromMpu9250,
                               &ADCS_IMU_delayMs,
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

    ADCS_IMU_calibrateMagnetometerMeasurements(imu_data->mag);
}

static void ADCS_IMU_calibrateMagnetometerMeasurements(float mag[3])
{
    float c[3];

    // c = mag - b
    c[0] = mag[0] - b[0];
    c[1] = mag[1] - b[1];
    c[2] = mag[2] - b[2];

    // mag = c * A
    mag[0] = c[0] * A[0][0] + c[1] * A[1][0] + c[2] * A[2][0];
    mag[1] = c[0] * A[0][1] + c[1] * A[1][1] + c[2] * A[2][1];
    mag[2] = c[0] * A[0][2] + c[1] * A[1][2] + c[2] * A[2][2];
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

static void ADCS_IMU_delayMs(unsigned ms)
{
    vTaskDelay(pdMS_TO_TICKS(ms));
}
