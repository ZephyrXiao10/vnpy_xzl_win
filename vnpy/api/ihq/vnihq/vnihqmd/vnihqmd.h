//ϵͳ
#ifdef WIN32
#include "pch.h"
#endif




#include "vnihq.h"
#include "pybind11/pybind11.h"
#include "ihq/IStoneMdApi.h"

using namespace pybind11;
using namespace istone::mdapi;

//����
#define ONFRONTCONNECTED 0
#define ONFRONTDISCONNECTED 1
#define ONHEARTBEATWARNING 2
#define ONRSPERROR 3
#define ONRSPUSERLOGIN 4
#define ONRSPUSERLOGOUT 5
#define ONRSPSUBALLMARKETDATA 6
#define ONRSPUNSUBALLMARKETDATA 7
#define ONRSPSUBALLTICKBYTICK 8
#define ONRSPUNSUBALLTICKBYTICK 9
#define ONRSPSUBMARKETDATA 10
#define ONRSPUNSUBMARKETDATA 11
#define ONRSPSUBTICKBYTICK 12
#define ONRSPUNSUBTICKBYTICK 13
#define ONRTNDEPTHMARKETDATA 14
#define ONRTNTICKBYTICK 15
#define ONRTNORDERBOOK 16
#define ONRSPSUBORDERBOOK 17
#define ONRSPUNSUBORDERBOOK 18



///-------------------------------------------------------------------------------------
///C++ SPI�Ļص���������ʵ��
///-------------------------------------------------------------------------------------

//API�ļ̳�ʵ��
class MdApi : public IStoneMdSpi
{
private:
	IStoneMdApi* api;				//API����
	thread task_thread;					//�����߳�ָ�루��python���������ݣ�
	TaskQueue task_queue;			    //�������
	bool active = false;				//����״̬

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
	virtual void OnFrontConnected() ;

	///���ͻ����뽻�׺�̨ͨ�����ӶϿ�ʱ���÷��������á���������������API���Զ��������ӣ��ͻ��˿ɲ�������
	///@param nReason ����ԭ��
	///        0x1001 �����ʧ��
	///        0x1002 ����дʧ��
	///        0x2001 ����������ʱ
	///        0x2002 ��������ʧ��
	///        0x2003 �յ�������
	virtual void OnFrontDisconnected(int nReason) ;

	///������ʱ���档����ʱ��δ�յ�����ʱ���÷��������á�
	///@param nTimeLapse �����ϴν��ձ��ĵ�ʱ��
	virtual void OnHeartBeatWarning(int nTimeLapse) ;

	///����Ӧ��
	virtual void OnRspError(CIStoneRspInfo* pRspInfo, int nRequestID, bool bIsLast) ;

	///�û���¼Ӧ��
	///@param pRspUserLogout	��¼�ɹ�ʱ�����ظ���Ϣ
	///@param pRspInfo			��¼�ɹ�ʱ��pRspInfoΪ�գ���¼ʧ��ʱ��pRspInfo���ش����������Ϣ
	virtual void OnRspUserLogin(CIStoneRspUserLogin* pRspUserLogin, CIStoneRspInfo* pRspInfo, int nRequestID, bool bIsLast) ;

	///�û��˳�Ӧ��
	///@param pRspUserLogout	�˳��ɹ�ʱ�����ظ���Ϣ
	///@param pRspInfo			�˳��ɹ�ʱ��pRspInfoΪ�գ��˳�ʧ��ʱ��pRspInfo���ش����������Ϣ
	virtual void OnRspUserLogout(CIStoneRspUserLogout* pRspUserLogout, CIStoneRspInfo* pRspInfo, int nRequestID, bool bIsLast) ;

	///�û��������ȫ����Ӧ��
	///@param tickType						��Ʊ����, �����Ͷ��Ŀ�ʹ�� ISTONE_TICKER_TYPE_SPOT | ISTONE_TICKER_TYPE_INDEX ��ʽ���ġ�
	///        ISTONE_TICKER_TYPE_SPOT		<��ͨ��Ʊ<ծȯ<����
	///        ISTONE_TICKER_TYPE_INDEX		<ָ��
	///        ISTONE_TICKER_TYPE_OPTION	<��Ȩ
	///@param exchangeType					���������ͣ� �ཻ������ʹ�� ISTONE_EXCHANGE_SH | ISTONE_EXCHANGE_SZ ��ʽ���ġ�
	///        ISTONE_EXCHANGE_SH			<��֤
	///        ISTONE_EXCHANGE_SZ			<��֤
	virtual void OnRspSubAllMarketData(int tickType, int exchangeID, int nRequestID, bool isSuccess) ;

