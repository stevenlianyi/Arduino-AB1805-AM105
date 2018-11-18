#include "AB1805.h"

AB1805::AB1805() {}

// Use these methods with I2c library instead of Wire
/*
void AB1805::begin()
{
	I2c.begin();
}

uint8_t AB1805::read_rtc_register(const uint8_t rtc_register) {
    
	I2c.write(AM1805, rtc_register);
    I2c.read(AM1805, (uint8_t) 1);
    
	return I2c.receive();
}

uint8_t AB1805::write_rtc_register(const uint8_t rtc_register, const uint8_t data) {
    
	I2c.write(AM1805, rtc_register, data);  
    
	return true;
}
*/

void AB1805::begin()
{
	Wire.begin();
}

//added by steven
void AB1805::begin(short sda_pin, short scl_pin)
{
	Wire.begin(sda_pin,scl_pin);
}


uint8_t AB1805::read_rtc_register(const uint8_t rtc_register) 
{
	uint8_t data;									// `data` will store the register data	 
	Wire.beginTransmission(AM1805);					// Initialize the Tx buffer
	Wire.write(rtc_register);	              		// Put slave register address in Tx buffer
	Wire.endTransmission(false);             		// Send the Tx buffer, but send a restart to keep connection alive
	Wire.requestFrom(AM1805, (uint8_t) 1);  	// Read one byte from slave register address 
	data = Wire.read();                      		// Fill Rx buffer with result
	return data;                             		// Return data read from slave register
}


uint8_t AB1805::write_rtc_register(const uint8_t rtc_register, const uint8_t data)
{
	Wire.beginTransmission(AM1805);  	// Initialize the Tx buffer
	Wire.write(rtc_register);    		// Put slave register address in Tx buffer
	Wire.write(data);					// Put data in Tx buffer
	Wire.endTransmission();				// Send the Tx buffer
	return true;
}

      
uint8_t AB1805::get_rtc_data(const uint8_t rtc_register, const uint8_t register_mask) 
{
  return (read_rtc_register(rtc_register) & register_mask);
}


uint8_t AB1805::get_hundredths(void) 
{
  return get_rtc_data(HUNDRETH_REGISTER, 0xFF);
}


uint8_t AB1805::get_second(void) 
{
  return get_rtc_data(SECOND_REGISTER, SECOND_MASK);
}


uint8_t AB1805::get_minute(void) 
{
  return get_rtc_data(MINUTE_REGISTER, MINUTE_MASK);
}

uint8_t AB1805::get_hour(void) 
{
  return get_rtc_data(HOUR_REGISTER, HOUR_24_MASK);
}


uint8_t AB1805::get_weekday(void) 
{
  return get_rtc_data(WEEKDAY_REGISTER, DAY_MASK);
}


uint8_t AB1805::get_date(void) 
{
  return get_rtc_data(DATE_REGISTER, DATE_MASK);
}


uint8_t AB1805::get_month(void) 
{
  return get_rtc_data(MONTH_REGISTER, MONTH_MASK);
}


uint8_t AB1805::get_year(void) 
{
  return get_rtc_data(YEAR_REGISTER, YEAR_MASK);
}


void AB1805::get_datetime(void)
{
  //rtc_check_valid();
  //_hundredths = get_hundredths();
  _second = get_second();
  _minute = get_minute();
  _hour = get_hour();
  _weekday = get_weekday();
  _date = get_date();
  _month = get_month();
  _year = get_year();
}


uint8_t AB1805::get_second_alarm(void)
{
  _alarm_second = get_rtc_data(SECOND_ALARM_REGISTER, SECOND_ALARM_MASK);
  return _alarm_second;
}


uint8_t AB1805::get_minute_alarm(void) 
{
  _alarm_minute = get_rtc_data(MINUTE_ALARM_REGISTER, MINUTE_ALARM_MASK);
  return _alarm_minute;
}


uint8_t AB1805::get_hour_alarm(void) 
{
  _alarm_hour = get_rtc_data(HOUR_ALARM_REGISTER, HOUR_24_ALARM_MASK);
  return _alarm_hour;
}


uint8_t AB1805::get_weekday_alarm(void) 
{
  _alarm_weekday = get_rtc_data(WEEKDAY_ALARM_REGISTER, WEEKDAY_ALARM_MASK);
  return _alarm_weekday;
}


uint8_t AB1805::get_date_alarm(void) 
{
  _alarm_date = get_rtc_data(DATE_ALARM_REGISTER, DATE_ALARM_MASK);
  return _alarm_date;
}


uint8_t AB1805::get_month_alarm(void) 
{
  _alarm_month = get_rtc_data(MONTH_ALARM_REGISTER, MONTH_ALARM_MASK);
  return _alarm_month;
}


void AB1805::set_second(const uint8_t value) 
{
  _second = value % MAX_SECOND;
  write_rtc_register(SECOND_REGISTER, _second);
}


void AB1805::set_minute(const uint8_t value) 
{
  _minute = value % MAX_MINUTE;
  write_rtc_register(MINUTE_REGISTER, _minute);
}


void AB1805::set_hour(const uint8_t value) 
{
  _hour = value % MAX_HOURS;
  write_rtc_register(HOUR_REGISTER, _hour);
}


void AB1805::set_weekday(const uint8_t value) 
{
  _weekday = value % MAX_DAY;
  write_rtc_register(WEEKDAY_REGISTER, _weekday);
}


void AB1805::set_date(const uint8_t value) 
{
  _date = value % MAX_DATE;
  write_rtc_register(DATE_REGISTER, _date);
}


