# STM32-Quadcopter
A quadcopter based on a STM32F103 board. It was designed after the work made on an Arduino UNO quadcopter.

On this project, evrything is homemade.

## The Board
The Board is powered by a STM32F103 board (bluepill), it has a MPU6050 as gyroscope & accelerometer and communicates with an NRF24L01. 

## The Software
The data from the MPU is computed with a kalman filter to reduce noise and get useable data.

The NRF24L01 cummunicates using the Universal-Controller Library to a 3D printed controller.

The commands to the ESC's are computed with a homemade PID library.
