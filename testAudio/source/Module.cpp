#include "Module.h"

Port* Module::getPortByName(const std::string& portName, PortType portType) const {
    switch(portType){
        case INPUT_PORT:
            for(InputPort* port: m_input_ports)
                if(port->getName() == portName)
                    return port;
            break;
        case OUTPUT_PORT:
            for(OutputPort* port: m_output_ports)
                if(port->getName() == portName)
                    return port;
            break;
        case UNDEFINED_PORT:
        default:
            for(Port* port: m_ports)
                if(port->getName() == portName)
                    return port;
            break;
    }
    return nullptr;
}