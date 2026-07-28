CC      = gcc
ASM     = nasm
CFLAGS  = -O2 -Wall -m64
ASMFLAGS = -f win64
TARGET  = arch2mp.exe

all: $(TARGET)

accel.obj: accel.asm
	$(ASM) $(ASMFLAGS) accel.asm -o accel.obj

$(TARGET): main.c accel.obj
	$(CC) $(CFLAGS) -o $(TARGET) main.c accel.obj

clean:
	-del /Q accel.obj $(TARGET) 2>NUL

.PHONY: all clean
