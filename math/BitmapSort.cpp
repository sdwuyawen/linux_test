/*****位图排序法**********/
/* 条件：
 * 1. 正整数的表示范围（取值返回）， 才好决定需要多少个bits来记录
 * 2. 这些元素没有重复 
 * 方法:
 * 1. 分配bits数组, 并清零
 * 2. 扫描数据，并设置相应的bit
 * 3. 扫描完后，读取bits来输出排好的结果
 */

#include <stdio.h>
#include <memory.h>

#define BYTESIZE 8

void SetBit(unsigned char *data,int pos)
{
	int i;
	
    for (i=0;i<pos/BYTESIZE;i++) {
        data++;
    }

    *data=(*data)|(0x01<<(pos%BYTESIZE));
}

void BitmapSort(int data[],int n)
{
	int i;
	
    if (NULL==data || n<=0) {
        return;
    }

    int max=data[0];
    for (i=1;i<n;i++) {
        if (data[i]>max) {
            max=data[i];
        }
    }

    int iByteNum=max/BYTESIZE+1;

    unsigned char *p=new unsigned char[iByteNum];
    unsigned char *q=p;
    memset(p,0,iByteNum);
    int *temp=new int[n];
    int num=0;

    for(i=0;i<n;i++) {
        SetBit(p,data[i]);
    }
    for (i=0;i<iByteNum;i++) {
        for (int j=0;j<BYTESIZE;j++) {
            if (((*p)&(0x1<<j))==(0x1<<j)) {
                temp[num]=i*BYTESIZE+j;
                num++;
            }
        }
        p++;
    }
    memcpy(data,temp,sizeof(int)*n);
    delete []q;
    delete []temp;

}

int main()
{
    int data[] = {8,7,1,2,4,5,12,11,10};
    
    BitmapSort(data,9);
    for (int i=0;i<9;i++) {
        printf("%d ", data[i]);
    }
    printf("\n");
    
    return 0;
}