void AB1805::set_month(const uint8_t value) 
{
  _month = value % MAX_MONTH;
  write_rtc_register(MONTH_REGISTER, _month);
}


void AB1805::set_year(const uint8_t value) 
{
  _year = value % MAX_YEAR;
  write_rtc_register(YEAR_REGISTER, value);
}


void AB1805::set_datetime(uint8_t year, uint8_t month, uint8_t date,  uint8_t weekday, uint8_t hour, uint8_t minutes, uint8_t seconds) 
{
    set_year(year);
    set_month(month);
    set_date(date);
    set_weekday(weekday);
    set_hour(hour);
    set_minute(minutes);
    set_second(seconds);

}


void AB1805::set_datetime_alarm(uint8_t month, uint8_t date, uint8_t hour, uint8_t minutes, uint8_t seconds) 
{
    set_month_alarm(month);
    set_date_alarm(date);
    set_hour_alarm(hour);
    set_minute_alarm(minutes);
    set_second_alarm(seconds);
    set_RPT_time_control(TIMER_CONTROL_RPT_ONCE_PER_YEAR);
}


void AB1805::set_datetime_alarm(uint8_t date, uint8_t hour, uint8_t minutes, uint8_t seconds) 
{
    set_date_alarm(date);
    set_hour_alarm(hour);
    set_minute_alarm(minutes);
    set_second_alarm(seconds);
    set_RPT_time_control(TIMER_CONTROL_RPT_ONCE_PER_MONTH);
}


void AB1805::set_week_alarm(uint8_t weekday, uint8_t hour, uint8_t minutes, uint8_t seconds) 
{
    set_weekday_alarm(weekday);
    set_hour_alarm(hour);
    set_minute_alarm(minutes);
    set_second_alarm(seconds);
    set_RPT_time_control(TIMER_CONTROL_RPT_ONCE_PER_WEEK);
}


void AB1805::set_datetime_alarm(uint8_t hour, uint8_t minutes, uint8_t seconds) 
{
    set_hour_alarm(hour);
    set_minute_alarm(minutes);
    set_second_alarm(seconds);
    set_RPT_time_control(TIMER_CONTROL_RPT_ONCE_PER_DAY);
}


void AB1805::set_datetime_alarm(uint8_t minutes, uint8_t seconds) 
{
    set_minute_alarm(minutes);
    set_second_alarm(seconds);
    set_RPT_time_control(TIMER_CONTROL_RPT_ONCE_PER_HOUR);
}


void AB1805::set_datetime_alarm(uint8_t seconds) 
{
    set_second_alarm(seconds);
    set_RPT_time_control(TIMER_CONTROL_RPT_ONCE_PER_MINUTE);
}

void AB1805::set_once_second_alarm(uint8_t hundredths) 
{
    //set_second_alarm(seconds);
    set_RPT_time_control(TIMER_CONTROL_RPT_ONCE_PER_SECOND);
}


bool AB1805::set_second_alarm(uint8_t value) {
    _alarm_second = value % MAX_SECOND;
    write_rtc_register(SECOND_ALARM_REGISTER,_alarm_second);
    _alarm_second = get_second_alarm();
    return (value == _alarm_second);
}


bool AB1805::set_minute_alarm(uint8_t value) {
    _alarm_minute = value % MAX_MINUTE;
    write_rtc_register(MINUTE_ALARM_REGISTER,_alarm_minute);
    _alarm_minute = get_minute_alarm();
    return (value == _alarm_minute);
}


bool AB1805::set_hour_alarm(uint8_t value) {
    _alarm_hour = value % MAX_HOURS;
    write_rtc_register(HOUR_ALARM_REGISTER,_alarm_hour);
    _alarm_hour = get_hour_alarm();
    return (value == _alarm_hour);
}


bool AB1805::set_weekday_alarm(uint8_t value) {
    _alarm_weekday = value % MAX_DAY;
    write_rtc_register(WEEKDAY_ALARM_REGISTER,_alarm_weekday);
    _alarm_weekday = get_weekday_alarm();
    return (value == _alarm_weekday);
}


bool AB1805::set_date_alarm(uint8_t value) {
    _alarm_date = value % MAX_DATE;
    write_rtc_register(DATE_ALARM_REGISTER,_alarm_date);
    _alarm_date = get_date_alarm();
    return (value == _alarm_date);
}


bool AB1805::set_month_alarm(uint8_t value) {
    _alarm_month = value % MAX_MONTH;
    write_rtc_register(MONTH_ALARM_REGISTER,_alarm_month);
    _alarm_month = get_month_alarm();
    return (value == _alarm_month);
}


String AB1805::get_date_string(void){
  
  String date_string;
  
  _date = get_date();
  date_string += ((_date & 0xF0) >> 4);
  date_string += (_date & 0x0F);
    
  date_string += '/';
  
  _month = get_month();
  date_string += ((_month & 0xF0) >> 4);
  date_string += (_month & 0x0F);
  
  date_string += '/';
  
  _year = get_year();
  date_string += ((_year & 0xF0) >> 4);
  date_string += (_year & 0x0F);
  
  date_string += ' ';
  
  _hour = get_hour();
  date_string += ((_hour & 0xF0) >> 4);
  date_string += (_hour & 0x0F);
  
  date_string += ':';
  
  _minute = get_minute();
  date_string += ((_minute & 0xF0) >> 4);
  date_string += (_minute & 0x0F);
  
  date_string += ':';
  
  _second = get_second();
  date_string += ((_second & 0xF0) >> 4);
  date_string += (_second & 0x0F);
    
  
  return date_string;
  

}


