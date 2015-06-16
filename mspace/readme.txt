1.dlmalloc函数内部是通过sbrk()来调整内存空间
  char *sbrk(int incr)
  返回值：函数调用成功返回一指针，指向下一个内存空间。函数调用失败则返回（void*）-1,将errno设为ENOMEM。  
  sbrk(0)返回可用堆的顶端地址， 其参数可以正负数(负数是减少mem的量，正数是增加mem的量)

2. dlmalloc()分析
1). small bins
(1). 
#define NSMALLBINS        (32U)
#define NTREEBINS         (32U)
#define SMALLBIN_SHIFT    (3U)
#define SMALLBIN_WIDTH    (SIZE_T_ONE << SMALLBIN_SHIFT)
#define TREEBIN_SHIFT     (8U)
#define MIN_LARGE_SIZE    (SIZE_T_ONE << TREEBIN_SHIFT)
#define MAX_SMALL_SIZE    (MIN_LARGE_SIZE - SIZE_T_ONE)  

#define MIN_REQUEST       (MIN_CHUNK_SIZE - CHUNK_OVERHEAD - SIZE_T_ONE)
#define MAX_SMALL_REQUEST (MAX_SMALL_SIZE - CHUNK_ALIGN_MASK - CHUNK_OVERHEAD)	

	对于32bit, 
	SIZE_T_ONE = 1
	CHUNK_OVERHEAD = 4
	CHUNK_ALIGN_MASK = 7
	MIN_CHUNK_SIZE = 16
	MIN_REQUEST = 16 - 7 - 1 = 8
	MAX_SMALL_REQUEST = 255 - 7 - 4 = 244
	
#define pad_request(req) \
   (((req) + CHUNK_OVERHEAD + CHUNK_ALIGN_MASK) & ~CHUNK_ALIGN_MASK)

#define small_index(s)      (bindex_t)((s)  >> SMALLBIN_SHIFT)

	nb = (bytes < MIN_REQUEST)? MIN_CHUNK_SIZE : pad_request(bytes);
	idx = small_index(nb);	
   
    nb 应该是大于等于16 
    smallbin是0 - 8 - 16 - 24 - ... - 248
    其中0, 8是的， 有效bin索引是从2到31, 有效大小是从16到248
    
(2). if (smallbits & 0x3U) != 0)
#define mark_smallmap(M,i)      ((M)->smallmap |=  idx2bit(i))
	把smallmap的i位置1
#define clear_smallmap(M,i)     ((M)->smallmap &= ~idx2bit(i))
	把smallmap的i位置0
#define smallmap_is_marked(M,i) ((M)->smallmap &   idx2bit(i))
	判断smallmap的i位是否置1

#define mark_treemap(M,i)       ((M)->treemap  |=  idx2bit(i))
	把treemap的i位置1
#define clear_treemap(M,i)      ((M)->treemap  &= ~idx2bit(i))
	把treemap的i位置0
#define treemap_is_marked(M,i)  ((M)->treemap  &   idx2bit(i))
	判断treemap的i位是否置1
	
/* Link a free chunk into a smallbin  */
#define insert_small_chunk(M, P, S) {\
  bindex_t I  = small_index(S);\
  mchunkptr_t B = smallbin_at(M, I);\
  mchunkptr_t F = B;\				//smallmap的I位为0时用到
  assert(S >= MIN_CHUNK_SIZE);\
  if (!smallmap_is_marked(M, I))\	//若smallmap的I位为0, 那么置1; 否则F指向bin->fd (已有chunk)
    mark_smallmap(M, I);\
  else if (RTCHECK(ok_address(M, B->fd)))\
    F = B->fd;\
  else {\
    CORRUPTION_ERROR_ACTION(M);\
  }\
  B->fd = P;\		//把P添加在bin->fd
  F->bk = P;\
  P->fd = F;\
  P->bk = B;\
}

/* Unlink a chunk from a smallbin  */
#define unlink_small_chunk(M, P, S) {\
  mchunkptr_t F = P->fd;\
  mchunkptr_t B = P->bk;\
  bindex_t I = small_index(S);\
  assert(P != B);\
  assert(P != F);\
  assert(chunksize(P) == small_index2size(I));\
  if (RTCHECK(F == smallbin_at(M,I) || (ok_address(M, F) && F->bk == P))) { \
    if (B == F) {\	//只有P一个, 清smallmap的I bit
      clear_smallmap(M, I);\
    }\
    else if (RTCHECK(B == smallbin_at(M,I) ||\
                     (ok_address(M, B) && B->fd == P))) {\
      F->bk = B;\	//移除P	
      B->fd = F;\
    }\
    else {\
      CORRUPTION_ERROR_ACTION(M);\
    }\
  }\
  else {\
    CORRUPTION_ERROR_ACTION(M);\
  }\
}

