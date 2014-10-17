/**
 * 	@filename :
 * 		test_size_class.cpp
 * 
 * 	@note :
 * 		./bin/test_size_class输出:
 * 			head2 id = 1
 * 			head3 id = 2
 * 			sizeof(TestHeader) = 8
 * 			TestHeader::next_id=3
 * 		每次调用TestHeader xxx(x1, x2), id=next_id++, 所以调用两次之后next_id=3;
 * 		sizeof(TestHeader)=8 表示TestHeader的大小包括实例的所有成员变量的大小，不包括静态变量和函数。
 **/
#include <stdio.h>
#include <stdint.h>

struct TestHeader
{
  uint16_t from_node; /**< Logical address where the message was generated */
  uint16_t to_node; /**< Logical address where the message is going */
  uint16_t id; /**< Sequential message ID, incremented every message */
  unsigned char type; /**< Type of the packet.  0-127 are user-defined types, 128-255 are reserved for system */
  unsigned char reserved; /**< Reserved for future use */

  static uint16_t next_id; /**< The message ID of the next message to be sent */

  TestHeader() {}

  TestHeader(uint16_t _to, unsigned char _type = 0): to_node(_to), id(next_id++), type(_type&0x7f) {}

};

uint16_t TestHeader::next_id = 1;

int main(void) 
{
	TestHeader header1;
	TestHeader header2(0, 'c');
	TestHeader header3(0, 'k');
	
	printf("head2 id = %d\n", header2.id);
	printf("head3 id = %d\n", header3.id);
	printf("sizeof(TestHeader) = %d\n", sizeof(TestHeader));
	printf("TestHeader::next_id=%d\n", TestHeader::next_id);
	
	return 0;
}