char *AB1805::get_date_time(void)
{
  char *currPos;
  currPos = data_time_string; 
  *currPos = 0;
  get_datetime();
  
  *currPos  = ((_date & 0xF0) >> 4) + 0x30;
  currPos++;
  *currPos  = (_date & 0x0F) + 0x30;
  currPos++;
    
  *currPos = '/';
  currPos++;
  
  *currPos = ((_month & 0xF0) >> 4) + 0x30;
  currPos++;
  *currPos = (_month & 0x0F) + 0x30;
  currPos++;
  
  *currPos = '/';
  currPos++;
  
  *currPos = ((_year & 0xF0) >> 4) + 0x30;
  currPos++;
  *currPos = (_year & 0x0F) + 0x30;
  currPos++;
  
  *currPos = ' ';
  currPos++;
  
  *currPos = ((_hour & 0xF0) >> 4) + 0x30;
  currPos++;
  *currPos = (_hour & 0x0F) + 0x30 ;
  currPos++;
  
  *currPos = ':';
  currPos++;
  
  *currPos = ((_minute & 0xF0) >> 4) + 0x30;
  currPos++;
  *currPos = (_minute & 0x0F) + 0x30;
  currPos++;
  
  *currPos = ':';
  currPos++;
  
  *currPos = ((_second & 0xF0) >> 4) + 0x30;
  currPos++;
  *currPos = (_second & 0x0F) + 0x30;
  currPos++;
  
  *currPos = ' ';
  currPos++;

  *currPos = ((_hundredth & 0xF0) >> 4) + 0x30;
  currPos++;
  *currPos = (_hundredth & 0x0F) + 0x30;
  currPos++;
  
  return data_time_string;
  
}


uint8_t AB1805::dec_hex(uint8_t tens, uint8_t digits)
{
  uint8_t ret;
  ret = (tens<<4) + digits;
  return ret;
}


uint8_t AB1805::hex_dec(uint8_t hex)
{
  uint8_t ret;
  ret = ((hex>>4) & 0xF * 10) + (hex &0xF);
  return ret;
}


//status
uint8_t AB1805::get_status(void)
{
  _status = get_rtc_data(STATU_REGISTER, STATUS_READ_ALL_MASK);
  return _status;
}


uint8_t AB1805::get_CB_status(void)
{
  get_status();
  return (_status & STATUS_CB_MASK >7);
};


uint8_t AB1805::get_BAT_status(void)
{
  get_status();
  return (_status & STATUS_BAT_MASK >6);
  
};


uint8_t AB1805::get_WDT_status(void)
{
  get_status();
  return (_status & STATUS_WDT_MASK >5);
  
};


uint8_t AB1805::get_BL_status(void)
{
  get_status();
  return (_status & STATUS_BL_MASK >4);
  
};


uint8_t AB1805::get_TIM_status(void)
{
  get_status();
  return (_status & STATUS_TIM_MASK >3);
  
};


uint8_t AB1805::get_ALM_status(void)
{
  get_status();
  return (_status & STATUS_ALM_MASK >2);
  
};
   
   
uint8_t AB1805::get_EX2_status(void)
{
  get_status();
  return (_status & STATUS_EX2_MASK >1);
  
};


uint8_t AB1805::get_EX1_status(void)
{
  get_status();
  return (_status & STATUS_EX1_MASK);
  
};


uint8_t AB1805::clean_CB_status(void)
{
  uint8_t c1;
  c1 = read_rtc_register(STATU_REGISTER);
  c1 &= ~STATUS_CB_MASK;
  write_rtc_register(STATU_REGISTER,c1);  
}


uint8_t AB1805::clean_BAT_status(void)
{
  uint8_t c1;
  c1 = read_rtc_register(STATU_REGISTER);
  c1 &= ~STATUS_BAT_MASK;
  write_rtc_register(STATU_REGISTER,c1);  
}



uint8_t AB1805::clean_WDT_status(void)
{
  uint8_t c1;
  c1 = read_rtc_register(STATU_REGISTER);
  c1 &= ~STATUS_WDT_MASK;
  write_rtc_register(STATU_REGISTER,c1);  
}


uint8_t AB1805::clean_BL_status(void)
{
  uint8_t c1;
  c1 = read_rtc_register(STATU_REGISTER);
  c1 &= ~STATUS_BL_MASK;
  write_rtc_register(STATU_REGISTER,c1);  
}


uint8_t AB1805::clean_TIM_status(void)
{
  uint8_t c1;
  c1 = read_rtc_register(STATU_REGISTER);
  c1 &= ~STATUS_TIM_MASK;
  write_rtc_register(STATU_REGISTER,c1);  
}


uint8_t AB1805::clean_ALM_status(void)
{
  uint8_t c1;
  c1 = read_rtc_register(STATU_REGISTER);
  c1 &= ~STATUS_ALM_MASK;
  write_rtc_register(STATU_REGISTER,c1);  
}


uint8_t AB1805::clean_EX2_status(void)
{
  uint8_t c1;
  c1 = read_rtc_register(STATU_REGISTER);
  c1 &= ~STATUS_EX2_MASK;
  write_rtc_register(STATU_REGISTER,c1);  
}


uint8_t AB1805::clean_EX1_status(void)
{
  uint8_t c1;
  c1 = read_rtc_register(STATU_REGISTER);
  c1 &= ~STATUS_EX1_MASK;
  write_rtc_register(STATU_REGISTER,c1);  
}


