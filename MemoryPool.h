/**
* @file MemoryPool.h
* @brief �ṩԤ�ȷ����ڴ��, �����ڴ���Ƭ, ��߷����ٶ�
* @author nzl
*
* ��ע: MemoryPool��ʹ��, Ҫ��ÿ��poolunit�Ĵ�С��poolunit����֮��ƽ����и��õ�����
* poolunit����������8���¿������ܸ���, ��ϵͳƽ��״̬ռ���ڴ�Ϊ5M, ÿ�����С����Ϊ4M
*
*/
#ifndef XQ_MEMORYPOOL_H
#define XQ_MEMORYPOOL_H

namespace idee
{

#define MAX_POOL_NUM (128) //Ĭ�ϵ��ڴ�صĸ���

//#pragma pack(1)
//struct poolunit{
//struct buddy		*alloc;//�ڴ������
//char				*buffer;//������
//};
//#pragma pack()


class XQ_EXPORT MemoryPool
{

struct poolunit
{
	struct buddy		*alloc;//�ڴ������
	char				*buffer;//������
};

private:
	unsigned short	block;					//��С��
	unsigned short	level;					//����
	unsigned int	size;					//����(����=2�ļ����η�)
	struct poolunit	pools[MAX_POOL_NUM];	//�ڴ�ص�Ԫ
	unsigned short	used;					//�Ѿ����ٵ��ڴ������
	
	Mutex locker_;							//��
public:
	MemoryPool();
	

	~MemoryPool();
	

/**
* @brief init
* ����һ���µ��ڴ��
* @param block �����ڴ���С��С�������ڴ�ؿ��Է������С��λ��
* @param maxsize �ڴ��block��������ʾ�ڴ�ؿ��Թ�����ڴ�Ĵ�С
* @return 
*
* maxsize�����2��n�η����룺���紫��1000�����1024������1038����Ϊ2048
*/
void init(unsigned short block, unsigned int maxsize);

/**
* @brief release
* ����: �����ڴ��
* @param pool �ڴ�ص�ַ
* @return: N/A
*
* ��ע: N/A
*/
void release();

/**
* @brief pool_alloc
* ����: ���ڴ�ط���һ���ڴ�
* @praam len ���ݳ���(������Ȳ��ܱ�maxsize����)
* @return char* ����������ڴ��ַ
*
* ��ע: ��С���䳤��Ϊ���ڴ�ص�һ����С��λ��block�Ĵ�С��
*/
char* pool_alloc(unsigned int len);

/**
* @brief pool_free
* ����: ���ڴ���ͷ�һ���ڴ�
* @param buf �ڴ��ַ
* @return 
*
* ��ע: N/A
*/
void pool_free(char* buf);

};

} //end idee
#endif