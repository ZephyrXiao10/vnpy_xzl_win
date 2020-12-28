// vnctpmd.cpp : ���� DLL Ӧ�ó���ĵ���������
//

#include "vnoesmd.h"
#include    <mds_api/mds_async_api.h>
#include    <mds_api/parser/mds_protocol_parser.h>
#include    <sutil/compiler.h>
#include    <sutil/string/spk_strings.h>
#include    <sutil/logger/spk_log.h>
#include    <iostream>

///-------------------------------------------------------------------------------------
///C++�Ļص����������ݱ��浽������
///-------------------------------------------------------------------------------------


int32 MdOnConnected(MdsAsyncApiChannelT *pAsyncChannel, void *pCallbackParam)
{
	if (md_api)
	{
		md_api->OnConnected(pAsyncChannel);
	};
	return 0;
};


int32 MdOnDisConnected(MdsAsyncApiChannelT *pAsyncChannel, void *pCallbackParam)
{
	if (md_api)
	{
		md_api->OnDisConnected(pAsyncChannel);
	};
	return 0;
};

int32 MdOnData(MdsApiSessionInfoT *pSessionInfo, SMsgHeadT *pMsgHead, void *pMsgBody, void *pCallbackParams)
{
	if (md_api)
	{
		md_api->OnData(pSessionInfo,pMsgHead,pMsgBody, pCallbackParams);
	};
	return 0;
};

int32 MdApi::OnConnected(MdsAsyncApiChannelT *pAsyncChannel)
{
	this->channel = pAsyncChannel;
	MdsApiSubscribeInfoT    *pSubscribeInfo = (MdsApiSubscribeInfoT *)NULL;

	if (pAsyncChannel->pChannelCfg->channelType == MDSAPI_CHANNEL_TYPE_TCP) {
		/* ��ȡ�ر�ͨ����Ӧ�Ķ���������Ϣ */
		pSubscribeInfo = MdsAsyncApi_GetChannelSubscribeCfg(pAsyncChannel);
		if (__spk_unlikely(!pSubscribeInfo)) {
			SLOG_ERROR("Illegal extended subscribe info! " \
				"pAsyncChannel[%p], channelTag[%s]",
				pAsyncChannel, pAsyncChannel->pChannelCfg->channelTag);
			SLOG_ASSERT(0);

			SPK_SET_ERRNO(EINVAL);
			return SPK_NEG(EINVAL);
		}
	}

	int32 dataTypes = MDS_SUB_DATA_TYPE_L1_SNAPSHOT;


	SLOG_ASSERT(pAsyncChannel && pAsyncChannel->pSessionInfo
		&& pAsyncChannel->pChannelCfg);

	/* ����SubscribeByString�ӿ�ʹ�õ�����ģʽ (tickType=1) */
	MdsApi_SetThreadSubscribeTickType(MDS_TICK_TYPE_LATEST_SIMPLIFIED);

	/* ����SubscribeByString�ӿ�ʹ�õ�������ݵ������ؽ���ʶ (ʵʱ����+�ؽ�����) */
	MdsApi_SetThreadSubscribeTickRebuildFlag(
		MDS_TICK_REBUILD_FLAG_EXCLUDE_REBUILDED);

	/* ����SubscribeByString�ӿ�ʹ�õĳ�ʼ���ն��ı�־ (isRequireInitialMktData) */
	MdsApi_SetThreadSubscribeRequireInitMd(FALSE);

	/* ���������Ϻ���Ʊ/ծȯ/����� Level-2 ���� */
	if (!MdsAsyncApi_SubscribeByString(pAsyncChannel,
		"000001.SH", (char *)NULL,
		MDS_EXCH_SSE, MDS_MD_PRODUCT_TYPE_INDEX, MDS_SUB_MODE_SET,
		dataTypes)) {
		SLOG_ERROR("������ָ֤������ʧ��!");
		return EFAULT;
	}


	SLOG_INFO("��������ɹ�! channelTag[%s]",
		pAsyncChannel->pChannelCfg->channelTag);

	int channelType = (eMdsApiChannelTypeT)pAsyncChannel->pChannelCfg->channelType;
	MdsApiSessionInfoT *pSessionInfo = pAsyncChannel->pSessionInfo;

	Task task = Task();
	task.task_name = ONCONNECTED;
	task.task_int = channelType;

	if (pSessionInfo)
	{
		MdsApiSessionInfoT *task_data = new MdsApiSessionInfoT();
		*task_data = *pSessionInfo;
		task.task_data = task_data;
	}
	this->task_queue.push(task);

	return EAGAIN;

};