/* Unlink the first chunk from a smallbin */
#define unlink_first_small_chunk(M, B, P, I) {\
  mchunkptr_t F = P->fd;\
  assert(P != B);\
  assert(P != F);\
  assert(chunksize(P) == small_index2size(I));\
  if (B == F) {\			//P是唯一个chunk, 就清smallmap中I位
    clear_smallmap(M, I);\
  }\
  else if (RTCHECK(ok_address(M, F) && F->bk == P)) {\	//从链表中移除P
    F->bk = B;\
    B->fd = F;\
  }\
  else {\
    CORRUPTION_ERROR_ACTION(M);\
  }\
}
	B = smallbin_at(M, i), P是first chunk, F是P的下一个, 如果B == F, 那么只有P一个.

	idx += ~smallbits & 1;       /* 如果最低为1, idx; 否则idx += 1;  */
	b = smallbin_at(ms, idx);	 // b为bin header
	p = b->fd;					 // p为第一个有效块	
	unlink_first_small_chunk(ms, b, p, idx); //从链表中移除p	
	set_inuse_and_pinuse(ms, p, small_index2size(idx));	//设置size, inuse, pinuse, 并next->head设置pinuse
	mem = chunk2mem(p);	//返回用户数据地址(p+偏移)
	
	直接分配small bin中的块
    
(3). if (nb > ms->dvsize) 
#define idx2bit(i)              ((binmap_t)(1) << (i))

/* isolate the least set bit of a bitmap */
#define least_bit(x)         ((x) & -(x))
	-x是取x的补码， -x = ~x + 1, 就是对x取反再加1
	证明如下：
	B = b(n)...b(i+1)b(i)b(i-1)...b(0)
	假设i是1的最低位, 那么b(i) = 1, b(i-1)...b(0)都是0
	对B取反, ~B = b`(n)...b`(i+1)b`(i)b`(i-1)...b`(0), b`(x) = !b(x), 那么b`(i) = 0, b`(i-1)...b`(0)都是`	
	然后加1, 即b`(0) + 1 = 1 + 1 = 0, 进位， 直到b`(i) + 1 = 0 + 1 = 1, 
	那么 -B = b`(n)...b`(i+1)b(i)b(i-1)...b(0)
	所以 (B & -B) = b(i)b(i-1)...b(0), 其中b(i) = 1, b(i-1)...b(0)都是0	
		
	所以((x) & -(x))可以求出最低1的值。	
	如 x = 0x10100, -x = 0xfffeff00, 那么least_bit(x) = 0x100
	
	而(B | -B) = 1...1b(i)b(i-1)...b(0)
	其中1...1b(i)都是1, b(i-1)...b(0)都是0, 
	可以证明left_bits(x)	

/* mask with all bits to left of least bit of x on */
#define left_bits(x)         ((x<<1) | -(x<<1))
    如 x = 1<<2 = 0x4, left_bits(x) = 0xfffffff8
    即是求出x左边的mask值
    
#define compute_bit2idx(X, I)\
{\
  unsigned int J;\
  J = __builtin_ctz(X); \
  I = (bindex_t)J;\
}
  __builtin_ctz计算返回后面(低位)的0个个数，和__builtin_clz相对

