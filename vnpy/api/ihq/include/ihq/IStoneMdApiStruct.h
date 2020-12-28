#pragma once
#include "IStoneApiDataType.h"

namespace istone
{
	namespace mdapi
	{
		///�û���¼����
		struct CIStoneReqUserLogin
		{
			char	TradingDay[ISTONE_TRADING_DAY_LEN];		//������
			char	UserID[ISTONE_USERID_LEN];				//�����û�����
			char	Password[ISTONE_USER_PW_LEN];			//����
			char	UserProductInfo[41];					//�û��˲�Ʒ��Ϣ
			char	IPAddress[30];							//IP��ַ
			char	MacAddress[30];							//Mac��ַ
		};

		///�û���¼Ӧ��
		struct CIStoneRspUserLogin
		{
			char	TradingDay[ISTONE_TRADING_DAY_LEN];		//������
			char	UserID[ISTONE_USERID_LEN];				//�����û�����
			int		MaxUserOrderLocalID;					//�û���󱾵ر�����
			char	LoginTime[ISTONE_TIMESTAMP_LEN];		//��¼�ɹ�ʱ��
			ISTONE_USERS_TYPE    userType;                  //�û�����
		};

		///�û��ǳ�����
		struct CIStoneReqUserLogout
		{
			char	UserID[ISTONE_USERID_LEN];				//�����û�����
		};

		///�û��ǳ�Ӧ��
		struct CIStoneRspUserLogout
		{
			char	UserID[ISTONE_USERID_LEN];				//�����û�����
		};

		///�û����Ŀ�������Ӧ��
		struct CIStoneSubParams
		{
			int TickType;					//��Ʊ����
			int ExchangeType;				//����������
		};

		///�û��������Ӧ��
		struct CIStoneRspSubTickByTick 
		{
			int16_t SubAllFlag;								//ȫ������
			ISTONE_EXCHANGE_TYPE ExchangeID;				// ����������
			char	Ticker[ISTONE_TICKER_LEN];				//��Լ����
		};
		///�û��������Ӧ��
		struct CIStoneRspUnSubTickByTick
		{
			int16_t UnSubAllFlag;								//ȫ������
			ISTONE_EXCHANGE_TYPE ExchangeID;				// ����������
			char	Ticker[ISTONE_TICKER_LEN];				//��Լ����		
		};


		///��Ӧ��Ϣ
		struct CIStoneRspInfo
		{
			int	Tid;										//ҵ��ID
			int	ErrorID;									//�������
			TIStoneErrorMsgType	ErrorMsg;					//������Ϣ
		};

		enum ISTONE_MARKETDATA_TYPE 
		{
			ISTONE_MARKETDATA_ACTUAL = 0,					// �ֻ�(��Ʊ/����/ծȯ��)
			ISTONE_MARKETDATA_OPTION = 1,					// ��Ȩ
		};

		///ʵʱ�����ѯ����
		struct CIStoneQryMarketData
		{
			ISTONE_EXCHANGE_TYPE	ExchangeID;		//����������
			char	Ticker[ISTONE_TICKER_LEN];		//��Լ����
		};

		struct CIStoneSpecificTicker
		{
			ISTONE_EXCHANGE_TYPE ExchangeID;				// ����������
			char	Ticker[ISTONE_TICKER_LEN];				//��Լ����
		};

