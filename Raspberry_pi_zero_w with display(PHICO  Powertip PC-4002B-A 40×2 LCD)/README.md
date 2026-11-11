# Raspberry Pi Zero WH + PHICO / Powertip PC-4002B-A 40×2 LCD

A beginner-friendly guide to wiring, testing, and displaying text on a
**PHICO / Powertip PC-4002B-A 40-column × 2-row character LCD** using a
**Raspberry Pi Zero WH** and Python.

This guide covers:

-   LCD pin identification and Raspberry Pi physical pin numbers
-   4-bit parallel mode wiring
-   Contrast adjustment
-   Backlight power
-   Python setup and a working text-display test
-   Troubleshooting and electrical safety

> **Important electrical warning:** Raspberry Pi GPIO pins use 3.3 V
> logic and are not 5 V tolerant. A 5 V-powered LCD may require 5 V
> logic-high signals. For a reliable and electrically safe installation,
> use a suitable 5 V logic buffer such as a 74HCT125 (or an equivalent
> buffer explicitly compatible with 3.3 V inputs and 5 V outputs). Do
> not assume that direct GPIO-to-LCD wiring is safe just because it
> appears to work.

------------------------------------------------------------------------

## 1. Hardware

### Required

-   Raspberry Pi Zero WH
-   PHICO / Powertip PC-4002B-A 40×2 character LCD
-   Breadboard and jumper wires
-   Stable Raspberry Pi power supply
-   Suitable 5 V logic buffer for the six LCD control/data signals
-   Common ground between the Pi, buffer, and LCD

### Recommended for contrast adjustment

-   10 kΩ linear potentiometer (often marked **B10K**)
-   Three jumper wires

### Backlight resistor

The LCD backlight needs current limiting unless the exact module already
has a suitable onboard resistor or current-control circuit. Check the
markings and documentation for your exact PC-4002B-A module. **Do not
connect the backlight LED directly to 5 V unless its onboard current
limiting is confirmed.** If there is no onboard resistor, choose a
series resistor using the backlight forward-voltage and rated-current
specifications.

------------------------------------------------------------------------

## 2. Understand the LCD pins

The LCD has a standard 16-pin parallel interface. The following table
describes the usual HD44780-compatible pin functions. Confirm the pin-1
orientation against the markings on your actual module before wiring.

    LCD pin Name       Function
  --------- ---------- ----------------------------------------------------
          1 VSS        Ground
          2 VDD        LCD logic supply, typically +5 V
          3 VO         Contrast input
          4 RS         Register select
          5 R/W        Read/write select; ground for write-only operation
          6 E          Enable
          7 DB0        Data bit 0
          8 DB1        Data bit 1
          9 DB2        Data bit 2
         10 DB3        Data bit 3
         11 DB4        Data bit 4
         12 DB5        Data bit 5
         13 DB6        Data bit 6
         14 DB7        Data bit 7
         15 A / LED+   Backlight anode
         16 K / LED−   Backlight cathode

This project uses **4-bit mode**, so LCD pins 7--10 are not connected.
Data is sent using DB4--DB7 (pins 11--14).

------------------------------------------------------------------------

## 3. Raspberry Pi Zero WH pin reference

The program uses BCM GPIO numbering. The physical pin numbers are
different, so follow the physical-pin column when wiring.

    Raspberry Pi physical pin BCM GPIO / function
  --------------------------- ---------------------
                            2 5 V
                            6 Ground
                           23 GPIO11
                           29 GPIO5
                           31 GPIO6
                           33 GPIO13
                           35 GPIO19
                           37 GPIO26

Useful references:

-   Physical pin 2 supplies 5 V.
-   Physical pin 6 is ground.
-   BCM GPIO numbers are used in the Python program, not physical pin
    numbers.

------------------------------------------------------------------------

## 4. Wiring the LCD

**Turn off and unplug the Raspberry Pi before connecting or moving
wires.**