#define replace_dv(M, P, S) {\
  size_t DVS = M->dvsize;\
  assert(is_small(DVS));\
  if (DVS != 0) {\
    mchunkptr_t DV = M->dv;\
    insert_small_chunk(M, DV, DVS);\
  }\
  M->dvsize = S;\
  M->dv = P;\
}

  smallbits = ms->smallmap >> idx;
  
  binmap_t leftbits = (smallbits << idx) & left_bits(idx2bit(idx)); //获得idx左边有效的bits，屏蔽idx右边无效的bits(置0)
  binmap_t leastbit = least_bit(leftbits);	//获得1所在最小位的值
  compute_bit2idx(leastbit, i);		//i等于1所在最小位  
  rsize = small_index2size(i) - nb;		//计算剩余量
  /* Fit here cannot be remainderless if 4byte sizes */
  if (SIZE_T_SIZE != 4 && rsize < MIN_CHUNK_SIZE)
	set_inuse_and_pinuse(ms, p, small_index2size(i));	//设置size, inuse, pinuse, 并next->head设置pinuse
  else { //分割开2个块
	set_size_and_pinuse_of_inuse_chunk(ms, p, nb);	//设置p的size, pinuse, inuse
	r = chunk_plus_offset(p, nb);	//分割出来的新块
	set_size_and_pinuse_of_free_chunk(r, rsize);
	replace_dv(ms, r, rsize);	//替换dv块， 如果旧dv存在，那么把旧dv插入到small bins中， 后更新dv块
  }
  mem = chunk2mem(p); //返回用户数据地址(p+偏移)
  
(4). 如果在(3)没有找到, 就调用tmalloc_small, 从dltree树中分配small chunk  

#define treebin_at(M,i)     (&((M)->treebins[i]))
  treebins[NTREEBINS] 保存dltree的根地址
  
/* A little helper macro for trees */
#define leftmost_child(t) ((t)->child[0] != 0? (t)->child[0] : (t)->child[1])
  获取最小子树
  左子树上所有结点的值均小于右子树上所有结点的值
    
static void* tmalloc_small(mstateptr_t m, size_t nb) {
  ...
  binmap_t leastbit = least_bit(m->treemap);	//获得1所在最小位的值
  compute_bit2idx(leastbit, i);	//i等于1所在最小位  
  v = t = *treebin_at(m, i);	//获取dltree的根地址
  rsize = chunksize(t) - nb;  
  
  while ((t = leftmost_child(t)) != 0) { //查找最小的块
    size_t trem = chunksize(t) - nb;
    if (trem < rsize) {
      rsize = trem;
      v = t;
    }
  }
  
    mchunkptr_t r = chunk_plus_offset(v, nb);	//r = v + nb
    if (RTCHECK(ok_next(v, r))) {
      unlink_large_chunk(m, v);		//把v移除
      if (rsize < MIN_CHUNK_SIZE)
        set_inuse_and_pinuse(m, v, (rsize + nb));	//整个chunk分配, 设置(rsize + nb), inuse, pinuse, 并next->head设置pinuse
      else {
        set_size_and_pinuse_of_inuse_chunk(m, v, nb);	//设置v的size, pinuse, inuse
        set_size_and_pinuse_of_free_chunk(r, rsize);	//设置r的size, pinuse
        replace_dv(m, r, rsize);	//替换dv块， 如果旧dv存在，那么把旧dv插入到small bins中， 后更新dv块
      }
      return chunk2mem(v);	//返回用户数据地址(v+偏移)
    }
  
  return 0;
}

(5). if (nb <= ms->dvsize)
      size_t rsize = ms->dvsize - nb;		//计算剩余rsize
      mchunkptr_t p = ms->dv;
      if (rsize >= MIN_CHUNK_SIZE) { /* 分裂dv块 */
        mchunkptr_t r = ms->dv = chunk_plus_offset(p, nb);	//更新dv块的地址(p+nb), 大小(rsize)
        ms->dvsize = rsize;
        set_size_and_pinuse_of_free_chunk(r, rsize);	//设置r的size, pinuse
        set_size_and_pinuse_of_inuse_chunk(ms, p, nb);	//设置p的size, pinuse, inuse
      }
      else { /* 全部dv分配 */
        size_t dvs = ms->dvsize;
        ms->dvsize = 0;						//dv清零
        ms->dv = 0;
        set_inuse_and_pinuse(ms, p, dvs);	//设置dvs, inuse, pinuse, 并next->head设置pinuse
      }
      mem = chunk2mem(p);	//返回用户数据地址(p+偏移)
                    
2). large bins
	struct malloc_tree_chunk {
	  /* The first four fields must be compatible with malloc_chunk */
	  size_t                    prev_foot;
	  size_t                    head;
	  struct malloc_tree_chunk* fd;
	  struct malloc_tree_chunk* bk;  
	  struct malloc_tree_chunk* child[2];
	  struct malloc_tree_chunk* parent;
	  bindex_t                  index;			//chunk所在的treebin的index
	};
  每一个箱子里的chunk块都被以dltree树结构组织起来，分了32个箱号，因此dlmalloc具有32个dltree树，记录该32个dltree树根节点的指针变量也定义在结构体malloc_state内
  每个箱子管理某个范围:
  [256, 384) [384, 512) [512, 768) [768, 1024) ....
  
  在查找某字节在哪个范围内的树上只需要顺序的比较各bit位就可以, 来确定哪个层(level)， 左(left)或右(right)子树。
  
