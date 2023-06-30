#ifndef TESTAUDIO_SOUNDENGINE_H
#define TESTAUDIO_SOUNDENGINE_H

#include "portaudio.h"
#include "OutputModule.h"

class SoundEngine {
public:

    // Static zone for callbacks

    static SoundEngine* GetInstance() {return m_static_instance;}
    static int PaCallback( const void *inputBuffer, void *outputBuffer, unsigned long framesPerBuffer,
                               const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags statusFlags, void *userData );
    static void StreamFinished(void* userData);

    // Member zone for data
    explicit SoundEngine(double outputLevel=1.0f);
    ~SoundEngine();
    PaError open();
    PaError start();
    PaError stop();
    PaError close();
    void recordError(PaError);

    [[nodiscard]] const OutputModule& getOutputModule() const {return m_output_module;}
        // Sample rates below 44100 kHz are not allowed
    [[nodiscard]] bool isInitialized() const {return m_initialized;}
    [[nodiscard]] bool isOpen() const {return m_open;}
    [[nodiscard]] bool isStarted() const {return m_started;}
    [[nodiscard]] bool getVerbose() const {return m_verbose;}
    void setVerbose(bool state){m_verbose=state;}
    void connectModules(Module* outputModule, const std::string& outputPortName,
                        Module* inputModule, const std::string& inputPortName);
    void connectToOutputLeft(Module* amodule, const std::string& portName);
    void connectToOutputRight(Module* amodule, const std::string& portName);


private:
    static SoundEngine* m_static_instance;
    PaStreamParameters m_outputParameters;
    PaStream* m_stream;
    PaError m_last_err;
    std::vector<PaError> m_error_history;
    OutputModule m_output_module;
    bool m_initialized=false;
    bool m_open=false;
    bool m_started=false;

    bool m_verbose=true;
};


#endif
