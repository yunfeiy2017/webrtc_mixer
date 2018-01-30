// 下列 ifdef 块是创建使从 DLL 导出更简单的
// 宏的标准方法。此 DLL 中的所有文件都是用命令行上定义的 AUDIOECHOCANCELLATION_EXPORTS
// 符号编译的。在使用此 DLL 的
// 任何其他项目上不应定义此符号。这样，源文件中包含此文件的任何其他项目都会将
// AUDIOECHOCANCELLATION_API 函数视为是从 DLL 导入的，而此 DLL 则将用此宏定义的
// 符号视为是被导出的。
#ifdef AUDIOECHOCANCELLATION_EXPORTS
#define AUDIOECHOCANCELLATION_API __declspec(dllexport)
#else
#define AUDIOECHOCANCELLATION_API __declspec(dllimport)
#endif

namespace audioechocancellation {
	// 此类是从 AudioEchoCancellation.dll 导出的
	class AUDIOECHOCANCELLATION_API CAudioEchoCancellation {
	public:
		CAudioEchoCancellation(void);
	public:

		static CAudioEchoCancellation* Create(int sampFreq, int scSampFreq);
		static void Destroy(CAudioEchoCancellation *ptr);

		virtual int BufferFarendFrame(const short* nFarend,
			size_t nrOfSamples) = 0;

		virtual int ProcessFrame(const short* nNearend,
			unsigned int num_bands,
			short* nOut,
			unsigned int nrOfSamples,
			short msInSndCardBuf,
			int skew) = 0;
	};
}

extern AUDIOECHOCANCELLATION_API int nAudioEchoCancellation;

AUDIOECHOCANCELLATION_API int fnAudioEchoCancellation(void);
