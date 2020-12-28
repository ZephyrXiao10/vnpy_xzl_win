//ϵͳ
#ifdef WIN32
#include "pch.h"
#endif

#include "vnoes.h"
#include "pybind11/pybind11.h"
#include    <mds_api/mds_async_api.h>


using namespace pybind11;
//using namespace Quant360;

//����
#define ONCONNECTED 0
#define ONDISCONNECTED 1
#define ONRTNSTOCKDATA 2
#define ONRTNINDEXDATA 3
#define ONRTNOPTIONDATA 4


///-------------------------------------------------------------------------------------
///C++ SPI�Ļص���������ʵ��
///-------------------------------------------------------------------------------------

	/* API���ǰ������ */
class MdsClientApi;


/**
 * ���׽ӿ���Ӧ��
 */




class MdApi
{

private:
	MdApi* api;            //API����
	thread task_thread;                    //�����߳�ָ�루��python���������ݣ�
	TaskQueue task_queue;                //�������
	bool active = false;                //����״̬
	MdsAsyncApiChannelT *channel;

public:
	MdApi()
	{

	};

	~MdApi()
	{
		if (this->active)
		{
			this->exit();
		}
	};

	//-------------------------------------------------------------------------------------
	//API�ص�����
	//-------------------------------------------------------------------------------------



	/* ���ӻ�����������ɺ�Ļص����� */
	int32       OnConnected(MdsAsyncApiChannelT *pAsyncChannel);
	/* ���ӶϿ���Ļص����� */
	int32       OnDisConnected(MdsAsyncApiChannelT *pAsyncChannel) ;

	int32		OnData(MdsApiSessionInfoT *pSessionInfo, SMsgHeadT *pMsgHead, void *pMsgBody, void *pCallbackParams);

	void		OnRtnStockData(const MdsMktDataSnapshotHeadT *head, const MdsStockSnapshotBodyT *stock);
	void		OnRtnIndexData(const MdsMktDataSnapshotHeadT *head, const MdsIndexSnapshotBodyT *index);
	void		OnRtnOptionData(const MdsMktDataSnapshotHeadT *head, const MdsStockSnapshotBodyT *option);
	//-------------------------------------------------------------------------------------
	//task������
	//-------------------------------------------------------------------------------------
	void processTask();

	int32 processConnected(Task *task);

	int32 processDisconnected(Task *task);

	void processRtnStockData(Task *task);
	void processRtnIndexData(Task *task);
	void processRtnOptionData(Task *task);

	//-------------------------------------------------------------------------------------
	//data���ص������������ֵ�
	//error���ص������Ĵ����ֵ�
	//id������id
	//last���Ƿ�Ϊ��󷵻�
	//i������
	//-------------------------------------------------------------------------------------


	virtual void onConnected(int channelType, const dict &data) {};

	virtual void onDisConnected(int channelType, const dict &data) {};

	virtual void onRtnStockData(const dict &error, const dict &data) {};
	virtual void onRtnIndexData(const dict &error, const dict &data) {};
	virtual void onRtnOptionData(const dict &error, const dict &data) {};

	//-------------------------------------------------------------------------------------
	//req:���������������ֵ�
	//-------------------------------------------------------------------------------------

	bool createMdApi(string pCfgFile, string username, string password);

	bool init();

	int exit();
	bool subscribeMarketData(string symbol, int exchange, int product_type);


	private:
		/* ��ֹ�������캯�� */
		MdApi(const MdApi&);
		/* ��ֹ��ֵ���� */
		MdApi&      operator=(const MdApi&);

	public:
		/* Ϊ�˷���ͻ���ʹ�ö����õ���ˮ�ż�����, ���Ի��ڸ��ֶ�������ά���ͻ���ί����ˮ�� */
		int32               defaultClSeqNo;

	protected:
		MdsApiClientCfgT    _apiCfg;
		MdsApiSessionInfoT  _qryChannel;
		BOOL                _isInitialized;
		BOOL                _isRunning;

		//MdSpi *_pSpi;
		MdsAsyncApiContextT *_pAsyncContext;
		MdsAsyncApiChannelT *_pDefaultTcpChannel;
		MdsApiSessionInfoT  *_pQryChannel;
};


MdApi *md_api;

int32 MdOnConnected(MdsAsyncApiChannelT *pAsyncChannel,void *pCallbackParam);
int32 MdOnDisConnected(MdsAsyncApiChannelT *pAsyncChannel, void *pCallbackParam);

int32 MdOnData(MdsApiSessionInfoT *pSessionInfo, SMsgHeadT *pMsgHead, void *pMsgBody, void *pCallbackParams);