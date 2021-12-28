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
    "'name':'fade_blend',"
    "'label':'Pattern Change Fade Blend',"
    "'type':" +
    String(INPUTNUMBER) +
    ","
    "'min':1,'max':255,"
    "'default':'8'"
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
    "{'v':'5','l':'Fireworks'}],"
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
    "{'v':'0','l':'Star Ladder'},"
    "{'v':'1','l':'Random Stars'},"
    "{'v':'2','l':'Lines'},"
    "{'v':'3','l':'Solid Lines'}"
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
    "'label':'Star Ladder: Interval (ms)',"
    "'type':" +
    String(INPUTNUMBER) +
    ","
    "'min':1,'max':65535,"
    "'default':'100'"
    "},"
    "{"
    "'name':'eff_rs_color',"
    "'label':'Random Stars: Color',"
    "'type':" +
    String(INPUTCOLOR) +
    ","
    "'default':'#ffffff'"
    "},"
    "{"
    "'name':'eff_rs_duration',"
    "'label':'Random Stars: Duration (ms)',"
    "'type':" +
    String(INPUTNUMBER) +
    ","
    "'min':1,'max':65535,"
    "'default':'3000'"
    "},"
    "{"
    "'name':'eff_rs_chance',"
    "'label':'Random Stars: 1 in n chance',"
    "'type':" +
    String(INPUTNUMBER) +
    ","
    "'min':1,'max':255,"
    "'default':'30'"
    "},"
    "{"
    "'name':'eff_rs_length',"
    "'label':'Random Stars: Star Length',"
    "'type':" +
    String(INPUTNUMBER) +
    ","
    "'min':1,'max':255,"
    "'default':'1'"
    "},"
    "{"
    "'name':'eff_rs_fade',"
    "'label':'Random Stars: Fade',"
    "'type':" +
    String(INPUTNUMBER) +
    ","
    "'min':1,'max':255,"
    "'default':'30'"
    "},"
    "{"
    "'name':'eff_line_color',"
    "'label':'Line: Color',"
    "'type':" +
    String(INPUTCOLOR) +
    ","
    "'default':'#ffffff'"
    "},"
    "{"
    "'name':'eff_line_speed',"
    "'label':'Line: Speed',"
    "'type':" +
    String(INPUTNUMBER) +
    ","
    "'min':1,'max':65535,"
    "'default':'255'"
    "},"
    "{"
    "'name':'eff_line_duration',"
    "'label':'Line: Duration',"
    "'type':" +
    String(INPUTNUMBER) +
    ","
    "'min':1,'max':65535,"
    "'default':'3000'"
    "},"
    "{"
    "'name':'eff_line_period',"
    "'label':'Line: Period',"
    "'type':" +
    String(INPUTNUMBER) +
    ","
    "'min':1,'max':255,"
    "'default':'10'"
    "},"
    "{"
    "'name':'eff_line_duty',"
    "'label':'Line: Duty',"
    "'type':" +
    String(INPUTNUMBER) +
    ","
    "'min':1,'max':255,"
    "'default':'5'"
    "},"
    "{"
    "'name':'eff_line_fade_dur',"
    "'label':'Line: Fade Duration',"
    "'type':" +
    String(INPUTNUMBER) +
    ","
    "'min':1,'max':65535,"
    "'default':'1000'"
    "},"
    "{"
    "'name':'eff_sline_color',"
    "'label':'Solid Line: Color',"
    "'type':" +
    String(INPUTCOLOR) +
    ","
    "'default':'#ffffff'"
    "},"
    "{"
    "'name':'eff_sline_duration',"
    "'label':'Solid Line: Duration',"
    "'type':" +
    String(INPUTNUMBER) +
    ","
    "'min':1,'max':65535,"
    "'default':'255'"
    "},"
    "{"
    "'name':'eff_sline_speed',"
    "'label':'Solid Line: Speed',"
    "'type':" +
    String(INPUTNUMBER) +
    ","
    "'min':1,'max':65535,"
    "'default':'255'"
    "}"
    "]";

#endif
