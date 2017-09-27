/* Engine
 * Copyright (C) 2017- Rokid Co., Ltd
 *
 * vsp_tester.cc: VSP tester device component
 *
 */

#include <string.h>
#include <fcntl.h>
#include <unistd.h>

#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <vector>

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>

#include "vsp_input.h"

#define VSP_DEV_PATH "/dev/" VSP_DEVICE_NAME

//=================================================================================================

VspInput::VspInput()
    : vspFd_(-1)
{
    // Open Device
    vspFd_ = open(VSP_DEV_PATH, O_RDWR);
    if (vspFd_ < 0) {
        printf("Failed to open device\n");
        return;
    }

    // Get Context size
    int result = ioctl(vspFd_, VSP_IOC_GET_INFO, &info_);
    if (result) {
        printf("Failed to get system info!\n");
        return;
    }

    printf("==========================================================\n");
    printf(" Message Version:               %x\n", info_.msg_version);
    printf(" Kernel IOC Version:            %x\n", info_.ioc_version);
    printf(" SenseFlow IOC Version:         %x\n", VSP_IOC_VERSION);
    printf(" Sample Rate:                   %d (Hz)\n", info_.sample_rate);
    printf(" MIC Channel Number:            %d (channels)\n", info_.mic_num);
    printf(" REF Channel Number:            %d (channels)\n", info_.ref_num);
    printf(" Audio Frame Length:            %d (ms)\n", info_.frame_length);
    printf(" Audio Frame Num in Context:    %d (frames)\n", info_.frame_num);
    printf(" LogFBanks Dimension:           %d (dims)\n", info_.logfbanks_dim);
    printf(" LogFBanks Group in Context:    %d (groups)\n", info_.logfbanks_group_num);
    printf(" Far-Field Pattern Dimension:   %d (dims)\n", info_.farfiled_pattern_dim);
    printf("==========================================================\n");

    // Reserve input/output buffers; sample_num = 10(ms) * 3(frame) * 16(KHz) /1000 = 480.
    int sample_num = info_.frame_length * info_.frame_num * info_.sample_rate / 1000;
    outChannel_.resize(sample_num);

    rawMicChannels_.resize(info_.mic_num);
    for (unsigned int i = 0; i < info_.mic_num; i++)
        rawMicChannels_[i].resize(sample_num);

    rawRefChannels_.resize(info_.ref_num);
    for (unsigned int i = 0; i < info_.ref_num; i++)
        rawRefChannels_[i].resize(sample_num);

    logFBanksBuffer_.resize(info_.logfbanks_group_num * info_.logfbanks_dim);

    // Fill Context Template
    memset(&contextTemplate_, 0, sizeof(contextTemplate_));
    contextTemplate_.logfbanks.addr = &logFBanksBuffer_[0];
    contextTemplate_.logfbanks.size = info_.logfbanks_group_num * info_.logfbanks_dim * sizeof(float);
    contextTemplate_.out_buffer.addr = &outChannel_[0];
    contextTemplate_.out_buffer.size = sample_num * sizeof(int16_t);
    for (unsigned int i = 0; i < info_.mic_num; i++) {
        contextTemplate_.mic_buffer[i].addr = &rawMicChannels_[i][0];
        contextTemplate_.mic_buffer[i].size = sample_num * sizeof(int16_t);
    }
    for (unsigned int i = 0; i < info_.ref_num; i++) {
        contextTemplate_.ref_buffer[i].addr = &rawRefChannels_[i][0];
        contextTemplate_.ref_buffer[i].size = sample_num * sizeof(int16_t);
    }

	ioctl(vspFd_, VSP_IOC_SWITCH_MODE, VSP_IOC_MODE_ACTIVE);
}

//-------------------------------------------------------------------------------------------------

VspInput::~VspInput()
{
    if (vspFd_ >= 0)
        close(vspFd_);
}

//=================================================================================================

bool VspInput::VspGetData_()
{
    VSP_IOC_CONTEXT context = contextTemplate_;

    if (vspFd_ < 0)
        return false;

	int result =ioctl(vspFd_, VSP_IOC_GET_CONTEXT, &context);

	//usleep(30*1000);
    if (!result) {
       	std::cerr << "Succeed to get context!" <<std::endl;
		return true;
    } else {
       	std::cerr << "Failed to get context!" << result <<std::endl;
		return false;
    }
}