(1) dltree树
  dltree树或者是一颗空树；或者是具有下列性质的二叉树：
  <1>.若左子树和右子树都不空，则左子树上所有结点的值均小于右子树上所有结点的值；
  <2>.左、右子树也分别为dltree树；

/* Insert chunk into tree */
#define insert_large_chunk(M, X, S) {\
  tbinptr_t* H;\
  bindex_t I;\
  compute_tree_index(S, I);\
  H = treebin_at(M, I);\				//H = &treebin[I]	
  X->index = I;\						
  X->child[0] = X->child[1] = 0;\
  if (!treemap_is_marked(M, I)) {\		//tree为空
    mark_treemap(M, I);\
    *H = X;\							//treebin[I] = X
    X->parent = (tchunkptr_t)H;\		//parent = &treebin[I]	
    X->fd = X->bk = X;\					//初始化X的链表
  }\
  else {\	//tree非空
    tchunkptr_t T = *H;\	
    size_t K = S << leftshift_for_tree_index(I);\
    for (;;) {\		//在tree中查找匹配size == S的chunk
      if (chunksize(T) != S) {\  //size != S
        tchunkptr_t* C = &(T->child[(K >> (SIZE_T_BITSIZE-SIZE_T_ONE)) & 1]);\
        K <<= 1;\
        if (*C != 0)\		//若*C != 0, T指向child, 继续查找
          T = *C;\			
        else if (RTCHECK(ok_address(M, C))) {\	//若*C == 0, T->child = X, 初始化X, 并退出查找
          *C = X;\			
          X->parent = T;\
          X->fd = X->bk = X;\
          break;\
        }\
        else {\
          CORRUPTION_ERROR_ACTION(M);\
          break;\
        }\
      }\
      else {\	//size == S
        tchunkptr_t F = T->fd;\
        if (RTCHECK(ok_address(M, T) && ok_address(M, F))) {\
          T->fd = F->bk = X;\	//把X添加到same-sized链表
          X->fd = F;\
          X->bk = T;\
          X->parent = 0;\		//parent清零， 和tree节点的属性区分开
          break;\
        }\
        else {\
          CORRUPTION_ERROR_ACTION(M);\
          break;\
        }\
      }\
    }\
  }\
}