	///�û��������ȫ�˶�Ӧ��
	///@param tickType						��Ʊ����, �����Ͷ��Ŀ�ʹ�� ISTONE_TICKER_TYPE_SPOT | ISTONE_TICKER_TYPE_INDEX ��ʽ���ġ�
	///        ISTONE_TICKER_TYPE_SPOT		<��ͨ��Ʊ<ծȯ<����
	///        ISTONE_TICKER_TYPE_INDEX		<ָ��
	///        ISTONE_TICKER_TYPE_OPTION	<��Ȩ
	///@param exchangeType					���������ͣ� �ཻ������ʹ�� ISTONE_EXCHANGE_SH | ISTONE_EXCHANGE_SZ ��ʽ���ġ�
	///        ISTONE_EXCHANGE_SH			<��֤
	///        ISTONE_EXCHANGE_SZ			<��֤
	virtual void OnRspUnSubAllMarketData(int tickType, int exchangeID, int nRequestID, bool isSuccess) ;

	///�û��������ȫ����Ӧ��
	///@param tickType						��Ʊ����, �����Ͷ��Ŀ�ʹ�� ISTONE_TICKER_TYPE_SPOT | ISTONE_TICKER_TYPE_INDEX ��ʽ���ġ�
	///        ISTONE_TICKER_TYPE_SPOT		<��ͨ��Ʊ<ծȯ<����
	///        ISTONE_TICKER_TYPE_INDEX		<ָ��
	///        ISTONE_TICKER_TYPE_OPTION	<��Ȩ
	///@param exchangeType					���������ͣ� �ཻ������ʹ�� ISTONE_EXCHANGE_SH | ISTONE_EXCHANGE_SZ ��ʽ���ġ�
	///        ISTONE_EXCHANGE_SH			<��֤
	///        ISTONE_EXCHANGE_SZ			<��֤
	virtual void OnRspSubAllTickByTick(int exchangeID, int nRequestID, bool isSuccess) ;

	///�û��������ȫ�˶�Ӧ��
	///@param tickType						��Ʊ����, �����Ͷ��Ŀ�ʹ�� ISTONE_TICKER_TYPE_SPOT | ISTONE_TICKER_TYPE_INDEX ��ʽ���ġ�
	///        ISTONE_TICKER_TYPE_SPOT		<��ͨ��Ʊ<ծȯ<����
	///        ISTONE_TICKER_TYPE_INDEX		<ָ��
	///        ISTONE_TICKER_TYPE_OPTION	<��Ȩ
	///@param exchangeType					���������ͣ� �ཻ������ʹ�� ISTONE_EXCHANGE_SH | ISTONE_EXCHANGE_SZ ��ʽ���ġ�
	///        ISTONE_EXCHANGE_SH			<��֤
	///        ISTONE_EXCHANGE_SZ			<��֤
	virtual void OnRspUnSubAllTickByTick(int exchangeID, int nRequestID, bool isSuccess) ;

	///�û�������鶩��Ӧ��
	///@param ppInstrumentID	��Ʊ����
	///@param nCount			��Ʊ����
	virtual void OnRspSubMarketData(char* tickers[], int count, int nRequestID, bool bIsSuccess) ;

	///�û���������˶�Ӧ��
	///@param ppInstrumentID	��Ʊ����
	///@param nCount			��Ʊ����
	virtual void OnRspUnSubMarketData(char* tickers[], int count, int nRequestID, bool bIsSuccess) ;

	///�û�������鶩��Ӧ��
	///@param ppInstrumentID	��Ʊ����
	///@param nCount			��Ʊ����
	virtual void OnRspSubTickByTick(char* tickers[], int count, int nRequestID, bool bIsSuccess) ;

	///�û���������˶�Ӧ��
	///@param ppInstrumentID	��Ʊ����
	///@param nCount			��Ʊ����
	virtual void OnRspUnSubTickByTick(char* tickers[], int count, int nRequestID, bool bIsSuccess) ;

	///�������֪ͨ
	virtual void OnRtnDepthMarketData(CIStoneDepthMarketData* data) ;

	///�������֪ͨ
	virtual void OnRtnTickByTick(CIStoneTickByTick* data) ;

	///������֪ͨ
	virtual void OnRtnOrderBook(CIStoneOrderBook* data) ;

	///�û�����������Ӧ��
	///@param ppInstrumentID	��Ʊ����
	///@param nCount			��Ʊ����
	virtual void OnRspSubOrderBook(char* tickers[], int count, int nRequestID, bool bIsSuccess) ;

	///�û��������˶�Ӧ��
	///@param ppInstrumentID	��Ʊ����
	///@param nCount			��Ʊ����
	virtual void OnRspUnSubOrderBook(char* tickers[], int count, int nRequestID, bool bIsSuccess) ;