		///��Ʊ������ծȯ�ȶ�������
		struct CIStoneMarketDataStockExData 
		{
			///ί����������(SH,SZ)
			int64_t TotalBidQty;
			///ί����������(SH,SZ)
			int64_t TotalAskQty;
			///��Ȩƽ��ί��۸�(SH,SZ)
			double MaBidPrice;
			///��Ȩƽ��ί���۸�(SH,SZ)
			double MaAskPrice;
			///ծȯ��Ȩƽ��ί��۸�(SH)
			double MaBondBidPrice;
			///ծȯ��Ȩƽ��ί���۸�(SH)
			double MaBondAskPrice;
			///ծȯ����������(SH)
			double YieldToMaturity;
			///����ʵʱ�ο���ֵ(SH,SZ)
			double Iopv;
			///ETF�깺����(SH)
			int32_t EtfBuyCount;
			///ETF��ر���(SH)
			int32_t EtfSellCount;
			///ETF�깺����(SH)
			double EtfBuyQty;
			///ETF�깺���(SH)
			double EtfBuyMoney;
			///ETF�������(SH)
			double EtfSellQty;
			///ETF��ؽ��(SH)
			double EtfSellMoney;
			///Ȩִ֤�е�������(SH)
			double TotalWarrantExecQty;
			///Ȩ֤��ͣ�۸�Ԫ��(SH)
			double WarrantLowerPrice;
			///Ȩ֤��ͣ�۸�Ԫ��(SH)
			double WarrantUpperPrice;
			///���볷������(SH)
			int32_t CancelBuyCount;
			///������������(SH)
			int32_t CancelSellCount;
			///���볷������(SH)
			double CancelBuyQty;
			///������������(SH)
			double CancelSellQty;
			///���볷�����(SH)
			double CancelBuyMoney;
			///�����������(SH)
			double CancelSellMoney;
			///�����ܱ���(SH)
			int64_t TotalBuyCount;
			///�����ܱ���(SH)
			int64_t TotalSellCount;
			///����ί�гɽ����ȴ�ʱ��(SH)
			int32_t DurationAfterBuy;
			///����ί�гɽ����ȴ�ʱ��(SH)
			int32_t DurationAfterSell;
			///��ί�м�λ��(SH)
			int32_t NumBidOrders;
			///����ί�м�λ��(SH)
			int32_t NumAskOrders;

			///����T-1�վ�ֵ(SZ)
			double PreIopv;
			///Ԥ��
			int64_t R1;
			///Ԥ��
			int64_t R2;
		};

		// ��Ȩ��������
		struct CIStoneMarketDataOptionExData 
		{
			///�������жϲο���(SH)
			double  AuctionPrice;
			///�������жϼ��Ͼ�������ƥ����(SH)
			int64_t AuctionQty;
			///���ѯ��ʱ��(SH)
			int64_t LastEnquiryTime;
		};

		///һ��50��ί������
		struct CIStoneTickerBid1
		{
			uint32_t			Bid1NumOfOrders;			// ��һ����λ��ί�б���
			uint8_t				Bid1NoOrders;				// ��һ����λ��ʾί�б���
			uint32_t			Bid1Orders[50];				// ��һ��50��ί��
			uint32_t			Ask1NumOfOrders;			// ��һ����λ��ί�б���
			uint8_t				Ask1NoOrders;				// ��һ����λ��ʾί�б���
			uint32_t			Ask1Orders[50];				// ��һ��50��ί��
		};

		///�������
		struct CIStoneDepthMarketData
		{
			//����������
			ISTONE_EXCHANGE_TYPE ExchangeID;
			/// ��Լ����
			ISTONE_TICKER_TYPE TickerType;
			///��Լ���루��������������Ϣ���������ո���'\0'��β
			char	Ticker[ISTONE_TICKER_LEN];

			///���¼�
			double	LastPrice;
			///������
			double	PreClosePrice;
			///����
			double	OpenPrice;
			///��߼�
			double	HighestPrice;
			///��ͼ�
			double	LowestPrice;
			///������
			double	ClosePrice;

			// ��Ȩ����
			///���ճֲ���(��)(Ŀǰδ��д)
			int64_t PreOpenInterest;
			///�ֲ���(��)
			int64_t	OpenInterest;
			///���ս����
			double	PreSettlementPrice;
			///���ս����
			double	SettlementPrice;

			///����ʵ��
			double	PreDelta;
			///����ʵ��
			double	CurrDelta;
			///��ͣ���
			double	UpperLimitPrice;
			///��ͣ���
			double	LowerLimitPrice;

			/// ʱ���࣬��ʽΪYYYYMMDDHHMMSSsss
			int64_t DateTime;

			// ��������
			///������Ϊ�ܳɽ�������λ�ɣ��뽻����һ�£�
			int64_t		Qty;
			///�ɽ���Ϊ�ܳɽ�����λԪ���뽻����һ�£�
			double	Turnover;
			///���վ���=(turnover/qty)
			double	AvgPrice;

			// ������
			///ʮ�������
			double Bid[10];
			///ʮ��������
			double	Ask[10];
			///ʮ��������
			int64_t	BidQty[10];
			///ʮ��������
			int64_t	AskQty[10];
			///�ɽ�����
			int64_t TradesCount;
			///��ǰ����״̬˵��
			char TickerStatus[8];

