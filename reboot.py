import serial
import subprocess

# Configure the serial port
port_name = 'COM9'  # Change this to the appropriate port name
baud_rate = 9600    # Set the baud rate

try:
    # Open the serial port
    ser = serial.Serial(port_name, baud_rate)
    print(f"Serial port {port_name} opened successfully.")

    # Main loop
    while True:
        # Read data from the serial port
        data = ser.readline().decode().strip()
        print("Received:", data)  # Debug print to check received data
        # If the received message is "reboot", trigger a system reboot
        if data == 'reboot':
            print("Rebooting system...")
            # Execute system reboot command
            subprocess.run(['C:\\Windows\\System32\\shutdown.exe', '/r', '/t', '0'], shell=True)
            break  # Exit the loop after triggering the reboot

    # Close the serial port
    ser.close()

except serial.SerialException as e:
    print(f"Failed to open serial port {port_name}: {e}")

except Exception as e:
    print(f"An error occurred: {e}")
