/********************************************************************
*@file		network_feedback.h
*file path	E:\work\webrtc_HLD_0516\trunk
*Author:	Shelley Liu 
*Data:		20130521
*@brief		网络反馈相关变量函数定义
*
*********************************************************************/

#ifndef NETWORK_FEEDBACK_H_
#define NETWORK_FEEDBACK_H_

/** 
* @defgroup	NetworkFeedback
*
* @brief	网络反馈相关函数
*
*/

/** 
* @ingroup NetworkFeedback
*@brief		网络参数结构体
*/
typedef struct 
{
///	@Todo
}NetParam;

/** 
* @ingroup NetworkFeedback
*@brief		统计各类包大小的结构体
*/
typedef struct 
{
	int I_Packets_Counts;
	int P_Packets_Counts;
	int B_Packets_Counts;
///	@Todo
}Packetstatics;

/** 
* @ingroup NetworkFeedback
* @brief	上行网络丢包，使用的函数和参数
*/
class NetworkFeedback
{
public:
	NetworkFeedback();

	virtual ~NetworkFeedback();

/**
* @ingroup NetworkFeedback
* @brief    计算整体网络状况函数
* @req		GVE-SRS-00X 2511
* @param[in]     nInf:网络参数结构体 
* @param[in]     avgLossRateThreshold：平均丢包率阈值
* @param[in]     avgDelayThreshold：平均延时阈值
* @param[in]     ratioDelay：延时调整系数
* @param[in]     meetingPeopleNumber：参会人数
* @returns		int, genBandWidth, 带宽
*/
	int genNetworkBandwidth(NetParam * nInf, int avgLossRateThreshold, int avgDelayThreshold, double ratioDelay, int meetingPeopleNumber);
	//genBandWidth = genNetworkBandwidth(nInf, avgLossRateThreshold, avgDelayThreshold, ratioDelay, meetingPeopleNumber);

/**
* @ingroup NetworkFeedback
* @brief    带宽调整量计算函数
* @req		GVE-SRS-00X 2511
* @param[in]     nInf:网络参数结构体 
* @param[in]     genBandWidth：综合网络带宽
* @param[in]     minBitRate：视频最小带宽
* @param[in]     maxBitRate：视频最大带宽   
* @returns   int,  videoBandWidth, 视频可用带宽
*/
	int upBandWidth(NetParam * nInf, int genBandWidth, int minBitRate, int maxBitRate);
	//videoBandWidth = upBandWidth(nInf, genBandWidth, minBitRate, maxBitRate);

/**
* @ingroup NetworkFeedback
* @brief    网络反馈工作模式选择
* @req		GVE-SRS-00X 2512
* @param[in]     runModel	工作模式
* @param[in]     adjustBandWidth	带宽调整量
*/
	int selectMode(int runModel, Packetstatics * sps, int adjustBandWidth);

/**
* @ingroup NetworkFeedback
* @brief    一般模式（均衡模式）函数
* @req		GVE-SRS-00X 25121
* @param[in]    sps:统计各类包大小的结构体
* @param[in]    adjustBandWidth:需要调整的带宽量    
* @returns   int, 调整参数，每个bit位代表不同的含义，具体参照SRS
*/
	int generalMode(Packetstatics * sps, int adjustBandWidth);
	//adjustType = generalMode(sps, adjustBandWidth);

/**
* @ingroup NetworkFeedback
* @brief    质量优先模式函数
* @req		GVE-SRS-00X 25122
* @param[in]    sps:统计各类包大小的结构体
* @param[in]    adjustBandWidth:需要调整的带宽量    
* @returns   int, 调整参数，每个bit位代表不同的含义，具体参照SRS
*/
	int qualityPriorityMode(Packetstatics * sps, int adjustBandWidth);

/**
* @ingroup NetworkFeedback
* @brief    运动优先模式函数
* @req		GVE-SRS-00X 25123
* @param[in]    sps:统计各类包大小的结构体
* @param[in]    adjustBandWidth:需要调整的带宽量    
* @returns   int, 调整参数，每个bit位代表不同的含义，具体参照SRS
*/
	int motionPriorityMode(Packetstatics * sps, int adjustBandWidth);

/**
* @ingroup NetworkFeedback
* @brief    码率调整
* @req		GVE-SRS-00X 2513
* @param[in]     BitRate	码率
* @param[in]     adjustType	调节参数
* @param[in]     adjustBandWidth	带宽调整量   
* @returns   int, 更新后的码率
*/
	int AdjustBitRate(int BitRate, int adjustType, int adjustBandWidth);

/**
* @ingroup NetworkFeedback
* @brief    lossPacketFilter 丢帧和丢层
* @req		GVE-SRS-00X 2514
* @param[in]     adjustType:调整策略参数
* @param[in] [out]     dataBuffer:RTP数据包的地址	
*/
	void lossPacketFilter(int adjustType, unsigned char * dataBuffer);
	//lossPacketFilter(adjustType, dataBuffer);

/**
* @ingroup NetworkFeedback
* @brief    upThrowLayerFilter 主动丢层（业务非网络反馈）
* @req		GVE-SRS-00X 2515
* @param[in]     layer:需要的层数
* @param[in] [out]     dataBuffer:RTP数据包的地址   
*/
	void upThrowLayerFilter(int layer, unsigned char * dataBuffer);
	//upThrowLayerFilter(layer, dataBuffer);

/**
* @ingroup NetworkFeedback
* @brief    统计各类包大小函数
* @req		GVE-SRS-00X 2516
* @param[in] [out]     sps:统计各类包大小的结构体
* @param[in]    dataBuffer:RTP数据包的地址
* @param[in]    dataSize:RTP数据包的大小 
*/
	void statisticsPacketSize(Packetstatics * sps, unsigned char *dataBuffer, int dataSize);
	//statisticsPacketSize(sps, dataBuffer, dataSize);
		 
protected:
private:
	NetParam * nInf;
	Packetstatics * sps;

	///@Todo
};


#endif