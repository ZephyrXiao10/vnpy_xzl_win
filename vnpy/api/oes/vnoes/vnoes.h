#include <string>
#include <queue>
#include <thread>
#include <mutex>
#include <iostream>
#include <codecvt>
#include <condition_variable>
#include <locale>

#include "pybind11/pybind11.h"


using namespace std;
using namespace pybind11;


//����ṹ��
struct Task
{
    int task_name;		//�ص��������ƶ�Ӧ�ĳ���
    void *task_data;	//����ָ��
    void *task_error;	//����ָ��
	void *task_head;
    int task_id;		//����id
    bool task_last;		//�Ƿ�Ϊ��󷵻�
	int task_int;
};

class TerminatedError : std::exception
{};

class TaskQueue
{
private:
    queue<Task> queue_;						//��׼�����
    mutex mutex_;							//������
    condition_variable cond_;				//��������

    bool _terminate = false;

public:

    //�����µ�����
    void push(const Task &task)
    {
        unique_lock<mutex > mlock(mutex_);
        queue_.push(task);					//������д�������
        mlock.unlock();						//�ͷ���
        cond_.notify_one();					//֪ͨ���������ȴ����߳�
    }

    //ȡ���ϵ�����
    Task pop()
    {
        unique_lock<mutex> mlock(mutex_);
        cond_.wait(mlock, [&]() {
            return !queue_.empty() || _terminate;
        });				//�ȴ���������֪ͨ
        if (_terminate)
            throw TerminatedError();
        Task task = queue_.front();			//��ȡ�����е����һ������
        queue_.pop();						//ɾ��������
        return task;						//���ظ�����
    }

    void terminate()
    {
        _terminate = true;
        cond_.notify_all();					//֪ͨ���������ȴ����߳�
    }
};


//���ֵ��л�ȡĳ����ֵ��Ӧ������������ֵ������ṹ������ֵ��
void getInt(const dict &d, const char *key, int *value)
{
    if (d.contains(key))		//����ֵ����Ƿ���ڸü�ֵ
    {
        object o = d[key];		//��ȡ�ü�ֵ
        *value = o.cast<int>();
    }
};


void getInt64(const dict &d, const char *key, INT64 *value)
{
	if (d.contains(key))		//����ֵ����Ƿ���ڸü�ֵ
	{
		object o = d[key];		//��ȡ�ü�ֵ
		*value = o.cast<INT64>();
	}
};

void getInt32(const dict &d, const char *key, INT32 *value)
{
	if (d.contains(key))		//����ֵ����Ƿ���ڸü�ֵ
	{
		object o = d[key];		//��ȡ�ü�ֵ
		*value = o.cast<INT32>();
	}
};

void getInt8(const dict &d, const char *key, INT8 *value)
{
	if (d.contains(key))		//����ֵ����Ƿ���ڸü�ֵ
	{
		object o = d[key];		//��ȡ�ü�ֵ
		*value = o.cast<INT8>();
	}
};


void getInt16(const dict &d, const char *key, INT16 *value)
{
	if (d.contains(key))		//����ֵ����Ƿ���ڸü�ֵ
	{
		object o = d[key];		//��ȡ�ü�ֵ
		*value = o.cast<INT16>();
	}
};

void getUint8(const dict &d, const char *key, UINT8 *value)
{
	if (d.contains(key))		//����ֵ����Ƿ���ڸü�ֵ
	{
		object o = d[key];		//��ȡ�ü�ֵ
		*value = o.cast<UINT8>();
	}
};

void getUint32(const dict &d, const char *key, UINT32 *value)
{
	if (d.contains(key))		//����ֵ����Ƿ���ڸü�ֵ
	{
		object o = d[key];		//��ȡ�ü�ֵ
		*value = o.cast<UINT32>();
	}
};

//���ֵ��л�ȡĳ����ֵ��Ӧ�ĸ�����������ֵ������ṹ������ֵ��
void getDouble(const dict &d, const char *key, double *value)
{
    if (d.contains(key))
    {
        object o = d[key];
        *value = o.cast<double>();
    }
};


//���ֵ��л�ȡĳ����ֵ��Ӧ���ַ�������ֵ������ṹ������ֵ��
void getChar(const dict &d, const char *key, char *value)
{
    if (d.contains(key))
    {
        object o = d[key];
        *value = o.cast<char>();
    }
};


template <size_t size>
using string_literal = char[size];

//���ֵ��л�ȡĳ����ֵ��Ӧ���ַ���������ֵ������ṹ������ֵ��
template <size_t size>
void getString(const pybind11::dict &d, const char *key, string_literal<size> &value)
{
    if (d.contains(key))
    {
        object o = d[key];
        string s = o.cast<string>();
        const char *buf = s.c_str();
        strcpy(value, buf);
    }
};

//��GBK������ַ���ת��ΪUTF8
inline string toUtf(const string &gb2312)
{
#ifdef _MSC_VER
    const static locale loc("zh-CN");
#else
    const static locale loc("zh_CN.GB18030");
#endif

    vector<wchar_t> wstr(gb2312.size());
    wchar_t* wstrEnd = nullptr;
    const char* gbEnd = nullptr;
    mbstate_t state = {};
    int res = use_facet<codecvt<wchar_t, char, mbstate_t> >
        (loc).in(state,
            gb2312.data(), gb2312.data() + gb2312.size(), gbEnd,
            wstr.data(), wstr.data() + wstr.size(), wstrEnd);

    if (codecvt_base::ok == res)
    {
        wstring_convert<codecvt_utf8<wchar_t>> cutf8;
        return cutf8.to_bytes(wstring(wstr.data(), wstrEnd));
    }

    return string();
}
