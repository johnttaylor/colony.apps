# Common stuff
../../../libdirs.b

# Board specific middle-ware
//colony.core/src/Cpl/TShell/Cmd/FreeRTOS


# Colony::Core libraries (note: the Feather52/Arduino boards uses FreeRTOS)
//colony.core/src/Cpl/System/FreeRTOS
//colony.core/src/Cpl/System/FreeRTOS/_fatalerror
//colony.core/src/Cpl/System/_trace/_arduino
//colony.core/src/Cpl/Io/Serial/Arduino
//Arduino/src/Bsp/Adafruit/grand_central_m4/gcc

# Ardunio Core library for the Adafruit's Grand Central M4 board (Atmel SAMD51 mcu)
$ARDUINO_TOOLS$/hardware/samd/$ARDUINO_BSP_VER$/cores/arduino/USB
$ARDUINO_TOOLS$/hardware/samd/$ARDUINO_BSP_VER$/libraries/Wire
$ARDUINO_TOOLS$/hardware/samd/$ARDUINO_BSP_VER$/variants/grand_central_m4
$ARDUINO_TOOLS$/hardware/samd/$ARDUINO_BSP_VER$/cores/arduino > main.cpp cortex_handlers.c
src/Bsp/Adafruit/grand_central_m4/gcc
src/Bsp/Adafruit/grand_central_m4/gcc/FreeRTOS/source
src/Bsp/Adafruit/grand_central_m4/gcc/FreeRTOS/source/portable/GCC/ARM_CM4F  
src/Bsp/Adafruit/grand_central_m4/gcc/FreeRTOS/source/portable/MemMang  
