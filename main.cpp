#include "mbed.h"

#include "LSM6DSLSensor.h"

static DevI2C devI2c(PB_11,PB_10);
static LSM6DSLSensor acc_gyro(&devI2c,LSM6DSL_ACC_GYRO_I2C_ADDRESS_LOW,PD_11); // low address

InterruptIn mybutton(USER_BUTTON);
DigitalOut myled(LED1);

volatile int mems_event = 0;
volatile int toggle_free_fall_enable = 0;
static int free_fall_is_enabled = 1;

/* User button callback. */
void pressed_cb()
{
    toggle_free_fall_enable = 1;
}
 
/* Interrupt 1 callback. */
void int1_cb()
{
    mems_event = 1;
}

int main()
{
   
    printf("Hebe added initialization\n");
    acc_gyro.init(NULL);
    acc_gyro.enable_x();
    acc_gyro.enable_g();
    
    uint8_t id;
    acc_gyro.read_id(&id);
    printf("LSM6DSL accelerometer&gyroscope id = 0x%X\r\n", id);

    int32_t axes[3]={0};

    /* Attach callback to User button press */
    mybutton.fall(&pressed_cb);
    /* Attach callback to LSM6DSO INT1 */
    acc_gyro.attach_int1_irq(&int1_cb);

    acc_gyro.enable_free_fall_detection();  //default INT1
    

    for(int i=0;i<100;i++) {
      
        acc_gyro.get_x_axes(axes);
        printf("LSM6DSL [acc/mg]:        %6ld, %6ld, %6ld\r\n", axes[0], axes[1], axes[2]);

        acc_gyro.get_g_axes(axes);
        printf("LSM6DSL [gyro/mdps]:     %6ld, %6ld, %6ld\r\n", axes[0], axes[1], axes[2]);
        
        
         if (toggle_free_fall_enable) {
            toggle_free_fall_enable = 0;
            if (free_fall_is_enabled == 0) {
                acc_gyro.enable_free_fall_detection();
                free_fall_is_enabled = 1;
            } else {
                acc_gyro.disable_free_fall_detection();
                free_fall_is_enabled = 0;
            }
        }
 
        if (mems_event) {
            mems_event = 0;
            LSM6DSL_Event_Status_t status;
            acc_gyro.get_event_status(&status);
            if (status.FreeFallStatus) {
                /* Led blinking. */
                myled = 1;
                wait(0.2);
                myled = 0;
 
                /* Output data. */
                printf("Free Fall Detected!\r\n");
            }
        }
        

    ThisThread::sleep_for(1000);
    }
    
}
