#pragma once
#include "IStoneMdApiStruct.h"

#if defined(ISLIB) && defined(WIN32)
#ifdef LIB_MD_API_EXPORT
#define MD_API_EXPORT __declspec(dllexport)
#else
#define MD_API_EXPORT __declspec(dllimport)
#endif
#else
#define MD_API_EXPORT 
#endif

using namespace istone::mdapi;
namespace istone
{
	namespace mdapi
	{
		class IStoneMdSpi
		{
		public:
			///���ͻ����뽻�׺�̨������ͨ������ʱ����δ��¼ǰ�����÷��������á�
			virtual void OnFrontConnected() {};

			///���ͻ����뽻�׺�̨ͨ�����ӶϿ�ʱ���÷��������á���������������API���Զ��������ӣ��ͻ��˿ɲ�������
			///@param nReason ����ԭ��
			///        0x1001 �����ʧ��
			///        0x1002 ����дʧ��
			///        0x2001 ����������ʱ
			///        0x2002 ��������ʧ��
			///        0x2003 �յ�������
			virtual void OnFrontDisconnected(int nReason) {};

			///������ʱ���档����ʱ��δ�յ�����ʱ���÷��������á�
			///@param nTimeLapse �����ϴν��ձ��ĵ�ʱ��
			virtual void OnHeartBeatWarning(int nTimeLapse) {};

			///����Ӧ��
			virtual void OnRspError(CIStoneRspInfo* pRspInfo, int nRequestID, bool bIsLast) {};

			///�û���¼Ӧ��
			///@param pRspUserLogout	��¼�ɹ�ʱ�����ظ���Ϣ
			///@param pRspInfo			��¼�ɹ�ʱ��pRspInfoΪ�գ���¼ʧ��ʱ��pRspInfo���ش����������Ϣ
			virtual void OnRspUserLogin(CIStoneRspUserLogin* pRspUserLogin, CIStoneRspInfo* pRspInfo, int nRequestID, bool bIsLast) {};

			///�û��˳�Ӧ��
			///@param pRspUserLogout	�˳��ɹ�ʱ�����ظ���Ϣ
			///@param pRspInfo			�˳��ɹ�ʱ��pRspInfoΪ�գ��˳�ʧ��ʱ��pRspInfo���ش����������Ϣ
			virtual void OnRspUserLogout(CIStoneRspUserLogout* pRspUserLogout, CIStoneRspInfo* pRspInfo, int nRequestID, bool bIsLast) {};

			///�û��������ȫ����Ӧ��
			///@param tickType						��Ʊ����, �����Ͷ��Ŀ�ʹ�� ISTONE_TICKER_TYPE_SPOT | ISTONE_TICKER_TYPE_INDEX ��ʽ���ġ�
			///        ISTONE_TICKER_TYPE_SPOT		<��ͨ��Ʊ<ծȯ<����
			///        ISTONE_TICKER_TYPE_INDEX		<ָ��
			///        ISTONE_TICKER_TYPE_OPTION	<��Ȩ
			///@param exchangeType					���������ͣ� �ཻ������ʹ�� ISTONE_EXCHANGE_SH | ISTONE_EXCHANGE_SZ ��ʽ���ġ�
			///        ISTONE_EXCHANGE_SH			<��֤
			///        ISTONE_EXCHANGE_SZ			<��֤
			virtual void OnRspSubAllMarketData(int tickType, int exchangeID, int nRequestID, bool isSuccess) {};

			///�û��������ȫ�˶�Ӧ��
			///@param tickType						��Ʊ����, �����Ͷ��Ŀ�ʹ�� ISTONE_TICKER_TYPE_SPOT | ISTONE_TICKER_TYPE_INDEX ��ʽ���ġ�
			///        ISTONE_TICKER_TYPE_SPOT		<��ͨ��Ʊ<ծȯ<����
			///        ISTONE_TICKER_TYPE_INDEX		<ָ��
			///        ISTONE_TICKER_TYPE_OPTION	<��Ȩ
			///@param exchangeType					���������ͣ� �ཻ������ʹ�� ISTONE_EXCHANGE_SH | ISTONE_EXCHANGE_SZ ��ʽ���ġ�
			///        ISTONE_EXCHANGE_SH			<��֤
			///        ISTONE_EXCHANGE_SZ			<��֤
			virtual void OnRspUnSubAllMarketData(int tickType, int exchangeID, int nRequestID, bool isSuccess) {};

