class OUT {
  public:
    OUT(byte pin) {
      _pin = pin;
      
      pinMode(_pin, OUTPUT);
    }

    int out(uint32_t _prd , bool flag_1 = 0) {

      if (flag_1)_tmr = millis();   
       
      if (!_flag && flag_1 && _prd >0) {
        _tmr = millis();
        _flag = 1;
        digitalWrite(_pin, _flag);
      }
      if (_flag && millis() - _tmr >= _prd) {
        _flag = 0;
        digitalWrite(_pin, _flag);
      }
      
      return _flag ;
    }

  private:
    byte _pin;
    uint32_t _tmr;
    uint32_t _prd;
    bool _flag;
};


int voltaj(byte pin){
  int val;
  val = analogRead(pin); // citeste datele
  val = map(val, 0, 1023, 0, 1000); //schimba diapazonul de masurare
  val = constrain(val, 0, 1000);//schimba diapazonul de masurare
  val = val * 0.49 *3;// valoarea dc dc
  return val;
  }
