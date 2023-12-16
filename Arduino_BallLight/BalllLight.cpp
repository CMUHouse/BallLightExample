

#include "BallLight.h"

static uint8_t lerp(uint8_t a, uint8_t b, uint8_t v)
{
  uint16_t sa = a;
  uint16_t sb = b;
  uint16_t mab = (sa * v) + (sb * (255 - v));
  return (uint8_t)(mab >> 8);
}

RGBColor
RGBColor::blend(const RGBColor& withColor, uint8_t alpha) const
{
  RGBColor ret(0,0,0);
  ret.r = lerp(this->r, withColor.r, alpha);
  ret.g = lerp(this->g, withColor.g, alpha);
  ret.b = lerp(this->b, withColor.b, alpha);
  return ret;
}


static RGBColor default_colors[] = {
  RED,
  GREEN,
  BLUE,
  ORANGE,
  PURPLE,
  YELLOW,
  TEAL,
  INDIGO,
  WHITE,
};

static const uint8_t kWhiteIdx = (sizeof(default_colors) / sizeof(RGBColor)) - 1;


static uint8_t randomBallColorIndex()
{
    uint32_t numCols = sizeof(default_colors) / sizeof(RGBColor);
    uint32_t idx = random(numCols);
    return idx;
}

static int32_t randomizedDuration(unsigned long dur, uint8_t variance)
{
  if (variance == 0) {
    return dur;
  }

  int32_t range = (dur *  variance) / 100;
  int32_t offset = random(-range/2, range/2);
  int32_t value = dur;
  return dur + offset;
}

static uint8_t getRandomLuminanceForColorIdx(uint8_t colorIdx, uint8_t minPercent)
{
    if (colorIdx == kWhiteIdx) {
        return 100;
    }
    return random(minPercent, 100);
}

static RGBColor applyLuminancePercentage(const RGBColor& color, uint8_t percentage)
{
  if (percentage >= 100) {
    return color;
  }

  float pp = (percentage / 100.f);
  float lr = color.r * (1.f - (pp * 0.299f));
  float lg = color.g * (1.f - (pp * 0.587f));
  float lb = color.b * (1.f - (pp * 0.114f));

  return RGBColor(lr,lg,lb);
}

static RGBColor colorForIndexAndLumaPercentage(uint8_t index, uint8_t percentage)
{
    const RGBColor& col = default_colors[index];
    return applyLuminancePercentage(col, percentage);
}

BallLight::BallLight()
: m_animStart_ms(0)
, m_animEnd_ms(0)
{

  m_startColorIdx = randomBallColorIndex();
  do {
    m_endColorIdx = randomBallColorIndex();
  } while (m_startColorIdx == m_endColorIdx);

    
    m_startColorLuminance = getRandomLuminanceForColorIdx(m_startColorIdx, 25);
    m_endColorLuminance = getRandomLuminanceForColorIdx(m_endColorIdx, 25);
}

RGBColor BallLight::updateForTime(unsigned long t, const BallLightSettings& set)
{
            
    Milliseconds offset = 0;
    const Milliseconds anim_dur = randomizedDuration(set.anim_dur_ms, set.dur_variance_percentage);

    if (m_animStart_ms == 0 || m_animEnd_ms == 0 || m_animStart_ms >= m_animEnd_ms) {
        m_animStart_ms = t;
        m_animEnd_ms = t + anim_dur;        
    }
    
    const RGBColor startColor = colorForIndexAndLumaPercentage(m_startColorIdx, m_startColorLuminance);
    const RGBColor endColor = colorForIndexAndLumaPercentage(m_endColorIdx, m_endColorLuminance);
    
    RGBColor currColor(0,0,0);
    
    if (t <= m_animStart_ms) {
        // reset to start color
        m_animStart_ms = t;
        m_animEnd_ms = t + anim_dur;
        
        currColor = startColor;
        
    } else if (t >= m_animEnd_ms) {
        
        // reset to end color
        currColor = endColor;
        
        if (m_startColorIdx == m_endColorIdx) {
            // change to something new
            do {
                m_endColorIdx = randomBallColorIndex();
            } while (m_startColorIdx == m_endColorIdx);

            m_endColorLuminance = getRandomLuminanceForColorIdx(m_endColorIdx, 25);
            
            m_animStart_ms = t;
            m_animEnd_ms = t + anim_dur;
        } else {
            //
            const Milliseconds anim_hold = randomizedDuration(set.anim_hold_ms, set.hold_variance_percentage);
            m_startColorIdx = m_endColorIdx;
            m_animStart_ms = t;
            m_animEnd_ms = t + anim_hold;
        }
        
    } else {

        Milliseconds inter = ((t - m_animStart_ms) * 255) / (m_animEnd_ms - m_animStart_ms);
        currColor = endColor.blend(startColor, inter);
    }
    
    return currColor;
}

BallLightSettings::BallLightSettings(unsigned long animation_dur, unsigned long anim_hold,
                                     uint8_t anim_variance, uint8_t hold_variance)
{
    this->anim_dur_ms = animation_dur;
    this->anim_hold_ms = anim_hold;
    this->dur_variance_percentage = min(anim_variance, 100);
    this->hold_variance_percentage = min(hold_variance, 100);
}
