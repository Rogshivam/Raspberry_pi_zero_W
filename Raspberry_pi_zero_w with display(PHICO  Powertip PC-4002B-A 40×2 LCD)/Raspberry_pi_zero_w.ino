
import RPi.GPIO as GPIO
import time

# Raspberry Pi GPIO pin assignments (BCM numbering)
LCD_RS = 26
LCD_E  = 19
LCD_D4 = 13
LCD_D5 = 6
LCD_D6 = 5
LCD_D7 = 11

LCD_WIDTH = 40

LCD_CHR = True
LCD_CMD = False

LCD_LINE_1 = 0x80
LCD_LINE_2 = 0xC0

E_PULSE = 0.0005
E_DELAY = 0.0005

GPIO.setmode(GPIO.BCM)
GPIO.setwarnings(False)

GPIO.setup(LCD_E, GPIO.OUT)
GPIO.setup(LCD_RS, GPIO.OUT)
GPIO.setup(LCD_D4, GPIO.OUT)
GPIO.setup(LCD_D5, GPIO.OUT)
GPIO.setup(LCD_D6, GPIO.OUT)
GPIO.setup(LCD_D7, GPIO.OUT)


def pulse_enable():
    time.sleep(E_DELAY)
    GPIO.output(LCD_E, True)
    time.sleep(E_PULSE)
    GPIO.output(LCD_E, False)
    time.sleep(E_DELAY)


def lcd_byte(bits, mode):
    GPIO.output(LCD_RS, mode)

    # Send upper 4 bits
    GPIO.output(LCD_D4, bool(bits & 0x10))
    GPIO.output(LCD_D5, bool(bits & 0x20))
    GPIO.output(LCD_D6, bool(bits & 0x40))
    GPIO.output(LCD_D7, bool(bits & 0x80))
    pulse_enable()

    # Send lower 4 bits
    GPIO.output(LCD_D4, bool(bits & 0x01))
    GPIO.output(LCD_D5, bool(bits & 0x02))
    GPIO.output(LCD_D6, bool(bits & 0x04))
    GPIO.output(LCD_D7, bool(bits & 0x08))
    pulse_enable()


def lcd_init():
    time.sleep(0.05)

    # HD44780 initialization sequence
    lcd_byte(0x33, LCD_CMD)
    lcd_byte(0x32, LCD_CMD)
    lcd_byte(0x28, LCD_CMD)
    lcd_byte(0x0C, LCD_CMD)
    lcd_byte(0x06, LCD_CMD)
    lcd_byte(0x01, LCD_CMD)

    time.sleep(0.005)


def lcd_string(message, line):
    message = message.ljust(LCD_WIDTH)[:LCD_WIDTH]

    lcd_byte(line, LCD_CMD)

    for character in message:
        lcd_byte(ord(character), LCD_CHR)


try:
    lcd_init()

    lcd_string("Hello Raspberry Pi!", LCD_LINE_1)
    lcd_string("PC-4002B-A LCD Test", LCD_LINE_2)

    print("LCD test running.")
    print("Press Ctrl+C to exit.")

    while True:
        time.sleep(1)

except KeyboardInterrupt:
    print("\nLCD test stopped.")

finally:
    GPIO.cleanup()
