from pynput import keyboard
import serial
import sys

MODE = 1
CHANGE_KEY = 'c'
CHANGE_STR = 'chmod'
STOP_SIGNAL = '-'

keys_to_press = ['a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', 'u', 'i', 'o']
piano_keys = ['C7','C#7','D7','D#7','E7','F7','F#7','G7','G#7','A7','A#7','B7']
key_pressed = False

ser = serial.Serial()
ser.baudrate = 115200
ser.port = sys.argv[1]
ser.open()


def print_intro():
    intro = """*****ZAHRATIE FAREBNEJ MELODIE - SEMESTRALNE ZADANIE*****\r\n\n
"     DOSTUPNE MODY: \r\n
"                   1 - NAPISTE RETAZEC POUZITELNYCH ZNAKOV A STLACTE ENTER\r\n
"                   2 - KLIKNITE JEDEN Z POUZITELNYCH ZNAKOV A PREHRA SA VAM TON (PIANO)\r\n\n
"AKTUALNE NASTAVENY MOD: 1\r\n\n
"PRE ZMENENIE MODU POUZITE PISMENO 'C'\r\n\n
"POUZITELNE ZNAKY: \r\n"
"                 • a - C7  \r\n
"                 • s - C#7 \r\n
"                 • d - D7  \r\n
"                 • f - D#7 \r\n
"                 • g - E7  \r\n
"                 • h - F7  \r\n
"                 • j - F#7 \r\n
"                 • k - G7  \r\n
"                 • l - G#7 \r\n
"                 • u - A7  \r\n
"                 • i - A#7 \r\n
"                 • o - B7  \r\n\n"""
    print(intro)

def on_press(key):
    global keys_to_press
    global key_pressed
    global MODE
    global piano_keys
    global CHANGE_KEY

    try:
        if MODE == 2:
            if key_pressed:
                return
            
            if key.char in keys_to_press:
                key_pressed = True
                send_start_signal(key.char)
                key_index = keys_to_press.index(key.char)
                print(f"Klaves {piano_keys[key_index]} bol stlaceny")

            elif key.char == CHANGE_KEY:
                print("Retazkovy mod")
                MODE = 1
                send_start_signal(CHANGE_KEY)
                return False
                #return False
    
    

    except AttributeError:
        print("Nespravy znak")

def on_release(key):
    global keys_to_press
    global key_pressed
    global MODE
    global STOP_SIGNAL

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
    global STOP_SIGNAL
    ser.write(bytes( STOP_SIGNAL,'utf-8'))

def send_string_tones(str_tones):
    global keys_to_press
    global ser
    global MODE

    valid_string_to_play = all(character in keys_to_press for character in str_tones)

    if valid_string_to_play:
        ser.write(bytes(str_tones,'utf-8'))
        print("Retazec bol posielany do zariadenia")

    else:
        if str_tones == CHANGE_STR:
            MODE = 2
            print("Klavir mode")
            ser.write(bytes(str_tones,'utf-8'))
        
        elif str_tones == "exit":
            print("Program sa skonci")
            sys.exit()

        else:
            print("Nespravny retazec")

if __name__ == '__main__':
    print_intro()

    while True:
        if MODE == 2:
            with keyboard.Listener(on_press=on_press, on_release=on_release) as listener:
                listener.join()

        elif MODE == 1:
            tones = input("Zadajte retazec na zahratie: ")
            send_string_tones(tones)

            

    