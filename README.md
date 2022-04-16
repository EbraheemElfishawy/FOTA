" EKM" F.O.T.A System- Our ITI graduation project.

-------------------------------------------------------

1- Description: 

   " EKM" Firmware over the air, FOTA, lets you :

   a) Upload your firmware wirelessly(using custom bootloader)

   b) Control more than one user application.

   c) RESET Microcontroller wirelessly.

-----

2- Used microcontroller& modules:

   a) ARM cortex m4-based: STM32f446 (on NUCLEO-F446re board). 

   b) Bluetooth module HC-05.

   c) LEDS for indication.

-----

3- Used IDEs:

   a) ST Cube IDE: for MCU development.

   b) CodeBlocks:  for 'Host' application.

-----

==> More detailed description:

1- Task 1: Upload firmware wirelessly:

We have two apps connected via Bluetooth, 1st is a host app on Laptop and the 2nd is on the microcontroller which is called custom "Bootloader"

- A bootloader is a code program that is responsible for booting the MCU and controls FLASH memory APIs (R/W and erase).

- Now you can use host app on your PC to ask the bootloader to write on the FLASH memory(Burning your code).

---

2- Task 2: Control more than one user application.

We used the benefit of FLASH sectors(defined by ST) to upload more than one application.

- Handling both linker script and startup code of each application we can use these info to burn each binary file and later to jump to whatever application desired.

---

3- Task 3: c) RESET Microcontroller wirelessly: using bluetooth connection you can ask the processor during any running app on the MCU to jump to the first location at FLASH.













