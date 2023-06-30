#include <cstring>
#include "Port.h"
#include "SoundUtils.h"
#include <cmath>
#include "Module.h"

void InputPort::update(double time) {
    if(m_connected){
        m_connectedPort->update(time);
        memcpy(m_value, m_connectedPort->getValueBuffer(), sizeof(float )*GLOBAL_BUFFER_SIZE);
    }
    else{
        memset(m_value, 0, sizeof(float )*GLOBAL_BUFFER_SIZE);
        /*
        for(int i=0; i<GLOBAL_BUFFER_SIZE; i++){
            m_value[i] = std::sin(((float)i/(float)GLOBAL_BUFFER_SIZE)*M_PI*2.0);
        }
         */
    }
}

void InputPort::connectTo(Port* port) {
    if(nullptr != port){
        if(port->getPortType() == OUTPUT_PORT){
            m_connectedPort = dynamic_cast<OutputPort*>(port);
            m_connected=true;
        }
    }
}

void OutputPort::update(double time) {
    if(nullptr != itsModule)
        itsModule->update(time);
}

void OutputPort::connectTo(Port* port) {
    if(nullptr != port){
        if(port->getPortType() == INPUT_PORT){
            m_connectedPorts.push_back(dynamic_cast<InputPort*>(port));
            m_connected=true;
        }
    }
}