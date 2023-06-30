#ifndef TESTAUDIO_MODULE_H
#define TESTAUDIO_MODULE_H

#include <iostream>
#include <vector>
#include "Port.h"

class Module {
public:
    Module() = default;
    virtual void update(double time)=0;
    [[nodiscard]] Port* getPortByName(const std::string& portName, PortType portType=UNDEFINED_PORT) const;
protected:
    std::string m_moduleName;
    std::vector<Port*> m_ports;
    std::vector<InputPort*> m_input_ports;
    std::vector<OutputPort*> m_output_ports;
};


#endif
