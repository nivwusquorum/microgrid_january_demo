const char* top_line;
const char* bottom_line;

long butter_start_ms;
int butter_length_dms;
int state;

void _butter(const char* text, int time_ms, long number) {
    state = 1; // butter on
    butter_start_ms = millis();
    butter_length_dms = time_ms;
    lcd.clear();
    lcd.home();
    if (progmem) lcd.print((const __FlashStringHelper*)text);
    else lcd.print(text);
    if (number != MAGIC_NUMBER) {
        lcd.print(number);
    }
}
void _set_line(const char* text, bool top, bool progmem) {
    if (top) {
        top_progmem = progmem;
        top_line = text;
    } else {
        bottom_progmem = progmem;
        bottom_line = text;
    }
    if (state == 0) { // butter off
        reset_ui();
    }
}

public:
LcdManager() : top_line(NULL),
               bottom_line(NULL),
               state(0) {  // butter off
}

void butter(const __FlashStringHelper* text, int time_ms, long number=MAGIC_NUMBER) {
    _butter((const char*)text, time_ms, number, true);
}

void butter(const char* text, int time_ms, long number=MAGIC_NUMBER) {
    _butter(text, time_ms, number, false);
}

void reset_ui() {
    lcd.clear();

    lcd.setCursor(0,0);
    if (top_line) {
        if (top_progmem) lcd.print((const __FlashStringHelper*)top_line);
        else lcd.print(top_line);
    } else {
        lcd.print(F(""));
    }

    lcd.setCursor(0,1);
    if (bottom_line) {
        if (bottom_progmem) lcd.print((const __FlashStringHelper*)bottom_line);
        else lcd.print(bottom_line);
    } else {
        lcd.print(F(""));
    }
    state = 0; // butter off
}

void set_line(const __FlashStringHelper* text, bool top=true) {
    _set_line((const char*)text, top, true);
}

void set_line(const char* text, bool top=true) {
    _set_line(text, top, false);
}

void step() {
    if (state == 1 && millis() - butter_start_ms > butter_length_dms) {
        //lcd.clear();
        //lcd.home()
        reset_ui();
    }
}


