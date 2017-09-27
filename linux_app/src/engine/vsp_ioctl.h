/* Engine
 * Copyright (C) 2017- Rokid Co., Ltd
 *
 * vsp_ioctl.h: VSP I/O Control command between user space application and device driver
 *
 */

#ifndef __VSP_IOCTL_H__
#define __VSP_IOCTL_H__

#define VSP_DEVICE_NAME     "vsp"
#define VSP_IRQ_NAME        "vsp"
#define VSP_CLASS_NAME      "gxvsp"

#define VSP_IOC_VERSION     0x20170618

typedef enum {
    VSP_IOC_MODE_IDLE,
    VSP_IOC_MODE_BOOT,
    VSP_IOC_MODE_STANDBY,
    VSP_IOC_MODE_ACTIVE,
    VSP_IOC_MODE_MODEM,
    VSP_IOC_MODE_BYPASS,
    VSP_IOC_MODE_TEST,
} VSP_IOC_MODE_TYPE;

typedef struct {
    void *buffer;
    unsigned int size;
} VSP_IOC_FIRMWARE;

typedef struct {
    /* Version related */
    unsigned int    msg_version;
    unsigned int    ioc_version;
    /* Audio related */
    unsigned int    sample_rate;
    unsigned int    mic_num;
    unsigned int    ref_num;
    unsigned int    frame_length;
    unsigned int    frame_num;              /* per context */
    /* Result related */
    unsigned int    logfbanks_dim;
    unsigned int    logfbanks_group_num;    /* per context */
    unsigned int    farfiled_pattern_dim;
} VSP_IOC_INFO;

typedef struct {
    void           *addr;
    unsigned int    size;
} VSP_IOC_BUFFER;

typedef struct {
    unsigned        mic_mask:16;            /* output */
    unsigned        ref_mask:16;            /* output */
    unsigned int    frame_index;            /* output */
    unsigned int    ctx_index;              /* output */
    unsigned        vad;                    /* output */
    unsigned int    kws;                    /* output */
    unsigned int    direction;              /* 0 - 360 degree */
    VSP_IOC_BUFFER  logfbanks;
    VSP_IOC_BUFFER  farfield_pattern;
    VSP_IOC_BUFFER  out_buffer;             /* only 1 channel */
    VSP_IOC_BUFFER  mic_buffer[8];          /* max 8 channel */
    VSP_IOC_BUFFER  ref_buffer[2];          /* max 2 channel */
} VSP_IOC_CONTEXT;

#define VSP_IOC_SWITCH_MODE     (0x1801)    /* VSP_IOC_MODE_TYPE */
#define VSP_IOC_LOAD_DSP        (0x1802)    /* VSP_IOC_FIRMWARE */
#define VSP_IOC_LOAD_MCU        (0x1803)    /* VSP_IOC_FIRMWARE */
#define VSP_IOC_START_STREAM    (0x1804)
#define VSP_IOC_STOP_STREAM     (0x1805)
#define VSP_IOC_GET_INFO        (0x1806)    /* VSP_IOC_INFO */
#define VSP_IOC_SET_TIMEOUT     (0x1807)    /* integer, millisecond */
#define VSP_IOC_GET_CONTEXT     (0x1808)    /* VSP_IOC_CONTEXT */
#define VSP_IOC_PUT_CONTEXT     (0x1809)    /* VSP_IOC_CONTEXT */

#endif  /* __VSP_IOCTL_H__ */