//set func
uint8_t AB1805::set_control1(uint8_t value)
{
  write_rtc_register(CONTROL1_REGISTER,value);
  _control1 = read_rtc_register(CONTROL1_REGISTER);
  return _control1;
}


uint8_t AB1805::set_1224(uint8_t value)
{
  uint8_t c1;
  c1 = read_rtc_register(CONTROL1_REGISTER);
  c1 &= ~CONTROL1_1224_MASK;
  if (value)
  {
    c1 |= CONTROL1_1224_MASK;
  }
  write_rtc_register(CONTROL1_REGISTER,c1);  
}


uint8_t AB1805::set_RSP(uint8_t value)
{
  uint8_t c1;
  c1 = read_rtc_register(CONTROL1_REGISTER);
  c1 &= ~CONTROL1_RSP_MASK;
  if (value)
  {
    c1 |= CONTROL1_RSP_MASK;
  }
  write_rtc_register(CONTROL1_REGISTER,c1);    
}


uint8_t AB1805::set_ARST(uint8_t value)
{
  uint8_t c1;
  c1 = read_rtc_register(CONTROL1_REGISTER);
  c1 &= ~CONTROL1_ARST_MASK;
  if (value)
  {
    c1 |= CONTROL1_ARST_MASK;
  }
  write_rtc_register(CONTROL1_REGISTER,c1);    
  
}


uint8_t AB1805::set_PWR2(uint8_t value)
{
  uint8_t c1;
  c1 = read_rtc_register(CONTROL1_REGISTER);
  c1 &= ~CONTROL1_PWR2_MASK;
  if (value)
  {
    c1 |= CONTROL1_PWR2_MASK;
  }
  write_rtc_register(CONTROL1_REGISTER,c1);    
  
}


uint8_t AB1805::set_WRTC(uint8_t value)
{
  uint8_t c1;
  c1 = read_rtc_register(CONTROL1_REGISTER);
  c1 &= ~CONTROL1_WRTC_MASK;
  if (value)
  {
    c1 |= CONTROL1_WRTC_MASK;
  }
  write_rtc_register(CONTROL1_REGISTER,c1);    
  
}
    

//interrupt
uint8_t AB1805::get_interrupt(void)
{
  _interrupt = read_rtc_register(INT_MASK_REGISTER);
  return _interrupt;
}


uint8_t AB1805::get_BLIE_interrupt(void)
{
  get_status();
  return (_interrupt & STATUS_BL_MASK >4);
  
};


uint8_t AB1805::get_TIE_interrupt(void)
{
  get_status();
  return (_interrupt & STATUS_TIM_MASK >3);
  
};


uint8_t AB1805::get_AIE_interrupt(void)
{
  get_status();
  return (_interrupt & STATUS_ALM_MASK >2);
  
};
    
    
uint8_t AB1805::get_EX2E_interrupt(void)
{
  get_status();
  return (_interrupt & STATUS_EX2_MASK >1);
  
};


uint8_t AB1805::get_EX1E_interrupt(void)
{
  get_status();
  return (_interrupt & STATUS_EX1_MASK);
  
};


uint8_t AB1805::set_interrupt(uint8_t value)
{
  write_rtc_register(INT_MASK_REGISTER,value);
  _interrupt = read_rtc_register(INT_MASK_REGISTER);
  return _interrupt;
}


uint8_t AB1805::set_BLIE_interrupt(uint8_t value)
{
  uint8_t c1;
  c1 = read_rtc_register(INT_MASK_REGISTER);
  c1 &= ~INTERRUPT_BLIE_MASK;
  if (value)
  {
    c1 |= INTERRUPT_BLIE_MASK;
  }
  write_rtc_register(INT_MASK_REGISTER,c1);  
}


uint8_t AB1805::set_TIE_interrupt(uint8_t value)
{
  uint8_t c1;
  c1 = read_rtc_register(INT_MASK_REGISTER);
  c1 &= ~INTERRUPT_TIE_MASK;
  if (value)
  {
    c1 |= INTERRUPT_TIE_MASK;
  }
  write_rtc_register(INT_MASK_REGISTER,c1);    
}


uint8_t AB1805::set_AIE_interrupt(uint8_t value)
{
  uint8_t c1;
  c1 = read_rtc_register(INT_MASK_REGISTER);
  c1 &= ~INTERRUPT_AIE_MASK;
  if (value)
  {
    c1 |= INTERRUPT_AIE_MASK;
  }
  write_rtc_register(INT_MASK_REGISTER,c1);    
  
}


uint8_t AB1805::set_EX1E_interrupt(uint8_t value)
{
  uint8_t c1;
  c1 = read_rtc_register(INT_MASK_REGISTER);
  c1 &= ~INTERRUPT_EX1E_MASK;
  if (value)
  {
    c1 |= INTERRUPT_EX1E_MASK;
  }
  write_rtc_register(INT_MASK_REGISTER,c1);    
  
}


uint8_t AB1805::set_EX2E_interrupt(uint8_t value)
{
  uint8_t c1;
  c1 = read_rtc_register(INT_MASK_REGISTER);
  c1 &= ~INTERRUPT_EX2E_MASK;
  if (value)
  {
    c1 |= INTERRUPT_EX2E_MASK;
  }
  write_rtc_register(INT_MASK_REGISTER,c1);    
  
}


//control2
uint8_t AB1805::get_control2(void)
{
  _control2 = read_rtc_register(CONTROL2_REGISTER);
  return _control2;
}


