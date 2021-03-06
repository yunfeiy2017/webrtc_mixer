/*
 *  Copyright (c) 2012 The WebRTC project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

#ifndef WEBRTC_MODULES_RTP_RTCP_SOURCE_MOCK_MOCK_RTP_PAYLOAD_REGISTRY_H_
#define WEBRTC_MODULES_RTP_RTCP_SOURCE_MOCK_MOCK_RTP_PAYLOAD_REGISTRY_H_

#include "gmock/gmock.h"
#include "webrtc/modules/rtp_rtcp/source/rtp_payload_registry.h"

namespace webrtc {

class MockRTPPayloadStrategy : public RTPPayloadStrategy {
 public:
  MOCK_CONST_METHOD0(CodecsMustBeUnique,
      bool());
  MOCK_CONST_METHOD4(PayloadIsCompatible,
      bool(const ModuleRTPUtility::Payload& payload,
           const WebRtc_UWord32 frequency,
           const WebRtc_UWord8 channels,
           const WebRtc_UWord32 rate));
  MOCK_CONST_METHOD2(UpdatePayloadRate,
      void(ModuleRTPUtility::Payload* payload, const WebRtc_UWord32 rate));
  MOCK_CONST_METHOD5(CreatePayloadType,
      ModuleRTPUtility::Payload*(
          const char payloadName[RTP_PAYLOAD_NAME_SIZE],
          const WebRtc_Word8 payloadType,
          const WebRtc_UWord32 frequency,
          const WebRtc_UWord8 channels,
          const WebRtc_UWord32 rate));
};

}  // namespace webrtc

#endif  // WEBRTC_MODULES_RTP_RTCP_SOURCE_MOCK_MOCK_RTP_PAYLOAD_REGISTRY_H_