int32 MdApi::OnDisConnected(MdsAsyncApiChannelT *pAsyncChannel)
{

	int channelType = (eMdsApiChannelTypeT)pAsyncChannel->pChannelCfg->channelType;
	MdsApiSessionInfoT *pSessionInfo = pAsyncChannel->pSessionInfo;

	Task task = Task();
	task.task_name = ONDISCONNECTED;
	task.task_int = channelType;

	if (pSessionInfo)
	{
		MdsApiSessionInfoT *task_data = new MdsApiSessionInfoT();
		*task_data = *pSessionInfo;
		task.task_data = task_data;
	}
	this->task_queue.push(task);

	return EAGAIN;
};



void MdApi::OnRtnStockData(const MdsMktDataSnapshotHeadT *head, const MdsStockSnapshotBodyT *stock)
{
	Task task = Task();
	task.task_name = ONRTNSTOCKDATA;
	if (head)
	{
		MdsMktDataSnapshotHeadT * task_head = new MdsMktDataSnapshotHeadT();
		*task_head = *head;
		task.task_head = task_head;
	}
	if (stock)
	{
		MdsStockSnapshotBodyT * task_data = new MdsStockSnapshotBodyT();
		*task_data = *stock;
		task.task_data = task_data;
	}
	this->task_queue.push(task);

};

int32 MdApi::OnData(MdsApiSessionInfoT *pSessionInfo, SMsgHeadT *pMsgHead, void *pMsgBody, void *pCallbackParams = NULL)
{
	MdsMktRspMsgBodyT	*pRspMsg = (MdsMktRspMsgBodyT *)pMsgBody;
	MdsMktDataSnapshotT	*pRptMsg = &pRspMsg->mktDataSnapshot;
	switch (pMsgHead->msgId) {

	cout << "@msgid" << pMsgHead->msgId << endl;
	case MDS_MSGTYPE_MARKET_DATA_SNAPSHOT_FULL_REFRESH: /** Level1 �г�������� (10/0x0A) */
		this->OnRtnStockData(&pRptMsg->head, &pRptMsg->stock);
		break;

	case MDS_MSGTYPE_INDEX_SNAPSHOT_FULL_REFRESH: /** Level1 �г�������� (10/0x0A) */
		this->OnRtnIndexData(&pRptMsg->head, &pRptMsg->index);
		break;

	case MDS_MSGTYPE_OPTION_SNAPSHOT_FULL_REFRESH: /** Level1/Level2 ��Ȩ������� (12/0x0C) */
		this->OnRtnOptionData(&pRptMsg->head, &pRptMsg->option);
		break;
	}
	return 0;
}




void MdApi::OnRtnIndexData(const MdsMktDataSnapshotHeadT *head, const MdsIndexSnapshotBodyT *index)
{
	Task task = Task();
	task.task_name = ONRTNINDEXDATA;
	if (head)
	{
		MdsMktDataSnapshotHeadT * task_head = new MdsMktDataSnapshotHeadT();
		*task_head = *head;
		task.task_head = task_head;
	}
	if (index)
	{
		MdsIndexSnapshotBodyT * task_data = new MdsIndexSnapshotBodyT();
		*task_data = *index;
		task.task_data = task_data;
	}
	this->task_queue.push(task);

};


