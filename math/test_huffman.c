/**
 * 	@filename :
 * 		test_huffman.c
 * 
 * 	@note :
 * 
 * 
 **/
#if 0
压缩过程
	压缩代码非常简单，首先用ASCII值初始化511个哈夫曼节点：
	CHuffmanNode nodes[511];
	for(int nCount = 0; nCount < 256; nCount++)
	nodes[nCount].byAscii = nCount;
	其次，计算在输入缓冲区数据中，每个ASCII码出现的频率：
	for(nCount = 0; nCount < nSrcLen; nCount++)
	nodes[pSrc[nCount]].nFrequency++;
	然后，根据频率进行排序：
	qsort(nodes, 256, sizeof(CHuffmanNode), frequencyCompare);
	哈夫曼树，获取每个ASCII码对应的位序列：
	int nNodeCount = GetHuffmanTree(nodes);[3]
	构造哈夫曼树
	构造哈夫曼树非常简单，将所有的节点放到一个队列中，用一个节点替换两个频率最低的节点，新节点的频率就是这两个节点的频率之和。这样，新节点就是两个被替换节点的父节点了。如此循环，直到队列中只剩一个节点（树根）。
	// parent node
	pNode = &nodes[nParentNode++];
	// pop first child
	pNode->pLeft = PopNode(pNodes, nBackNode--, false);
	// pop second child
	pNode->pRight = PopNode(pNodes, nBackNode--, true);
	// adjust parent of the two poped nodes
	pNode->pLeft->pParent = pNode->pRight->pParent = pNode;
	// adjust parent frequency
	pNode->nFrequency = pNode->pLeft->nFrequency + pNode->pRight->nFrequency;[3]
	注意事项
	有一个好的诀窍来避免使用任何队列组件。ASCII码只有256个，但实际分配了511个(CHuffmanNode nodes[511])，前255个记录ASCII码，而用后255个记录哈夫曼树中的父节点。并且在构造树的时候只使用一个指针数组(ChuffmanNode *pNodes[256])来指向这些节点。同样使用两个变量来操作队列索引(int nParentNode = nNodeCount;nBackNode = nNodeCount –1)。
	接着，压缩的最后一步是将每个ASCII编码写入输出缓冲区中：
	int nDesIndex = 0;
	// loop to write codes
	for(nCount = 0; nCount < nSrcLen; nCount++)
	{
	*(DWORD*)(pDesPtr+(nDesIndex>>3)) |=
	nodes[pSrc[nCount]].dwCode << (nDesIndex&7);
	nDesIndex += nodes[pSrc[nCount]].nCodeLength;
	}
	(nDesIndex>>3): >>3 以8位为界限右移后到达右边字节的前面
	(nDesIndex&7): &7 得到最高位.
	此外，在压缩缓冲区中，必须保存哈夫曼树的节点以及位序列，这样才能在解压缩时重新构造哈夫曼树（只需保存ASCII值和对应的位序列）
#endif 
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define N 100
#define M 2*N-1

typedef char * HuffmanCode[2*M];//haffman编码
typedef struct {
	int weight;//权值
	int parent;//父节节点
	int LChild;//左子节点
	int RChild;//右子节点
}HTNode,Huffman[M+1];//huffman树

typedef struct Node {
	int weight; //叶子结点的权值
	char c; //叶子结点
	int num; //叶子结点的二进制码的长度
}WNode,WeightNode[N];

/***产生叶子结点的字符和权值***/
void CreateWeight(char ch[],int *s,WeightNode CW,int *p)
{
	int i,j,k;
	int tag;
	*p=0;//叶子节点个数
	
	//统计字符出现个数,放入CW
	for (i=0; ch[i]!='\0'; i++) {
		tag=1;
		for (j=0; j<i; j++) {
			if (ch[j]==ch[i]) {
				tag=0;
				break;
			}
		}
		
		if (tag)	{
			CW[++*p].c=ch[i];
			CW[*p].weight=1;
			for (k=i+1; ch[k]!='\0'; k++) {
				if (ch[i] == ch[k]) {
					CW[*p].weight++;//权值累加
				}
			}
		}
	}
	*s=i;//字符串长度
}

/********创建HuffmanTree********/
void CreateHuffmanTree(Huffman ht,WeightNode w,int n)
{
	int i,j;
	int s1,s2;

	//初始化哈夫曼树
	for(i=1;i<=n;i++) {
		ht[i].weight =w[i].weight;
		ht[i].parent=0;
		ht[i].LChild=0;
		ht[i].RChild=0;
	}
	
	for(i=n+1;i<=2*n-1;i++)	{
		ht[i].weight=0;
		ht[i].parent=0;
		ht[i].LChild=0;
		ht[i].RChild=0;
	}

	for(i=n+1;i<=2*n-1;i++)	{
		for(j=1;j<=i-1;j++) {
			if(!ht[j].parent) {
				break;
			}
		}
		
		s1=j; //找到第一个双亲不为零的结点
		for(;j<=i-1;j++) {
			if(!ht[j].parent) {
				s1=ht[s1].weight>ht[j].weight?j:s1;
			}
		}
		ht[s1].parent=i;
		ht[i].LChild=s1;
		for(j=1;j<=i-1;j++) {
			if(!ht[j].parent) {
				break;
			}
		}
		s2=j; //找到第二个双亲不为零的结点
		for(;j<=i-1;j++) {
			if(!ht[j].parent) {
				s2=ht[s2].weight>ht[j].weight?j:s2;
			}
		}
		
		ht[s2].parent=i;
		ht[i].RChild=s2;
		ht[i].weight=ht[s1].weight+ht[s2].weight;//权值累加
	}
}

