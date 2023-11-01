/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2020 Jerzy Kasenbreg
 * Copyright (c) 2021 Koji KITAYAMA
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 */

#ifndef _USB_DESCRIPTORS_H_
#define _USB_DESCRIPTORS_H_

/* Time stamp base clock. It is a deprecated parameter. */
#define UVC_CLOCK_FREQUENCY    27000000
/* video capture path */
#define UVC_ENTITY_CAP_INPUT_TERMINAL  0x01
#define UVC_ENTITY_CAP_OUTPUT_TERMINAL 0x02

#define FRAME_WIDTH   1536
#define FRAME_HEIGHT  1
#define FRAME_RATE    30

enum {
  ITF_NUM_VIDEO_CONTROL,
  ITF_NUM_VIDEO_STREAMING,
  ITF_NUM_TOTAL
};

#define TUD_VIDEO_CAPTURE_DESC_UNCOMPR_LEN (\
    TUD_VIDEO_DESC_IAD_LEN\
    /* control */\
    + TUD_VIDEO_DESC_STD_VC_LEN\
    + (TUD_VIDEO_DESC_CS_VC_LEN + 1 /* bInCollection */)\
    + TUD_VIDEO_DESC_CAMERA_TERM_LEN\
    + TUD_VIDEO_DESC_OUTPUT_TERM_LEN\
    /* Interface 1, Alternate 0 */\
    + TUD_VIDEO_DESC_STD_VS_LEN\
    + (TUD_VIDEO_DESC_CS_VS_IN_LEN + 1/* bNumFormats x bControlSize */)\
    + TUD_VIDEO_DESC_CS_VS_FMT_UNCOMPR_LEN\
    + TUD_VIDEO_DESC_CS_VS_FRM_UNCOMPR_CONT_LEN\
    + TUD_VIDEO_DESC_CS_VS_COLOR_MATCHING_LEN\
    /* Interface 1, Alternate 1 */\
    + TUD_VIDEO_DESC_STD_VS_LEN\
    + 7/* Endpoint */\
  )

#define TUD_VIDEO_CAPTURE_DESC_UNCOMPR_BULK_LEN (\
    TUD_VIDEO_DESC_IAD_LEN\
    /* control */\
    + TUD_VIDEO_DESC_STD_VC_LEN\
    + (TUD_VIDEO_DESC_CS_VC_LEN + 1/* bInCollection */)\
    + TUD_VIDEO_DESC_CAMERA_TERM_LEN\
    + TUD_VIDEO_DESC_OUTPUT_TERM_LEN\
    /* Interface 1, Alternate 0 */\
    + TUD_VIDEO_DESC_STD_VS_LEN\
    + (TUD_VIDEO_DESC_CS_VS_IN_LEN + 1/* bNumFormats x bControlSize */)\
    + TUD_VIDEO_DESC_CS_VS_FMT_UNCOMPR_LEN\
    + TUD_VIDEO_DESC_CS_VS_FRM_UNCOMPR_CONT_LEN\
    + TUD_VIDEO_DESC_CS_VS_COLOR_MATCHING_LEN\
    + 7/* Endpoint */\
  )

/* Windows support YUY2 and NV12
 * https://docs.microsoft.com/en-us/windows-hardware/drivers/stream/usb-video-class-driver-overview */

#define TUD_VIDEO_DESC_CS_VS_FMT_YUY2(_fmtidx, _numfmtdesc, _frmidx, _asrx, _asry, _interlace, _cp) \
  TUD_VIDEO_DESC_CS_VS_FMT_UNCOMPR(_fmtidx, _numfmtdesc, TUD_VIDEO_GUID_YUY2, 16, _frmidx, _asrx, _asry, _interlace, _cp)
#define TUD_VIDEO_DESC_CS_VS_FMT_NV12(_fmtidx, _numfmtdesc, _frmidx, _asrx, _asry, _interlace, _cp) \
  TUD_VIDEO_DESC_CS_VS_FMT_UNCOMPR(_fmtidx, _numfmtdesc, TUD_VIDEO_GUID_NV12, 12, _frmidx, _asrx, _asry, _interlace, _cp)