void MdApi::OnRtnOptionData(const MdsMktDataSnapshotHeadT *head, const MdsStockSnapshotBodyT *option)
{
	Task task = Task();
	task.task_name = ONRTNOPTIONDATA;
	if (head)
	{
		MdsMktDataSnapshotHeadT * task_head = new MdsMktDataSnapshotHeadT();
		*task_head = *head;
		task.task_head = task_head;
	}
	if (option)
	{
		MdsStockSnapshotBodyT * task_data = new MdsStockSnapshotBodyT();
		*task_data = *option;
		task.task_data = task_data;
	}
	this->task_queue.push(task);

};



///-------------------------------------------------------------------------------------
///�����̴߳Ӷ�����ȡ�����ݣ�ת��Ϊpython����󣬽�������
///-------------------------------------------------------------------------------------

void MdApi::processTask()
{
	try
	{
		while (this->active)
		{
			Task task = this->task_queue.pop();

			switch (task.task_name)
			{
			case ONCONNECTED:
			{
				this->processConnected(&task);
				break;
			}

			case ONDISCONNECTED:
			{
				this->processDisconnected(&task);
				break;
			}

			case ONRTNSTOCKDATA:
			{
				this->processRtnStockData(&task);
				break;
			}

			case ONRTNINDEXDATA:
			{
				this->processRtnIndexData(&task);
				break;
			}
			case ONRTNOPTIONDATA:
			{
				this->processRtnOptionData(&task);
				break;
			}

			};
		}
	}
	catch (const TerminatedError&)
	{
	}
};

int32 MdApi::processConnected(Task *task)
{
	gil_scoped_acquire acquire;
	dict data;
	if (task->task_data)
	{
		MdsApiSessionInfoT *task_data = (MdsApiSessionInfoT*)task->task_data;
		data["remoteHostNum"] = task_data->remoteHostNum;
		delete task_data;
	}
	this->onConnected(task->task_int, data);
};

int32 MdApi::processDisconnected(Task *task)
{
	gil_scoped_acquire acquire;
	dict data;
	if (task->task_data)
	{
		MdsApiSessionInfoT *task_data = (MdsApiSessionInfoT*)task->task_data;
		data["remoteHostNum"] = task_data->remoteHostNum;
		delete task_data;
	}
	this->onDisConnected(task->task_int, data);
};

void MdApi::processRtnStockData(Task *task)
{
	gil_scoped_acquire acquire;
	dict head;
	if (task->task_head)
	{
		MdsMktDataSnapshotHeadT *task_head = (MdsMktDataSnapshotHeadT*)task->task_head;
		head["exchId"] = task_head->exchId;
		head["mdProductType"] = task_head->mdProductType;
		head["tradeDate"] = task_head->tradeDate;
		head["updateTime"] = task_head->updateTime;
		head["instrId"] = task_head->instrId;
		delete task_head;
	}
	dict data;
	if (task->task_data)
	{
		MdsStockSnapshotBodyT * task_data = (MdsStockSnapshotBodyT*)task->task_data;
		data["SecurityID"] = task_data->SecurityID;
		data["TradingPhaseCode"] = task_data->TradingPhaseCode;
		data["__filler"] = task_data->__filler;
		data["NumTrades"] = task_data->NumTrades;
		data["TotalVolumeTraded"] = task_data->TotalVolumeTraded;
		data["TotalValueTraded"] = task_data->TotalValueTraded;
		data["PrevClosePx"] = task_data->PrevClosePx;
		data["OpenPx"] = task_data->OpenPx;
		data["HighPx"] = task_data->HighPx;
		data["LowPx"] = task_data->LowPx;
		data["TradePx"] = task_data->TradePx;
		data["ClosePx"] = task_data->ClosePx;
		data["IOPV"] = task_data->IOPV;
		data["NAV"] = task_data->NAV;
		data["TotalLongPosition"] = task_data->TotalLongPosition;

		pybind11::list ask;
		pybind11::list bid;
		pybind11::list ask_qty;
		pybind11::list bid_qty;

		for (int i = 0; i < 5; i++)
		{
			ask.append(task_data->OfferLevels[i].Price);
			bid.append(task_data->BidLevels[i].Price);
			ask_qty.append(task_data->OfferLevels[i].OrderQty);
			bid_qty.append(task_data->BidLevels[i].OrderQty);
		}

		data["ask"] = ask;
		data["bid"] = bid;
		data["bid_qty"] = bid_qty;
		data["ask_qty"] = ask_qty;

		delete task_data;
	}
	this->onRtnStockData(head, data);

};

