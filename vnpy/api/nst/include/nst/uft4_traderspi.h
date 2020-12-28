#ifndef _UFT4_TRADESPI_H
#define _UFT4_TRADESPI_H

#include "uft4_UserApiStruct.h"
#include "uft4_UserApiDataType.h"


#ifdef _WIN32
#if !defined( FUNCTION_CALL_MODE )
#define FUNCTION_CALL_MODE		__stdcall
#endif
#else
#define FUNCTION_CALL_MODE
#endif

#if (defined WIN32) || (defined WIN64)  
#define TRADE_API_EXPORT _declspec(dllexport)
#else
#ifdef HSTRADEAPI_EXPORTS
#define TRADE_API_EXPORT __attribute__((visibility("default"))) 
#else
#define TRADE_API_EXPORT
#endif
#endif

class CUft4_TraderSpi
{
public:
	virtual void OnFrontConnected(){}
	virtual void OnRspUserLogin(CUftRspUserLoginField* pRspUserLogin, CUftRspInfoField* pRspInfo ){}

	//��̨����¼����Ӧ
	virtual void OnAnsOrderInsert(CUftAnsOrderInsertField *pOrderRsp, CUftRspErrorField* pError){}

	//����������¼����Ӧ
	virtual void OnRspOrderInsert(CUftRspnOrderInsertField *pOrderRsp, CUftRspErrorField* pError){}

	//��̨��������������Ӧ
	virtual void OnAnsOrderAction(CUftAnsOrderActionField *pOrderAction, CUftRspErrorField* pError){}

	//��������������������Ӧ
	virtual void OnRspOrderAction(CUftRspnOrderActionField *pOrderAction, CUftRspErrorField* pError){}


	//�����ر�
	virtual void OnOrderRtn(CUftRtnnOrderField* pOrder){}

	//�ɽ��ر�
	virtual void OnTradeRtn(CUftRtnnTradeField* pOrder){}

	//��ѯ�ʽ�ر�
	virtual void OnRspTradingAccount(CUftAnsQueryFundField *pRspFund){}

	virtual void OnRspError(CUftRspErrorField* pError){}


	///�����ѯ������Ӧ
	virtual void OnRspQryOrder(CUftAnsQueryOrderField *pEntrust, bool bIsLast) {};

	///�����ѯ�ɽ���Ӧ
	virtual void OnRspQryTrade(CUftAnsQueryTradeField *pTrade, bool bIsLast) {};

	///�����ѯͶ���ֲ߳���Ӧ
	virtual void OnRspQryInvestorPosition(CUftAnsQueryPositionField *pInvestorPosition, bool bIsLast) {};



	///�޸�������Ӧ
	virtual void OnRspQryChangePwd() {};

	virtual void OnRspLogout(const char* szMsg) {};

	virtual void OnRtnInstrumentStatus(CUftRtnInstrumentStatusField *pInstStatus){};

	virtual void OnRspTest(CUftRspTest *pTest){};

	///����¼�����ر�
	virtual void OnErrRtnOrderInsert(CUftRspErrorField* pError) {};

	///������������ر�
	virtual void OnErrRtnOrderAction(CUftRspErrorField* pError) {};

};

class CUft4_TraderApi
{
public:
#ifndef OS_WIN
	//nCnnMode������ͨ�ŷ�ʽ: 0, udp��ʽ; 1, tcp��ʽ
	static CUft4_TraderApi *CreateUft4Shfe_TraderApi(int nCnnMode = 0);

	static const char* GetApiVersion();
#endif
	///ɾ���ӿڶ�����
	///@remark ����ʹ�ñ��ӿڶ���ʱ,���øú���ɾ���ӿڶ���
	virtual void Release() = 0;

	///API��˲���,	֧�ֽ��ջر��ͽ��������̰߳�CPU
	///�˺���������Init֮ǰ����
	///window�治֧��
	virtual void TasksetCPU(int nBindingRspCpuId, int nBindingRtnCpuId) = 0;

	///��ʼ��
	///@remark ��ʼ�����л���,ֻ�е��ú�,�ӿڲſ�ʼ����
	/*
	*	@param szAccount��֤ȯ�˺�
	*	@param szPwd������
	*	@param nReConnTimeout����ʱ����ʱ��
	*	@param nTimeStampFlag��ʱ���
	*	@param szAccount���������΢��
	*	@param eExType���г�
		����ֵ
		  0, OK
		 -1,sockͨѶ����
		 -2,�˺Ÿ�ʽ����
		 -3,�ͻ��˽���ģʽ�����˲�ƥ�����
		 -4,����˷��ش���
	*/
	virtual int Init(const char* szAccount, const char* szPwd, int nReConnTimeout = 0, int nTimeStampFlag = 0, int nHeartBtFreq=100,E_EXCHANGE_TYPE eExType = EXCHANGE_CFFEX, int nNoBlock = 0) = 0;

