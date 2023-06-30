#ifndef TESTAUDIO_OUTPUTMODULE_H
#define TESTAUDIO_OUTPUTMODULE_H

#include "SoundUtils.h"
#include "Module.h"
#include <map>

class OutputModule : public Module{
public:
    explicit OutputModule(float level);
    ~OutputModule();
    void update(double time) override;
    [[nodiscard]] float getLeftValue(int i) const {
        return m_leftValue[i%GLOBAL_BUFFER_SIZE];
    }
    [[nodiscard]] float getRightValue(int i) const {
        return m_rightValue[i%GLOBAL_BUFFER_SIZE];
    }

protected:
    float m_level=1.0f;
    InputPort m_leftInput;
    InputPort m_rightInput;
    float m_leftValue[GLOBAL_BUFFER_SIZE];
    float m_rightValue[GLOBAL_BUFFER_SIZE];
    // For tests
    std::map<float, float> m_history;
};


#endif //TESTAUDIO_OUTPUTMODULE_H
