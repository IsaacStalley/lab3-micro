import serial

ser = serial.Serial('COM3', 9600) # Open conection to COM2-COM3

filename = 'voltage_data.csv' # Filename

file_open = False
while True:
    line = ser.readline().decode('utf-8').strip()

    if line != None:  # Check if the received line is not empty
        if not file_open:
            file = open(filename, 'a') # Open file to continue writting
            file_open = True
        file.write(line + '\n')
        file.flush()
    elif file_open:
        file.close() # Close file when not receiving data
        file_open = False