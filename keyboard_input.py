from pynput import keyboard
import serial
import sys

MODE = 2
CHANGE_KEY = 'c'
CHANGE_STR = 'chmod'
CHANGE_SIGNAL = '-'

keys_to_press = {'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', 'u', 'i', 'o'}
key_pressed = False

ser = serial.Serial()
ser.baudrate = 115200
ser.port = 'COM4'
ser.open()


def print_intro():
    intro = "*****ZAHRATIE FAREBNEJ MELODIE - SEMESTRALNE ZADANIE*****\n\nDOSTUPNE MODY: 1 - NAPISTE RETAZEC POUZITELNYCH ZNAKOV A STLACTE ENTER\n2 - KLIKNITE JEDEN Z POUZITELNYCH ZNAKOV A PREHRA SA VAM TON (PIANO)\n\nAKTUALNE NASTAVENY MOD: 1\n\nPRE ZMENENIE MODU POUZITE PRIKAZ: chmod 1 alebo chmod 2\n\nPOUZITELNE ZNAKY: 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', 'u', 'i', 'o' \n\n"
    print(intro)

def on_press(key):
    global keys_to_press
    global key_pressed
    global MODE

    try:
        if MODE == 2:
            if key_pressed:
                return
            
            if key.char in keys_to_press:
                key_pressed = True
                send_start_signal(key.char)
                print("Piano key pressed")

            elif key.char == CHANGE_KEY:
                print("String mode")
                MODE = 1
                send_start_signal(CHANGE_KEY)
                return False
                #return False

    

    except AttributeError:
        print("Invalid key pressed")

def on_release(key):
    global keys_to_press
    global key_pressed
    global MODE

    try:
        if key_pressed:
            key_pressed = False
            send_stop_signal()

        
    except AttributeError:
        pass

def send_start_signal(key):
    global ser
    ser.write(bytes(key,'utf-8'))

def send_stop_signal():
    global ser
    ser.write(bytes('-','utf-8'))

def send_string_tones(str_tones):
    global keys_to_press
    global ser
    global MODE

    valid_string_to_play = all(character in keys_to_press for character in str_tones)

    if valid_string_to_play:
        ser.write(bytes(str_tones,'utf-8'))
        print("String sent to device")

    else:
        if str_tones == CHANGE_STR:
            MODE = 2
            print("Piano mode")
            ser.write(bytes(str_tones,'utf-8'))
        
        elif str_tones == "exit":
            print("Program terminates")
            sys.exit()

        else:
            print("Invalid string")

if __name__ == '__main__':
    #print_intro()

    while True:
        if MODE == 2:
            with keyboard.Listener(on_press=on_press, on_release=on_release) as listener:
                listener.join()

        elif MODE == 1:
            tones = input("Enter a string to play: ")
            send_string_tones(tones)

            

    