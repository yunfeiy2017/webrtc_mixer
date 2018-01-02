/*
 *  Copyright (c) 2011 The WebRTC project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

#include "acm_speex.h"
#include "acm_codec_database.h"
#include "acm_common_defs.h"
#include "acm_neteq.h"
#include "trace.h"
#include "webrtc_neteq.h"
#include "webrtc_neteq_help_macros.h"

#ifdef WEBRTC_CODEC_SPEEX
    // NOTE! Speex is not included in the open-source package. The following
    // interface file is needed:
    //
    // /modules/audio_coding/codecs/speex/main/interface/speex_interface.h
    //
    // The API in the header file should match the one below.
    //
    // int16_t WebRtcSpeex_CreateEnc(SPEEX_encinst_t **SPEEXenc_inst,
    //                               int32_t fs);
    // int16_t WebRtcSpeex_FreeEnc(SPEEX_encinst_t *SPEEXenc_inst);
    // int16_t WebRtcSpeex_CreateDec(SPEEX_decinst_t **SPEEXdec_inst,
    //                               int32_t fs,
    //                               int16_t enh_enabled);
    // int16_t WebRtcSpeex_FreeDec(SPEEX_decinst_t *SPEEXdec_inst);
    // int16_t WebRtcSpeex_Encode(SPEEX_encinst_t *SPEEXenc_inst,
    //                            int16_t *speechIn,
    //                            int32_t rate);
    // int16_t WebRtcSpeex_EncoderInit(SPEEX_encinst_t *SPEEXenc_inst,
    //                                 int16_t vbr, int16_t complexity,
    //                                 int16_t vad_enable);
namespace webrtc {


WebRtc_Word16 WebRtcSpeex_Decode(void* state, WebRtc_Word16* encoded, WebRtc_Word16 len,
								 WebRtc_Word16* decoded, WebRtc_Word16* speechType)
{
//	short *sdata =(short *) decoded;
	int index = 0;
	int flen = 0;
	SpeexBits dbits;
	int srate = 0;
	speex_decoder_ctl(state,SPEEX_GET_SAMPLING_RATE,&srate);
	switch(srate)
	{
	case 8000:
		{
			flen = 160 ;
			break;
		}
	case 16000:
		{
			flen = 320  ;
			break;
		}
	case 32000:
		{
			flen = 640 ;
			break;
		}
	default:
		return -1;
	}
	speex_bits_init(&dbits);

	speex_bits_read_from(&dbits,(char *)encoded,len);
	while(speex_bits_remaining(&dbits) > 0 && index<2)
	{
		if (speex_decode_int(state,&dbits,decoded+flen*index) != 0)
		{
			break;
		}
		index++;
	}
	speex_bits_destroy(&dbits);
	return flen*index;

}

WebRtc_Word16 WebRtcSpeex_DecodePlc(void* state, WebRtc_Word16* decodec,
									WebRtc_Word16 noOfLostFrames)
{
	int flen = 0;

	/* typecast pointer to real structure */
	int srate = 0;
	speex_decoder_ctl(state,SPEEX_GET_SAMPLING_RATE,&srate);

	/* Limit number of frames to two = 60 msec. Otherwise we exceed data vectors */
	//if(noOfLostFrames > 2)
	{
		noOfLostFrames = 2;
	}

	/* Get the number of samples per frame */
	switch(srate)
	{
	case 8000:
		{
			flen = 160 ;
			break;
		}
	case 16000:
		{
			flen = 320  ;
			break;
		}
	case 32000:
		{
			flen = 640 ;
			break;
		}
	default:
		return -1;
	}
	for(int i=0;i<noOfLostFrames;i++)
		speex_decode_int(state,NULL,decodec+i*flen); 
	/* Set output samples to zero */
	return flen * noOfLostFrames;
}
int16_t WebRtcSpeex_DecoderInit(void *SPEEXdec_inst)
{
	return 0;
}
    // void WebRtcSpeex_Version(char *versionStr, short len);
    //#include "speex_interface.h"