uint8_t AB1805::get_RS1E_control2(void)
{
  get_control2();
  return (_control2 & CONTROL2_RS1E_MASK >5);  
};


uint8_t AB1805::get_OUT2S_control2(void)
{
  get_control2();
  return (_control2 & CONTROL2_OUT2S_MASK >2);  
};



uint8_t AB1805::get_OUT1S_control2(void)
{
  get_control2();
  return (_control2 & CONTROL2_OUT1S_MASK);  
};


uint8_t AB1805::set_control2(uint8_t value)
{
  write_rtc_register(CONTROL2_REGISTER,value);
  _control2 = read_rtc_register(CONTROL2_REGISTER);
  return _control2;
}


uint8_t AB1805::set_RS1E_control2(uint8_t value)
{
  uint8_t c1;
  c1 = read_rtc_register(CONTROL2_REGISTER);
  c1 &= ~CONTROL2_RS1E_MASK;
  if (value)
  {
    c1 |= CONTROL2_RS1E_MASK;
  }
  write_rtc_register(CONTROL2_REGISTER,c1); 
  return c1;   
}


uint8_t AB1805::set_OUT2S_control2(uint8_t value)
{
  uint8_t c1;
  c1 = read_rtc_register(CONTROL2_REGISTER);
  c1 &= ~CONTROL2_OUT2S_MASK;
  c1 |= (value << 2) & CONTROL2_OUT2S_MASK;
  write_rtc_register(CONTROL2_REGISTER,c1); 
  return c1;   
}


uint8_t AB1805::set_OUT1S_control2(uint8_t value)
{
  uint8_t c1;
  c1 = read_rtc_register(CONTROL2_REGISTER);
  c1 &= ~CONTROL2_OUT1S_MASK;
  c1 |= (value) & CONTROL2_OUT1S_MASK;
  write_rtc_register(CONTROL2_REGISTER,c1); 
  return c1;   
}


//sleep mode
uint8_t AB1805::get_sleep(void)
{
  _sleep_mode = read_rtc_register(SLEEP_CONTROL_REGISTER);
  return _sleep_mode;
}


uint8_t AB1805::get_SLP_sleep(void)
{
  get_sleep();
  return (_sleep_mode & SLEEP_CONTROL_SLP_MASK >7);
  
};


uint8_t AB1805::get_SLRES_sleep(void)
{
  get_sleep();
  return (_sleep_mode & SLEEP_CONTROL_SLRES_MASK >6);
  
};


uint8_t AB1805::get_EX2P_sleep(void)
{
  get_sleep();
  return (_sleep_mode & SLEEP_CONTROL_EX2P_MASK >5);
  
};
    
    
uint8_t AB1805::get_EX1P_sleep(void)
{
  get_sleep();
  return (_sleep_mode & SLEEP_CONTROL_EX1P_MASK >4);
  
};


uint8_t AB1805::get_SLST_sleep(void)
{
  get_sleep();
  return (_sleep_mode & SLEEP_CONTROL_SLST_MASK >3);
  
};


uint8_t AB1805::get_SLTO_sleep(void)
{
  get_sleep();
  return (_sleep_mode & SLEEP_CONTROL_SLTO_MASK);
  
};


uint8_t AB1805::set_sleep(uint8_t value)
{
  write_rtc_register(SLEEP_CONTROL_REGISTER,value);
  _sleep_mode = read_rtc_register(SLEEP_CONTROL_REGISTER);
  return _sleep_mode;
}


uint8_t AB1805::set_SLP_sleep(uint8_t value)
{
  uint8_t c1;
  c1 = read_rtc_register(SLEEP_CONTROL_REGISTER);
  c1 &= ~SLEEP_CONTROL_SLP_MASK;
  if (value)
  {
    c1 |= SLEEP_CONTROL_SLP_MASK;
  }
  write_rtc_register(SLEEP_CONTROL_REGISTER,c1); 
  return c1;   
}


uint8_t AB1805::set_SLRES_sleep(uint8_t value)
{
  uint8_t c1;
  c1 = read_rtc_register(SLEEP_CONTROL_REGISTER);
  c1 &= ~SLEEP_CONTROL_SLRES_MASK;
  if (value)
  {
    c1 |= SLEEP_CONTROL_SLRES_MASK;
  }
  write_rtc_register(SLEEP_CONTROL_REGISTER,c1);    
  return c1;   
}


uint8_t AB1805::set_EX2P_sleep(uint8_t value)
{
  uint8_t c1;
  c1 = read_rtc_register(SLEEP_CONTROL_REGISTER);
  c1 &= ~SLEEP_CONTROL_EX2P_MASK;
  if (value)
  {
    c1 |= SLEEP_CONTROL_EX2P_MASK;
  }
  write_rtc_register(SLEEP_CONTROL_REGISTER,c1);    
  return c1;     
}


uint8_t AB1805::set_EX1P_sleep(uint8_t value)
{
  uint8_t c1;
  c1 = read_rtc_register(SLEEP_CONTROL_REGISTER);
  c1 &= ~SLEEP_CONTROL_EX1P_MASK;
  if (value)
  {
    c1 |= SLEEP_CONTROL_EX1P_MASK;
  }
  write_rtc_register(SLEEP_CONTROL_REGISTER,c1);    
  return c1;     
}