### 4.1 LCD power, contrast, and control

  ---------------------------------------------------------------------
                      LCD pin LCD signal           Connection
  --------------------------- -------------------- --------------------
                            1 VSS                  Pi physical pin 6
                                                   (GND)

                            2 VDD                  Pi physical pin 2 (5
                                                   V), provided the Pi
                                                   supply has
                                                   sufficient current
                                                   capacity

                            3 VO                   Potentiometer middle
                                                   terminal (wiper)

                            5 R/W                  Pi physical pin 6
                                                   (GND)

                           15 A / LED+             Backlight supply
                                                   through verified
                                                   onboard or external
                                                   current limiting

                           16 K / LED−             GND, according to
                                                   the module's
                                                   backlight polarity
  ---------------------------------------------------------------------

### 4.2 Contrast potentiometer

Connect a **10 kΩ linear potentiometer** as follows:

  Potentiometer terminal   Connection
  ------------------------ -------------------------
  Outer terminal 1         Pi physical pin 2 (5 V)
  Middle terminal          LCD pin 3 (VO)
  Outer terminal 3         Pi physical pin 6 (GND)

Turn the potentiometer slowly after powering the LCD until the
characters are clearly visible. If turning it in one direction makes the
display less readable, turn it the other way.

A temporary test on many standard character LCDs is to connect VO (pin
3) to GND. This often gives very high contrast, which may look too dark.
It is not a substitute for an adjustable contrast circuit. Never connect
VO directly to 5 V.

### 4.3 Backlight pins 15 and 16

-   Pin 15 is normally the backlight anode (+).
-   Pin 16 is normally the backlight cathode (−).
-   Verify the module's backlight voltage/current rating and whether it
    has an onboard resistor.
-   If there is no onboard current limiting, use a correctly calculated
    series resistor.
-   The backlight may use a noticeable amount of current. Check that the
    Raspberry Pi's power supply and total current budget can support
    both the Pi and LCD.

The backlight and character display are separate circuits. A glowing
backlight does not prove that the LCD controller is initialized.

### 4.4 Four-bit signal wiring

The GPIO assignments used by the supplied Python program are:

    LCD pin Signal     Pi BCM GPIO   Pi physical pin
  --------- -------- ------------- -----------------
          4 RS              GPIO26                37
          6 E               GPIO19                35
         11 DB4             GPIO13                33
         12 DB5              GPIO6                31
         13 DB6              GPIO5                29
         14 DB7             GPIO11                23

Pins 7, 8, 9, and 10 are left unconnected in 4-bit mode.

**Logic-level note:** Put a suitable 5 V logic buffer between the Pi
GPIO outputs and LCD pins 4, 6, and 11--14. Power the buffer from an
appropriate 5 V supply and connect its ground to Pi ground and LCD
ground. A 74HCT-family buffer is commonly used because its inputs can
recognize 3.3 V logic while operating from 5 V; check the exact part's
datasheet and wiring requirements. Do not connect a 5 V buffer output to
a Raspberry Pi GPIO input.

------------------------------------------------------------------------

## 5. Prepare Raspberry Pi OS

Open a terminal on the Pi or connect over SSH.

Update package information:

``` bash
sudo apt update
```

Install the GPIO library packaged for Raspberry Pi OS:

``` bash
sudo apt install -y python3-rpi.gpio
```

Check that Python can import it:

``` bash
python3 -c "import RPi.GPIO as GPIO; print('RPi.GPIO is available')"
```

If the import succeeds, continue.

------------------------------------------------------------------------

## 6. Project code

The LCD program for this project is maintained in the project's GitHub
repository. I wrote the code there, so use the repository version rather
than copying a separate code listing from this README.

**Open the source code:**