void MdApi::processRtnOptionData(Task *task)
{
	gil_scoped_acquire acquire;
	dict head;
	if (task->task_head)
	{
		MdsMktDataSnapshotHeadT *task_head = (MdsMktDataSnapshotHeadT*)task->task_head;
		head["exchId"] = task_head->exchId;
		head["mdProductType"] = task_head->mdProductType;
		head["tradeDate"] = task_head->tradeDate;
		head["updateTime"] = task_head->updateTime;
		head["instrId"] = task_head->instrId;
		delete task_head;
	}
	dict data;
	if (task->task_data)
	{
		MdsStockSnapshotBodyT * task_data = (MdsStockSnapshotBodyT*)task->task_data;
		data["SecurityID"] = task_data->SecurityID;
		data["TradingPhaseCode"] = task_data->TradingPhaseCode;
		data["__filler"] = task_data->__filler;
		data["NumTrades"] = task_data->NumTrades;
		data["TotalVolumeTraded"] = task_data->TotalVolumeTraded;
		data["TotalValueTraded"] = task_data->TotalValueTraded;
		data["PrevClosePx"] = task_data->PrevClosePx;
		data["OpenPx"] = task_data->OpenPx;
		data["HighPx"] = task_data->HighPx;
		data["LowPx"] = task_data->LowPx;
		data["TradePx"] = task_data->TradePx;
		data["ClosePx"] = task_data->ClosePx;
		data["IOPV"] = task_data->IOPV;
		data["NAV"] = task_data->NAV;
		data["TotalLongPosition"] = task_data->TotalLongPosition;

		pybind11::list ask;
		pybind11::list bid;
		pybind11::list ask_qty;
		pybind11::list bid_qty;

		for (int i = 0; i < 5; i++)
		{
			ask.append(task_data->OfferLevels[i].Price);
			bid.append(task_data->BidLevels[i].Price);
			ask_qty.append(task_data->OfferLevels[i].OrderQty);
			bid_qty.append(task_data->BidLevels[i].OrderQty);
		}

		data["ask"] = ask;
		data["bid"] = bid;
		data["bid_qty"] = bid_qty;
		data["ask_qty"] = ask_qty;

		delete task_data;
	}
	this->onRtnOptionData(head, data);

};

void MdApi::processRtnIndexData(Task *task)
{
	gil_scoped_acquire acquire;
	dict head;
	if (task->task_head)
	{
		MdsMktDataSnapshotHeadT *task_head = (MdsMktDataSnapshotHeadT*)task->task_head;
		head["exchId"] = task_head->exchId;
		head["mdProductType"] = task_head->mdProductType;
		head["tradeDate"] = task_head->tradeDate;
		head["updateTime"] = task_head->updateTime;
		head["instrId"] = task_head->instrId;
		delete task_head;
	}
	dict data;
	if (task->task_data)
	{
		MdsIndexSnapshotBodyT * task_data = (MdsIndexSnapshotBodyT*)task->task_data;
		data["SecurityID"] = task_data->SecurityID;
		data["TradingPhaseCode"] = task_data->TradingPhaseCode;
		data["__filler"] = task_data->__filler;
		data["NumTrades"] = task_data->NumTrades;
		data["TotalVolumeTraded"] = task_data->TotalVolumeTraded;
		data["TotalValueTraded"] = task_data->TotalValueTraded;
		data["PrevCloseIdx"] = task_data->PrevCloseIdx;
		data["OpenIdx"] = task_data->OpenIdx;
		data["HighIdx"] = task_data->HighIdx;
		data["LowIdx"] = task_data->LowIdx;
		data["LastIdx"] = task_data->LastIdx;
		data["CloseIdx"] = task_data->CloseIdx;
		data["StockNum"] = task_data->StockNum;
		data["__filler1"] = task_data->__filler1;
		delete task_data;
	}
	this->onRtnIndexData(head, data);
};