uint8_t AB1805::clean_SLST_sleep(void)
{
  uint8_t c1;
  c1 = read_rtc_register(SLEEP_CONTROL_REGISTER);
  c1 &= ~SLEEP_CONTROL_SLST_MASK;
  write_rtc_register(SLEEP_CONTROL_REGISTER,c1);    
  return c1;       
}

uint8_t AB1805::set_SLTO_sleep(uint8_t value)
{
  uint8_t c1;
  c1 = read_rtc_register(SLEEP_CONTROL_REGISTER);
  c1 &= ~SLEEP_CONTROL_SLTO_MASK;
  c1 |= value;
  write_rtc_register(SLEEP_CONTROL_REGISTER,c1);    
  return c1;     
}
    

//countdown control
uint8_t AB1805::get_time_control(void)
{
  _timer_control_mode = read_rtc_register(TIMER_CONTROL_REGISTER);
  return _timer_control_mode;
}


uint8_t AB1805::get_TE_time_control(void)
{
  get_time_control();
  return (_timer_control_mode & TIMER_CONTROL_TE_MASK >7);
  
};


uint8_t AB1805::get_TM_time_control(void)
{
  get_time_control();
  return (_timer_control_mode & TIMER_CONTROL_TM_MASK >6);
};


uint8_t AB1805::get_TRPT_time_control(void)
{
  get_time_control();
  return (_timer_control_mode & TIMER_CONTROL_TRPT_MASK >5);
  
};
    
    
uint8_t AB1805::get_RPT_time_control(void)
{
  get_time_control();
  return (_timer_control_mode & TIMER_CONTROL_RPT_MASK >2);
  
};


uint8_t AB1805::get_TFS_time_control(void)
{
  get_time_control();
  return (_timer_control_mode & TIMER_CONTROL_TFS_MASK);
  
};


uint8_t AB1805::set_time_control(uint8_t value)
{
  write_rtc_register(TIMER_CONTROL_REGISTER,value);
  _timer_control_mode = read_rtc_register(TIMER_CONTROL_REGISTER);
  return _timer_control_mode;
}


uint8_t AB1805::set_TE_time_control(uint8_t value)
{
  uint8_t c1;
  c1 = read_rtc_register(TIMER_CONTROL_REGISTER);
  c1 &= ~TIMER_CONTROL_TE_MASK;
  if (value)
  {
    c1 |= TIMER_CONTROL_TE_MASK;
  }
  write_rtc_register(TIMER_CONTROL_REGISTER,c1);  
  return c1;
}


uint8_t AB1805::set_TM_time_control(uint8_t value)
{
  uint8_t c1;
  c1 = read_rtc_register(TIMER_CONTROL_REGISTER);
  c1 &= ~TIMER_CONTROL_TM_MASK;
  if (value)
  {
    c1 |= TIMER_CONTROL_TM_MASK;
  }
  write_rtc_register(TIMER_CONTROL_REGISTER,c1);    
  return c1;
}


uint8_t AB1805::set_TRPT_time_control(uint8_t value)
{
  uint8_t c1;
  c1 = read_rtc_register(TIMER_CONTROL_REGISTER);
  c1 &= ~TIMER_CONTROL_TRPT_MASK;
  if (value)
  {
    c1 |= TIMER_CONTROL_TRPT_MASK;
  }
  write_rtc_register(TIMER_CONTROL_REGISTER,c1);    
  return c1;  
}


uint8_t AB1805::set_RPT_time_control(uint8_t value)
{
  uint8_t c1;
  c1 = read_rtc_register(TIMER_CONTROL_REGISTER);
  c1 &= ~TIMER_CONTROL_RPT_MASK;
  c1 |= (value << 2) & TIMER_CONTROL_RPT_MASK;
  write_rtc_register(TIMER_CONTROL_REGISTER,c1);    
  return c1;  
}
    

uint8_t AB1805::set_TFS_time_control(uint8_t value)
{
  uint8_t c1;
  c1 = read_rtc_register(TIMER_CONTROL_REGISTER);
  c1 &= ~TIMER_CONTROL_TFS_MASK;
  c1 |= value;
  write_rtc_register(TIMER_CONTROL_REGISTER,c1);    
  return c1;  
}

//outcontrol registers
uint8_t AB1805::get_outcontrol(void)
{
  _outcontrol = read_rtc_register(OCTRL_REGISTER);
  return _outcontrol;
  
}


uint8_t AB1805::get_outcontrol(uint8_t mask)
{
  uint8_t ret;
  ret = get_outcontrol() & mask;
  switch(mask)
  {
    case OCTRL_WDBM_MASK:
      ret = ret >> 7;
      break;
    case OCTRL_EXBM_MASK:
      ret = ret >> 6;
      break;
    case OCTRL_WDDS_MASK:
      ret = ret >> 5;
      break;
    case OCTRL_EXDS_MASK:
      ret = ret >> 4;
      break;
    case OCTRL_RSEN_MASK:
      ret = ret >> 3;
      break;
    case OCTRL_O4EN_MASK:
      ret = ret >> 2;
      break;
    case OCTRL_O3EN_MASK:
      ret = ret >> 1;
      break;
    case OCTRL_O1EN_MASK:
      ret = ret;
      break;
    default:
      ret = _outcontrol;
  }
  return ret;
}


uint8_t AB1805::set_outcontrol(uint8_t value)
{
  write_rtc_register(OCTRL_REGISTER,value);
  _outcontrol = read_rtc_register(OCTRL_REGISTER);
  return _outcontrol;
  
}


