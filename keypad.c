#include "keypad.h"

char KEYS[]= {
  '1','2','3',//'A',
  '4','5','6',//'B',
  '7','8','9',//'C',
  '*','0','#',//'D'
};

int key_to_reading[NUM_KEYS];
int debounce_time_ms = 30;

void Keypad_init_static(void) {
    int on_stack[] = { 0, 1012, 507, 337, 217, 181, 155, 123, 109, 99, 87, 79, 74 };
    int i =0;
    while(i<=NUM_KEYS) {
        key_to_reading[i]=on_stack[i];
        ++i;
    }
}

void Keypad_init_eeprom(int address) {
    key_to_reading[0] = 0;
    int i = 0;
    while(i < NUM_KEYS) {
        int value;
        // THOSE ARE 16-bit not 8-bit!!!
        // EEPROM_read(address+2*i, value);
        key_to_reading[i+1] = value;
        ++i;
    }

}



void Keypad_update_map(int key, int reading) {
    key_to_reading[key] = reading;
}

// 1 2 3 4 5 6 7 8 9 * 0 #
int Keypad_get_key_from_reading(int reading) {
    int key = 0; // no key
    int i = 0;
    while(i<=NUM_KEYS) {
        if (abs(reading - key_to_reading[i]) < abs(reading - key_to_reading[key]) ) {
            key = i;
        }
        ++i;
    }
    return key;
}

char Keypad_char_for_key(int key) {
    if (1<=key && key<=9) return '0' + key;
    else if (key==10) return '*';
    else if (key==11) return '0';
    else if (key==12) return '#';
}

int Keypad_get_key(void) {
    int reading = analog_read(ANALOG_PIN);
    if (reading >= 10) {
        delay_ms(debounce_time_ms);
        int new_reading = analog_read(ANALOG_PIN);
        if (abs(new_reading-reading) < 10) {
            return Keypad_get_key_from_reading((reading+new_reading)/2);
        } else {
            return 0; // no key
        }
    } else {
        return 0; // no key
    }
}


void Keypad_calibrate(int eeprom_address) {
    int i = 0;
    while (i < 12) {
        LCD_reset();
        LCD_print("Press ", 0);
        LCD_char(KEYS[i]);
        int done = 0;
        int good_readings = 0;
        int sum_readings = 0;
        while(!done) {
            int reading = analog_read(ANALOG_PIN);
            if (reading >= 10 && (good_readings <= 1 || abs(reading - sum_readings/good_readings) < 5)) {
                ++good_readings;
                sum_readings += reading;
                if (good_readings >= 15) {
                    done = 1;
                }
            } else {
                good_readings = 0;
                sum_readings = 0;
            }
            delay_ms(20);
        }
        Keypad_update_map(i+1, sum_readings/good_readings);
        
        //EEPROM_write(eeprom_address+2*i, sum_readings/good_readings);

        LCD_reset();
        LCD_print("M ", 0);
        LCD_char(KEYS[i]);
        LCD_char(' ');
        LCD_char('t');
        LCD_char('o');
        LCD_char(' ');
        LCD_int(sum_readings/good_readings);
        delay_ms(1000);
        done = 0;
        good_readings = 0;
        while (!done) {
            if (analog_read(ANALOG_PIN) >= 10) {
                good_readings = 0;
            } else {
                good_readings++;
                if (good_readings >= 15) {
                    done = 1;
                }
            }
        }
        delay_ms(100);
        ++i;
    }
    LCD_reset();
    LCD_print("done", 0);
    delay_ms(1000);
}