///-------------------------------------------------------------------------------------
///��������
///-------------------------------------------------------------------------------------


bool MdApi::createMdApi(string pCfgFile, string username, string password)
{
	md_api = this;

	MdsApiClientCfgT        tmpApiCfg;
	MdsAsyncApiContextT     *pAsyncContext = (MdsAsyncApiContextT *)NULL;
	MdsAsyncApiChannelT     *pTcpChannel = (MdsAsyncApiChannelT *)NULL;

	/* ��ʼ����־��¼�� */
	if (!MdsApi_InitLogger((char*)pCfgFile.c_str(), MDSAPI_CFG_DEFAULT_SECTION_LOGGER)) {
		SLOG_ERROR("��ʼ��API��¼��ʧ��! cfgFile[%s], cfgSection[%s]",
			(char*)pCfgFile.c_str(), MDSAPI_CFG_DEFAULT_SECTION_LOGGER);
		return FALSE;
	}



	/* �����߳��û����� ����*/
	MdsApi_SetThreadUsername((char*)username.c_str());
	MdsApi_SetThreadPassword((char*)password.c_str());




	/* ���������ļ� */
	memset(&tmpApiCfg, 0, sizeof(MdsApiClientCfgT));
	if (!MdsApi_ParseAllConfig((char*)pCfgFile.c_str(), &tmpApiCfg)) {
		SLOG_ERROR("���������ļ�ʧ��! cfgFile[%s]", (char*)pCfgFile.c_str());
		return FALSE;
	}


	if (&tmpApiCfg != &_apiCfg) {
		memcpy(&_apiCfg, &tmpApiCfg, sizeof(MdsApiClientCfgT));
	}

	/* �����첽API������ʱ���� (��ʼ����־, ���������Ļ���) */
	pAsyncContext = MdsAsyncApi_CreateContext((char*)pCfgFile.c_str());
	if (!pAsyncContext) {
		SLOG_ERROR("�����첽API������ʱ����ʧ��!");
		return FALSE;
	}


	/* ��ӳ�ʼ��TCPͨ�� */
	if (_apiCfg.tcpChannelCfg.addrCnt > 0) {
		pTcpChannel = MdsAsyncApi_AddChannel(
			pAsyncContext,
			MDSAPI_CFG_DEFAULT_KEY_TCP_ADDR,
			&_apiCfg.tcpChannelCfg,
			(MdsApiSubscribeInfoT *)NULL,
			MdOnData,  nullptr,
			MdOnConnected, nullptr,
			MdOnDisConnected, nullptr
		);
		if (__spk_unlikely(!pTcpChannel)) {
			SLOG_ERROR("���TCPͨ��ʧ��! channelTag[%s]",
				MDSAPI_CFG_DEFAULT_KEY_TCP_ADDR);
			MdsAsyncApi_ReleaseContext(pAsyncContext);
			return FALSE;
		}
	}


	_pAsyncContext = pAsyncContext;
	_pDefaultTcpChannel = pTcpChannel;
	_isInitialized = TRUE;
	return TRUE;

}


bool MdApi::init()
{
	this->active = true;
	this->task_thread = thread(&MdApi::processTask, this);

	////�������׽ӿ�ʵ�� 

		/* �����첽API�߳� (����TCPͨ����UDPͨ��) */
		if (MdsAsyncApi_GetChannelCount(_pAsyncContext) > 0) {
			MdsAsyncApi_SetPreconnectAble(_pAsyncContext, TRUE);

			if (!MdsAsyncApi_Start(_pAsyncContext)) {
				SLOG_ERROR("�����첽API�߳�ʧ��! error[%d - %s]",
					MdsApi_GetLastError(),
					MdsApi_GetErrorMsg(MdsApi_GetLastError()));

				exit();
				return FALSE;
			}
		}
		else {
			SLOG_INFO("δ����TCPͨ����UDPͨ��, �������Ӳ�ѯͨ��, ���������첽API�߳�!");
		}

		SLOG_INFO("�������׽ӿ�ʵ��...");
		_isRunning = TRUE;

		return TRUE;
}