/*
  Unlink steps:

  1. If x is a chained node, unlink it from its same-sized fd/bk links
     and choose its bk node as its replacement.
  2. If x was the last node of its size, but not a leaf node, it must
     be replaced with a leaf node (not merely one with an open left or
     right), to make sure that lefts and rights of descendents
     correspond properly to bit masks.  We use the rightmost descendent
     of x.  We could use any other leaf, but this is easy to locate and
     tends to counteract removal of leftmosts elsewhere, and so keeps
     paths shorter than minimally guaranteed.  This doesn't loop much
     because on average a node in a tree is near the bottom.
  3. If x is the base of a chain (i.e., has parent links) relink
     x's parent and children to x's replacement (or null if none).
*/  
#define unlink_large_chunk(M, X) {\
  tchunkptr_t XP = X->parent;\
  tchunkptr_t R;\
  if (X->bk != X) {\		//same-sized链表不止一个chunk, 从链表中移除X
    tchunkptr_t F = X->fd;\
    R = X->bk;\				//R为链表的下一个chunk， 后面R重新连到XP, 作为tree的一个节点
    if (RTCHECK(ok_address(M, F) && F->bk == X && R->fd == X)) {\
      F->bk = R;\
      R->fd = F;\
    }\
    else {\
      CORRUPTION_ERROR_ACTION(M);\
    }\
  }\
  else {\	//same-sized链表只有一个chunk, 下面查找子树下的最大页子，赋值给R
    tchunkptr_t* RP;\
    if (((R = *(RP = &(X->child[1]))) != 0) ||\
        ((R = *(RP = &(X->child[0]))) != 0)) {\
      tchunkptr_t* CP;\
      while ((*(CP = &(R->child[1])) != 0) ||\
             (*(CP = &(R->child[0])) != 0)) {\
        R = *(RP = CP);\
      }\
      if (RTCHECK(ok_address(M, RP)))\
        *RP = 0;\					//把指向R的child指针置0, 相当在tree中移除R
      else {\
        CORRUPTION_ERROR_ACTION(M);\
      }\
    }\
  }\
  if (XP != 0) {\	//后面R重新连到XP, 作为tree的一个节点
    tbinptr_t* H = treebin_at(M, X->index);\
    if (X == *H) {\		//如果是树根
      if ((*H = R) == 0) \	//树根=R, 如果R = 0, 清除treemap的index bit
        clear_treemap(M, X->index);\
    }\
    else if (RTCHECK(ok_address(M, XP))) {\
      if (XP->child[0] == X) \	//使XP->child = R
        XP->child[0] = R;\
      else \
        XP->child[1] = R;\
    }\
    else\
      CORRUPTION_ERROR_ACTION(M);\
    if (R != 0) {\				//如果R有效
      if (RTCHECK(ok_address(M, R))) {\
        tchunkptr_t C0, C1;\
        R->parent = XP;\		//使R->parent指向XP
        if ((C0 = X->child[0]) != 0) {\		//R->child[0] = X->child[0]
          if (RTCHECK(ok_address(M, C0))) {\
            R->child[0] = C0;\
            C0->parent = R;\
          }\
          else\
            CORRUPTION_ERROR_ACTION(M);\
        }\
        if ((C1 = X->child[1]) != 0) {\		//R->child[1] = X->child[1]
          if (RTCHECK(ok_address(M, C1))) {\
            R->child[1] = C1;\
            C1->parent = R;\
          }\
          else\
            CORRUPTION_ERROR_ACTION(M);\
        }\
      }\
      else\
        CORRUPTION_ERROR_ACTION(M);\
    }\
  }\
}

#define is_small(s)         (((s) >> SMALLBIN_SHIFT) < NSMALLBINS)		//((s>>3) < 32)

#define insert_chunk(M, P, S)\
  if (is_small(S)) insert_small_chunk(M, P, S)\
  else { tchunkptr_t TP = (tchunkptr_t)(P); insert_large_chunk(M, TP, S); }

#define unlink_chunk(M, P, S)\
  if (is_small(S)) unlink_small_chunk(M, P, S)\
  else { tchunkptr_t TP = (tchunkptr_t)(P); unlink_large_chunk(M, TP); }

(2). tmalloc_large
#define compute_tree_index(S, I)\
{\
  unsigned int X = S >> TREEBIN_SHIFT;\
  if (X == 0)\
    I = 0;\
  else if (X > 0xFFFF)\
    I = NTREEBINS-1;\
  else {\
    unsigned int K = (unsigned) sizeof(X)*__CHAR_BIT__ - 1 - (unsigned) __builtin_clz(X); \
    I =  (bindex_t)((K << 1) + ((S >> (K + (TREEBIN_SHIFT-1)) & 1)));\
  }\
}
  X = S /256
  K = 32 - 1 - __builtin_clz(X) = 31 - __builtin_clz(X)
    K为不为0的最高位(0 ~ 31)
	__builtin_clz(X)返回X前导的0的个数
  I = (K<<1) + (S>>(K+8-1) & 1)
	(S>>(K+8-1) & 1)是求S中(K+8-1)位是否为1

  那么  K = 0            K = 0            K = 1            K = 1            ... K
  区间: [256, 384) -> 0  [384, 512) -> 1  [512, 768) -> 2  [768, 1024) -> 3 ... [n, n + (1<<(K-8-1)))
  大小:	127              127              255              255    			...	1<<(K+8-1) - 1

