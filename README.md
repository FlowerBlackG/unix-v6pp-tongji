# Unix V6++ Tongji's Edition for Education

## GTY �༭��䶯

Unix V6++����ȥ��2008�꿪����ɵģ�����2024��4��30�գ����в��15����ʷ���䲿�����ƫ�Ͼɣ��������ִ�����������С�ͬʱ����ϵͳ���������Ż��ĵط���

����ɱ��Ʊ�ҵ�����Ŀ��ͬʱ��GTY �� Unix V6++ ������һЩ�Ķ�����������ڸ��ִ����Ļ��������У���������ϵͳ�ڵĲ�����ơ�

���潫�Ա����ĵĲ�������ϸ˵����

### ����ϵͳ

ԭ���ı���ϵͳ��ȫ���� Makefile���ֽ�����ϵͳ��Ϊ CMake �� Makefile ���ʹ�á����У�Makefile ���ڴ�����ָ������ CMake ��ɱ��빹���ȹ�����

�Ż��󣬱������в�����Ҫ����ǰ�������û������������ڶ����ݷ�ʽ��������ȥ��

### VSCode �����ļ�

���ǵ�������ϲ���� VSCode ��������������Ŀ�м��� VSCode �����ļ������� C++ ͷ�ļ�����·���Լ�����������������

���ļ��У�`.vscode`

### ��������

������� Bochs ���� QEMU

����������ڴ�� 32M ������ 64M���ڴ�����Ķ�����ûʲô���壩

### �ļ�ϵͳģ���Ų����

ԭ���İ汾�����ں��ļ����ܳ��� 99K����ʵ�ϣ��ϰ汾ϵͳ�Ĵ�С�Ѿ��պÿ�ס���ֵ�ˡ�һ������¹��ܣ��ں˴�С������ͻ��������ޣ������ں��޷����������ء�

ͨ�����ļ�ϵͳ INode ���Ժ������������ƣ����ǽ��ں˴�С���������� 398 ���̿飬�� 199K����ʱ���㹻ʹ�á�

| �������             | ���ǰ����̿� | ��������̿� |
| -------------------- | -------------- | -------------- |
| ����������boot.bin�� | [0, 0]         | [0, 0]         |
| �ںˣ�kernel.bin��   | [1, 199]       | [1, 399]       |
| SuperBlock           | [200, 201]     | [400, 401]     |
| INode                | [202, 1023]    | [402, 1223]    |
| Data                 | [1024, 17999]  | [1224, 18199]  |
| Swap                 | [18000, 20159] | [18200, 20359] |

**�ر�ע�⣺�ļ�ϵͳ�Ų����������ͬѧ�ڡ�����ϵͳ�γ���ơ���ҵ���������ļ�ϵͳ�༭�����޷�ֱ��ʹ�á���Ҫ���������̿�λ���й����ø��º�ſɲο���**

��������

```cpp

// git diff ./src/include/FileSystem.h
-       static const int SUPER_BLOCK_SECTOR_NUMBER = 200; 
+       static const int SUPER_BLOCK_SECTOR_NUMBER = 400;   

-       static const int INODE_ZONE_START_SECTOR = 202; 
-       static const int INODE_ZONE_SIZE = 1024 - 202;  
+       static const int INODE_ZONE_START_SECTOR = 402; 
+       static const int INODE_ZONE_SIZE = 1224 - 402;  
 
-       static const int DATA_ZONE_START_SECTOR = 1024;   
-       static const int DATA_ZONE_END_SECTOR = 18000 - 1;  
-       static const int DATA_ZONE_SIZE = 18000 - DATA_ZONE_START_SECTOR; 
+       static const int DATA_ZONE_START_SECTOR = 1224;   
+       static const int DATA_ZONE_END_SECTOR = 18200 - 1; 
+       static const int DATA_ZONE_SIZE = 18200 - DATA_ZONE_START_SECTOR; 

// git diff ./src/mm/SwapperManager.cpp
-unsigned int SwapperManager::SWAPPER_ZONE_START_BLOCK = 18000;
+unsigned int SwapperManager::SWAPPER_ZONE_START_BLOCK = 18200;
```

### PSE

���������������У����� CPU PSE ���ܣ���֧�� 4MB ��ҳӳ�䡣

�ο���[https://wiki.osdev.org/Paging](https://wiki.osdev.org/Paging)

### VESA

ʹ�� VESA ������ʾ������������ʵ��һ����ʾ�ռ����ɫ�ʸ�������֧����Ļ�����Ŀ���̨��

Ŀǰ���� VESA �������� QEMU ƽ̨���Գɹ�������֤�����������µ�׼ȷ�ԡ�

Ŀǰ��VESA �Դ�ӳ��ռ䱻�������ں����� 128MB λ�ã��� 3GB + 128 MB �������Դ��ܴ�СԼΪ 2MB��

VESA ֧�ֿ����� src/CMakeLists.txt ���ֶ����ء�

| ԭ�棨CRT��                            | �°棨VESA��                           |
| -------------------------------------- | -------------------------------------- |
| ![img](./img/qemu-v6pp-without-vesa.jpg) | ![img](./img/qemu-v6pp-vesa-enabled.jpg) |

### DMA

ԭ���Ĵ��벢û���ֶ����� DMA ���ܡ���������Ϊ Bochs Ĭ�������ˣ�����֮ǰ�Ĵ���һֱû����

QEMU ģ���оƬ��Ĭ�Ϲر� DMA ���ܣ���Ҫ�ֶ�������

### libyrosstd

�������� [https://github.com/FlowerBlackG/YurongOS/blob/master/src/lib](https://github.com/FlowerBlackG/YurongOS/blob/master/src/lib) �Ļ�������롣�ÿ�ӵ�и�ǿ�Ĺ�������õ����ܡ�

����

1. ʹ�òο������ģ��� `glibc` �� `strlen` ������һ���Կ����ж� 4 ���ֽڡ�
2. ֧�ֿ����ڴ濽���� `memcpy`�����������ʱһ���Կ��� 4 �ֽڡ�����Ľ����Ժܴ�̶����� VESA Console �Ĺ������顣

## �ο����л���

��ǰ��������������������ɹ����У�

* `Arch Linux`
* `GNU/Linux 6.8.7-arch1-Adashima-T2 PREEMPT_DYNAMIC`
* `gcc version 13.2.1 20240417`
* `GNU gdb 14.2`
* `cmake version 3.29.2`
* `GNU Make 4.4.1`
* `QEMU version 9.0.0 (qemu-system-i386)`
* `KDE Plasma 6.0.4 (Wayland)`
