/* engine 
 * Copyright (C) 2017- Rokid Co., Ltd
 *
 * vsp_input.h: VSP input device component
 *
 */

#ifndef __SF_VSP_INPUT_H__
#define __SF_VSP_INPUT_H__

#include "vsp_ioctl.h"
#include <vector>
#include <cstdint>

//-------------------------------------------------------------------------------------------------

class VspInput 
{
public:
    int pWorkmode;              // String
    int pMcuFirmwarePath;       // String
    int pDspFirmwarePath;       // String
    int pPlay;                  // Trigger
    
    VspInput();
    ~VspInput();
    
    int GetBufferSize() const;
    int GetSampleRate() const;
    virtual bool VspGetData_();
   // virtual bool ParameterUpdating_(int index, Parameter const& param);
    
    /* For Buffers */
    std::vector<int16_t> outChannel_;
    std::vector< std::vector<int16_t> > rawMicChannels_;

    std::vector< std::vector<int16_t> > rawRefChannels_;
    std::vector<float> logFBanksBuffer_;
    
    /* For Output */
    std::vector< std::vector<double> > logFBanksGroups_;

private:
    int vspFd_;
    VSP_IOC_INFO info_;
    VSP_IOC_CONTEXT contextTemplate_;
};

//-------------------------------------------------------------------------------------------------

#endif  // __SF_VSP_TESTER_H__