#endif

//namespace webrtc {

#ifndef WEBRTC_CODEC_SPEEX
ACMSPEEX::ACMSPEEX(WebRtc_Word16  codecID):m_encodecst(NULL),m_decodecst(NULL)
{
	_codecID = codecID;
    return;
}

ACMSPEEX::~ACMSPEEX()
{
    return;
}

WebRtc_Word16
ACMSPEEX::InternalEncode(
    WebRtc_UWord8* /* bitStream        */,
    WebRtc_Word16* /* bitStreamLenByte */)
{
    return -1;
}

WebRtc_Word16
ACMSPEEX::DecodeSafe(
    WebRtc_UWord8* /* bitStream        */,
    WebRtc_Word16  /* bitStreamLenByte */,
    WebRtc_Word16* /* audio            */,
    WebRtc_Word16* /* audioSamples     */,
    WebRtc_Word8*  /* speechType       */)
{
    return -1;
}

WebRtc_Word16
ACMSPEEX::EnableDTX()
{
    return -1;
}

WebRtc_Word16
ACMSPEEX::DisableDTX()
{
    return -1;
}

WebRtc_Word16
ACMSPEEX::InternalInitEncoder(
    WebRtcACMCodecParams*  codecParams )
{
	codecParams->codecInstant
    return -1;
}

WebRtc_Word16
ACMSPEEX::InternalInitDecoder(
    WebRtcACMCodecParams* /* codecParams */)
{
    return -1;
}

WebRtc_Word32
ACMSPEEX::CodecDef(
    WebRtcNetEQ_CodecDef& /* codecDef  */,
    const CodecInst&      /* codecInst */)
{
    return -1;
}

ACMGenericCodec*
ACMSPEEX::CreateInstance(void)
{
    return NULL;
}

WebRtc_Word16
ACMSPEEX::InternalCreateEncoder()
{
    return -1;
}

void
ACMSPEEX::DestructEncoderSafe()
{
    return;
}


WebRtc_Word16
ACMSPEEX::InternalCreateDecoder()
{
    return -1;
}

void
ACMSPEEX::DestructDecoderSafe()
{
    return;
}

WebRtc_Word16
ACMSPEEX::SetBitRateSafe(
    const WebRtc_Word32 /* rate */)
{
    return -1;
}

void
ACMSPEEX::InternalDestructEncoderInst(
    void* /* ptrInst */)
{
    return;
}

WebRtc_Word16
ACMSPEEX::UnregisterFromNetEqSafe(
    ACMNetEQ*     /* netEq       */,
    WebRtc_Word16 /* payloadType */)
{
    return -1;
}

#ifdef UNUSEDSPEEX
WebRtc_Word16
ACMSPEEX::EnableVBR()
{
    return -1;
}

WebRtc_Word16
ACMSPEEX::DisableVBR()
{
    return -1;
}

WebRtc_Word16
ACMSPEEX::SetComplMode(
    WebRtc_Word16 mode)
{
    return -1;
}
#endif

#else     //===================== Actual Implementation =======================

ACMSPEEX::ACMSPEEX(WebRtc_Word16 codecID):
_encoderInstPtr(NULL),
_decoderInstPtr(NULL)
{
    codec_id_ = codecID;

    // Set sampling frequency, frame size and rate Speex
    if(codec_id_ == ACMCodecDB::kSPEEX8)
    {
        _samplingFrequency = 8000;
        _samplesIn20MsAudio = 160;
        _encodingRate = 11000;
    }
    else if(codec_id_ == ACMCodecDB::kSPEEX16)
    {
        _samplingFrequency = 16000;
        _samplesIn20MsAudio = 320;
        _encodingRate = 32000;
    }
else if(codec_id_ == ACMCodecDB::kSPEEX32)
	{
		_samplingFrequency = 32000;
		_samplesIn20MsAudio = 640;
		_encodingRate = 64000;
	}
    else
    {
        WEBRTC_TRACE(webrtc::kTraceError, webrtc::kTraceAudioCoding, unique_id_,
            "Wrong codec id for Speex.");

        _samplingFrequency = -1;
        _samplesIn20MsAudio = -1;
        _encodingRate = -1;
    }
	_encodingRate = 64000;
    has_internal_dtx_ = false;
    dtx_enabled_ = false;
    _vbrEnabled = false;
    _complMode =  3; // default complexity value
	speex_bits_init(&m_encbits);
	speex_bits_init(&m_decbits);
    return;
}

