#pragma once

#include "vnsec.h"
#include "pybind11/pybind11.h"
#include "DFITCSECMdApi.h"

using namespace pybind11;
using namespace std;


//����
#define ONFRONTCONNECTED 0
#define ONFRONTDISCONNECTED 1
#define ONRTNNOTICE 2
#define ONRSPERROR 3
#define ONRSPSTOCKUSERLOGIN 4
#define ONRSPSTOCKUSERLOGOUT 5
#define ONRSPSOPUSERLOGIN 6
#define ONRSPSOPUSERLOGOUT 7
#define ONRSPFASLUSERLOGIN 8
#define ONRSPFASLUSERLOGOUT 9
#define ONRSPSTOCKSUBMARKETDATA 10
#define ONRSPSTOCKUNSUBMARKETDATA 11
#define ONRSPSOPSUBMARKETDATA 12
#define ONRSPSOPUNSUBMARKETDATA 13
#define ONSTOCKMARKETDATA 14
#define ONSOPMARKETDATA 15
#define ONRSPSTOCKAVAILABLEQUOT 16
#define ONRSPSOPAVAILABLEQUOT 17
#define ONRSPUSERMDPASSWORDUPDATE 18

///-------------------------------------------------------------------------------------
///C++ SPI�Ļص���������ʵ��
///-------------------------------------------------------------------------------------

//API�ļ̳�ʵ��

class MdApi : public DFITCSECMdSpi
{
private:
	DFITCSECMdApi* api;				//API����
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

		/**
		 * ��������������Ӧ
		 */
	virtual void OnFrontConnected();

	/**
 * �������Ӳ�������Ӧ
 */
	virtual void OnFrontDisconnected(int nReason);

	/**
	 * SEC-��Ϣ֪ͨ
	 */
	virtual void OnRtnNotice(DFITCSECRspNoticeField *pNotice);

	/**
	 * ERR-����Ӧ��
	 * @param pRspInfo:������ʧ�ܣ����ش�����Ϣ��ַ
	 */
	virtual void OnRspError(struct DFITCSECRspInfoField *pRspInfo);

	/**
	 * STOCK-��¼��Ӧ
	 * @param pRspUserLogin:ָ�����ǿ�,�����û���¼��Ӧ��Ϣ�ṹ��ַ,������¼����ɹ�
	 * @param pRspInfo::ָ�����ǿգ����ش�����Ϣ��ַ��������¼����ʧ��
	 */
	virtual void OnRspStockUserLogin(struct DFITCSECRspUserLoginField * pRspUserLogin, struct DFITCSECRspInfoField * pRspInfo);

	/**
	 * STOCK-�ǳ���Ӧ
	 * @param pRspUsrLogout:ָ�����ǿ�,�����û��ǳ���Ӧ��Ϣ�ṹ��ַ,�����ǳ�����ɹ�
	 * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ�������ǳ�����ʧ��
	 */
	virtual void OnRspStockUserLogout(struct DFITCSECRspUserLogoutField * pRspUsrLogout, struct DFITCSECRspInfoField * pRspInfo);

		/**
		 * SOP-��¼��Ӧ
		 * @param pRspUserLogin:ָ�����ǿ�,�����û���¼��Ӧ��Ϣ�ṹ��ַ,������¼����ɹ�
		 * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ��������¼����ʧ��
		 */
		virtual void OnRspSOPUserLogin(struct DFITCSECRspUserLoginField * pRspUserLogin, struct DFITCSECRspInfoField * pRspInfo);

	/**
	 * SOP-�ǳ���Ӧ
	 * @param pRspUsrLogout:ָ�����ǿ�,�����û��ǳ���Ӧ��Ϣ�ṹ��ַ,�����ǳ�����ɹ�
	 * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ�������ǳ�����ʧ��
	 */
		virtual void OnRspSOPUserLogout(struct DFITCSECRspUserLogoutField * pRspUsrLogout, struct DFITCSECRspInfoField * pRspInfo);

	/**
	 * FASL-��¼��Ӧ
	 * @param pRspUserLogin:ָ�����ǿ�,�����û���¼��Ӧ��Ϣ�ṹ��ַ,������¼����ɹ�
	 * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ��������¼����ʧ��
	 */
		virtual void OnRspFASLUserLogin(struct DFITCSECRspUserLoginField * pRspUserLogin, struct DFITCSECRspInfoField * pRspInfo);

