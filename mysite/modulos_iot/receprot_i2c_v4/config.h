// Address Peripherals
#define ADDRESS_SERVO_01         0x00
#define ADDRESS_SERVO_02         0x01
#define ADDRESS_SERVO_03         0x02
#define ADDRESS_SERVO_04         0x03
#define ADDRESS_SERVO_05         0x04
#define ADDRESS_SERVO_06         0x05
#define ADDRESS_SERVO_07         0x06
#define ADDRESS_SERVO_08         0x07
#define ADDRESS_SERVO_09         0x08
#define ADDRESS_SERVO_10         0x09
#define ADDRESS_LED_RGB_01_RED   0x0A
#define ADDRESS_LED_RGB_01_GREEN 0x0B
#define ADDRESS_LED_RGB_01_BLUE  0x0C
#define ADDRESS_LED_RGB_02_RED   0x0D
#define ADDRESS_LED_RGB_02_GREEN 0x0E
#define ADDRESS_LED_RGB_02_BLUE  0x0F
#define ADDRESS_LED_RGB_03_RED   0x10
#define ADDRESS_LED_RGB_03_GREEN 0x11
#define ADDRESS_LED_RGB_03_BLUE  0x12
#define ADDRESS_LED_RGB_04_RED   0x13
#define ADDRESS_LED_RGB_04_GREEN 0x14
#define ADDRESS_LED_RGB_04_BLUE  0x15
#define ADDRESS_LED_RGB_05_RED   0x16
#define ADDRESS_LED_RGB_05_GREEN 0x17
#define ADDRESS_LED_RGB_05_BLUE  0x18
#define ADDRESS_LED_RGB_06_RED   0x19
#define ADDRESS_LED_RGB_06_GREEN 0x1A
#define ADDRESS_LED_RGB_06_BLUE  0x1B
#define ADDRESS_LED_RGB_07_RED   0x1C
#define ADDRESS_LED_RGB_07_GREEN 0x1D
#define ADDRESS_LED_RGB_07_BLUE  0x1E
#define ADDRESS_LED_RGB_08_RED   0x1F
#define ADDRESS_LED_RGB_08_GREEN 0x20
#define ADDRESS_LED_RGB_08_BLUE  0x21
#define ADDRESS_LED_RGB_09_RED   0x22
#define ADDRESS_LED_RGB_09_GREEN 0x23
#define ADDRESS_LED_RGB_09_BLUE  0x24
#define ADDRESS_LED_RGB_10_RED   0x25
#define ADDRESS_LED_RGB_10_GREEN 0x26
#define ADDRESS_LED_RGB_10_BLUE  0x27
#define ADDRESS_LED_LARGE_01     0x28
#define ADDRESS_LED_LARGE_02     0x29
#define ADDRESS_LED_LARGE_03     0x2A
#define ADDRESS_LED_LARGE_04     0x2B
#define ADDRESS_LED_LARGE_05     0x2C
#define ADDRESS_LED_LARGE_06     0x2D
#define ADDRESS_LED_LARGE_07     0x2E

// gpio Peripherals
#define GPIO_SERVO_01            A0
#define GPIO_SERVO_02            A1
#define GPIO_SERVO_03            A0
#define GPIO_SERVO_04            A1
#define GPIO_SERVO_05            A0
#define GPIO_SERVO_06            A1
#define GPIO_SERVO_07            A2
#define GPIO_SERVO_08            A0
#define GPIO_SERVO_09            A1
#define GPIO_SERVO_10            A2
#define GPIO_LED_RGB_01_RED      2  // SEM PWM
#define GPIO_LED_RGB_01_GREEN    3  // COM PWM
#define GPIO_LED_RGB_01_BLUE     5  // COM PWM
#define GPIO_LED_RGB_02_RED      6  // COM PWM
#define GPIO_LED_RGB_02_GREEN    7  // SEM PWM
#define GPIO_LED_RGB_02_BLUE     9  // COM PWM
#define GPIO_LED_RGB_03_RED      10 // COM PWM
#define GPIO_LED_RGB_03_GREEN    11 // COM PWM
#define GPIO_LED_RGB_03_BLUE     8  // SEM PWM
#define GPIO_LED_RGB_04_RED      2  // SEM PWM
#define GPIO_LED_RGB_04_GREEN    3  // COM PWM
#define GPIO_LED_RGB_04_BLUE     5  // COM PWM
#define GPIO_LED_RGB_05_RED      6  // COM PWM
#define GPIO_LED_RGB_05_GREEN    7  // SEM PWM
#define GPIO_LED_RGB_05_BLUE     9  // COM PWM
#define GPIO_LED_RGB_06_RED      10 // COM PWM
#define GPIO_LED_RGB_06_GREEN    11 // COM PWM
#define GPIO_LED_RGB_06_BLUE     8  // SEM PWM
#define GPIO_LED_RGB_07_RED      3  // COM PWM
#define GPIO_LED_RGB_07_GREEN    5  // COM PWM
#define GPIO_LED_RGB_07_BLUE     6  // COM PWM
#define GPIO_LED_RGB_08_RED      9  // COM PWM
#define GPIO_LED_RGB_08_GREEN    10 // COM PWM
#define GPIO_LED_RGB_08_BLUE     11 // COM PWM
#define GPIO_LED_RGB_09_RED      3  // COM PWM
#define GPIO_LED_RGB_09_GREEN    5  // COM PWM
#define GPIO_LED_RGB_09_BLUE     6  // COM PWM
#define GPIO_LED_RGB_10_RED      9  // COM PWM
#define GPIO_LED_RGB_10_GREEN    10 // COM PWM
#define GPIO_LED_RGB_10_BLUE     11 // COM PWM
#define GPIO_LED_LARGE_01        A3 // SEM PWM
#define GPIO_LED_LARGE_02        A3 // SEM PWM
#define GPIO_LED_LARGE_03        A4 // SEM PWM
#define GPIO_LED_LARGE_04        A3 // SEM PWM
#define GPIO_LED_LARGE_05        A4 // SEM PWM
#define GPIO_LED_LARGE_06        A3 // SEM PWM
#define GPIO_LED_LARGE_07        A4 // SEM PWM

// Address Master and Slave in I2C Communication
#define I2C_ADDRESS_MASTER           0X00
#define I2C_ADDRESS_SLAVE_01         0X08
#define I2C_ADDRESS_SLAVE_02         0X09
#define I2C_ADDRESS_SLAVE_03         0X0A
#define I2C_ADDRESS_SLAVE_04         0X0B

// Command to send to Master to get Slave Setup
#define GET_SETUP_TO_MASTER  0x01

// Number from bytes to command set peripheral values
#define N_BYTE_SET_PERIPHERAL 14
                          // byte 0 e 1 - peripheral
                          // byte 2 e 3 - function
                          // byte 4 e 5 - valueOn
                          // byte 6 e 7 - valueOff
                          // byte 8 e 9 - timeOn
                          // byte 10 e 11 - timeOff
                          // byte 12 e 13 - timeIddle

// Peripherals numbers
#define N_TOTAL_PERIPHERAL        47

// Servo numbers to slave
#define N_SERVO_TO_SLAVE          5

// Peripherals numbers to slave
#define N_PERIPHERAL_TO_SLAVE     10

#define TIME_PRINT_ALIVE_MS       10000