			///�û��������ȫ����Ӧ��
			///@param tickType						��Ʊ����, �����Ͷ��Ŀ�ʹ�� ISTONE_TICKER_TYPE_SPOT | ISTONE_TICKER_TYPE_INDEX ��ʽ���ġ�
			///        ISTONE_TICKER_TYPE_SPOT		<��ͨ��Ʊ<ծȯ<����
			///        ISTONE_TICKER_TYPE_INDEX		<ָ��
			///        ISTONE_TICKER_TYPE_OPTION	<��Ȩ
			///@param exchangeType					���������ͣ� �ཻ������ʹ�� ISTONE_EXCHANGE_SH | ISTONE_EXCHANGE_SZ ��ʽ���ġ�
			///        ISTONE_EXCHANGE_SH			<��֤
			///        ISTONE_EXCHANGE_SZ			<��֤
			virtual void OnRspSubAllTickByTick(int exchangeID, int nRequestID, bool isSuccess) {};

			///�û��������ȫ�˶�Ӧ��
			///@param tickType						��Ʊ����, �����Ͷ��Ŀ�ʹ�� ISTONE_TICKER_TYPE_SPOT | ISTONE_TICKER_TYPE_INDEX ��ʽ���ġ�
			///        ISTONE_TICKER_TYPE_SPOT		<��ͨ��Ʊ<ծȯ<����
			///        ISTONE_TICKER_TYPE_INDEX		<ָ��
			///        ISTONE_TICKER_TYPE_OPTION	<��Ȩ
			///@param exchangeType					���������ͣ� �ཻ������ʹ�� ISTONE_EXCHANGE_SH | ISTONE_EXCHANGE_SZ ��ʽ���ġ�
			///        ISTONE_EXCHANGE_SH			<��֤
			///        ISTONE_EXCHANGE_SZ			<��֤
			virtual void OnRspUnSubAllTickByTick(int exchangeID, int nRequestID, bool isSuccess) {};

			///�û�������鶩��Ӧ��
			///@param ppInstrumentID	��Ʊ����
			///@param nCount			��Ʊ����
			virtual void OnRspSubMarketData(char* tickers[], int count, int nRequestID, bool bIsSuccess) {};

			///�û���������˶�Ӧ��
			///@param ppInstrumentID	��Ʊ����
			///@param nCount			��Ʊ����
			virtual void OnRspUnSubMarketData(char* tickers[], int count, int nRequestID, bool bIsSuccess) {};

			///�û�������鶩��Ӧ��
			///@param ppInstrumentID	��Ʊ����
			///@param nCount			��Ʊ����
			virtual void OnRspSubTickByTick(char* tickers[], int count, int nRequestID, bool bIsSuccess) {};

			///�û���������˶�Ӧ��
			///@param ppInstrumentID	��Ʊ����
			///@param nCount			��Ʊ����
			virtual void OnRspUnSubTickByTick(char* tickers[], int count, int nRequestID, bool bIsSuccess) {};

			///�������֪ͨ
			virtual void OnRtnDepthMarketData(CIStoneDepthMarketData* data) {};

			///�������֪ͨ
			virtual void OnRtnTickByTick(CIStoneTickByTick* data) {};

			///������֪ͨ
			virtual void OnRtnOrderBook(CIStoneOrderBook* data) {};

			///�û�����������Ӧ��
			///@param ppInstrumentID	��Ʊ����
			///@param nCount			��Ʊ����
			virtual void OnRspSubOrderBook(char* tickers[], int count, int nRequestID, bool bIsSuccess) {};

			///�û��������˶�Ӧ��
			///@param ppInstrumentID	��Ʊ����
			///@param nCount			��Ʊ����
			virtual void OnRspUnSubOrderBook(char* tickers[], int count, int nRequestID, bool bIsSuccess) {};
		};

		class MD_API_EXPORT IStoneMdApi
		{
		public:
			///����MduserApi
			///@param pszFlowPath ����������Ϣ�ļ���Ŀ¼��Ĭ��Ϊ��ǰĿ¼
			///@return ��������UserApi
			static IStoneMdApi* CreateMdUserApi(const char* pszFlowPath = "");

			///��ȡAPI�İ汾��Ϣ
			///@retrun ��ȡ���İ汾��
			static const char* GetVersion();

			///ɾ���ӿڶ�����
			///@remark ����ʹ�ñ��ӿڶ���ʱ,���øú���ɾ���ӿڶ���
			virtual void Release() = 0;

			///��ʼ��
			///@remark ��ʼ�����л���,ֻ�е��ú�,�ӿڲſ�ʼ����
			virtual int Init() = 0;