	/**
	 * FASL-�ǳ���Ӧ
	 * @param pRspUsrLogout:ָ�����ǿ�,�����û��ǳ���Ӧ��Ϣ�ṹ��ַ,�����ǳ�����ɹ�
	 * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ�������ǳ�����ʧ��
	 */
		virtual void OnRspFASLUserLogout(struct DFITCSECRspUserLogoutField * pRspUsrLogout, struct DFITCSECRspInfoField * pRspInfo);

	/**
	 * STOCK-���鶩����Ӧ
	 * @param pSpecificInstrument:ָ�����ǿ�,�����û�ָ����Լ���鶩����Ӧ�ṹ��ַ,����ָ����Լ���鶩�ĳɹ�
	 * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ������ָ����Լ���鶩��ʧ��
	 */
		virtual void OnRspStockSubMarketData(struct DFITCSECSpecificInstrumentField * pSpecificInstrument, struct DFITCSECRspInfoField * pRspInfo);

	/**
	 * STOCK-ȡ������������Ӧ
	 * @param pSpecificInstrument:ָ�����ǿ�,�����û�ָ����Լ����ȡ��������Ӧ�ṹ��ַ,����ָ����Լ����ȡ�����ĳɹ�
	 * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ������ָ����Լ����ȡ������ʧ��
	 */
		virtual void OnRspStockUnSubMarketData(struct DFITCSECSpecificInstrumentField * pSpecificInstrument, struct DFITCSECRspInfoField * pRspInfo);

	/**
	 * SOP-���鶩����Ӧ
	 * @param pSpecificInstrument:ָ�����ǿ�,�����û�ָ����Լ���鶩����Ӧ�ṹ��ַ,����ָ����Լ���鶩�ĳɹ�
	 * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ������ָ����Լ���鶩��ʧ��
	 */
		virtual void OnRspSOPSubMarketData(struct DFITCSECSpecificInstrumentField * pSpecificInstrument, struct DFITCSECRspInfoField * pRspInfo);

	/**
	 * SOP-ȡ������������Ӧ
	 * @param pSpecificInstrument:ָ�����ǿ�,�����û�ָ����Լ����ȡ��������Ӧ�ṹ��ַ,����ָ����Լ����ȡ�����ĳɹ�
	 * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ������ָ����Լ����ȡ������ʧ��
	 */
		virtual void OnRspSOPUnSubMarketData(struct DFITCSECSpecificInstrumentField * pSpecificInstrument, struct DFITCSECRspInfoField * pRspInfo);

	/**
	 * STOCK-����������Ӧ
	 * @param pMarketDataField:ָ�����ǿ�,��������������Ӧ�ṹ��ַ
	 */
		virtual void OnStockMarketData(struct DFITCStockDepthMarketDataField * pMarketDataField);

	/**
	 * SOP-����������Ӧ
	 * @param pMarketDataField:ָ�����ǿ�,��������������Ӧ�ṹ��ַ
	 */
		virtual void OnSOPMarketData(struct DFITCSOPDepthMarketDataField * pMarketDataField);

	/**
	 * STOCK-����������Ӧ
	 * @param pAvailableQuotInfo:ָ�����ǿ�,���ؿ��õ�������Ϣ
	 * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ������ָ����Լ����ȡ������ʧ��
	 * @param flag  Ϊ���Ǳ�ʾ���һ����Ϊ��ʱ��ʾ���к�����
	 */
		virtual void OnRspStockAvailableQuot(struct DFITCRspQuotQryField * pAvailableQuotInfo, struct DFITCSECRspInfoField * pRspInfo, bool flag);

	/**
	 * SOP-����������Ӧ
	 * @param pAvailableQuotInfo:ָ�����ǿ�,���ؿ��õ�������Ϣ
	 * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ������ָ����Լ����ȡ������ʧ��
	 * @param flag  Ϊ���Ǳ�ʾ���һ����Ϊ��ʱ��ʾ���к�����
	 */
		virtual void OnRspSopAvailableQuot(struct DFITCRspQuotQryField * pAvailableQuotInfo, struct DFITCSECRspInfoField * pRspInfo, bool flag);

