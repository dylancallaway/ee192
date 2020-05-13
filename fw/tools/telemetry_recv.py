import serial
import binascii

addr = "/dev/rfcomm0"
baud = 9600

ser = serial.Serial(port=addr, baudrate=baud,
                    bytesize=serial.EIGHTBITS,
                    parity=serial.PARITY_NONE,
                    stopbits=serial.STOPBITS_ONE)

seq = []
count = 1

while True:
    if ser.readable():
        read_data = ser.readline()
        print(read_data)
    user_input = input("Enter serial command: ")
    ser.write([ord(c) for c in user_input])
    #     for read_int in ser.read():
    #         c = chr(read_int)
    #         seq.append(c)
    # joined_seq = ''.join(str(v)
    #                      for v in seq)  # Make a string from array
    # print(joined_seq)