ACMSPEEX::~ACMSPEEX()
{
    if(_encoderInstPtr != NULL)
    {
        speex_encoder_destroy(_encoderInstPtr);
        _encoderInstPtr = NULL;
    }
    if(_decoderInstPtr != NULL)
    {
        speex_decoder_destroy(_decoderInstPtr);
        _decoderInstPtr = NULL;
    }
    return;
}

WebRtc_Word16
ACMSPEEX::InternalEncode(
    WebRtc_UWord8* bitStream,
    WebRtc_Word16* bitStreamLenByte)
{
    WebRtc_Word16 numEncodedSamples = 0;
	int status = 0;
	int n = 0;

    while( numEncodedSamples < frame_len_smpl_)
    {
         status = speex_encode_int(_encoderInstPtr, &in_audio_[in_audio_ix_read_],
            &m_encbits);

        // increment the read index this tell the caller that how far
        // we have gone forward in reading the audio buffer
        in_audio_ix_read_ += _samplesIn20MsAudio;
        numEncodedSamples += _samplesIn20MsAudio;

		if (status < 0) {
			//WEBRTC_TRACE(webrtc::kTraceError, webrtc::kTraceAudioCoding, unique_id_,
			//	"Error in Speex encoder");
			return status;
		}

		// Update VAD, if internal DTX is used
		if (has_internal_dtx_ && dtx_enabled_) {
			vad_label_[n++] = status;
			vad_label_[n++] = status;
		}

		if (status == 0) {
			// This frame is detected as inactive. We need send whatever
			// encoded so far.
			*bitStreamLenByte = speex_bits_write(&m_encbits,(char *)bitStream,*bitStreamLenByte);
			return *bitStreamLenByte;
		}
    }
	//speex_bits_pack(&m_encbits,15,5);
	*bitStreamLenByte = speex_bits_write(&m_encbits,(char *)bitStream,*bitStreamLenByte);
	speex_bits_reset(&m_encbits);

    return *bitStreamLenByte;
}

WebRtc_Word16
ACMSPEEX::DecodeSafe(
    WebRtc_UWord8*  bitStream        ,
    WebRtc_Word16   bitStreamLenByte ,
    WebRtc_Word16*  audio            ,
    WebRtc_Word16*  audioSamples     ,
    WebRtc_Word8*   /*speechType*/       )
{
	if (_decoderInstPtr)
	{
		short *sdata =(short *) audio;
		int index = 0;
		int flen = _samplesIn20MsAudio;
		speex_bits_read_from(&m_decbits,(char *)bitStream,bitStreamLenByte);
		while(speex_bits_remaining(&m_decbits) > 0)
		{
			if (speex_decode_int(_decoderInstPtr,&m_decbits,sdata+flen*index) != 0)
			{
				break;
			}
			index++;
		}

		return flen*index*2;
	}
    return 0;
}