#define TUD_VIDEO_DESC_CS_VS_FMT_M420(_fmtidx, _numfmtdesc, _frmidx, _asrx, _asry, _interlace, _cp) \
  TUD_VIDEO_DESC_CS_VS_FMT_UNCOMPR(_fmtidx, _numfmtdesc, TUD_VIDEO_GUID_M420, 12, _frmidx, _asrx, _asry, _interlace, _cp)
#define TUD_VIDEO_DESC_CS_VS_FMT_I420(_fmtidx, _numfmtdesc, _frmidx, _asrx, _asry, _interlace, _cp) \
  TUD_VIDEO_DESC_CS_VS_FMT_UNCOMPR(_fmtidx, _numfmtdesc, TUD_VIDEO_GUID_I420, 12, _frmidx, _asrx, _asry, _interlace, _cp)

#define TUD_VIDEO_CAPTURE_DESCRIPTOR_UNCOMPR(_stridx, _epin, _width, _height, _fps, _epsize) \
    TUD_VIDEO_DESC_IAD( \
        ITF_NUM_VIDEO_CONTROL, \
        0x02 /* 2 Interfaces */, \
        _stridx \
    ), \
    /* Video control 0 */ \
    TUD_VIDEO_DESC_STD_VC( \
        ITF_NUM_VIDEO_CONTROL, \
        0, \
        _stridx \
    ), \
    TUD_VIDEO_DESC_CS_VC( \
        0x0150, /* UVC 1.5 */ \
        TUD_VIDEO_DESC_CAMERA_TERM_LEN + TUD_VIDEO_DESC_OUTPUT_TERM_LEN, /* wTotalLength - bLength */ \
        UVC_CLOCK_FREQUENCY, \
        ITF_NUM_VIDEO_STREAMING \
    ), \
    TUD_VIDEO_DESC_CAMERA_TERM( \
        UVC_ENTITY_CAP_INPUT_TERMINAL, \
        0, \
        0, \
        0, /* wObjectiveFocalLengthMin */ \
        0, /* wObjectiveFocalLengthMax */ \
        0, /* wObjectiveFocalLength */ \
        0 /* bmControls */ \
    ), \
    TUD_VIDEO_DESC_OUTPUT_TERM( \
        UVC_ENTITY_CAP_OUTPUT_TERMINAL, \
        VIDEO_TT_STREAMING, \
        0, \
        1, \
        0 \
    ), \
    /* Video stream alt. 0 */ \
    TUD_VIDEO_DESC_STD_VS( \
        ITF_NUM_VIDEO_STREAMING, \
        0, \
        0, \
        _stridx \
    ), \
    /* Video stream header for without still image capture */ \
    TUD_VIDEO_DESC_CS_VS_INPUT( \
        1, /* bNumFormats */ \
        TUD_VIDEO_DESC_CS_VS_FMT_UNCOMPR_LEN + TUD_VIDEO_DESC_CS_VS_FRM_UNCOMPR_CONT_LEN + TUD_VIDEO_DESC_CS_VS_COLOR_MATCHING_LEN, /* wTotalLength - bLength */ \
        _epin, \
        0, /* bmInfo */ \
        UVC_ENTITY_CAP_OUTPUT_TERMINAL, /* bTerminalLink */ \
        0, /* bStillCaptureMethod */ \
        0, /* bTriggerSupport */ \
        0, /* bTriggerUsage */ \
        0 /* bmaControls(1) */ \
    ), \
    /* Video stream format */ \
    TUD_VIDEO_DESC_CS_VS_FMT_YUY2( \
        1, /* bFormatIndex */ \
        1, /* bNumFrameDescriptors */ \
        1, /* bDefaultFrameIndex */ \
        0, \
        0, \
        0, \
        0 /* bCopyProtect */ \
    ), \
    /* Video stream frame format */ \
    TUD_VIDEO_DESC_CS_VS_FRM_UNCOMPR_CONT( \
        1, /* bFrameIndex */ \
        0, \
        _width, \
        _height, \
        _width * _height * 16, \
        _width * _height * 16 * _fps, \
        _width * _height * 16, \
        (10000000 / _fps), \
        (10000000 / _fps), \
        (10000000 / _fps) * _fps, \
        (10000000 / _fps) \
    ), \
    TUD_VIDEO_DESC_CS_VS_COLOR_MATCHING( \
        VIDEO_COLOR_PRIMARIES_BT709, \
        VIDEO_COLOR_XFER_CH_BT709, \
        VIDEO_COLOR_COEF_SMPTE170M \
    ), \
    /* VS alt 1 */ \
    TUD_VIDEO_DESC_STD_VS( \
        ITF_NUM_VIDEO_STREAMING, \
        1, \
        1, \
        _stridx \
    ), \
    /* EP */ \
    TUD_VIDEO_DESC_EP_ISO( \
        _epin, \
        _epsize, \
        1 \
    )


