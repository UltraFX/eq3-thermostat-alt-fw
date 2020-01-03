# eq3-thermostat-alt-fw
This is an alternative firmware for the cheap eq3-thermostats using an STM8L microcontroller.

## File Info
The code is built upon a template-project from the STM8L Standard-Peripheral-Library from ST Microelectronics.

It's under their MCD-ST Liberty SW License Agreement V2 license mostly. I just made a few modifications and added files for setting up and controlling parts of the Thermostat like the Display and motor.

## Building your project
The project can be built using ST Visual Develop: https://www.st.com/en/development-tools/stvd-stm8.html
opening the file "project.stw". For compiling you'll need the free Cosmic C Compiler: https://www.cosmic-software.com/download.php (free for usage with STM8)

and programmed on the STM8L using a ST-Link with SWIM Interface and the ST Visual Programmer: https://www.st.com/en/development-tools/stvp-stm32.html

## Contributing
This project is far from finished. The LCD library kinda works, but the UART communication and motor control definitely needs some overhaul. You're free to contact me via e-Mail: webmaster@ultrafx.eu and help me improving the code.

