//
// Created by a98012636 on 29/06/2023.
//

#include "VCOModule.h"
#include <cmath>
#include <fstream>

VCOModule::VCOModule(float frequency, float amp): m_output("Output"), m_input_freq("Input Frequency"),
        m_frequency(frequency), m_amplitude(amp)
{
    m_start = std::chrono::system_clock::now();
    m_ports.push_back(&m_output);
    m_output_ports.push_back(&m_output);
    m_ports.push_back(&m_input_freq);
    m_input_ports.push_back(&m_input_freq);
    m_output.setItsModule(this);
    m_input_freq.setItsModule(this);
}

void VCOModule::update(double time) {
    double freq=m_frequency;
    if(m_input_freq.isConnected()){
        m_input_freq.update(time);
        freq += m_input_freq.getValue(0)*5000.0f;
        //std::cout << m_input_freq.getValue(0)*1.0f << std::endl;
    }
    m_phase += (time-m_lastTime)*freq*2.0f*M_PI;
    m_phase = fmod(m_phase, 2.0f*M_PI);
    for(int i=0; i<GLOBAL_BUFFER_SIZE; ++i){
        /*
        if(m_input_freq.isConnected())
            m_frequency = m_input_freq.getValue(i)*1.0f;
        */
        double dt = ((double)i)/((double)GLOBAL_SAMPLE_RATE);
        double value = m_amplitude*sin(m_phase+dt*2*M_PI*freq);
        m_output.setValue(i, value);
        m_history[time+dt] = value;
    }
    m_lastTime = time;
}

VCOModule::~VCOModule(){
    std::ofstream file_out;
    file_out.open("VCOValues.txt", std::ios_base::out);
    for(std::map<float, float>::iterator it=m_history.begin(); it!=m_history.end(); ++it)
        file_out << it->first << " " << it->second << std::endl;
}