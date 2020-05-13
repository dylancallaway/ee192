#include "mbed.h"
#include "encoder.h"
#include "motor.h"
#include "controller.h"
#include "camera.h"
#include "servo.h"
#include "telemetry.h"
// #include "MPU6050.h"

// Random useful stuff
Serial pc(USBTX, USBRX, 115200);
Serial bt(PC_10, PC_11, 9600);
DigitalOut grn_led(LED2);

// Button is E-stop for now
InterruptIn but(USER_BUTTON);
volatile bool but_pressed = 0;
void button_isr();
DigitalOut main_3v3(PA_4);

// Devices
Motor motor(PB_7, PA_15, PH_1);
Encoder encoder(PC_12, PD_2);
Servo servo(PB_0);

// (PB_5) CLK
// (PB_4) SI
// (PA_7) SIG
Camera camera(PB_5, PB_4, PA_7);

// Control stuff
Estimator estimator(&encoder, &camera);
Controller controller(&estimator, &motor, &servo);

Telemetry telemetry(&estimator, &controller, &bt);

// MPU6050 mpu;

int main()
{
    osThreadSetPriority(osThreadGetId(), osPriorityIdle);

    // Button interrupts
    but.rise(&button_isr);
    but.fall(&button_isr);
    grn_led.write(0);

    // Start cool stuff
    motor.Disable();
    main_3v3.write(1); // power all aux devices

    pc.printf("\nInitialized!\n");
    bt.printf("\nInitialized!\n");

    // i2c.frequency(400000);

    telemetry.CriticalCommandLinkInit(); // need this for all telemetry commands
}

void button_isr()
{
    motor.Disable();
    but_pressed = 1;
}