	//-------------------------------------------------------------------------------------
	//task������
	//-------------------------------------------------------------------------------------

	void processTask();

	void processFrontConnected(Task *task);

	void processFrontDisconnected(Task *task);

	void processHeartBeatWarning(Task *task);

	void processRspError(Task *task);

	void processRspUserLogin(Task *task);

	void processRspUserLogout(Task *task);

	void processRspSubAllMarketData(Task *task);

	void processRspUnSubAllMarketData(Task *task);

	void processRspSubAllTickByTick(Task *task);

	void processRspUnSubAllTickByTick(Task *task);

	void processRspSubMarketData(Task *task);

	void processRspUnSubMarketData(Task *task);

	void processRspSubTickByTick(Task *task);

	void processRspUnSubTickByTick(Task *task);

	void processRtnDepthMarketData(Task *task);

	void processRtnTickByTick(Task *task);

	void processRtnOrderBook(Task *task);

	void processRspSubOrderBook(Task *task);

	void processRspUnSubOrderBook(Task *task);


	//-------------------------------------------------------------------------------------
	//data���ص������������ֵ�
	//error���ص������Ĵ����ֵ�
	//id������id
	//last���Ƿ�Ϊ��󷵻�
	//i������
	//-------------------------------------------------------------------------------------

	virtual void onFrontConnected() {};

	virtual void onFrontDisconnected(int nReason) {};

	virtual void onHeartBeatWarning(int nTimeLapse) {};

	virtual void onRspError(const dict &error, int nRequestID, bool bIsLast) {};

	virtual void onRspUserLogin(const dict &data, const dict &error, int nRequestID, bool bIsLast) {};

	virtual void onRspUserLogout(const dict &data, const dict &error, int nRequestID, bool bIsLast) {};

	virtual void onRspSubAllMarketData(int tickType, int exchangeID, int nRequestID, bool isSuccess) {};

	virtual void onRspUnSubAllMarketData(int tickType, int exchangeID, int nRequestID, bool isSuccess) {};

	virtual void onRspSubAllTickByTick(int exchangeID, int nRequestID, bool isSuccess) {};

	virtual void onRspUnSubAllTickByTick(int exchangeID, int nRequestID, bool isSuccess) {};

	virtual void onRspSubMarketData(string tickers, int count, int nRequestID, bool bIsSuccess) {};

	virtual void onRspUnSubMarketData(string tickers, int count, int nRequestID, bool bIsSuccess) {};

	virtual void onRspSubTickByTick(string tickers, int count, int nRequestID, bool bIsSuccess) {};

	virtual void onRspUnSubTickByTick(string tickers, int count, int nRequestID, bool bIsSuccess) {};

	virtual void onRtnDepthMarketData(const dict &data) {};

	virtual void onRtnTickByTick(const dict &data) {};

	virtual void onRtnOrderBook(const dict &data) {};

	virtual void onRspSubOrderBook(string tickers, int count, int nRequestID, bool bIsSuccess) {};

	virtual void onRspUnSubOrderBook(string tickers, int count, int nRequestID, bool bIsSuccess) {};



	//-------------------------------------------------------------------------------------
	//req:���������������ֵ�
	//-------------------------------------------------------------------------------------

	void createMdUserApi(string pszFlowPath = "");

	void release();

	void init();

	int join();

	int exit();

	string getTradingDay();

	void registerFront(string pszFrontAddress);

	void registerFpgaFront(string pMutilcastAddr);

	void registerTcpLocalAddr(string localTcpAddr);

	void registerUdpLocalAddr(string localAddr);

	void registerEfviDevice(string device);

	void setHeartbeatTimeout(uint32_t timeout);

	int reqUserLogin(const dict &req, int reqid);

	int reqUserLogout(const dict &req, int reqid);


	int subAllMarketData(int tickType, int exchangeID, int nRequestID);

	int unSubAllMarketData(int tickType, int exchangeID, int nRequestID);

	int subAllTickByTick(int exchangeID, int nRequestID);

	int unSubAllTickByTick(int exchangeID, int nRequestID);

	int subMarketData(string ppInstrumentID,int nRequestID);

	int unSubMarketData(string ppInstrumentID, int nRequestID);

	int subTickByTick(string ppInstrumentID, int nRequestID);

	int unSubTickByTick(string ppInstrumentID,int nRequestID);

	int subAllOrderBook(int exchangeID, int nRequestID);

	int unSubAllOrderBook(int exchangeID, int nRequestID);

	int subOrderBook(string ppInstrumentID, int nRequestID);

	int unSubOrderBook(string ppInstrumentID, int nRequestID);
};
