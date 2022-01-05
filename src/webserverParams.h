#ifndef WEBSERVER_PARAMS_H
#define WEBSERVER_PARAMS_H

String params =
    "["
    "{"
    "'name':'ssid',"
    "'label':'Name des WLAN',"
    "'type':" +
    String(INPUTTEXT) +
    ","
    "'default':''"
    "},"
    "{"
    "'name':'pwd',"
    "'label':'WLAN Passwort',"
    "'type':" +
    String(INPUTPASSWORD) +
    ","
    "'default':''"
    "},"
    "{"
    "'name':'brightness',"
    "'label':'Brightness',"
    "'type':" +
    String(INPUTNUMBER) +
    ","
    "'min':0,'max':255,"
    "'default':'60'"
    "},"
    "{"
    "'name':'mode',"
    "'label':'Mode',"
    "'type':" +
    String(INPUTRADIO) +
    ","
    "'options':["
    "{'v':'0','l':'Manual'},"
    "{'v':'1','l':'Auto'}],"
    "'default':'0'"
    "},"
    "{"
    "'name':'auto_interval',"
    "'label':'Auto Cycle Interval (s)',"
    "'type':" +
    String(INPUTNUMBER) +
    ","
    "'min':1,'max':65535,"
    "'default':'10'"
    "},"
    "{"
    "'name':'fade_duration',"
    "'label':'Pattern Change Fade Duration',"
    "'type':" +
    String(INPUTNUMBER) +
    ","
    "'min':1,'max':65535,"
    "'default':'3000'"
    "},"
    "{"
    "'name':'pattern_num',"
    "'label':'Pattern',"
    "'type':" +
    String(INPUTRADIO) +
    ","
    "'options':["
    "{'v':'0','l':'Wave 0'},"
    "{'v':'1','l':'Wave 1'},"
    "{'v':'2','l':'Wave 2'},"
    "{'v':'3','l':'Wave 3'},"
    "{'v':'4','l':'Wave 4'},"
    "{'v':'5','l':'Fireworks'},"
    "{'v':'6','l':'Meteors'},"
    "{'v':'7','l':'Star Ladder'},"
    "{'v':'9','l':'Solid (Sec Color)'}],"
    "'default':'0'"
    "},"
    "{"
    "'name':'sec_color',"
    "'label':'Secondary Color',"
    "'type':" +
    String(INPUTCOLOR) +
    ","
    "'default':'#00ff00'"
    "},"
    "{"
    "'name':'move_speed',"
    "'label':'Move Speed',"
    "'type':" +
    String(INPUTNUMBER) +
    ","
    "'min':1,'max':65535,"
    "'default':'255'"
    "},"
    "{"
    "'name':'base_color',"
    "'label':'Base Color',"
    "'type':" +
    String(INPUTCOLOR) +
    ","
    "'default':'#ffffff'"
    "},"
    "{"
    "'name':'wave_color',"
    "'label':'Wave Color',"
    "'type':" +
    String(INPUTCOLOR) +
    ","
    "'default':'#ffffff'"
    "},"
    "{"
    "'name':'wave0_speed',"
    "'label':'Wave 0 Speed',"
    "'type':" +
    String(INPUTNUMBER) +
    ","
    "'min':1,'max':65535,"
    "'default':'255'"
    "},"
    "{"
    "'name':'wave0_freq',"
    "'label':'Wave 0 Frequency Scale',"
    "'type':" +
    String(INPUTFLOAT) +
    ","
    "'min':1,'max':255,"
    "'default':'1'"
    "},"
    "{"
    "'name':'wave0_duty',"
    "'label':'Wave 0 Duty Scale',"
    "'type':" +
    String(INPUTNUMBER) +
    ","
    "'min':-254,'max':127,"
    "'default':'0'"
    "},"
    "{"
    "'name':'wave1_speed',"
    "'label':'Wave 1 Speed',"
    "'type':" +
    String(INPUTNUMBER) +
    ","
    "'min':1,'max':65535,"
    "'default':'255'"
    "},"
    "{"
    "'name':'wave1_freq',"
    "'label':'Wave 1 Frequency Scale',"
    "'type':" +
    String(INPUTFLOAT) +
    ","
    "'min':1,'max':255,"
    "'default':'1'"
    "},"
    "{"
    "'name':'wave1_duty',"
    "'label':'Wave 1 Duty Scale',"
    "'type':" +
    String(INPUTNUMBER) +
    ","
    "'min':-254,'max':127,"
    "'default':'0'"
    "},"
    "{"
    "'name':'wave2_speed',"
    "'label':'Wave 2 Speed',"
    "'type':" +
    String(INPUTNUMBER) +
    ","
    "'min':1,'max':65535,"
    "'default':'255'"
    "},"
    "{"
    "'name':'wave2_freq',"
    "'label':'Wave 2 Frequency Scale',"
    "'type':" +
    String(INPUTFLOAT) +
    ","
    "'min':1,'max':255,"
    "'default':'1'"
    "},"
    "{"
    "'name':'wave2_duty',"
    "'label':'Wave 2 Duty Scale',"
    "'type':" +
    String(INPUTNUMBER) +
    ","
    "'min':-254,'max':127,"
    "'default':'0'"
    "},"
    "{"
    "'name':'wave3_speed',"
    "'label':'Wave 3 Speed',"
    "'type':" +
    String(INPUTNUMBER) +
    ","
    "'min':1,'max':65535,"
    "'default':'255'"
    "},"
    "{"
    "'name':'wave3_freq',"
    "'label':'Wave 3 Frequency Scale',"
    "'type':" +
    String(INPUTFLOAT) +
    ","
    "'min':1,'max':255,"
    "'default':'1'"
    "},"
    "{"
    "'name':'wave3_duty',"
    "'label':'Wave 3 Duty Scale',"
    "'type':" +
    String(INPUTNUMBER) +
    ","
    "'min':-254,'max':127,"
    "'default':'0'"
    "},"
    "{"
    "'name':'wave4_speed',"
    "'label':'Wave 4 Speed',"
    "'type':" +
    String(INPUTNUMBER) +
    ","
    "'min':1,'max':65535,"
    "'default':'255'"
    "},"
    "{"
    "'name':'wave4_freq',"
    "'label':'Wave 4 Frequency Scale',"
    "'type':" +
    String(INPUTFLOAT) +
    ","
    "'min':1,'max':255,"
    "'default':'1'"
    "},"
    "{"
    "'name':'wave4_duty',"
    "'label':'Wave 4 Duty Scale',"
    "'type':" +
    String(INPUTNUMBER) +
    ","
    "'min':-254,'max':127,"
    "'default':'0'"
    "},"
    "{"
    "'name':'meteors_color',"
    "'label':'Meteors Color',"
    "'type':" +
    String(INPUTCOLOR) +
    ","
    "'default':'#ffffff'"
    "},"
    "{"
    "'name':'meteors_speed',"
    "'label':'Meteors Speed',"
    "'type':" +
    String(INPUTNUMBER) +
    ","
    "'min':1,'max':65535,"
    "'default':'255'"
    "},"
    "{"
    "'name':'meteors_period',"
    "'label':'Meteors Period',"
    "'type':" +
    String(INPUTNUMBER) +
    ","
    "'min':1,'max':255,"
    "'default':'10'"
    "},"
    "{"
    "'name':'meteors_fade',"
    "'label':'Meteors Fade',"
    "'type':" +
    String(INPUTNUMBER) +
    ","
    "'min':1,'max':255,"
    "'default':'10'"
    "},"
    "{"
    "'name':'meteors_length',"
    "'label':'Meteors Length',"
    "'type':" +
    String(INPUTNUMBER) +
    ","
    "'min':1,'max':255,"
    "'default':'1'"
    "},"
    "{"
    "'name':'sl_color',"
    "'label':'Star Ladder Color',"
    "'type':" +
    String(INPUTCOLOR) +
    ","
    "'default':'#ffffff'"
    "},"
    "{"
    "'name':'sl_led_step',"
    "'label':'Star Ladder LED Step',"
    "'type':" +
    String(INPUTNUMBER) +
    ","
    "'min':1,'max':255,"
    "'default':'10'"
    "},"
    "{"
    "'name':'sl_step_time',"
    "'label':'Star Ladder Step Time (ms)',"
    "'type':" +
    String(INPUTNUMBER) +
    ","
    "'min':1,'max':65535,"
    "'default':'500'"
    "},"
    "{"
    "'name':'sl_fade_time',"
    "'label':'Star Ladder Fade Time (ms)',"
    "'type':" +
    String(INPUTNUMBER) +
    ","
    "'min':1,'max':65535,"
    "'default':'100'"
    "},"
    "{"
    "'name':'sl_cycle_time',"
    "'label':'Star Ladder Cycle Time (ms)',"
    "'type':" +
    String(INPUTNUMBER) +
    ","
    "'min':1,'max':65535,"
    "'default':'1000'"
    "},"
    "{"
    "'name':'sl_star_len',"
    "'label':'Star Ladder Star Length',"
    "'type':" +
    String(INPUTNUMBER) +
    ","
    "'min':1,'max':255,"
    "'default':'1'"
    "},"
    "{"
    "'name':'fw_color_0',"
    "'label':'Fireworks Color 0',"
    "'type':" +
    String(INPUTCOLOR) +
    ","
    "'default':'#cbb1d6'"
    "},"
    "{"
    "'name':'fw_color_1',"
    "'label':'Fireworks Color 1',"
    "'type':" +
    String(INPUTCOLOR) +
    ","
    "'default':'#fecf03'"
    "},"
    "{"
    "'name':'fw_color_2',"
    "'label':'Fireworks Color 2',"
    "'type':" +
    String(INPUTCOLOR) +
    ","
    "'default':'#faa564'"
    "},"
    "{"
    "'name':'fw_color_3',"
    "'label':'Fireworks Color 3',"
    "'type':" +
    String(INPUTCOLOR) +
    ","
    "'default':'#fef7ef'"
    "},"
    "{"
    "'name':'fw_color_4',"
    "'label':'Fireworks Color 4',"
    "'type':" +
    String(INPUTCOLOR) +
    ","
    "'default':'#fee9f0'"
    "},"
    "{"
    "'name':'fw_increment',"
    "'label':'Fireworks Width Scaling',"
    "'type':" +
    String(INPUTNUMBER) +
    ","
    "'min':1,'max':255,"
    "'default':'1'"
    "},"
    "{"
    "'name':'eff_caps_color',"
    "'label':'Caps: Color',"
    "'type':" +
    String(INPUTCOLOR) +
    ","
    "'default':'#ffffff'"
    "},"
    "{"
    "'name':'eff_caps_dur',"
    "'label':'Caps: Duration (ms)',"
    "'type':" +
    String(INPUTNUMBER) +
    ","
    "'min':1,'max':65535,"
    "'default':'1000'"
    "},"
    "{"
    "'name':'eff_caps_slew',"
    "'label':'Caps: Slew Rate',"
    "'type':" +
    String(INPUTNUMBER) +
    ","
    "'min':1,'max':255,"
    "'default':'1'"
    "},"
    "{"
    "'name':'led_pos_display',"
    "'label':'LED Position Display',"
    "'type':" +
    String(INPUTCHECKBOX) +
    ","
    "'default':'0'"
    "},"
    "{"
    "'name':'led_mid',"
    "'label':'LED: Index of mid split',"
    "'type':" +
    String(INPUTNUMBER) +
    ","
    "'min':1,'max':65535,"
    "'default':'360'"
    "},"
    "{"
    "'name':'led_arc_corner',"
    "'label':'LED: Index of arc begin',"
    "'type':" +
    String(INPUTNUMBER) +
    ","
    "'min':1,'max':65535,"
    "'default':'720'"
    "},"
    "{"
    "'name':'led_arc_mid',"
    "'label':'LED: Index of arc split',"
    "'type':" +
    String(INPUTNUMBER) +
    ","
    "'min':1,'max':65535,"
    "'default':'850'"
    "},"
    "{"
    "'name':'lox_min_0',"
    "'label':'Sensor Slot 0: Min Distance (mm)',"
    "'type':" +
    String(INPUTNUMBER) +
    ","
    "'min':1,'max':65535,"
    "'default':'150'"
    "},"
    "{"
    "'name':'lox_max_0',"
    "'label':'Sensor Slot 0: Max Distance (mm)',"
    "'type':" +
    String(INPUTNUMBER) +
    ","
    "'min':1,'max':65535,"
    "'default':'1000'"
    "},"
    "{"
    "'name':'lox_min_1',"
    "'label':'Sensor Slot 1: Min Distance (mm)',"
    "'type':" +
    String(INPUTNUMBER) +
    ","
    "'min':1,'max':65535,"
    "'default':'150'"
    "},"
    "{"
    "'name':'lox_max_1',"
    "'label':'Sensor Slot 1: Max Distance (mm)',"
    "'type':" +
    String(INPUTNUMBER) +
    ","
    "'min':1,'max':65535,"
    "'default':'1000'"
    "},"
    "{"
    "'name':'lox_min_2',"
    "'label':'Sensor Slot 2: Min Distance (mm)',"
    "'type':" +
    String(INPUTNUMBER) +
    ","
    "'min':1,'max':65535,"
    "'default':'150'"
    "},"
    "{"
    "'name':'lox_max_2',"
    "'label':'Sensor Slot 2: Max Distance (mm)',"
    "'type':" +
    String(INPUTNUMBER) +
    ","
    "'min':1,'max':65535,"
    "'default':'1000'"
    "},"
    "{"
    "'name':'lox_min_3',"
    "'label':'Sensor Slot 3: Min Distance (mm)',"
    "'type':" +
    String(INPUTNUMBER) +
    ","
    "'min':1,'max':65535,"
    "'default':'150'"
    "},"
    "{"
    "'name':'lox_max_3',"
    "'label':'Sensor Slot 3: Max Distance (mm)',"
    "'type':" +
    String(INPUTNUMBER) +
    ","
    "'min':1,'max':65535,"
    "'default':'1000'"
    "}"
    "]";

#endif