/***********叶子结点的编码***********/
void CrtHuffmanNodeCode(Huffman ht,HuffmanCode h,WeightNode weight,int n)
{
	int i,c,p,start;
	char *cd;
	
	cd = (char *)malloc(n*sizeof(char));
	cd[n-1]='\0';//末尾置0

	for(i=1;i<=n;i++) {
		start=n-1; //cd串每次从末尾开始
		c=i;
		p=ht[i].parent;//p在n+1至2n-1
		
		//沿父亲方向遍历,直到为0
		while (p) {
			start--;//依次向前置值
			if (ht[p].LChild==c) {//与左子相同,置0
				cd[start]='0';
			} else {//否则置1
				cd[start]='1';
			}
			c=p;
			p=ht[p].parent;
		}
		weight[i].num=n-start; //二进制码的长度(包含末尾0)
		h[i]=(char *)malloc((n-start)*sizeof(char));
		strcpy(h[i],&cd[start]);//将二进制字符串拷贝到指针数组h中
	}
	
	free(cd);//释放cd内存
	//system("pause");
}

/*********所有字符的编码*********/
void CrtHuffmanCode(char ch[],HuffmanCode h,HuffmanCode hc,WeightNode weight,int n, int m)
{
	int i,k;
	for (i=0;i<m;i++) {
		for (k=1;k<=n;k++) {/*从weight[k].c中查找与ch[i]相等的下标K*/
			if (ch[i] == weight[k].c) {
				break;
			}
		}
		
		hc[i]=(char *)malloc((weight[k].num)*sizeof(char));
		strcpy(hc[i],h[k]); //拷贝二进制编码
	}
}

/*****解码*****/
void TrsHuffmanTree(Huffman ht,WeightNode w,HuffmanCode hc,int n,int m)
{
	int i=0,j,p;
	printf("***StringInformation***\n");

	while(i<m) {
		p=2*n-1;//从父亲节点向下遍历直到叶子节点
		for(j=0;hc[i][j]!='\0';j++)	{
			if(hc[i][j]=='0') {
				p=ht[p].LChild;
			} else {
				p=ht[p].RChild;
			}
		}
		
		printf("%c",w[p].c); /*打印原信息*/
		i++;
	}
	printf("\n");
}

/*****释放huffman编码内存*****/
void FreeHuffmanCode(HuffmanCode h,HuffmanCode hc,int n,int m)
{
	int i;

	for(i=1;i<=n;i++) {//释放叶子结点的编码
		free(h[i]);
	}
	
	for(i=0;i<m;i++) {//释放所有结点的编码
		free(hc[i]);
	}
}

void main()
{
	int i,n=0; /*n为叶子结点的个数*/
	int m=0; /*m为字符串ch[]的长度*/
	char ch[N]; /*ch[N]存放输入的字符串*/
	Huffman ht; /*Huffman二叉数*/
	HuffmanCode h,hc; /*h存放叶子结点的编码，hc 存放所有结点的编码*/
	WeightNode weight; /*存放叶子结点的信息*/

	printf("\t***HuffmanCoding***\n");
	printf("please input information :");
	gets(ch); /*输入字符串*/
	CreateWeight(ch,&m,weight,&n); /*产生叶子结点信息，m为字符串ch[]的长度*/
	
	printf("***WeightInformation***\n Node ");
	for(i=1;i<=n;i++) {/*输出叶子结点的字符与权值*/
		printf("%c ",weight[i].c);
	}
	
	printf("\nWeight ");
	for(i=1;i<=n;i++) {
		printf("%d ",weight[i].weight);
	}
	
	CreateHuffmanTree(ht,weight,n); /*产生Huffman树*/
	printf("\n***HuffamnTreeInformation***\n");
	printf("\ti\tweight\tparent\tLChild\tRChild\n");
	for(i=1;i<=2*n-1;i++) {/*打印Huffman树的信息*/
		printf("\t%d\t%d\t%d\t%d\t%d\n",i,ht[i].weight,ht[i].parent,ht[i].LChild,ht[i].RChild);
	}
	CrtHuffmanNodeCode(ht,h,weight,n); /*叶子结点的编码*/
	
	printf(" ***NodeCode***\n"); /*打印叶子结点的编码*/
	for(i=1;i<=n;i++) {
		printf("\t%c:",weight[i].c);
		printf("%s\n",h[i]);
	}
	CrtHuffmanCode(ch,h,hc,weight,n,m); /*所有字符的编码*/
	printf("***StringCode***\n"); /*打印字符串的编码*/
	for(i=0;i<m;i++) {
		printf("%s",hc[i]);
	}
	printf("\n");
	//system("pause");
	TrsHuffmanTree(ht,weight,hc,n,m); /*解码*/
	FreeHuffmanCode(h,hc,n,m);
	//system("pause");
}
