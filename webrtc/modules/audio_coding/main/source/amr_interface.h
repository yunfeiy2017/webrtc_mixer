#ifndef WEBRTC_MODULES_AUDIO_CODING_MAIN_SOURCE_AMR_INTERFACE_H_
#define WEBRTC_MODULES_AUDIO_CODING_MAIN_SOURCE_AMR_INTERFACE_H_

#include "typedefs.h"
#include "interf_enc.h"
#include "interf_dec.h"

struct AMR_encinst_t_
{
public:
	void* _state;
    int16_t _dtx;
    unsigned char rawdata[64];

public: 
    AMR_encinst_t_();
    ~AMR_encinst_t_();

	void Init(int16_t dtx_mode);
	int16_t Encode(int16_t mode, int16_t* input, int16_t input_len, unsigned char* output);
};

struct AMR_decinst_t_
{
public: 
	void* _decstate;
	unsigned char rawdata[64];

public: 
    AMR_decinst_t_();
    ~AMR_decinst_t_();

	int16_t Decode(int16_t* input, int16_t input_len, int16_t* output);
};

int16_t WebRtcAmr_CreateEnc(AMR_encinst_t_** enc_inst);
int16_t WebRtcAmr_EncoderInit(AMR_encinst_t_* enc_inst, int16_t dtx_mode);
int16_t WebRtcAmr_EncodeBitmode(AMR_encinst_t_* enc_inst, int format);
int16_t WebRtcAmr_Encode(AMR_encinst_t_* enc_inst, int16_t* input, int16_t len, int16_t* output, int16_t mode);
int16_t WebRtcAmr_FreeEnc(AMR_encinst_t_* enc_inst);

int16_t WebRtcAmr_CreateDec(AMR_decinst_t_** dec_inst);
int16_t WebRtcAmr_DecoderInit(AMR_decinst_t_* dec_inst);
int16_t WebRtcAmr_DecodeBitmode(AMR_decinst_t_* dec_inst, int format);
int16_t WebRtcAmr_Decode(AMR_decinst_t_* dec_inst, int16_t* input, int16_t input_len, int16_t* output);
int16_t WebRtcAmr_DecodePlc(AMR_decinst_t_* dec_inst);
int16_t WebRtcAmr_FreeDec(AMR_decinst_t_* dec_inst);

#endif  // WEBRTC_MODULES_AUDIO_CODING_MAIN_SOURCE_AMR_INTERFACE_H_
