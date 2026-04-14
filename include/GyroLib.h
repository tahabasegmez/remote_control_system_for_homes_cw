#ifndef GYROLIB_H
#define GYROLIB_H

// MPU9250 I2C Configuration
#define I2C_DEVICE_ADDR 0x68
#define I2C_MAGDEVICE_ADDR 0x0C

// Power Management
#define PWR_MGMT_1_ADDR 0x6B
#define PWR_MGMT_1_VAL 0x00
#define USER_CTRL 0x6A

// Accelerometer Configuration
#define ACCEL_XOUT_H_ADDR 0x3B
#define ACCEL_YOUT_H_ADDR 0x3D
#define ACCEL_ZOUT_H_ADDR 0x3F
#define ACCEL_CONFIG_ADDR 0x1C

// Accelerometer Scales
#define ACCEL_FSCALE_2G 0x00
#define ACCEL_FSCALE_4G 0x01
#define ACCEL_FSCALE_8G 0x02
#define ACCEL_FSCALE_16G 0x03

// Accelerometer Sensitivity Values
#define ACCEL_SENSITIVITY_2G 16384.0
#define ACCEL_SENSITIVITY_4G 8192.0
#define ACCEL_SENSITIVITY_8G 4096.0
#define ACCEL_SENSITIVITY_16G 2048.0

// Gyroscope Configuration
#define GYRO_XOUT_H_ADDR 0x43
#define GYRO_YOUT_H_ADDR 0x45
#define GYRO_ZOUT_H_ADDR 0x47
#define GYRO_CONFIG_ADDR 0x1B

// Gyroscope Scales
#define GYRO_FSCALE_250 0x00
#define GYRO_FSCALE_500 0x01
#define GYRO_FSCALE_1000 0x02
#define GYRO_FSCALE_2000 0x03

// Gyroscope Sensitivity Values
#define GYRO_SENSITIVITY_250 131.0
#define GYRO_SENSITIVITY_500 65.5
#define GYRO_SENSITIVITY_1000 32.8
#define GYRO_SENSITIVITY_2000 16.4

// Temperature Configuration
#define TEMP_OUT_H_ADDR 0x41
#define ROOMTEMP_OFFSET 4.0
#define TEMP_SENSITIVITY 333.87

// Gravitational constant
#define GRAVITATIONAL_CONST 9.7803

class Gyro {
public:
    // Required methods from specifications
    bool init();               // Initialize sensor
    
    // Gyroscope readings (degrees/second)
    double getGyroX();
    double getGyroY(); 
    double getGyroZ();
    
    // Accelerometer readings (m/s²)
    double getAccelX();
    double getAccelY();
    double getAccelZ();
    
    // Temperature reading (Celsius)
    double getTemperature();
    
private:
    bool initialized;
    int device_handle;
    double gyro_scale_factor;
    double accel_scale_factor;
    
    // Internal helper functions
    int readWord(int address);
    bool wakeUpDevice();
    void setGyroScale(int scale_val);
    void setAccelScale(int scale_val);
    double scaleGyro(int raw_gyro);
    double scaleAccel(int raw_accel);
    double scaleTemp(int raw_temp);
    void deviceWait(int milliseconds);
};

#endif // GYROLIB_H 