	///�ȴ��ӿ��߳̽�������
	///@return �߳��˳�����
	virtual int Join() = 0;

	/*
	*	@param 	pszFrontAddress ��������ַ
	*	@param 	nPort �������˿�
	*	@param 	pszLocalAddr ������ַ
	*	@param 	nReqPort ����˿�
	*	@param 	nRspPort Ӧ��˿�
	*	@param 	nLocalNotifyPort ���ƶ˿�
	*/
	virtual void RegisterFront(const char *pszFrontAddress, uint16_t nPort, const char* pszLocalAddr, uint16_t nReqPort, uint16_t nRspPort, uint16_t nLocalNotifyPort) = 0;
	virtual void RegisterSpi(CUft4_TraderSpi *pSpi) = 0;

	///�û���¼����
	/*
		����ֵ
			-1,ʧ��
			0,�ɹ�
	*/
	virtual int ReqUserLogin() = 0;
	///�û��ǳ�����
	/*
		����ֵ
			-1,ʧ��
			0,�ɹ�
	*/
	virtual int ReqUserLogout() = 0;

	///����¼������
	/*
	//  ֤ȯNST����������� begin
	*	@param 	szCode ֤ȯ����
	*	@param 	bs ��������
	*	@param 	dbPrice �۸�
	*	@param 	nVolume ����
	*	@param 	cOrderPriceType ί�����
	*	@param 	nSeatId ϯλID�� Ĭ��Ϊ0
	//  ֤ȯNST����������� end
		����ֵ,-1,ʧ��
				����0, ����sendid
	*/
	virtual int ReqOrderInsert(
		const char *szCode, 
		T_UFT_FtdcOrderBs bs,
		char kp, 
		double dbPrice, 
		int nVolume,
		int64_t nOrderRef,
		T_UFT_FtdcTimeConditionType cTimeCondtion = UFT_FTDC_TC_GFD,
		T_UFT_FtdcVolumeConditionType cVolumeCondition = UFT_FTDC_VC_AV,
		T_UFT_FtdcVolumeType	nMinVolume = 1,
		char cMargincheck = '1',
		T_UFT_FtdcOrderPriceTypeType cOrderPriceType = UFT_FTDC_OPT_LimitPrice,
		T_UFT_FtdcHedgeFlagType cHedgeFlagType = UFT_FTDC_HF_Speculation,
		uint8_t nSeatId = 0) = 0;

	///������������
	/*
		����ֵ,-1,ʧ��
				����0,����sendid
	*/
	virtual int ReqOrderAction(int nOrderIndex, int64_t nOrderRef) = 0;
	///
	/*
		����ֵ,-1,ʧ��
				����0,����sendid
	*/

	///��Ա�ʽ��ѯ����
	/*
		����ֵ,-1,ʧ��
				����0,����sendid
	*/
	virtual int ReqTradingAccount()=0;

	///�����ѯ����
	/*
	*	@param 	szInstrumentID ֤ȯ����
	*	@param 	nStartTime ��ʼʱ��
	*	@param 	nEndTime ����ʱ��
		����ֵ,-1,ʧ��
				����0,����sendid
	*/
	virtual int ReqQryOrder(const char* szInstrumentID, int32_t nStartTime, int32_t nEndTime, const char* szOrderSysID, int64_t nOrderRef,  uint32_t nOrderIndex, bool bAllSession = false) = 0;
	
	///�����ѯ�ɽ�
	/*
		����ֵ,-1,ʧ��
				����0,����sendid
	*/
	virtual int ReqQryTrade(const char* szInstrumentID, int32_t nStartTime, int32_t nEndTime, const char* szOrderSysID, uint32_t nOrderIndex,int64_t nOrderRef, bool bAllSession = false) = 0;

	///�����ѯͶ���ֲ߳�
	/*
		����ֵ,-1,ʧ��
				����0,����sendid
	*/
	virtual int ReqQryInvestorPosition(const char* szInstrumentID) = 0;


	///�޸���������
	/*
		����ֵ,-1,ʧ��
				����0,����sendid
	*/
	virtual int ReqChangePwd(const char* szNewPwd, const char* szOldPwd) = 0;

	//ͨѶ����
	/*
		����ֵ,-1,ʧ��
				����0,����sendid
	*/
	virtual int ReqQryTest() = 0;
	virtual uint64_t GetToken() = 0;
	virtual int GetReqPort() = 0;
};
#ifdef OS_WIN
#ifdef __cplusplus
extern "C"
{
#endif

	TRADE_API_EXPORT CUft4_TraderApi* FUNCTION_CALL_MODE CreateUft4Shfe_TraderApi(int nCnnMode = 0);

	TRADE_API_EXPORT const char* FUNCTION_CALL_MODE GetApiVersion();

#ifdef __cplusplus
}
#endif
#endif

#endif