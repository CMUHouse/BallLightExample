

#ifndef BALL_LIGHT_H
#define BALL_LIGHT_H

#if (ARDUINO >= 100)
 #include <Arduino.h>
#else
 #include <WProgram.h>
 #include <pins_arduino.h>
#endif

class RGBColor {
public:
  uint8_t r = 0;
  uint8_t g = 0;
  uint8_t b = 0;

  RGBColor(uint8_t _r, uint8_t _g, uint8_t _b)
  : r(_r)
  , g(_g)
  , b(_b)
  {}

  bool operator==(RGBColor& rhs) const {
    return (r == rhs.r) && (g == rhs.g) && (b == rhs.b);
  }
  
  RGBColor blend(const RGBColor& withColor, uint8_t alpha) const;

};

#define WHITE  RGBColor(255,255,255)
#define BLACK  RGBColor(0,0,0)
#define RED  RGBColor(255,0,0)
#define GREEN  RGBColor(0,255,0)
#define BLUE  RGBColor(0,0,255)
#define ORANGE  RGBColor(255,128,0)
#define YELLOW   RGBColor(255,255,0)
#define PURPLE  RGBColor(128,0,255)
#define TEAL  RGBColor(0,0,128)
#define INDIGO RGBColor(255,0,127)

class BallLightSettings
{
public:
    
    BallLightSettings(unsigned long animation_dur, unsigned long anim_hold,
                 uint8_t anim_variance, uint8_t hold_variance);

    unsigned long anim_dur_ms = 0;
    unsigned long anim_hold_ms = 0;
    uint8_t dur_variance_percentage = 0;
    uint8_t hold_variance_percentage = 0;
    
};

class BallLight {
    
  public:
    
    BallLight();
    RGBColor updateForTime(unsigned long t, const BallLightSettings& set);

  private:

    typedef unsigned long Milliseconds;
    
    Milliseconds m_animStart_ms = 0;
    Milliseconds m_animEnd_ms = 0;

    uint8_t m_startColorIdx = 0;
    uint8_t m_endColorIdx = 0;
    uint8_t m_startColorLuminance = 0;
    uint8_t m_endColorLuminance = 0;
};

#endif // BALL_LIGHT_H
