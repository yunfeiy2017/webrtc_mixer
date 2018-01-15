# webrtc_mixer
An audio mixer based on webrtc
调用Create创建Mixer引擎，start开始，stop结束
通过MixerEngineCallback回调混音后的pcm数据
引擎接收rtp包数据，端口默认从6666开始的偶数。