int MdApi::exit()
{
	this->active = false;
	this->task_queue.terminate();
	this->task_thread.join();

	/* ֹͣ�������첽API�߳� */
	if (_pAsyncContext) {
		MdsAsyncApi_Stop(_pAsyncContext);
		MdsAsyncApi_ReleaseContext(_pAsyncContext);
		_pAsyncContext = NULL;
	}

	_isRunning = FALSE;
	this->api = NULL;
	return 1;
}


bool MdApi::subscribeMarketData(string symbol, int exchange, int product_type)
{
	/* ����SubscribeByString�ӿ�ʹ�õ�����ģʽ (tickType=1) */
	MdsApi_SetThreadSubscribeTickType(MDS_TICK_TYPE_LATEST_SIMPLIFIED);

	/* ����SubscribeByString�ӿ�ʹ�õ�������ݵ������ؽ���ʶ (ʵʱ����+�ؽ�����) */
	MdsApi_SetThreadSubscribeTickRebuildFlag(
		MDS_TICK_REBUILD_FLAG_EXCLUDE_REBUILDED);

	/* ����SubscribeByString�ӿ�ʹ�õĳ�ʼ���ն��ı�־ (isRequireInitialMktData) */
	MdsApi_SetThreadSubscribeRequireInitMd(FALSE);
	return MdsAsyncApi_SubscribeByString(
		this->channel,
		(char*)symbol.c_str(), (char *)NULL,
		(eMdsExchangeIdT)exchange,(eMdsMdProductTypeT)product_type,
		MDS_SUB_MODE_APPEND,
		MDS_SUB_DATA_TYPE_L1_SNAPSHOT);

}


///-------------------------------------------------------------------------------------
///Boost.Python��װ
///-------------------------------------------------------------------------------------

class PyMdApi : public MdApi
{
public:
	using MdApi::MdApi;

	void onConnected(int channelType, const dict &data) override
	{
		try
		{
			PYBIND11_OVERLOAD(void, MdApi, onConnected, channelType, data);
		}
		catch (const error_already_set &e)
		{
			cout << e.what() << endl;
		}
	};

	void onDisConnected(int channelType, const dict &data) override
	{
		try
		{
			PYBIND11_OVERLOAD(void, MdApi, onDisConnected, channelType, data);
		}
		catch (const error_already_set &e)
		{
			cout << e.what() << endl;
		}
	};

	void onRtnStockData(const dict &head, const dict &data) override
	{
		try
		{
			PYBIND11_OVERLOAD(void, MdApi, onRtnStockData, head, data);
		}
		catch (const error_already_set &e)
		{
			cout << e.what() << endl;
		}
	};
	void onRtnOptionData(const dict &head, const dict &data) override
	{
		try
		{
			PYBIND11_OVERLOAD(void, MdApi, onRtnOptionData, head, data);
		}
		catch (const error_already_set &e)
		{
			cout << e.what() << endl;
		}
	};

	void onRtnIndexData(const dict &head, const dict &data) override
	{
		try
		{
			PYBIND11_OVERLOAD(void, MdApi, onRtnIndexData, head, data);
		}
		catch (const error_already_set &e)
		{
			cout << e.what() << endl;
		}
	};

};

PYBIND11_MODULE(vnoesmd, m)
{
	class_<MdApi, PyMdApi> MdApi(m, "MdApi", module_local());
	MdApi
		.def(init<>())
		.def("createMdApi", &MdApi::createMdApi)
		.def("init", &MdApi::init)
		.def("exit", &MdApi::exit)
		.def("subscribeMarketData", &MdApi::subscribeMarketData)


		.def("onConnected", &MdApi::onConnected)
		.def("onDisConnected", &MdApi::onDisConnected)
		.def("OnRtnStockData", &MdApi::OnRtnStockData)
		.def("OnRtnIndexData", &MdApi::OnRtnIndexData)
		.def("OnRtnOptionData", &MdApi::OnRtnOptionData)
		;

}
