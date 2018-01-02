/********************************************************************
*@file		network_feedback.cc
*file path	E:\work\webrtc_HLD_0516\trunk
*Author:	Shelley Liu 
*Data:		20130521	
*@brief		网络反馈
*
*********************************************************************/

#include "network_feedback.h"

/**
* @brief    计算整体网络状况函数
* @req		GVE-SRS-00X 2511
* @param[in]     nInf:网络参数结构体 
* @param[in]     avgLossRateThreshold：平均丢包率阈值
* @param[in]     avgDelayThreshold：平均延时阈值
* @param[in]     ratioDelay：延时调整系数
* @param[in]     meetingPeopleNumber：参会人数
* @returns		int, genBandWidth, 带宽
*/
int NetworkFeedback::genNetworkBandwidth(NetParam * nInf, int avgLossRateThreshold, int avgDelayThreshold, double ratioDelay, int meetingPeopleNumber)
{
	int genBandWidth;
	//@Todo
	return genBandWidth;
}
//genBandWidth = genNetworkBandwidth(nInf, avgLossRateThreshold, avgDelayThreshold, ratioDelay, meetingPeopleNumber);

/**
* @brief    带宽调整量计算函数
* @req		GVE-SRS-00X 2511
* @param[in]     nInf:网络参数结构体 
* @param[in]     genBandWidth：综合网络带宽
* @param[in]     minBitRate：视频最小带宽
* @param[in]     maxBitRate：视频最大带宽   
* @returns   int,  videoBandWidth, 视频可用带宽
*/
int NetworkFeedback::upBandWidth(NetParam * nInf, int genBandWidth, int minBitRate, int maxBitRate);
{
	int videoBandWidth;
	//@Todo
	return videoBandWidth;
}
//videoBandWidth = upBandWidth(nInf, genBandWidth, minBitRate, maxBitRate);


/**
* @brief    网络反馈工作模式选择
* @req		GVE-SRS-00X 2512
* @param[in]     runModel	工作模式
* @param[in]     adjustBandWidth	带宽调整量
*/
int NetworkFeedback::selectMode(int runModel, Packetstatics * sps, int adjustBandWidth)
{
	int adjustType;
	if (runModel == 1) //一般模式
	{
		adjustType = generalMode(sps, adjustBandWidth);
	} 
	else if (runModel == 2) //质量优先
	{
		adjustType = qualityPriorityMode(sps, adjustBandWidth);
	} 
	else //运动优先
	{
		adjustType = motionPriorityMode(sps, adjustBandWidth);
	}
	//@Todo
	return adjustType;
}

/**
* @brief    一般模式（均衡模式）函数
* @req		GVE-SRS-00X 25121
* @param[in]    sps:统计各类包大小的结构体
* @param[in]    adjustBandWidth:需要调整的带宽量    
* @returns   int, 调整参数，每个bit位代表不同的含义，具体参照SRS
*/
int NetworkFeedback::generalMode(Packetstatics * sps, int adjustBandWidth)
{
	int adjustType;
	//@Todo
	return adjustType;
}
//adjustType = generalMode(sps, adjustBandWidth);

/**
* @brief    质量优先模式函数
* @req		GVE-SRS-00X 25122
* @param[in]    sps:统计各类包大小的结构体
* @param[in]    adjustBandWidth:需要调整的带宽量    
* @returns   int, 调整参数，每个bit位代表不同的含义，具体参照SRS
*/
int NetworkFeedback::qualityPriorityMode(Packetstatics * sps, int adjustBandWidth)
{
	int adjustType;
	//@Todo
	return adjustType;
}

/**
* @brief    运动优先模式函数
* @req		GVE-SRS-00X 25123
* @param[in]    sps:统计各类包大小的结构体
* @param[in]    adjustBandWidth:需要调整的带宽量    
* @returns   int, 调整参数，每个bit位代表不同的含义，具体参照SRS
*/
int NetworkFeedback::motionPriorityMode(Packetstatics * sps, int adjustBandWidth)
{
	int adjustType;
	//@Todo
	return adjustType;
}

/**
* @brief    码率调整
* @req		GVE-SRS-00X 2513
* @param[in]     BitRate	码率
* @param[in]     adjustType	调节参数
* @param[in]     adjustBandWidth	带宽调整量   
* @returns   int, 更新后的码率
*/
int NetworkFeedback::AdjustBitRate(int BitRate, int adjustType, int adjustBandWidth)
{
	int New_BitRate;
	//@Todo;
	return New_BitRate;
}

/**
* @brief    lossPacketFilter 丢帧和丢层
* @req		GVE-SRS-00X 2514
* @param[in]     adjustType:调整策略参数
* @param[in] [out]     dataBuffer:RTP数据包的地址	
*/
void NetworkFeedback::lossPacketFilter(int adjustType, unsigned char * dataBuffer)
{
	//@Todo
}
//lossPacketFilter(adjustType, dataBuffer);

/**
* @brief    upThrowLayerFilter 主动丢层（业务非网络反馈）
* @req		GVE-SRS-00X 2515
* @param[in]     layer:需要的层数
* @param[in] [out]     dataBuffer:RTP数据包的地址   
*/
void NetworkFeedback::upThrowLayerFilter(int layer, unsigned char * dataBuffer)
{
	//@Todo
}
//upThrowLayerFilter(layer, dataBuffer);

/**
* @brief    统计各类包大小函数
* @req		GVE-SRS-00X 2516
* @param[in] [out]     sps:统计各类包大小的结构体
* @param[in]    dataBuffer:RTP数据包的地址
* @param[in]    dataSize:RTP数据包的大小 
*/
void NetworkFeedback::statisticsPacketSize(Packetstatics * sps, unsigned char *dataBuffer, int dataSize)
{
	//@Todo
}
//statisticsPacketSize(sps, dataBuffer, dataSize);