WebRtc_Word16
ACMSPEEX::EnableDTX()
{
	return ACMGenericCodec::EnableDTX();
    if(dtx_enabled_)
    {
        return 0;
    }
    else if(encoder_exist_)  // check if encoder exist
    {
        // enable DTX
		int dtx = 1;
        if(speex_encoder_ctl(_encoderInstPtr,SPEEX_SET_DTX, &dtx) < 0)
        {
            WEBRTC_TRACE(webrtc::kTraceError, webrtc::kTraceAudioCoding, unique_id_,
            "Cannot enable DTX for Speex");
            return -1;
        }
        dtx_enabled_ = true;
        return 0;
    }
    else
    {
        return -1;
    }

    return 0;
}

WebRtc_Word16
ACMSPEEX::DisableDTX()
{
	return ACMGenericCodec::DisableDTX();
    if(!dtx_enabled_)
    {
        return 0;
    }
    else if(encoder_exist_)  // check if encoder exist
    {
        // disable DTX
		int dtx = 0;
        if(speex_encoder_ctl(_encoderInstPtr,SPEEX_SET_DTX,&dtx ) < 0)
        {
            WEBRTC_TRACE(webrtc::kTraceError, webrtc::kTraceAudioCoding, unique_id_,
            "Cannot disable DTX for Speex");
            return -1;
        }
        dtx_enabled_ = false;
        return 0;
    }
    else
    {
        // encoder doesn't exists, therefore disabling is harmless
        return 0;
    }

    return 0;
}

WebRtc_Word16
ACMSPEEX::InternalInitEncoder(
    WebRtcACMCodecParams* codecParams)
{
    // sanity check

    if (_encoderInstPtr == NULL)
    {
		if (_samplingFrequency == 8000)
		{
			_encoderInstPtr = speex_encoder_init(&speex_nb_mode);
		}
		else if (_samplingFrequency == 16000)
		{
			_encoderInstPtr = speex_encoder_init(&speex_wb_mode);
		}
		else if (_samplingFrequency == 32000)
		{
			_encoderInstPtr = speex_encoder_init(&speex_uwb_mode);
		}
		else
		{
			return -1;
		}
		//v = 4;
		//speex_encoder_ctl(_encoderInstPtr,SPEEX_SET_QUALITY,&v);
    }
	if (_encoderInstPtr)
	{
		int v = 0;		
		//speex_encoder_ctl(_encoderInstPtr,SPEEX_SET_VAD,&v);
		//speex_encoder_ctl(_encoderInstPtr,SPEEX_SET_DTX,&v);
		//speex_encoder_ctl(_encoderInstPtr,SPEEX_SET_BITRATE,&_encodingRate);
		//speex_encoder_ctl(_encoderInstPtr,SPEEX_SET_ABR,&_encodingRate);
		speex_encoder_ctl(_encoderInstPtr,SPEEX_SET_VBR,&v);
		//v = 8;
		//speex_encoder_ctl(_encoderInstPtr,SPEEX_SET_QUALITY,&v);
		//v = 64000;
		//speex_encoder_ctl(_encoderInstPtr,SPEEX_SET_VBR_MAX_BITRATE,&v);
	}
	return 0;
}

WebRtc_Word16
ACMSPEEX::InternalInitDecoder(
    WebRtcACMCodecParams* /* codecParams */)
{

	
    // sanity check
    if (_decoderInstPtr == NULL)
    {
		if (_samplingFrequency == 8000)
		{
			_decoderInstPtr = speex_decoder_init(&speex_nb_mode);
		}
		else if (_samplingFrequency == 16000)
		{
			_decoderInstPtr = speex_decoder_init(&speex_wb_mode);
		}
		else if (_samplingFrequency == 32000)
		{
			_decoderInstPtr = speex_decoder_init(&speex_uwb_mode);
		}
		else
		{
			return -1;
		}
		int v=1;
		speex_decoder_ctl(_decoderInstPtr,SPEEX_SET_ENH,&v);

    }
	return 0;
}