uint8_t AB1805::set_outcontrol(uint8_t value,uint8_t mask)
{
  uint8_t c1;
  c1 = get_outcontrol();
  switch(mask)
  {
    case OCTRL_WDBM_MASK:
      c1 &= ~OCTRL_WDBM_MASK;
      c1 |= (value << 7) & OCTRL_WDBM_MASK;
      break;
    case OCTRL_EXBM_MASK:
      c1 &= ~OCTRL_EXBM_MASK;
      c1 |= (value << 6) & OCTRL_EXBM_MASK;
      break;
    case OCTRL_WDDS_MASK:
      c1 &= ~OCTRL_WDDS_MASK;
      c1 |= (value << 5) & OCTRL_WDDS_MASK;
      break;
    case OCTRL_EXDS_MASK:
      c1 &= ~OCTRL_EXDS_MASK;
      c1 |= (value << 4) & OCTRL_EXDS_MASK;
      break;
    case OCTRL_RSEN_MASK:
      c1 &= ~OCTRL_RSEN_MASK;
      c1 |= (value << 3) & OCTRL_RSEN_MASK;
      break;
    case OCTRL_O4EN_MASK:
      c1 &= ~OCTRL_O4EN_MASK;
      c1 |= (value << 2) & OCTRL_O4EN_MASK;
      break;
    case OCTRL_O3EN_MASK:
      c1 &= ~OCTRL_O3EN_MASK;
      c1 |= (value << 1) & OCTRL_O3EN_MASK;
      break;
    case OCTRL_O1EN_MASK:
      c1 &= ~OCTRL_O1EN_MASK;
      c1 |= (value ) & OCTRL_O1EN_MASK;
      break;
    case CCTRL_SLEEP_MODE_MASK:
      //special mode for sleep mode, to Avoiding Unexpected Leakage Paths
      c1 &= ~CCTRL_SLEEP_MODE_MASK;
      c1 |= (value ) & CCTRL_SLEEP_MODE_MASK;
    default:
      c1 = value;
  }
  return set_outcontrol(c1);
}


//OSC control registers
uint8_t AB1805::get_osc_control(void)
{
  _osc_control = read_rtc_register(OSC_CONTROL_REGISTER);
  return _osc_control;
  
}


uint8_t AB1805::get_osc_control(uint8_t mask)
{
  uint8_t ret;
  ret = get_osc_control() & mask;
  switch(mask)
  {
    case OSC_CONTROL_OSEL_MASK:
      ret = ret >> 7;
      break;
    case OSC_CONTROL_ACAL_MASK:
      ret = ret >> 5;
      break;
    case OSC_CONTROL_AOS_MASK:
      ret = ret >> 4;
      break;
    case OSC_CONTROL_FOS_MASK:
      ret = ret >> 3;
      break;
    case OSC_CONTROL_PWGT_MASK:
      ret = ret >> 2;
      break;
    case OSC_CONTROL_OFIE_MASK:
      ret = ret >> 1;
      break;
    case OSC_CONTROL_ACIE_MASK:
      ret = ret;
      break;
    default:
      ret = _osc_control;
  }
  return ret;
}


uint8_t AB1805::set_osc_control(uint8_t value)
{
  write_rtc_register(OSC_CONTROL_REGISTER,value);
  _osc_control = read_rtc_register(OSC_CONTROL_REGISTER);
  return _osc_control;
  
}


uint8_t AB1805::set_osc_control(uint8_t value,uint8_t mask)
{
  uint8_t c1;
  c1 = get_osc_control();
  switch(mask)
  {
    case OSC_CONTROL_OSEL_MASK:
      c1 &= ~OSC_CONTROL_OSEL_MASK;
      c1 |= (value << 7) & OSC_CONTROL_OSEL_MASK;
      break;
    case OSC_CONTROL_ACAL_MASK:
      c1 &= ~OSC_CONTROL_ACAL_MASK;
      c1 |= (value << 5) & OSC_CONTROL_ACAL_MASK;
      break;
    case OSC_CONTROL_AOS_MASK:
      c1 &= ~OSC_CONTROL_AOS_MASK;
      c1 |= (value << 4) & OSC_CONTROL_AOS_MASK;
      break;
    case OSC_CONTROL_FOS_MASK:
      c1 &= ~OSC_CONTROL_FOS_MASK;
      c1 |= (value << 3) & OSC_CONTROL_FOS_MASK;
      break;
    case OSC_CONTROL_PWGT_MASK:
      c1 &= ~OSC_CONTROL_PWGT_MASK;
      c1 |= (value << 2) & OSC_CONTROL_PWGT_MASK;
      break;
    case OSC_CONTROL_OFIE_MASK:
      c1 &= ~OSC_CONTROL_OFIE_MASK;
      c1 |= (value << 1) & OSC_CONTROL_OFIE_MASK;
      break;
    case OSC_CONTROL_ACIE_MASK:
      c1 &= ~OSC_CONTROL_ACIE_MASK;
      c1 |= (value ) & OSC_CONTROL_ACIE_MASK;
      break;
    default:
      c1 = value;
  }
  return set_osc_control(c1);
}


//WDT register
uint8_t AB1805::get_WDT_register(void)
{
  _wdt_register = read_rtc_register(WDT_REGISTER);
  return _wdt_register;
  
}


uint8_t AB1805::get_WDT_register(uint8_t mask)
{
  uint8_t ret;
  ret = get_WDT_register() & mask;
  switch(mask)
  {
    case WDT_REGISTER_WDS_MASK:
      ret = ret >> 7;
      break;
    case WDT_REGISTER_BMB_MASK:
      ret = ret >> 2;
      break;
    case WDT_REGISTER_WRB_MASK:
      ret = ret;
      break;
    default:
      ret = _wdt_register;
  }
  return ret;
}


