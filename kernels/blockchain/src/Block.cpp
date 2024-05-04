#include"Block.h"
#include"sha256.h"
#include"time.h"
#include<sstream>
Block::Block(uint32_t nIndexIn, const string &sDataIn) :_nIndex(nIndexIn), _sData(sDataIn)
//构造函数Block的两个参数为nIndexIn和sDataIn，分别赋值到Block中的_nIndex和_sData（构造函数初始化用法）
{
	_nNonce = -1;//Nounce设置为-1
	_tTime = time(nullptr);//设置时间
}
string Block::GetHash()//返回哈希值函数的实现
{
	return _sHash;
}
void Block::MineBlock(uint32_t nDifficulty)//挖矿函数，参数为难度值。
{
	//char cstr[nDifficulty + 1];
	char cstr[10 + 1];//这个数组实际上设置多大都可以，但是要大于nDifficulty的值
	for (uint32_t i = 0; i < nDifficulty; ++i)//填充数组，使数组的前nDifficulty位都为0，作为难度。
	{
		cstr[i] = '0';
	}
	cstr[nDifficulty] = '\0';
	string str(cstr);//创建一个string类的对象，初始化为cstr（将字符串数组转换为string类对象）

	do
	{
		_nNonce++;
		_sHash = _CalculateHash();

	} while (_sHash.substr(0, nDifficulty) != str);//substr表示从下标0开始--->nDifficulty的内容
	//要寻找一个Nounce使得总体哈希值的前n位的0（即0的个数）和难度值的个数相同，则挖矿成功。
	cout << "Block mined:" << _sHash << endl;
}
inline string Block::_CalculateHash() const
{
	stringstream ss;//该对象可以通过<<接收多个数据，保存到ss对象中，并通过str方法，将内容赋给一个string对象
	ss << _nIndex << _tTime << _sData << _nNonce << sPrevHash;
	//return sha256(ss.str());
	return sha256(sha256(ss.str()));
}