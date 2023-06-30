#include "OutputModule.h"
#include <fstream>

OutputModule::OutputModule(float level): m_leftInput("Left Input"), m_rightInput("Right Input") {
    m_level = std::min(std::abs(level), 1.0f);
    m_input_ports.push_back(&m_leftInput);
    m_input_ports.push_back(&m_rightInput);
    m_ports.push_back(&m_leftInput);
    m_ports.push_back(&m_rightInput);
    m_leftInput.setItsModule(this);
    m_rightInput.setItsModule(this);
}

void OutputModule::update(double time) {
    static int iter=0;
    for(InputPort* port: m_input_ports)
        port->update(time);
    for(int i=0; i<GLOBAL_BUFFER_SIZE; ++i){
        m_leftValue[i] = m_leftInput.getValue(i) * m_level;
        m_rightValue[i] = m_rightInput.getValue(i) * m_level;
        m_history[iter*GLOBAL_BUFFER_SIZE+i] = m_leftValue[i];
    }
    iter++;
}

OutputModule::~OutputModule(){
    std::ofstream file_out;
    file_out.open("OutputValues.txt", std::ios_base::out);
    for(std::map<float, float>::iterator it=m_history.begin(); it!=m_history.end(); ++it)
        file_out << it->first << " " << it->second << std::endl;
}