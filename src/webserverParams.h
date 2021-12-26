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
    "'name':'pattern_num',"
    "'label':'Pattern',"
    "'type':" +
    String(INPUTRADIO) +
    ","
    "'options':["
    "{'v':'0','l':'Wave'},"
    "{'v':'1','l':'Fireworks'}],"
    "'default':'0'"
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
    "'min':1,'max':255,"
    "'default':'1'"
    "},"
    "{"
    "'name':'wave_freq',"
    "'label':'Wave Frequency Scale',"
    "'type':" +
    String(INPUTFLOAT) +
    ","
    "'min':1,'max':255,"
    "'default':'1'"
    "},"
    "{"
    "'name':'wave_duty',"
    "'label':'Wave Duty Scale',"
    "'type':" +
    String(INPUTNUMBER) +
    ","
    "'min':-254,'max':127,"
    "'default':'0'"
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
    "'name':'effect_num',"
    "'label':'Effect',"
    "'type':" +
    String(INPUTRADIO) +
    ","
    "'options':["
    "{'v':'0','l':'Star Ladder'}"
    "],'default':'0'"
    "},"
    "{"
    "'name':'eff_sl_color',"
    "'label':'Star Ladder: Color',"
    "'type':" +
    String(INPUTCOLOR) +
    ","
    "'default':'#ffffff'"
    "},"
    "{"
    "'name':'eff_sl_step',"
    "'label':'Star Ladder: Step Size',"
    "'type':" +
    String(INPUTNUMBER) +
    ","
    "'min':1,'max':255,"
    "'default':'5'"
    "},"
    "{"
    "'name':'eff_sl_length',"
    "'label':'Star Ladder: Length',"
    "'type':" +
    String(INPUTNUMBER) +
    ","
    "'min':1,'max':255,"
    "'default':'2'"
    "},"
    "{"
    "'name':'eff_sl_interval',"
    "'label':'Star Ladder: Interval',"
    "'type':" +
    String(INPUTNUMBER) +
    ","
    "'min':1,'max':65535,"
    "'default':'100'"
    "},"
    "{"
    "'name':'param1',"
    "'label':'Parameter1',"
    "'type':" +
    String(INPUTNUMBER) +
    ","
    "'min':0,'max':65535,"
    "'default':'60'"
    "},"
    "{"
    "'name':'param2',"
    "'label':'Parameter2',"
    "'type':" +
    String(INPUTNUMBER) +
    ","
    "'min':0,'max':65535,"
    "'default':'60'"
    "},"
    "{"
    "'name':'param3',"
    "'label':'Parameter3',"
    "'type':" +
    String(INPUTNUMBER) +
    ","
    "'min':0,'max':255,"
    "'default':'60'"
    "}"
    "]";

#endif
