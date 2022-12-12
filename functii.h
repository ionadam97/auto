class OUT {
  public:
    OUT(byte pin) {
      _pin = pin;
      
      pinMode(_pin, OUTPUT);
    }

    int out(uint16_t _prd = 600) {
      if (!_flag) {
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
    uint16_t _prd;
    bool _flag;
};