uint8_t AB1805::set_WDT_register(uint8_t value)
{
  write_rtc_register(WDT_REGISTER,value);
  _wdt_register = read_rtc_register(WDT_REGISTER);
  return _wdt_register;
  
}


uint8_t AB1805::set_WDT_register(uint8_t value,uint8_t mask)
{
  uint8_t c1;
  c1 = get_WDT_register();
  switch(mask)
  {
    case WDT_REGISTER_WDS_MASK:
      c1 &= ~WDT_REGISTER_WDS_MASK;
      c1 |= (value << 7) & WDT_REGISTER_WDS_MASK;
      break;
    case WDT_REGISTER_BMB_MASK:
      c1 &= ~WDT_REGISTER_BMB_MASK;
      c1 |= (value << 2) & WDT_REGISTER_BMB_MASK;
      break;
    case WDT_REGISTER_WRB_MASK:
      c1 &= ~WDT_REGISTER_WRB_MASK;
      c1 |= (value ) & WDT_REGISTER_WRB_MASK;
      break;
    default:
      c1 = value;
  }
  return set_WDT_register(c1);
}


void AB1805::enter_sleep_mode_PWR(void)
{
  enter_sleep_mode_PWR(SLEEP_CONTROL_SLTO_MAX_VALUE);
}


void AB1805::enter_sleep_mode_PWR(uint8_t value)
{
  if (get_SLST_sleep())
  {// clean previous sleep 
    clean_SLST_sleep();
  }
  //set OUT2S sleep mode
  //the OUT2S field must be set to a value of 6 to select the SLEEP output.
  set_OUT2S_control2(CONTROL2_SLEEP_MODE);
  
  //Avoiding Unexpected Leakage Paths
  //clearing the RSEN,O1EN,O4EN,O3EN,EXDS,WDDS bits in output control register
  set_outcontrol(0,CCTRL_SLEEP_MODE_MASK);
  
  //The I2C or SPI interface pins are disabled in Sleep Mode by setting the PWGT bit in the Oscillator Control Register.
  set_osc_control(0,OSC_CONTROL_PWGT_MASK);

  //The number of 7.8 ms periods after SLP is set until the Power Control SM goes into the SLEEP state.
  set_SLTO_sleep(value);
  
  //set sleep mode
  set_SLP_sleep(1);  

}

/** Parameter:
 *  timeout - minimum timeout period in 7.8 ms periods (0 to 7)
 *  mode - sleep mode (nRST modes not available in AM08xx)
 *      0 => nRST is pulled low in sleep mode
 *      1 => PSW/nIRQ2 is pulled high on a sleep
 *      2 => nRST pulled low and PSW/nIRQ2 pulled high on sleep
 *  error ?returned value of the attempted sleep command
 *      0 => sleep request accepted, sleep mode will be initiated in timeout seconds
 *      1 => illegal input values
 *      2 => sleep request declined, interrupt is currently pending
 *      3 => sleep request declined, no sleep trigger interrupt enabled
**/
uint8_t AB1805::enter_sleep_mode(uint8_t timeout, uint8_t mode)
{
    uint8_t ret = 0;
    uint8_t slres = 0;
    char temp = 0;
 
#ifdef DEBUG_SIO    
    temp = get_SLST_sleep();       // Get SLST bit (temp & 0x08)
    
    if ( (temp) == 0)
    {
        DBGPRINT("\r\nPrevious Sleep Failed");
    } else {
        DBGPRINT("\r\nPrevious Sleep Successful");    
    }
    clean_SLST_sleep();                     // Clear SLST
    
    temp = get_SLST_sleep();       // Get SLST bit (temp & 0x08)
    
    if ( ( temp ) == 0)
    {
        DBGPRINT("\r\nClear Succ");
    } else {
        DBGPRINT("\r\nClear Fail");    
    }
    clean_SLST_sleep();                     // Clear SLST   
#endif
 
    if (mode > 0)
    {
        /* Sleep to PSW/nIRQ2 */
        temp = get_OUT2S_control2();   // Read OUT2S
        temp = (temp & 0xE3) | 0x18;                        // MUST NOT WRITE OUT2S WITH 000
        set_OUT2S_control2(temp);      // Write value to OUT2S
        slres = 0;
    } 
    
    temp = timeout | (slres << 6) | 0x80;                   // Assemble SLEEP register value
    set_sleep(temp);      // Write to the register    
 
#ifdef DEBUG_SIO
    /* Determine if SLEEP was accepted */
    temp = get_SLP_sleep();       // Get SLP bit (temp & 0x80)
    
    if ( ( temp ) == 0)
    {
        char reg_wdi_value = 0;
        /* SLEEP did not happen - determine why and return reason. */
        temp = get_interrupt();         // Get status register interrupt enables
        reg_wdi_value = get_WDT_register();
        if ((( temp & 0x0F ) == 0) & (((reg_wdi_value & 0x7C) == 0) || ((reg_wdi_value & 0x80) == 0x80)))
        {
            ret = 3;
            DBGPRINT("\r\nNo trigger interrupts enabled");
        }
        else
        { 
            ret = 2;
            DBGPRINT("\r\nInterrupt pending");
        }
    }
    else
    {
        ret = 0;
        DBGPRINT("\r\nSLEEP request successful");
    }
#endif
    return ret;
}
 
