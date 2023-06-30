#ifndef TESTAUDIO_PORT_H
#define TESTAUDIO_PORT_H

#include <iostream>
#include <vector>
#include "SoundUtils.h"

class Module;

enum PortType{UNDEFINED_PORT, INPUT_PORT, OUTPUT_PORT};

class Port {
public:
    explicit Port(const std::string& name) {m_port_name=name;}
    virtual void update(double time) = 0;
    [[nodiscard]] float getValue(int i) const {
        i = abs(i) % GLOBAL_BUFFER_SIZE;
        return m_value[i];
    }
    [[nodiscard]] const float* getValueBuffer() const {return m_value;}
    [[nodiscard]] float getMinValue() const {return m_min_value;}
    [[nodiscard]] float getMaxValue() const {return m_max_value;}
    void setValue(int i, float value)  {
        // Value is forced to be between min and max with a threshold effect
        i = abs(i) % GLOBAL_BUFFER_SIZE;
        m_value[i] = std::min(m_max_value, std::max(m_min_value, value));
    }
    void setMinValue(float value)  {
        m_min_value = std::min(m_max_value, value);
    }
    void setMaxValue(float value)  {
        m_max_value = std::max(m_min_value, value);
    }
    void setItsModule(Module* aModule) {itsModule = aModule;}
    [[nodiscard]] bool isConnected() const {return m_connected;}
    virtual void connectTo(Port* port) = 0;
    [[nodiscard]] std::string getName() const {return m_port_name;}
    virtual PortType getPortType() {return UNDEFINED_PORT;}
protected:
    float m_min_value=-1.0f;
    float m_max_value=1.0f;
    bool m_connected=false;
    std::string m_port_name;
    const PortType m_type=UNDEFINED_PORT;
    float m_value[GLOBAL_BUFFER_SIZE];
    Module* itsModule=nullptr;
};

class OutputPort; // Forward declarations for mutual references
class InputPort;

class InputPort: public Port{
public:
    explicit InputPort(const std::string& name): Port(name) {}
    void update(double time) override;
    void connectTo(Port* port) override;
    virtual PortType getPortType() {return INPUT_PORT;}
protected:
    OutputPort* m_connectedPort=nullptr;
};

class OutputPort: public Port{
public:
    explicit OutputPort(const std::string& name): Port(name) {}
    void update(double time) override;
    void connectTo(Port* port) override;
    virtual PortType getPortType() {return OUTPUT_PORT;}
protected:
    std::vector<InputPort*> m_connectedPorts;
};

#endif //TESTAUDIO_PORT_H
