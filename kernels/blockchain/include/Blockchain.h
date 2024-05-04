#pragma once
#include"Block.h"
#include<vector>//向量库
class Blockchain
{
public:
	Blockchain();//默认构造函数
	void AddBlock(Block bNew);//增加区块函数
private:
	uint32_t _nDifficulty;//难度值
	vector<Block> _vChain;//保存区块的变量
	Block _GetLastBlock() const;//获取最新的区块，由const关键字，表示输出的内容不可更改
};