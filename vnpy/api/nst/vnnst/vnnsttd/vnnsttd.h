//ϵͳ
#ifdef WIN32
#include "pch.h"
#endif

#include "vnnst.h"
#include "pybind11/pybind11.h"
#include "nst/uft4_traderspi.h"


//����
#define ONFRONTCONNECTED 0
#define ONRSPUSERLOGIN 1
#define ONANSORDERINSERT 2
#define ONRSPORDERINSERT 3
#define ONANSORDERACTION 4
#define ONRSPORDERACTION 5
#define ONORDERRTN 6
#define ONTRADERTN 7
#define ONRSPTRADINGACCOUNT 8
#define ONRSPERROR 9
#define ONRSPQRYORDER 10
#define ONRSPQRYTRADE 11
#define ONRSPQRYINVESTORPOSITION 12
#define ONRSPQRYCHANGEPWD 13
#define ONRSPLOGOUT 14
#define ONRTNINSTRUMENTSTATUS 15
#define ONRSPTEST 16
#define ONERRRTNORDERINSERT 17
#define ONERRRTNORDERACTION 18




///-------------------------------------------------------------------------------------
///C++ SPI�Ļص���������ʵ��
///-------------------------------------------------------------------------------------

//API�ļ̳�ʵ��
class TdApi : public CUft4_TraderSpi
{
private:
	CUft4_TraderApi* api;                     //API����
    thread task_thread;                    //�����߳�ָ�루��python���������ݣ�
    TaskQueue task_queue;                //�������
    bool active = false;                //����״̬

public:
    TdApi()
    {
    };

    ~TdApi()
    {
        if (this->active)
        {
            this->exit();
        }
    };

    //-------------------------------------------------------------------------------------
    //API�ص�����
    //-------------------------------------------------------------------------------------

	virtual void OnFrontConnected();
	virtual void OnRspUserLogin(CUftRspUserLoginField* pRspUserLogin, CUftRspInfoField* pRspInfo);

	//��̨����¼����Ӧ
	virtual void OnAnsOrderInsert(CUftAnsOrderInsertField *pOrderRsp, CUftRspErrorField* pError);

	//����������¼����Ӧ
	virtual void OnRspOrderInsert(CUftRspnOrderInsertField *pOrderRsp, CUftRspErrorField* pError);

	//��̨��������������Ӧ
	virtual void OnAnsOrderAction(CUftAnsOrderActionField *pOrderAction, CUftRspErrorField* pError);

	//��������������������Ӧ
	virtual void OnRspOrderAction(CUftRspnOrderActionField *pOrderAction, CUftRspErrorField* pError);


	//�����ر�
	virtual void OnOrderRtn(CUftRtnnOrderField* pOrder);

	//�ɽ��ر�
	virtual void OnTradeRtn(CUftRtnnTradeField* pOrder);

	//��ѯ�ʽ�ر�
	virtual void OnRspTradingAccount(CUftAnsQueryFundField *pRspFund);

	virtual void OnRspError(CUftRspErrorField* pError);


	///�����ѯ������Ӧ
	virtual void OnRspQryOrder(CUftAnsQueryOrderField *pEntrust, bool bIsLast);

	///�����ѯ�ɽ���Ӧ
	virtual void OnRspQryTrade(CUftAnsQueryTradeField *pTrade, bool bIsLast);

	///�����ѯͶ���ֲ߳���Ӧ
	virtual void OnRspQryInvestorPosition(CUftAnsQueryPositionField *pInvestorPosition, bool bIsLast);


	///�޸�������Ӧ
	virtual void OnRspQryChangePwd();

	virtual void OnRspLogout(const char* szMsg);

	virtual void OnRtnInstrumentStatus(CUftRtnInstrumentStatusField *pInstStatus);

	virtual void OnRspTest(CUftRspTest *pTest);

	///����¼�����ر�
	virtual void OnErrRtnOrderInsert(CUftRspErrorField* pError);