[Raspberry_pi_zero_w.ino ---
GitHub](https://github.com/Rogshivam/Raspberry_pi_zero_W/blob/master/Raspberry_pi_zero_w/Raspberry_pi_zero_w.ino)

You can view the source, copy it, or download it from GitHub using the
**Raw** or **Download raw file** option.

> **Note:** The linked file has a `.ino` extension. That is an Arduino
> sketch format, whereas this guide's Raspberry Pi setup section
> describes Python with `RPi.GPIO`. Follow the language and hardware
> instructions in the linked source itself; do not run an `.ino` file
> using `python3`. If you intend to run the Python program on Raspberry
> Pi OS, use a `.py` source file instead.

### Open the code from the Raspberry Pi terminal

If Git is installed on your Raspberry Pi, you can clone the repository:

``` bash
git clone https://github.com/Rogshivam/Raspberry_pi_zero_W.git
```

Then enter the project directory:

``` bash
cd Raspberry_pi_zero_W/Raspberry_pi_zero_w
```

Inspect the file:

``` bash
ls -l
```

The source can also be opened in a browser directly using the GitHub
link above.

------------------------------------------------------------------------

## 8. Troubleshooting

### Backlight is ON, but the screen is blank

Check:

1.  LCD pin 1 is connected to GND.
2.  LCD pin 2 is connected to the correct supply.
3.  LCD pin 3 (VO) is connected to the potentiometer wiper, not left
    floating.
4.  LCD pin 5 (R/W) is connected to GND.
5.  RS, E, DB4, DB5, DB6, and DB7 match the wiring table.
6.  The buffer is powered correctly and shares a common ground.
7.  The correct Python file is being run.

### Dark blocks appear, but no text appears

The LCD may have power and contrast but may not be receiving valid
initialization commands. Recheck:

-   RS and E wiring
-   DB4--DB7 order
-   The four-bit initialization sequence
-   Common ground
-   Logic-buffer direction and power

### Text is visible but too dark or too faint

Adjust the 10 kΩ contrast potentiometer slowly. Contrast is an
electrical voltage on VO; changing Python text or GPIO code does not
adjust it.

### Text appears corrupted

Check the order of DB4, DB5, DB6, and DB7. Confirm that the code uses
BCM numbering and that the physical pin numbers match the table. Also
check for loose jumper wires and poor breadboard contacts.

### `ModuleNotFoundError: No module named 'RPi'`

Install the package:

``` bash
sudo apt update
sudo apt install -y python3-rpi.gpio
```

Then retry:

``` bash
sudo python3 lcd_display.py
```

### GPIO busy or permission errors

Stop any other script using the same GPIO pins. Run the program with
`sudo` as shown above.

------------------------------------------------------------------------

## 9. Useful terminal commands

  Task                   Command
  ---------------------- ----------------------------------------
  Show Pi pinout         `pinout`
  Check Python version   `python3 --version`
  Install GPIO library   `sudo apt install -y python3-rpi.gpio`
  Create/edit program    `nano lcd_display.py`
  Run program            `sudo python3 lcd_display.py`
  Stop program           `Ctrl + C`

------------------------------------------------------------------------

## 10. Important notes

-   This project uses a **parallel 4-bit interface**, not I²C.
-   LCD pins 7--10 are intentionally unused.
-   The Python code uses BCM GPIO numbering.
-   The LCD's contrast is adjusted electrically through pin 3 (VO).
-   The backlight is powered separately from the character controller.
-   Do not assume the backlight has a built-in resistor; verify the
    exact module.
-   Do not apply 5 V to any Raspberry Pi GPIO.
-   A 40×2 LCD has 40 character positions on each of two rows. This
    program writes to the standard first- and second-row addresses
    (`0x80` and `0xC0`) used by common HD44780-compatible 40×2 modules.
    If your particular controller/module uses a different mapping,
    consult its datasheet.

------------------------------------------------------------------------

## 11. Project status checklist

Use this checklist as you complete the build:

-   [ ] Raspberry Pi Zero WH boots and is accessible.
-   [ ] LCD supply and ground are connected correctly.
-   [ ] LCD backlight works with verified current limiting.
-   [ ] LCD contrast is adjustable and text is readable.
-   [ ] LCD R/W pin is grounded for write-only operation.
-   [ ] 5 V logic buffering is installed and wired correctly.
-   [ ] RS, E, and DB4--DB7 wiring is checked.
-   [ ] Python GPIO library is installed.
-   [ ] `lcd_display.py` runs without errors.
-   [ ] Both LCD rows display the expected messages.

------------------------------------------------------------------------

## 12. Next improvements

Once the basic display is stable, this setup can be extended to show:

-   Raspberry Pi hostname and IP address
-   Wi-Fi connection status
-   CPU temperature and system uptime
-   Network status for a home-lab device
-   Sensor readings
-   Status messages from other Python projects

Keep the electrical wiring and contrast adjustment stable before adding
more software features.

**End of guide.**
