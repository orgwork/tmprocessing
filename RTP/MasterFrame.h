#ifndef MASTERFRAME_H
#define MASTERFRAME_H


#include "../includes/SystemFiles.h"
#include "../includes/TMShmStruct.h"

class MasterFrame
{
public:
    MasterFrame();
    uint16_t updateFrame(char *rawFrame);
    void clearMasterFrameData();

    uint8_t *rawFrameMap[32];

private:
    int curFrameId;
    int subFrameLength;
    int numOfSubFrames;

};

#endif // MASTERFRAME_H
