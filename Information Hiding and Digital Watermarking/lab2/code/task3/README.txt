task3针对实验要求3：测试E_SIMPLE_8/D_SIMPLE_8水印检测系统对不同信息长度的检测成功率
文件夹包含如下文件：
    task3.cpp：源代码，其中 E_SIMPLE() 与 D_SIMPLE()分别实现了二者对应的功能，使用与不同的信息长度（len作为参数传入）
    task3.exe：可执行程序，首先需要输入最大的信息长度MAX_LEN（必须为8的倍数）；信息长度从8开始以8为公差递增，直至MAX_LEN结束。
               对每一个信息长度len，程序读入../data_bmp文件夹内的所有图片作为cover work，测试一随即生成的长度为len的水印的检测成功率与fasle positive/negative rate。
               最后会输出各长度的信息的水印检测成功率。