	/**
	 * �������������Ӧ
	 * @param pMDPasswordUpdate:ָ�����ǿ�,�����û�����������Ӧ��Ϣ�ṹ��ַ,���������޸ĳɹ���
	 * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ�����������޸�ʧ�ܡ�
	 */
		virtual void OnRspUserMDPasswordUpdate(struct DFITCSECRspMDPasswordUpdateField *pMDPasswordUpdate, struct DFITCSECRspInfoField * pRspInfo);

	//-------------------------------------------------------------------------------------
	//task������
	//-------------------------------------------------------------------------------------

	void processTask();

	void processFrontConnected(Task *task);

	void processFrontDisconnected(Task *task);

	void processRtnNotice(Task *task);

	void processRspError(Task *task);

	void processRspStockUserLogin(Task *task);

	void processRspStockUserLogout(Task *task);

	void processRspSOPUserLogin(Task *task);

	void processRspSOPUserLogout(Task *task);

	void processRspFASLUserLogin(Task *task);

	void processRspFASLUserLogout(Task *task);

	void processRspStockSubMarketData(Task *task);

	void processRspStockUnSubMarketData(Task *task);

	void processRspSOPSubMarketData(Task *task);

	void processRspSOPUnSubMarketData(Task *task);

	void processStockMarketData(Task *task);

	void processSOPMarketData(Task *task);

	void processRspStockAvailableQuot(Task *task);

	void processRspSopAvailableQuot(Task *task);

	void processRspUserMDPasswordUpdate(Task *task);

	//-------------------------------------------------------------------------------------
	//data���ص������������ֵ�
	//error���ص������Ĵ����ֵ�
	//id������id
	//last���Ƿ�Ϊ��󷵻�
	//i������
	//-------------------------------------------------------------------------------------

	virtual void onFrontConnected() {};

	virtual void onFrontDisconnected(int reqid) {};

	virtual void onRtnNotice(const dict &data) {};

	virtual void onRspError(const dict &data) {};

	virtual void onRspStockUserLogin(const dict &data, const dict &error) {};

	virtual void onRspStockUserLogout(const dict &data, const dict &error) {};

	virtual void onRspSOPUserLogin(const dict &data, const dict &error) {};

	virtual void onRspSOPUserLogout(const dict &data, const dict &error) {};

	virtual void onRspFASLUserLogin(const dict &data, const dict &error) {};

	virtual void onRspFASLUserLogout(const dict &data, const dict &error) {};

	virtual void onRspStockSubMarketData(const dict &data, const dict &error) {};

	virtual void onRspStockUnSubMarketData(const dict &data, const dict &error) {};

	virtual void onRspSOPSubMarketData(const dict &data, const dict &error) {};

	virtual void onRspSOPUnSubMarketData(const dict &data, const dict &error) {};

	virtual void onStockMarketData(const dict &data) {};

	virtual void onSOPMarketData(const dict &data) {};

	virtual void onRspStockAvailableQuot(const dict &data, const dict &error, bool last) {};

	virtual void onRspSopAvailableQuot(const dict &data, const dict &error, bool last) {};

	virtual void onRspUserMDPasswordUpdate(const dict &data, const dict &error) {};

	//-------------------------------------------------------------------------------------
	//req:���������������ֵ�
	//-------------------------------------------------------------------------------------

	void createDFITCMdApi(string pszFlowPath);

	void release();

	int init(string protocol);

	int exit();

	int subscribeStockMarketData(string ppInstrumentID, int reqid);

	int unSubscribeStockMarketData(string ppInstrumentID, int reqid);

	int subscribeSOPMarketData(string ppInstrumentID, int reqid);

	int unSubscribeSOPMarketData(string ppInstrumentID, int reqid);

	int reqStockUserLogin(const dict &req);

	int reqStockUserLogout(const dict &req);

	int reqSOPUserLogin(const dict &req);

	int reqFASLUserLogin(const dict &req);

	int reqSOPUserLogout(const dict &req);

	int reqFASLUserLogout(const dict &req);

	int reqStockAvailableQuotQry(const dict &req);

	int reqSopAvailableQuotQry(const dict &req);

	int reqUserMDPasswordUpdate(const dict &req);


};