/* Bit representing maximum resolved size in a treebin at i */
#define bit_for_tree_index(i) \
   (i == NTREEBINS-1)? (SIZE_T_BITSIZE-1) : (((i) >> 1) + TREEBIN_SHIFT - 2)
	
	返回 31 or ((i>>1) + 8 - 2)
	因为 K = I<<1, 而期间的范围为R = 1<<(K + 8 - 1), 那么子树的范围R` = R >> 1 = 1<<(K + 8 - 2)
	
/* Shift placing maximum resolved bit in a treebin at i as sign bit */
#define leftshift_for_tree_index(i) \
   ((i == NTREEBINS-1)? 0 : \
    ((SIZE_T_BITSIZE-SIZE_T_ONE) - (((i) >> 1) + TREEBIN_SHIFT - 2)))
    
    返回 0 or 31 - ((i>>1) + 8 - 2)

/* The size of the smallest chunk held in bin with index i */
#define minsize_for_tree_index(i) \
   ((SIZE_T_ONE << (((i) >> 1) + TREEBIN_SHIFT)) |  \
   (((size_t)((i) & SIZE_T_ONE)) << (((i) >> 1) + TREEBIN_SHIFT - 1)))
  
/* allocate a large request from the best fitting chunk in a treebin */
static void* tmalloc_large(mstateptr_t m, size_t nb) {
  tchunkptr_t v = 0;
  size_t rsize = -nb; /* Unsigned negation */
  tchunkptr_t t;
  bindex_t idx;
  compute_tree_index(nb, idx);		//idx等于nb对应的treebin的index (0 ~ 31)
  if ((t = *treebin_at(m, idx)) != 0) {	//tree非空， 查找此树直到size == nb或 t == 0 (叶子)
    /* Traverse tree for this bin looking for node with size == nb */
    size_t sizebits = nb << leftshift_for_tree_index(idx);	//保留nb的((idx>>1) + 8 - 2)位到0位, 并左移到最高位
    tchunkptr_t rst = 0;  /* The deepest untaken right subtree */
    for (;;) {
      tchunkptr_t rt;
      size_t trem = chunksize(t) - nb;
      if (trem < rsize) {
        v = t;						//v = 最小的chunk
        if ((rsize = trem) == 0)	//rsize = trem, 并判断rsize是否等于0; 等于0, 就是size == nb, 退出查找
          break;
      }
      rt = t->child[1];		//right tree
      t = t->child[(sizebits >> (SIZE_T_BITSIZE-SIZE_T_ONE)) & 1]; //通过判断最高位， 来取left or right tree
      if (rt != 0 && rt != t)
        rst = rt;				//使rst一直等于right tree
      if (t == 0) { //如果找不到， 使t = rst, 并退出查找
        t = rst; /* set t to least subtree holding sizes > nb */
        break;
      }
      sizebits <<= 1;	//左移1位
    }
  }
  if (t == 0 && v == 0) { /* set t to root of next non-empty treebin */
    binmap_t leftbits = left_bits(idx2bit(idx)) & m->treemap;	//获取有效的treemap(大于idx2bit(idx))
    if (leftbits != 0) {
      bindex_t i;
      binmap_t leastbit = least_bit(leftbits);	//获取匹配的最小的tree
      compute_bit2idx(leastbit, i);	//获得最小的index
      t = *treebin_at(m, i);	//获取树根
    }
  }

  while (t != 0) { /* find smallest of tree or subtree */ //查找匹配的chunk, 这一块代码可移到上一个if里面的尾端
    size_t trem = chunksize(t) - nb;
    if (trem < rsize) {
      rsize = trem;
      v = t;
    }
    t = leftmost_child(t);	//获取最小的子树
  }

  /*  If dv is a better fit, return 0 so malloc will use it */
  if (v != 0 && rsize < (size_t)(m->dvsize - nb)) {	//rsize < (size_t)(m->dvsize - nb)是判断v是否比dv更适合; 如果dv适合，返回0, 后面用dv来分配
    if (RTCHECK(ok_address(m, v))) { /* split */
      mchunkptr_t r = chunk_plus_offset(v, nb);
      assert(chunksize(v) == rsize + nb);
      if (RTCHECK(ok_next(v, r))) {
        unlink_large_chunk(m, v);	//把v移除
        if (rsize < MIN_CHUNK_SIZE)
          set_inuse_and_pinuse(m, v, (rsize + nb));		//整chunk分配, 设置p的size, pinuse, inuse, 并next->head设置pinuse 
        else {	//分割成2块
          set_size_and_pinuse_of_inuse_chunk(m, v, nb); //设置p的size, pinuse, inuse
          set_size_and_pinuse_of_free_chunk(r, rsize);	//设置r的size, pinuse
          insert_chunk(m, r, rsize);	//把分出来的r插回bins中
        }
        return chunk2mem(v);
      }
    }
    CORRUPTION_ERROR_ACTION(m);
  }
  return 0;
}
 
3. dlfree()分析
  dlfree释放当前chunk, 并合并前后的空闲块
