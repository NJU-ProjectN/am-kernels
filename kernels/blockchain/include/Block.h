#pragma once
#include<cstdint>//包含了uint32_t等无符号整型
#include<iostream>//标准输入输出库
using namespace std;

//创建区块类
class Block
{
public:
	string sPrevHash;//前一个区块的哈希值
	Block(uint32_t nIndexIn, const string &sDataIn);//构造函数
	string GetHash();//返回哈希值
	void MineBlock(uint32_t nDifficulty);//挖矿，其参数nDifficulty表示指定的难度值

private:
	uint32_t _nIndex;//区块索引值，第几个区块，从0开始计算
	int64_t _nNonce;//区块随机数
	string _sData;//区块描述字符
	string _sHash;//区块Hash值
	time_t _tTime;//区块生成时间
	string _CalculateHash() const;//计算Hash值，const保证输出的函数值不能被改变。
};