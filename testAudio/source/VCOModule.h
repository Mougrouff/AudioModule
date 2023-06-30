#ifndef TESTAUDIO_VCOMODULE_H
#define TESTAUDIO_VCOMODULE_H

#include "Module.h"
#include <chrono>
#include <map>

class VCOModule: public Module {
public:
    explicit VCOModule(float frequency, float m_amplitude=1.0f);
    ~VCOModule();
    void update(double time) override;
    void setFrequency(float freq) {m_frequency=std::max(freq, 0.0f);}

protected:
    float m_frequency=1.0f;
    float m_amplitude=1.0f;
    double m_lastTime=0.0f;
    double m_phase=0.0f;
    std::chrono::time_point<std::chrono::system_clock> m_start;
    OutputPort m_output;
    InputPort m_input_freq;
    // For tests
    std::map<float, float> m_history;
};


#endif //TESTAUDIO_VCOMODULE_H
