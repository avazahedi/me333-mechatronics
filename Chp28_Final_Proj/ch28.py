# chapter 28 in python

# sudo apt-get install python3-pip
# python3 -m pip install pyserial
# sudo apt-get install python3-matplotlib

import serial
ser = serial.Serial('/dev/ttyUSB0',230400)
print('Opening port: ')
print(ser.name)

import matplotlib.pyplot as plt 
from statistics import mean 
def read_plot_matrix():
    n_str = ser.read_until(b'\n');  # get the number of data points to receive
    n_int = int(n_str) # turn it into an int
    print('Data length = ' + str(n_int))
    ref = []
    data = []
    data_received = 0
    while data_received < n_int:
        dat_str = ser.read_until(b'\n');  # get the data as a string, ints seperated by spaces
        dat_f = list(map(float,dat_str.split())) # now the data is a list
        ref.append(dat_f[0])
        data.append(dat_f[1])
        data_received = data_received + 1
    meanzip = zip(ref,data)
    meanlist = []
    for i,j in meanzip:
        meanlist.append(abs(i-j))
    score = mean(meanlist)
    t = range(len(ref)) # index array
    plt.plot(t,ref,'r*-',t,data,'b*-')
    plt.title('Score = ' + str(score))
    plt.ylabel('value')
    plt.xlabel('index')
    plt.show()

from genref import genRef

has_quit = False
# menu loop
while not has_quit:
    print('PIC32 MOTOR DRIVER INTERFACE')
    # display the menu options; this list will grow
    print('\tb: Read current sensor (mA) \n'
          '\tc: Get encoder count \n'
          '\td: Give a number, get num+1 \n'
          '\te: Set encoder count to 0 \n'
          '\tf: Set PWM (-100 to 100) \n'
          '\tg: Set current gains \n'
          '\th: Get current gains \n'
          '\ti: Set position gains \n'
          '\tj: Get position gains \n'
          '\tk: Test current gains \n'
          '\tl: Go to angle (deg) \n'
          '\tm: Load step trajectory \n'
          '\tn: Load cubic trajectory \n'
          '\to: Execute trajectory \n'
          '\tp: Unpower the motor \n'
          '\tr: Get mode \n'
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
        print('PWM set\n') # print it to the screen
    
    elif (selection == 'g'):
        gains_str = input('Set current gains Kp Ki: ') # get the gains to send
        kp_str, ki_str = gains_str.split(' ', 1)
        kp_flt = float(kp_str) # turn it into a float
        ki_flt = float(ki_str)
        print('Kp = ' + str(kp_flt) + ', Ki = ' + str(ki_flt)) # print it to the screen to double check

        ser.write((str(kp_flt) + ' ' + str(ki_flt) +'\n').encode()); # send the number
        print('Current gains set')
    
    elif (selection == 'h'):
        print('Current gains: ')
        kp_str = ser.read_until(b'\n');  # get the incremented number back
        kp_flt = float(kp_str)  # turn it into a float

        ki_str = ser.read_until(b'\n');  # get the incremented number back
        ki_flt = float(ki_str)  # turn it into a float

        print('Kp = ' + str(kp_flt) + '\r\nKi = ' + str(ki_flt) + '\n') # print it to the screen

    elif (selection == 'i'):
        gains_str = input('Set position gains Kp Ki Kd: ') # get the gains to send
        kp_str, ki_str, kd_str = gains_str.split(' ', 2)
        kp_flt = float(kp_str) # turn it into a float
        ki_flt = float(ki_str)
        kd_flt = float(kd_str)
        print('Kp = ' + str(kp_flt) + ', Ki = ' + str(ki_flt) + ', Kd = ' + str(kd_flt) + '\n') # print it to the screen to double check

        ser.write((str(kp_flt) + ' ' + str(ki_flt) + ' ' + str(kd_flt) +'\n').encode()); # send the number
        print('Position gains set')

    elif (selection == 'j'):
        print('Current gains: ')
        kp_str = ser.read_until(b'\n');  # get the incremented number back
        kp_flt = float(kp_str)  # turn it into a float

        ki_str = ser.read_until(b'\n');  # get the incremented number back
        ki_flt = float(ki_str)  # turn it into a float

        kd_str = ser.read_until(b'\n');  # get the incremented number back
        kd_flt = float(kd_str)  # turn it into a float

        print('Kp = ' + str(kp_flt) + ', Ki = ' + str(ki_flt) + ', Kd = ' + str(kd_flt) + '\n') # print it to the screen

    elif (selection == 'k'):
        read_plot_matrix()

    elif (selection == 'l'):
        ang_str = input('Go to angle (deg): ') # get the angle to send
        ang = float(ang_str) # turn it into a float
        print('Desired angle: ' + str(ang)) # print it to the screen to double check

        ser.write((str(ang) + '\n').encode()); # send the number
        print('Desired angle set \n')

        read_plot_matrix()

    elif (selection == 'm'):        
        ref = genRef('step')

        for i in range(len(ref)):
            ser.write((str(ref[i]) + '\n').encode()); # send each ref position

        # t = range(len(ref))
        # plt.plot(t,ref,'r*-')
        # plt.ylabel('value')
        # plt.xlabel('index')
        # plt.show()

    elif (selection == 'n'):
        ref = genRef('cubic')

        for i in range(len(ref)):
            ser.write((str(ref[i]) + '\n').encode()); # send each ref position

        # t = range(len(ref))
        # plt.plot(t,ref,'r*-')
        # plt.ylabel('value')
        # plt.xlabel('index')
        # plt.show()

    elif (selection == 'o'):
        read_plot_matrix()
    
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