	///������������ر�
	virtual void OnErrRtnOrderAction(CUftRspErrorField* pError);


    //-------------------------------------------------------------------------------------
    //task������
    //-------------------------------------------------------------------------------------
    void processTask();

	void processFrontConnected(Task *task);

	void processRspUserLogin(Task *task);

	void processAnsOrderInsert(Task *task);

	void processRspOrderInsert(Task *task);

	void processAnsOrderAction(Task *task);

	void processRspOrderAction(Task *task);

	void processOrderRtn(Task *task);

	void processTradeRtn(Task *task);

	void processRspTradingAccount(Task *task);

	void processRspError(Task *task);

	void processRspQryOrder(Task *task);

	void processRspQryTrade(Task *task);

	void processRspQryInvestorPosition(Task *task);

	void processRspQryChangePwd(Task *task);

	void processRspLogout(Task *task);

	void processRtnInstrumentStatus(Task *task);

	void processRspTest(Task *task);

	void processErrRtnOrderInsert(Task *task);

	void processErrRtnOrderAction(Task *task);



    //-------------------------------------------------------------------------------------
    //data���ص������������ֵ�
    //error���ص������Ĵ����ֵ�
    //id������id
    //last���Ƿ�Ϊ��󷵻�
    //i������
    //-------------------------------------------------------------------------------------    

	virtual void onFrontConnected() {};

	virtual void onRspUserLogin(const dict &data, const dict &error) {};

	virtual void onAnsOrderInsert(const dict &data, const dict &error) {};

	virtual void onRspOrderInsert(const dict &data, const dict &error) {};

	virtual void onAnsOrderAction(const dict &data, const dict &error) {};

	virtual void onRspOrderAction(const dict &data, const dict &error) {};

	virtual void onOrderRtn(const dict &data) {};

	virtual void onTradeRtn(const dict &data) {};

	virtual void onRspTradingAccount(const dict &data) {};

	virtual void onRspError(const dict &error) {};

	virtual void onRspQryOrder(const dict &data, bool last) {};

	virtual void onRspQryTrade(const dict &data, bool last) {};

	virtual void onRspQryInvestorPosition(const dict &data, bool last) {};

	virtual void onRspQryChangePwd() {};

	virtual void onRspLogout(string data) {};

	virtual void onRtnInstrumentStatus(const dict &data) {};

	virtual void onRspTest(const dict &data) {};

	virtual void onErrRtnOrderInsert(const dict &error) {};

	virtual void onErrRtnOrderAction(const dict &error) {};



    //-------------------------------------------------------------------------------------
    //req:���������������ֵ�
    //-------------------------------------------------------------------------------------

	void newTradeApi(int nCnnMode);

	void tasksetCPU(int nBindingRspCpuId, int nBindingRtnCpuId);

    int init(string szAccount, string szPwd, int eExType);

    int join();

	int exit();

	void registerFront(string pszFrontAddress, uint16_t nPort, string pszLocalAddr, uint16_t nReqPort, uint16_t nRspPort, uint16_t nLocalNotifyPort);

	int reqUserLogin();

	int reqUserLogout();

	int reqOrderInsert(string szCode, char bs, char kp, double dbPrice, int nVolume, int64_t nOrderRef, char cOrderPriceType);

	int reqOrderAction(int nOrderIndex, int nOrderRef);

	int reqTradingAccount();

	int reqQryOrder(string szInstrumentID, int nStartTime, int nEndTime, string szOrderSysID, int nOrderRef, int nOrderIndex);

	int reqQryTrade(string szInstrumentID, int nStartTime, int nEndTime, string szOrderSysID, int nOrderIndex, int nOrderRef);

	int reqQryInvestorPosition(string szInstrumentID);

	int reqChangePwd(string szNewPwd, string szOldPwd);

	int reqQryTest();

	uint64_t getToken();

	int getReqPort();


};
