#include <iostream>
#include <ctime>
#include <unistd.h>
#include "portaudio.h"
#include "source/SoundEngine.h"
#include "source/VCOModule.h"

int main(void);
int main(void) {
    SoundEngine soundEngine(0.5);
    if(!soundEngine.isInitialized()){
        return -1;
    }
    soundEngine.open();
    if(!soundEngine.isOpen()){
        return -1;
    }
    soundEngine.start();
    if(!soundEngine.isStarted()){
        soundEngine.close();
        return -1;
    }

    VCOModule vcoModulator(120.0f, 0.02);
    VCOModule vcoModule(440.0f);
    //VCOModule vcoModule(440.0f);
    soundEngine.connectToOutputLeft(&vcoModule, "Output");
    soundEngine.connectModules(&vcoModulator, "Output", &vcoModule, "Input Frequency");

    Pa_Sleep(5000);
    soundEngine.stop();
    soundEngine.close();
    getchar();
    return 0;
}
