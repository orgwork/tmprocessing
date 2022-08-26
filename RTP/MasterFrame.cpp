#include "MasterFrame.h"

MasterFrame::MasterFrame()
{

}


uint16_t MasterFrame::updateFrame(char *rawFrame)
{
    uint16_t frameId = rawFrame[10] & 0x1f;

    this->rawFrameMap[frameId] = (uint8_t *)rawFrame;

    return frameId;
}


void MasterFrame::clearMasterFrameData()
{
    //    std::vector<uint8_t> frame(256, 0);

    //    std::map<uint16_t, uint8_t *>::iterator it;
    //    for (int i = 0; i < numOfSubFrames; i++)
    //    {
    //        it = rawFrameMap.find(i);
    //        if (it != rawFrameMap.end())
    //        {
    //            rawFrameMap.erase(it);
    //        }

    //        rawFrameMap[i] = (uint8_t *) frame.data();
    //    }
}
