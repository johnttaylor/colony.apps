# Common stuff
../../../libdirs.b

# Board specific directories....
src/Storm/Thermostat/Main/_adafruit_grand_central_m4
src/Storm/Thermostat/SimHouse

# Board specific middle-ware
//colony.core/src/Cpl/TShell/Cmd/FreeRTOS
//colony.core/src/Cpl/System/FreeRTOS
//colony.core/src/Cpl/System/FreeRTOS/_fatalerror
//colony.core/src/Cpl/System/_trace/_arduino
//colony.core/src/Cpl/System/_trace
//arduino/src/Cpl/Io/Serial/Arduino
//arduino/src/Bsp/Adafruit/grand_central_m4/gcc
//arduino/src/Bsp/Adafruit/grand_central_m4/gcc/FreeRTOS/source
//arduino/src/Bsp/Adafruit/grand_central_m4/gcc/FreeRTOS/source/portable/GCC/ARM_CM4F  
//arduino/src/Bsp/Adafruit/grand_central_m4/gcc/FreeRTOS/source/portable/MemMang  

# Arduino Core library for the Adafruit's Grand Central M4 board (Atmel SAMD51 mcu)
$ARDUINO_TOOLS$/hardware/samd/$ARDUINO_BSP_VER$/cores/arduino/USB
$ARDUINO_TOOLS$/hardware/samd/$ARDUINO_BSP_VER$/libraries/Wire
$ARDUINO_TOOLS$/hardware/samd/$ARDUINO_BSP_VER$/variants/grand_central_m4
$ARDUINO_TOOLS$/hardware/samd/$ARDUINO_BSP_VER$/cores/arduino > main.cpp cortex_handlers.c wiring.c

# NeoPixe library
$ARDUINO_SKETCH_FOLDER$/libraries/Adafruit_NeoPixel
