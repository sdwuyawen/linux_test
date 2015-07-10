/*
 * 	@filename :
 *		test_lfsr.c
 * 
 * 	@note : 
 * 		线性反馈移位寄存器(LFSR)是内测试电路中最基本的标准模块结构，既用作伪随机测试码产生器，也作为压缩测试结果数据的特征分析器		
 *		一个n阶的LFSR由n个触发器和若干个异或门组成。在实际应用当中，主要用到两种类型的LFSR，即
 * 		1). 异或门外接线性反馈移位寄存器（IE型LFSR）
 * 		2). 异或门内接线性反馈移位寄存器（EE型LFSR）
 * 		一个n位的LFSR能够在重复之前产生2^n－1个的伪随机序列
 * 		M(X)输入序列
 * 		G(X)多项式
 * 		G(X) = gn * X^n + ... + g1 * X + g0
 * 		gn是反馈系数 取值为0或1, 是否有反馈
 */

#include <stdio.h>
#include <inttypes.h>

/*
 n = 8
 g0 = 1
 g1 = 1
 g2 = 1
 g8 = 1
 属于IE型LFSR
 */
static uint8_t test_lfsr_get_8bit(uint8_t seed, uint8_t max_limit)
{
  static uint8_t pseudoreg = 0xff; // Can never be zero
  uint8_t shiftbit;

  if(seed > 0) {
    pseudoreg = seed;
  }

  shiftbit = (pseudoreg << 7) & 0x80;
  shiftbit ^= (pseudoreg << 6) & 0x80;
  shiftbit ^= (pseudoreg << 5) & 0x80;
  shiftbit ^= (pseudoreg & 0x80);

  pseudoreg = (shiftbit | (pseudoreg >> 1));

  //return pseudoreg % max_limit;
  return pseudoreg;
}

int main(int argc, char *argv[])
{
	int i = 0; 
	int seed = 0;
	int max_cnt = 5;
	
	if (argc > 1) {
		seed = atoi(argv[1]);
		if (argc > 2) {
			max_cnt = atoi(argv[2]);
		}
	}

	test_lfsr_get_8bit(seed, 1);
	for (i=0; i<max_cnt; i++) {
		printf("%d\n", test_lfsr_get_8bit(0, max_cnt));
	}
	
	return 0;
}