WebRtc_Word32
ACMSPEEX::CodecDef(
    WebRtcNetEQ_CodecDef& codecDef,
    const CodecInst&      codecInst)
{
    if (!decoder_initialized_)
    {
        WEBRTC_TRACE(webrtc::kTraceError, webrtc::kTraceAudioCoding, unique_id_,
        "Error, Speex decoder is not initialized");
        return -1;
    }

    // Fill up the structure by calling
    // "SET_CODEC_PAR" & "SET_SPEEX_FUNCTION."
    // Then call NetEQ to add the codec to it's
    // database.

    switch(_samplingFrequency)
    {
    case 8000:
        {
            SET_CODEC_PAR((codecDef), kDecoderSPEEX_8, codecInst.pltype,
                _decoderInstPtr, 8000);
			SET_SPEEX_FUNCTIONS(codecDef);
            break;
        }
    case 16000:
        {
            SET_CODEC_PAR((codecDef), kDecoderSPEEX_16, codecInst.pltype,
                _decoderInstPtr, 16000);
			SET_SPEEX_FUNCTIONS(codecDef);
			break;
        }
	case 32000:
		{
			SET_CODEC_PAR((codecDef), kDecoderSPEEX_32, codecInst.pltype,
				_decoderInstPtr, 32000);
			SET_SPEEX_FUNCTIONS(codecDef);
			break;
		}
    default:
        {
            WEBRTC_TRACE(webrtc::kTraceError, webrtc::kTraceAudioCoding, unique_id_,
            "Unsupported sampling frequency for Speex");

            return -1;
        }
    }

    return 0;
}

ACMGenericCodec*
ACMSPEEX::CreateInstance(void)
{
    return NULL;
}

WebRtc_Word16
ACMSPEEX::InternalCreateEncoder()
{
	if (_encoderInstPtr != NULL)
	{
		speex_encoder_destroy(_encoderInstPtr);
		_encoderInstPtr = NULL;
	}

	if (_encoderInstPtr == NULL)
	{
		if (_samplingFrequency == 8000)
		{
			_encoderInstPtr = speex_encoder_init(&speex_nb_mode);
		}
		else if (_samplingFrequency == 16000)
		{
			_encoderInstPtr = speex_encoder_init(&speex_wb_mode);
		}
		else if (_samplingFrequency == 32000)
		{
			_encoderInstPtr = speex_encoder_init(&speex_uwb_mode);
		}
		else
		{
			return -1;
		}
		return 0;
	}
	return -1;
}

void
ACMSPEEX::DestructEncoderSafe()
{
    if(_encoderInstPtr != NULL)
    {
        speex_encoder_destroy(_encoderInstPtr);
        _encoderInstPtr = NULL;
    }
    // there is no encoder set the following
    encoder_exist_ = false;
    encoder_initialized_ = false;
    _encodingRate = 0;
}


WebRtc_Word16
ACMSPEEX::InternalCreateDecoder()
{
	if (_decoderInstPtr != NULL)
	{
		speex_decoder_destroy(_decoderInstPtr);
		_decoderInstPtr = NULL;
	}

	// sanity check
	if (_decoderInstPtr == NULL)
	{
		if (_samplingFrequency == 8000)
		{
			_decoderInstPtr = speex_decoder_init(&speex_nb_mode);
		}
		else if (_samplingFrequency == 16000)
		{
			_decoderInstPtr = speex_decoder_init(&speex_wb_mode);
		}
		else if (_samplingFrequency == 32000)
		{
			_decoderInstPtr = speex_decoder_init(&speex_uwb_mode);
		}
		else
		{
			return -1;
		}
		return 0;
	}
	return -1;
}

void
ACMSPEEX::DestructDecoderSafe()
{
    if(_decoderInstPtr != NULL)
    {
        speex_decoder_destroy(_decoderInstPtr);
        _decoderInstPtr = NULL;
    }
    // there is no encoder instance set the followings
    decoder_exist_ = false;
    decoder_initialized_ = false;
}

