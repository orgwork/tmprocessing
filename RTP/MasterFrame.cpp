#include "MasterFrame.h"

MasterFrame::MasterFrame()
{
    this->numOfSubFrames = 32;
    this->clearMasterFrameData();
}


uint16_t MasterFrame::updateFrame(char *rawFrame)
{
    uint16_t frameId = rawFrame[10] & 0x1f;

    this->rawFrameMap[frameId] = (uint8_t *)rawFrame;

    return frameId;
}


void MasterFrame::clearMasterFrameData()
{
    for (int frameId = 0; frameId < numOfSubFrames; frameId++)
    {
        this->rawFrameMap[frameId] = new uint8_t[256];
        for (int i = 0; i < 256; i++)
            this->rawFrameMap[frameId][i] = 0;
    }
}