			///�ȴ��ӿ��߳̽�������
			///@return �߳��˳�����
			virtual int Join() = 0;

			///��ȡ��ǰ������
			///@retrun ��ȡ���Ľ�����
			///@remark ֻ�е�¼�ɹ���,���ܵõ���ȷ�Ľ�����
			virtual const char* GetTradingDay() = 0;

			///ע��ǰ�û������ַ
			///@param pszFrontAddress��ǰ�û������ַ��
			///@remark �����ַ�ĸ�ʽΪ����protocol://ipaddress:port�����磺��tcp://127.0.0.1:17001���� 
			///@remark ��tcp��������Э�飬��127.0.0.1�������������ַ����17001������������˿ںš�
			virtual void RegisterFront(const char* pszFrontAddress) = 0;

			///ע�������鲥��ַ
			///@param pMutilcastAddr�������鲥��ַ
			///@remark �����ַ�ĸ�ʽΪ����protocol://ipaddress:port�����磺��udp://224.0.0.1:17002�� 
			///@remark ��udp��������Э�飬��224.0.0.1�����������鲥��ַ����17001�������鲥�˿ں�
			virtual void RegisterFpgaFront(const char* pMutilcastAddr) = 0;

			///ע������ǰ�õı��ص�ַ
			///@param localTcpAddr������ǰ�õı���������ַ��������ʱϵͳ�Զ�ʶ��
			///@remark �����ַ�ĸ�ʽΪ����protocol://ipaddress:port�����磺��tcp://127.0.0.1:17001���� 
			///@remark ��tcp��������Э�飬��127.0.0.1�������������ַ����17001������������˿ںš�
			virtual void RegisterTcpLocalAddr(const char* localTcpAddr) = 0;

			///ע������鲥�ı��ص�ַ
			///@param localAddr�������鲥�ı���������ַ��������ʱϵͳ�Զ�ʶ��
			///@remark �����ַ�ĸ�ʽΪ����ipaddress�����磺��127.0.0.1�� 
			///@remark ����127.0.0.1����������鲥�ı���������ַ��
			virtual void RegisterUdpLocalAddr(const char* localAddr) = 0;

			///ע��ʹ��efvi����udp�������豸����
			///@param device�������豸���� ������"ens1"
			virtual void RegisterEfviDevice(const char* device) = 0;

			///ע��ص��ӿ�
			///@param pSpi �����Իص��ӿ����ʵ��
			virtual void RegisterSpi(IStoneMdSpi* pSpi) = 0;

			///����������ʱʱ�䡣
			///@param timeout ������ʱʱ��(��),��С������ʱʱ�䲻С��10��  
			virtual void SetHeartbeatTimeout(uint32_t timeout) = 0;

			///�û���¼����
			///@param pReqUserLogin ��¼��Ϣ, UserID �� Password Ϊ�����ֶΣ�����δѡ���ֶ� 
			virtual int ReqUserLogin(CIStoneReqUserLogin* pReqUserLogin, int nRequestID) = 0;

			///�û��˳�����
			///@param pReqUserLogin �˳���Ϣ, UserID Ϊ�����ֶΣ�����δѡ���ֶ� 
			virtual int ReqUserLogout(CIStoneReqUserLogout* pReqUserLogout, int nRequestID) = 0;

			///���Ĺ�Ʊ�������
			///@param tickType						��Ʊ����, �����Ͷ��Ŀ�ʹ�� ISTONE_TICKER_TYPE_SPOT | ISTONE_TICKER_TYPE_INDEX ��ʽ���ġ�
			///        ISTONE_TICKER_TYPE_SPOT		<��ͨ��Ʊ<ծȯ<����
			///        ISTONE_TICKER_TYPE_INDEX		<ָ��
			///        ISTONE_TICKER_TYPE_OPTION	<��Ȩ
			///@param exchangeType					���������ͣ� �ཻ������ʹ�� ISTONE_EXCHANGE_SH | ISTONE_EXCHANGE_SZ ��ʽ���ġ�
			///        ISTONE_EXCHANGE_SH			<��֤
			///        ISTONE_EXCHANGE_SZ			<��֤
			virtual int SubAllMarketData(int tickType, int exchangeID, int nRequestID) = 0;

