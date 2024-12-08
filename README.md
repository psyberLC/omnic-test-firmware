# _Test firmware for OMNIC technical interview_

This firmware was made from the simplest buildable example used by command `idf.py create-project` that copies the project to user specified path and set its name. 
For more information follow the [docs page](https://docs.espressif.com/projects/esp-idf/en/latest/api-guides/build-system.html#start-a-new-project).

## Firmware and LED configuration
By default firmware assumes that user's LED pin is GPIO_NUM_2 and user's LED is connected to the microcontroller with a pull-down resistor (i.e. LED is active on high signal).
If user's configuration is different, there are 2 options to change the settings:
### Using [led_config.py](led_config.py) script (recommended):
Before building the firmware, run the [led_config.py](led_config.py) Python script from project's root directory with following arguments:
```
<user_Python_interpreter> led_config.py <LED_pin_number> [pu/pd]
```
where `pu` stands for pull-up resistor configuration (i.e. LED is active on low signal) and `pd` stands for pull-down resistor configuration (i.e. LED is active on high signal).
### Changing the macro values in source file [main.c](main/main.c) directly:
Change the values of macros `LED_PIN`, `LED_ON` and `LED_OFF` in source file [main.c](main/main.c) to desired ones.

## How to use the firmware
A recommended way to build the project is to follow the instructions on a [docs page](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/get-started/index.html#build-your-first-project).

## Project folder contents

The project **omnic_test_firmware** contains one source file in C language [main.c](main/main.c). The file is located in folder [main](main).
Additionally, the project contains [led_config.py](led_config.py) Python script to modify the source file according to user's LED configuration before building and flashing procedure.

This ESP-IDF project is built using CMake. The project build configuration is contained in `CMakeLists.txt`
files that provide set of directives and instructions describing the project's source files and targets
(executable, library, or both). 

Below is short explanation of remaining files in the project folder:

```
├── .gitignore
├── CMakeLists.txt
├── version.txt
├── main
│   ├── CMakeLists.txt
│   └── main.c
├── led_config.py
└── README.md                  This is the file you are currently reading
```
