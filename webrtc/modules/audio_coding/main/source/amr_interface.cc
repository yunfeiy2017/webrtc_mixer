#include <stdlib.h>
#include <string.h>
#include "amr_interface.h"
#include "webrtc/system_wrappers/interface/trace.h"
#include "webrtc/system_wrappers/interface/tick_util.h"


AMR_encinst_t_::AMR_encinst_t_()
{
        _dtx=0;
	_state = Encoder_Interface_init(_dtx);
}

AMR_encinst_t_::~AMR_encinst_t_()
{
	if (_state!=NULL)
		Encoder_Interface_exit(_state);
}

void AMR_encinst_t_::Init(int16_t dtx_mode)
{
	if(dtx_mode!=_dtx)
	{
		_dtx = dtx_mode;
		Encoder_Interface_exit(_state);
		_state = Encoder_Interface_init(_dtx);
	}	
}

int16_t AMR_encinst_t_::Encode(int16_t mode, int16_t* input, int16_t input_len, unsigned char* output)
{
	int pos = 0;

	output[pos] = 0xf0;//CMR
	pos++;

	unsigned char* tocs = output+pos;
	int toclen = input_len/160;
	if (toclen<1)//invalid packet size
	{
		return 0;
	}
	
	pos += toclen;

	for (int i=0; i<toclen; i++)
	{
		int ret = Encoder_Interface_Encode(_state, (Mode)mode, input+i*160, rawdata, _dtx);
		if (ret <= 0 || ret > 32)//Encoder error
		{
			return 0;
		}
		tocs[i] = rawdata[0] | 0x80;//Not last payload
		memcpy(output+pos,rawdata+1,ret-1);
		pos += ret-1;
	}
	tocs[toclen-1] &= 0x7F; // last payload
	
	return pos;
}


const int sizes[] = { 12, 13, 15, 17, 19, 20, 26, 31, 5, 6, 5, 5, 0, 0, 0, 0 };
//const int bits[] = {95, 103, 118, 134, 148, 159, 204, 244, 39, 43, 38, 37, 0, 0, 0, 0};

#define toc_get_f(toc) ((toc) >> 7)
#define toc_get_ft(toc)	((toc>>3) & 0xf)

static int toc_list_check(unsigned char *tl, size_t buflen) 
{
	int s = 1;
	while (toc_get_f(*tl)) 
	{
		tl++;
		s++;
		if (s > (int)buflen) 
		{
			return -1;
		}
	}
	return s;
}


AMR_decinst_t_::AMR_decinst_t_()
{
	_decstate = Decoder_Interface_init();
}

AMR_decinst_t_::~AMR_decinst_t_()
{
	if (_decstate!=NULL)
		Decoder_Interface_exit(_decstate);
}

int16_t AMR_decinst_t_::Decode(int16_t* input, int16_t input_len, int16_t* output)
{
	unsigned char* pdata = (unsigned char*)input;
	int16_t* pout = output;
	int i = 0;
	int pos = 0;

	if (input_len!=65)//invalid packet
	{
		return -1;
	}
	pos += 1;//skip CMR
	unsigned char* tocs = pdata+pos;
	int toclen = toc_list_check(tocs, input_len);
	if (toclen==-1)//Bad AMR toc list
	{
		return -2;
	}
	pos += toclen;//skip tocs

	for (i=0;i<toclen;i++)
	{
		int ft = toc_get_ft(tocs[i]);
		if (ft>9)//Bad AMR toc
		{
			return -3;
		}
		if (pos+sizes[ft]>input_len)//Bad AMR toc len
		{
			return -4;
		}
		
		//rawdata[0]=(ft<<3)|0x04;
		rawdata[0]=tocs[i];
		memcpy(rawdata+1,pdata+pos,sizes[ft]);

		Decoder_Interface_Decode(_decstate, rawdata, pout, 0);
		pout += 160;
		pos += sizes[ft];			
	}
	
	return (int16_t)(pout-output);
}

enum ACMAMRPackingFormat {
    AMRUndefined = -1,
    AMRBandwidthEfficient = 0,
    AMROctetAlligned = 1,
    AMRFileStorage = 2
};

int16_t WebRtcAmr_CreateEnc(AMR_encinst_t_** enc_inst)
{
	*enc_inst = new AMR_encinst_t_();
	return 0;
}

int16_t WebRtcAmr_EncoderInit(AMR_encinst_t_* enc_inst, int16_t dtx_mode)
{
	if (enc_inst)
		enc_inst->Init(dtx_mode);	
	return 0;
}

int16_t WebRtcAmr_EncodeBitmode(AMR_encinst_t_* enc_inst, int format)
{
	if (AMRBandwidthEfficient == format)
		return 0;
	return 0;
}

int16_t WebRtcAmr_Encode(AMR_encinst_t_* enc_inst, int16_t* input, int16_t len, int16_t* output, int16_t mode)
{
	if (enc_inst)
	{	
		return  enc_inst->Encode(mode, input, len, (unsigned char*)output);
	}
	return 0;
}

int16_t WebRtcAmr_FreeEnc(AMR_encinst_t_* enc_inst)
{
	if (enc_inst)
		delete enc_inst;
	return 0;
}


int16_t WebRtcAmr_CreateDec(AMR_decinst_t_** dec_inst)
{
	*dec_inst = new AMR_decinst_t_();
	return 0;
}

int16_t WebRtcAmr_DecoderInit(AMR_decinst_t_* dec_inst)
{
	return 0;
}

int16_t WebRtcAmr_DecodeBitmode(AMR_decinst_t_* dec_inst, int format)
{
	if (AMRBandwidthEfficient == format)
		return 0;
	return 0;
}

int16_t WebRtcAmr_Decode(AMR_decinst_t_* dec_inst, int16_t* input, int16_t input_len, int16_t* output)
{
	if (dec_inst)
	{
		static WebRtc_Word64 last_time = webrtc::TickTime::MillisecondTimestamp(); 
		WebRtc_Word64 now = webrtc::TickTime::MillisecondTimestamp();

		WebRtc_Word16 dec_len = dec_inst->Decode(input, input_len, output);

		// 2s ´òÓ¡Ò»´Î
		if (now - last_time >= 2000)
		{
			last_time = now;
			webrtc::WEBRTC_TRACE(webrtc::kTraceStateInfo, webrtc::kTraceVoice, 0,
				"AMR decoder info dec_len=%d input_len=%d", dec_len, input_len);
		}

		return dec_len;
	}
		 
	return 0;
}

int16_t WebRtcAmr_DecodePlc(AMR_decinst_t_* dec_inst)
{
	return 0;
}

int16_t WebRtcAmr_FreeDec(AMR_decinst_t_* dec_inst)
{
	if (dec_inst)
		delete dec_inst;
	return 0;
}