WebRtc_Word16
ACMSPEEX::SetBitRateSafe(
    const WebRtc_Word32 rate)
{
    // Check if changed rate
    if (rate == _encodingRate) {
        return 0;
    } else if (rate > 2000) {
        _encodingRate = rate;
        encoder_params_.codec_inst.rate = rate;
    } else {
        WEBRTC_TRACE(webrtc::kTraceError, webrtc::kTraceAudioCoding, unique_id_,
        "Unsupported encoding rate for Speex");

        return -1;
    }

    return 0;
}


void
ACMSPEEX::InternalDestructEncoderInst(
    void* ptrInst)
{
    if(ptrInst != NULL)
    {
        speex_encoder_destroy(ptrInst);
    }
    return;
}


WebRtc_Word16
ACMSPEEX::UnregisterFromNetEqSafe(
    ACMNetEQ*     netEq,
    WebRtc_Word16 payloadType)
{
    if(payloadType != decoder_params_.codec_inst.pltype)
    {
        WEBRTC_TRACE(webrtc::kTraceError, webrtc::kTraceAudioCoding, unique_id_,
            "Cannot unregister codec %s given payload-type %d does not match \
the stored payload type",
            decoder_params_.codec_inst.plname,
            payloadType,
            decoder_params_.codec_inst.pltype);
        return -1;
    }


    switch(_samplingFrequency)
    {
    case 8000:
        {
            return netEq->RemoveCodec(kDecoderSPEEX_8);
        }
    case 16000:
        {
            return netEq->RemoveCodec(kDecoderSPEEX_16);
        }
	case 32000:
		{
			return netEq->RemoveCodec(kDecoderSPEEX_32);
		}
    default:
        {
            WEBRTC_TRACE(webrtc::kTraceError, webrtc::kTraceAudioCoding, unique_id_,
            "Could not unregister Speex from NetEQ. Sampling frequency doesn't match");
            return -1;
        }
    }
}



// This API is currently not in use. If requested to be able to enable/disable VBR
// an ACM API need to be added.
WebRtc_Word16
ACMSPEEX::EnableVBR()
{
    if(_vbrEnabled)
    {
        return 0;
    }
    else if(encoder_exist_)  // check if encoder exist
    {
        // enable Variable Bit Rate (VBR)
		int v = 1;
		speex_encoder_ctl(_encoderInstPtr,SPEEX_SET_VBR,&v);

        _vbrEnabled = true;
        return 0;
    }
    else
    {
        return -1;
    }
}


// This API is currently not in use. If requested to be able to enable/disable VBR
// an ACM API need to be added.
WebRtc_Word16
ACMSPEEX::DisableVBR()
{
    if(!_vbrEnabled)
    {
        return 0;
    }
    else if(encoder_exist_)  // check if encoder exist
    {
        // disable DTX
		int v = 0;
		speex_encoder_ctl(_encoderInstPtr,SPEEX_SET_VBR,&v);

        _vbrEnabled = false;
        return 0;
    }
    else
    {
        // encoder doesn't exists, therefore disabling is harmless
        return 0;
    }
}
#ifdef UNUSEDSPEEX

// This API is currently not in use. If requested to be able to set complexity
// an ACM API need to be added.
WebRtc_Word16
ACMSPEEX::SetComplMode(
    WebRtc_Word16 mode)
{
    // Check if new mode
    if(mode == _complMode)
    {
        return 0;
    }
    else if(_encoderExist)  // check if encoder exist
    {
        // Set new mode
        if(WebRtcSpeex_EncoderInit(_encoderInstPtr, 0, mode, (_dtxEnabled? 1:0)) < 0)
        {
            WEBRTC_TRACE(webrtc::kTraceError, webrtc::kTraceAudioCoding, _uniqueID,
            "Error in complexity mode for Speex");
            return -1;
        }
        _complMode = mode;
        return 0;
    }
    else
    {
        // encoder doesn't exists, therefore disabling is harmless
        return 0;
    }
}

#endif

#endif

} // namespace webrtc