#define TUD_VIDEO_CAPTURE_DESCRIPTOR_UNCOMPR_BULK(_stridx, _epin, _width, _height, _fps, _epsize) \
    TUD_VIDEO_DESC_IAD( \
        ITF_NUM_VIDEO_CONTROL, \
        0x02, /* 2 Interfaces */ \
        _stridx \
    ), \
    /* Video control 0 */ \
    TUD_VIDEO_DESC_STD_VC( \
        ITF_NUM_VIDEO_CONTROL, \
        0, \
        _stridx \
    ), \
    TUD_VIDEO_DESC_CS_VC( \
        0x0150, /* UVC 1.5 */ \
        TUD_VIDEO_DESC_CAMERA_TERM_LEN + TUD_VIDEO_DESC_OUTPUT_TERM_LEN, /* wTotalLength - bLength */ \
        UVC_CLOCK_FREQUENCY, \
        ITF_NUM_VIDEO_STREAMING \
    ), \
    TUD_VIDEO_DESC_CAMERA_TERM( \
        UVC_ENTITY_CAP_INPUT_TERMINAL, \
        0, \
        0, \
        0, /* wObjectiveFocalLengthMin */ \
        0, /* wObjectiveFocalLengthMax */ \
        0, /* wObjectiveFocalLength */ \
        0  /* bmControls */ \
    ), \
    TUD_VIDEO_DESC_OUTPUT_TERM( \
        UVC_ENTITY_CAP_OUTPUT_TERMINAL, \
        VIDEO_TT_STREAMING, \
        0, \
        1, \
        0 \
    ), \
    /* Video stream alt. 0 */ \
    TUD_VIDEO_DESC_STD_VS( \
        ITF_NUM_VIDEO_STREAMING, \
        0, \
        1, \
        _stridx \
    ), \
    /* Video stream header for without still image capture */ \
    TUD_VIDEO_DESC_CS_VS_INPUT( \
        1, /* bNumFormats */ \
        TUD_VIDEO_DESC_CS_VS_FMT_UNCOMPR_LEN + TUD_VIDEO_DESC_CS_VS_FRM_UNCOMPR_CONT_LEN + TUD_VIDEO_DESC_CS_VS_COLOR_MATCHING_LEN, /* wTotalLength - bLength */ \
        _epin, \
        0, /* bmInfo */ \
        UVC_ENTITY_CAP_OUTPUT_TERMINAL, /* bTerminalLink */ \
        0, /* bStillCaptureMethod */ \
        0, /* bTriggerSupport */ \
        0, /* bTriggerUsage */ \
        0 /* bmaControls(1) */ \
    ), \
    /* Video stream format */ \
    TUD_VIDEO_DESC_CS_VS_FMT_YUY2( \
        1, /* bFormatIndex */ \
        1, /* bNumFrameDescriptors */ \
        1, /* bDefaultFrameIndex */ \
        0, \
        0, \
        0, \
        0 /* bCopyProtect */ \
    ), \
    /* Video stream frame format */ \
    TUD_VIDEO_DESC_CS_VS_FRM_UNCOMPR_CONT( \
        1, /* bFrameIndex */ \
        0, \
        _width, \
        _height, \
        _width * _height * 16, \
        _width * _height * 16 * _fps, \
        _width * _height * 16, \
        (10000000 / _fps), \
        (10000000 / _fps), \
        (10000000 / _fps) * _fps, \
        (10000000 / _fps) \
    ), \
    TUD_VIDEO_DESC_CS_VS_COLOR_MATCHING( \
        VIDEO_COLOR_PRIMARIES_BT709, \
        VIDEO_COLOR_XFER_CH_BT709, \
        VIDEO_COLOR_COEF_SMPTE170M \
    ), \
    TUD_VIDEO_DESC_EP_BULK( \
        _epin, \
        _epsize, \
        1 \
    )

#endif