			///�˶���Ʊ�������
			///@param tickType						��Ʊ����, �����Ͷ��Ŀ�ʹ�� ISTONE_TICKER_TYPE_SPOT | ISTONE_TICKER_TYPE_INDEX ��ʽ���ġ�
			///        ISTONE_TICKER_TYPE_SPOT		<��ͨ��Ʊ<ծȯ<����
			///        ISTONE_TICKER_TYPE_INDEX		<ָ��
			///        ISTONE_TICKER_TYPE_OPTION	<��Ȩ
			///@param exchangeType					���������ͣ� �ཻ������ʹ�� ISTONE_EXCHANGE_SH | ISTONE_EXCHANGE_SZ ��ʽ���ġ�
			///        ISTONE_EXCHANGE_SH			<��֤
			///        ISTONE_EXCHANGE_SZ			<��֤
			virtual int UnSubAllMarketData(int tickType, int exchangeID, int nRequestID) = 0;

			///���Ĺ�Ʊ�������
			///@param tickType						��Ʊ����, �����Ͷ��Ŀ�ʹ�� ISTONE_TICKER_TYPE_SPOT | ISTONE_TICKER_TYPE_INDEX ��ʽ���ġ�
			///        ISTONE_TICKER_TYPE_SPOT		<��ͨ��Ʊ<ծȯ<����
			///        ISTONE_TICKER_TYPE_INDEX		<ָ��
			///        ISTONE_TICKER_TYPE_OPTION	<��Ȩ
			///@param exchangeType					���������ͣ� �ཻ������ʹ�� ISTONE_EXCHANGE_SH | ISTONE_EXCHANGE_SZ ��ʽ���ġ�
			///        ISTONE_EXCHANGE_SH			<��֤
			///        ISTONE_EXCHANGE_SZ			<��֤
			virtual int SubAllTickByTick(int exchangeID, int nRequestID) = 0;

			///�˶���Ʊ�������
			///@param tickType						��Ʊ����, �����Ͷ��Ŀ�ʹ�� ISTONE_TICKER_TYPE_SPOT | ISTONE_TICKER_TYPE_INDEX ��ʽ���ġ�
			///        ISTONE_TICKER_TYPE_SPOT		<��ͨ��Ʊ<ծȯ<����
			///        ISTONE_TICKER_TYPE_INDEX		<ָ��
			///        ISTONE_TICKER_TYPE_OPTION	<��Ȩ
			///@param exchangeType					���������ͣ� �ཻ������ʹ�� ISTONE_EXCHANGE_SH | ISTONE_EXCHANGE_SZ ��ʽ���ġ�
			///        ISTONE_EXCHANGE_SH			<��֤
			///        ISTONE_EXCHANGE_SZ			<��֤
			virtual int UnSubAllTickByTick(int exchangeID, int nRequestID) = 0;

			///���ĺ�Լ����
			///@param ppInstrumentID	��Ʊ����
			///@param nCount			��Ʊ����
			virtual int SubMarketData(char* ppInstrumentID[], int nCount, int nRequestID) = 0;

			///�˶���Լ����
			///@param ppInstrumentID	��Ʊ����
			///@param nCount			��Ʊ����
			virtual int UnSubMarketData(char* ppInstrumentID[], int nCount, int nRequestID) = 0;

			///���ĺ�Լ����
			///@param ppInstrumentID	��Ʊ����
			///@param nCount			��Ʊ����
			virtual int SubTickByTick(char* ppInstrumentID[], int nCount, int nRequestID) = 0;

			///�˶���Լ����
			///@param ppInstrumentID	��Ʊ����
			///@param nCount			��Ʊ����
			virtual int UnSubTickByTick(char* ppInstrumentID[], int nCount, int nRequestID) = 0;

			///�������й�Ʊ������
			///@param exchangeID ������ID,Ŀǰ��֧�����   1������֤  2������֤
			virtual int SubAllOrderBook(int exchangeID, int nRequestID) = 0;

			///�˶����й�Ʊ������
			///@param exchangeID ������ID,Ŀǰ��֧�����   1������֤  2������֤
			virtual int UnSubAllOrderBook(int exchangeID, int nRequestID) = 0;

			///���Ķ�����
			///@param ppInstrumentID	��Ʊ����
			///@param nCount			��Ʊ����
			virtual int SubOrderBook(char* ppInstrumentID[], int nCount, int nRequestID) = 0;

			///�˶�������
			///@param ppInstrumentID	��Ʊ����
			///@param nCount			��Ʊ����
			virtual int UnSubOrderBook(char* ppInstrumentID[], int nCount, int nRequestID) = 0;

		protected:
			~IStoneMdApi() {};
		};
	}
}
