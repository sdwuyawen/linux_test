1. kmalloc出来的内存是虚拟地址，通过virt_to_page转为物理地址，并调用
	SetPageReserved(virt_to_page(buffer)); //防止交换
	设置为保留。

2. 如果是io addr map, 那么例子如下：
#define VIDEO_TEXT_ADDR 0xb8000
#define VIDEO_TEXT_SIZE 0x8000

    vma->vm_flags|=VM_RESERVED;
    vma->vm_flags|=VM_IO;
    
    result=remap_pfn_range(vma,
            vma->vm_start,
            VIDEO_TEXT_ADDR>>PAGE_SHIFT,
            VIDEO_TEXT_SIZE,
            vma->vm_page_prot);
            
            
            
