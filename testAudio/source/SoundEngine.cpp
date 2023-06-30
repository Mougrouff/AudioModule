//
// Created by a98012636 on 29/06/2023.
//

#include "SoundEngine.h"

int SoundEngine::PaCallback( const void *inputBuffer, void *outputBuffer, unsigned long framesPerBuffer,
                       const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags statusFlags, void *userData ){
    OutputModule* outputModule = reinterpret_cast<OutputModule*>(userData);
    float *out = (float*)outputBuffer;
    (void) timeInfo; /* Prevent unused variable warnings. */
    (void) statusFlags;
    (void) inputBuffer;

    outputModule->update(timeInfo->outputBufferDacTime);
    static int phase=0;

    for( int i=0; i<framesPerBuffer; i++ )
    {

        *out++ = outputModule->getLeftValue(i%GLOBAL_BUFFER_SIZE);
        *out++ = outputModule->getRightValue(i%GLOBAL_BUFFER_SIZE);
        //std::cout <<  *out << std::endl;
    }

    return paContinue;
}

void SoundEngine::StreamFinished(void* userData){
    std::cout << "Stream finished" << std::endl;
}

SoundEngine::SoundEngine(double outputLevel): m_output_module(outputLevel)
{
    PaError err = Pa_Initialize();
    if( err != paNoError )
        recordError(err);
    m_outputParameters.device = Pa_GetDefaultOutputDevice(); /* default output device */
    if (m_outputParameters.device == paNoDevice) {
        if(m_verbose)
            std::cout << "[Sound Engine] - ERROR : No default output device" << std::endl;
        return;
    }
    m_outputParameters.channelCount = 2;       /* stereo output */
    m_outputParameters.sampleFormat = paFloat32; /* 32 bit floating point output */
    m_outputParameters.suggestedLatency = Pa_GetDeviceInfo( m_outputParameters.device )->defaultLowOutputLatency;
    m_outputParameters.hostApiSpecificStreamInfo = nullptr;
    m_initialized=true;
}

PaError SoundEngine::open(){
    PaError err = Pa_OpenStream(
            &m_stream,
            nullptr, /* no input */
            &m_outputParameters,
            GLOBAL_SAMPLE_RATE,
            GLOBAL_BUFFER_SIZE,
            paClipOff,      /* we won't output out of range samples so don't bother clipping them */
            &SoundEngine::PaCallback,
            &m_output_module );
    if( err != paNoError ) recordError(err);
    err = Pa_SetStreamFinishedCallback( m_stream, &SoundEngine::StreamFinished);
    if( err != paNoError ) recordError(err);
    if(err == paNoError)
        m_open=true;
    const PaDeviceInfo* pInfo = Pa_GetDeviceInfo(m_outputParameters.device);
    if (pInfo != 0)
        printf("Output device name: '%s'\r", pInfo->name);
    return err;
}

PaError SoundEngine::start(){
    PaError err = Pa_StartStream( m_stream );
    if( err != paNoError ) recordError(err);
    m_started=true;
    return err;
}

PaError SoundEngine::stop(){
    PaError err = Pa_StopStream( m_stream );
    if( err != paNoError ) recordError(err);
    m_started=false;
    return err;
}

PaError SoundEngine::close(){
    PaError err = Pa_CloseStream( m_stream );
    if( err != paNoError ) recordError(err);
    m_open=false;
    return err;
}

void SoundEngine::recordError(PaError err){
    m_error_history.push_back(err);
    m_last_err = err;
    if(m_verbose)
        std::cout << "[Sound Engine] - ERROR " << err << " : " << Pa_GetErrorText( err ) << std::endl;
}

SoundEngine::~SoundEngine(){
    Pa_Terminate();
}

void SoundEngine::connectModules(Module* outputModule, const std::string& outputPortName,
                    Module* inputModule, const std::string& inputPortName){
    OutputPort* outputPort = dynamic_cast<OutputPort*>(outputModule->getPortByName(outputPortName, OUTPUT_PORT));
    InputPort* inputPort = dynamic_cast<InputPort*>(inputModule->getPortByName(inputPortName, INPUT_PORT));
    if(nullptr != outputPort && nullptr != inputPort){
        outputPort->connectTo(inputPort);
        inputPort->connectTo(outputPort);
    }
    if(nullptr == outputPort)
        std::cout << "outputPort is NULL" << std::endl;
    if(nullptr == inputPort)
        std::cout << "inputPort is NULL" << std::endl;
}

void SoundEngine::connectToOutputLeft(Module* amodule, const std::string& portName){
    OutputPort* outputPort = dynamic_cast<OutputPort*>(amodule->getPortByName(portName, OUTPUT_PORT));
    InputPort* inputPort = dynamic_cast<InputPort*>(m_output_module.getPortByName("Left Input", INPUT_PORT));
    if(nullptr != outputPort && nullptr != inputPort){
        outputPort->connectTo(inputPort);
        inputPort->connectTo(outputPort);
    }
    if(nullptr == outputPort)
        std::cout << "outputPort is NULL" << std::endl;
    if(nullptr == inputPort)
        std::cout << "inputPort is NULL" << std::endl;
}

void SoundEngine::connectToOutputRight(Module* amodule, const std::string& portName){
    OutputPort* outputPort = dynamic_cast<OutputPort*>(amodule->getPortByName(portName, OUTPUT_PORT));
    InputPort* inputPort = dynamic_cast<InputPort*>(m_output_module.getPortByName("Right Input", INPUT_PORT));
    if(nullptr != outputPort && nullptr != inputPort){
        outputPort->connectTo(inputPort);
        inputPort->connectTo(outputPort);
    }
    if(nullptr == outputPort)
        std::cout << "outputPort is NULL" << std::endl;
    if(nullptr == inputPort)
        std::cout << "inputPort is NULL" << std::endl;
}