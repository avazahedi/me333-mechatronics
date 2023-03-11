# chapter 28 in python

# sudo apt-get install python3-pip
# python3 -m pip install pyserial
# sudo apt-get install python3-matplotlib

import serial
ser = serial.Serial('/dev/ttyUSB0',230400)
print('Opening port: ')
print(ser.name)

has_quit = False
# menu loop
while not has_quit:
    print('PIC32 MOTOR DRIVER INTERFACE')
    # display the menu options; this list will grow
    print('\tb: Read current sensor (mA) \n\\'
          '\tc: Get encoder count \n\\'
          '\td: Give a number, get num+1 \n\\'
          '\te: Set encoder count to 0 \n\\'
          '\tf: Set PWM (-100 to 100) \n\\'
          '\tp: Unpower the motor) \n\\'
          '\tr: Get mode \n\\'
          '\tq: Quit') # '\t' is a tab
    # read the user's choice
    selection = input('\nENTER COMMAND: ')
    selection_endline = selection+'\n'
     
    # send the command to the PIC32
    ser.write(selection_endline.encode()); # .encode() turns the string into a char array
    
    # take the appropriate action
    # there is no switch() in python, using if elif instead
    if (selection == 'd'):
        # example operation
        n_str = input('Enter number: ') # get the number to send
        n_int = int(n_str) # turn it into an int
        print('number = ' + str(n_int)) # print it to the screen to double check

        ser.write((str(n_int)+'\n').encode()); # send the number
        n_str = ser.read_until(b'\n');  # get the incremented number back
        n_int = int(n_str) # turn it into an int
        print('Got back: ' + str(n_int) + '\n') # print it to the screen
    elif (selection == 'b'):
        n_str = ser.read_until(b'\n');  # get the current sensor (mA) back
        n_float = float(n_str)
        print('Current sensor: ' + str(n_float) + ' mA \n')
    elif (selection == 'c'):
        ser.write((str(n_int)+'\n').encode());  # send the number
        n_str = ser.read_until(b'\n');  # get the incremented number back
        n_int = int(n_str)  # turn it into an int
        print('Got back encoder count: ' + str(n_int) + '\n')   # print it to the screen
    elif (selection == 'e'):
        print('Reset encoder ' + '\n')  # print it to the screen
    elif (selection == 'f'):
        n_str = input('Set PWM (-100 to 100): ') # get the number to send
        n_int = int(n_str) # turn it into an int
        print('number = ' + str(n_int)) # print it to the screen to double check

        ser.write((str(n_int)+'\n').encode()); # send the number
        # n_str = ser.read_until(b'\n');  # get the incremented number back
        # n_int = int(n_str) # turn it into an int
        print('PWM set\n') # print it to the screen
    elif (selection == 'p'):
        print('Unpowered the motor')
    elif (selection == 'r'):
        n_str = ser.read_until(b'\n');  # get the mode int back
        n_int = int(n_str)
        # get equivalent name
        if n_int == 0:
            mode_name = 'IDLE'
        elif n_int == 1:
            mode_name = 'PWM'
        elif n_int == 2:
            mode_name = 'ITEST'
        elif n_int == 3:
            mode_name = 'HOLD'
        elif n_int == 4:
            mode_name = 'TRACK'
        else:
            mode_name = str(n_int)
        print('Mode: ' + mode_name + ' ' + str(n_int) + '\n')
    elif (selection == 'q'):
        print('Exiting client')
        has_quit = True; # exit client
        # be sure to close the port
        ser.close()
    else:
        print('Invalid Selection ' + selection_endline)