			//һ��50��ί��
			CIStoneTickerBid1 tickBid1;

			///������union��������������
			ISTONE_MARKETDATA_TYPE DataType;
			// ��������
			///����
			union {
				CIStoneMarketDataStockExData  Stk;
				CIStoneMarketDataOptionExData Opt;
			};
			///Ԥ��
			int32_t R4;
		};

		///��Ʊ���龲̬��Ϣ
		struct CIStoneQuoteStaticInfo 
		{
			///����������
			ISTONE_EXCHANGE_TYPE ExchangeID;
			///��Լ���루��������������Ϣ���������ո���'\0'��β
			char    Ticker[ISTONE_TICKER_LEN];
			/// ��Լ����
			char    TickerName[ISTONE_TICKER_NAME_LEN];
			/// ��Լ����
			ISTONE_TICKER_TYPE TickerType;
			///������
			double  PreClosePrice;
			///��ͣ���
			double  UpperLimitPrice;
			///��ͣ���
			double  LowerLimitPrice;
			///��С�䶯��λ
			double  PriceTick;
			/// ��Լ��С������(��)
			int32_t  BuyQtyUnit;
			/// ��Լ��С������(��)
			int32_t SellQtyUnit;
		};


		///���������������������
		struct CIStoneOrderBook 
		{
			///����������
			ISTONE_EXCHANGE_TYPE ExchangeID;
			///��Լ���루��������������Ϣ���������ո���'\0'��β
			char    Ticker[ISTONE_TICKER_LEN];

			///���¼�
			double LastPrice;
			///������Ϊ�ܳɽ���
			int64_t Qty;
			///�ɽ���Ϊ�ܳɽ����
			double  Turnover;
			///�ɽ�����
			int64_t TradesCount;

			// ������
			///ʮ�������
			double Bid[10];
			///ʮ��������
			double  Ask[10];
			///ʮ��������
			int64_t BidQty[10];
			///ʮ��������
			int64_t AskQty[10];
			/// ʱ����
			int64_t DataTime;

			///����ʮ����Ӧ�����к�
			string bidSeq[10];
			string askSeq[10];
		};

		////////////////////////////////// �������


		///���ί��(���������)
		struct CIStoneTickByTickEntrust 
		{
			///Ƶ������
			int32_t ChannalNo;
			///ί�����(��ͬһ��channel_no��Ψһ����1��ʼ����)
			int64_t Seq;
			///ί�м۸�
			double  Price;
			///ί������
			int64_t Qty;
			///'1':��; '2':��; 'G':����; 'F':����
			char  Side;
			///�������: '1': �м�; '2': �޼�; 'U': ��������
			char OrdType;
		};

		///��ʳɽ�
		struct CIStoneTickByTickTrade 
		{
			///Ƶ������
			int32_t ChannalNo;
			///ί�����(��ͬһ��channel_no��Ψһ����1��ʼ����)
			int64_t Seq;
			///�ɽ��۸�
			double Price;
			///�ɽ���
			int64_t Qty;
			///�ɽ����(�������Ͻ���)
			double Money;
			///�򷽶�����
			int64_t BidNo;
			///����������
			int64_t AskNo;
			/// SH: �����̱�ʶ('B':������; 'S':������; 'N':δ֪)
			/// SZ: �ɽ���ʶ('4':��; 'F':�ɽ�)
			char TradeFlag;
		};

		///���������Ϣ
		struct CIStoneTickByTick 
		{
			///����������
			ISTONE_EXCHANGE_TYPE ExchangeID;
			///��Լ���루��������������Ϣ���������ո���'\0'��β
			char Ticker[ISTONE_TICKER_LEN];
			///Ԥ��
			int64_t Seq;
			///ί��ʱ�� or �ɽ�ʱ��
			int64_t DataTime;
			///ί�� or �ɽ�
			ISTONE_TBT_TYPE Type;

			union {
				CIStoneTickByTickEntrust Entrust;
				CIStoneTickByTickTrade   Trade;
			};
		};

		///����ѯ��������Ϣ
		struct CIStoneTickerPriceInfo 
		{
			///����������
			ISTONE_EXCHANGE_TYPE ExchangeID;
			///��Լ���루��������������Ϣ���������ո���'\0'��β
			char Ticker[ISTONE_TICKER_LEN];
			///���¼�
			double LastPrice;
		};
	}
}
