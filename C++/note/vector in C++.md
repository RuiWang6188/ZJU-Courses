## vector详解

#### 1. vector头文件

 vector收录在std库中，因此调用vector需要 `#include<vector>` 且将名字空间设置为`std`

#### 2. vector声明与构造函数

- vector声明

  可以通过`vector<data_type>vector_name`来声明一个空的vector对象

- vector赋初值

  - `vector<T> vc(v)`：创建一个向量`v`的副本
  - `vector<T> vc(begin, end)`：复制[begin, end)区间内另一个向量的元素到vc中
  - `vector<T> M(n,i)` ：创建长度为 $n$ 的T类型的向量`M`，且每个元素都是 $i$
  - `vector<T> M(n)`：创建长度为 $n$ 的T类型的向量`M`，且每个元素都为 $0$

#### 3. vector中的基本操作

> 假设当前向量为`vector<T> M`

##### 3.1 插入元素操作

- 在尾部添加元素：`M.push_back(val)`

  ​	原型：`void push_back (const T& val);` 

- 在任意位置插入元素：`M.insert()`

  - `iterator insert(iterator it, const T&x)`：向量中迭代器指向元素前增加一个元素x

  - `iterator insert(iterator it, int n, const T&x)`：向量中迭代器指向元素前增加n个相同的元素x

  - `iterator insert(iterator it, const_iterator first, const_iterator last)`：向量中迭代器指向元素前插入另一个相同类型向量的[first, last)间的数据

    ```c++
      std::vector<int> anothervector (2,400);
      myvector.insert (it+2,anothervector.begin(),anothervector.end());
    ```

##### 3.2 删除元素操作

- 根据迭代器删除元素：`M.erase()`
  - `iterator erase(iterator it)`：删除向量中迭代器`it`所指向的元素
  - `iterator erase(iterator first, iterator last)`：删除向量中[first, last)的元素
- 删除向量中的最后一个元素: `M.pop_back()`
- 清空向量中的所有元素：`M.clear()`

##### 3.3 遍历元素

- 通过下标（相当于数组）

- 通过迭代器

  ​	迭代器相当于一个pointer，可以指向向量中的任意一个元素的地址。如顺序遍历输出元素值代码如下:

  ```c++
  // 1. traversal via index
  for(int i = 0; i < M.size(); i++)
      cout << obj[i] << endl;
  // 2. traversal via iterator
  for(iterator it = M.begin(); it != M.end(); it++)
      cout << *it << endl;
  // 3. use auto in c++11
  for(auto it = M.begin(), it != M.end(); it++)
      cout << *it << endl;
  ```

> - `reference at(int pos)` 
>
>    与数组下标访问方式相同，但区别是会进行下标检测，不会出现越界情况   `M[i] = M.at(i)`
>
> - `reference front()`：返回首元素的引用
>
> - `reference back()`： 返回尾元素的引用
>
> - `iterator begin()`：返回向量头指针，指向第一个元素
>
> - `iterator end()`：返回向量尾指针，指向向量最后一个元素的**下一个**位置

##### 3.3 判断向量是否为空

​	原型：`bool empty() const`

##### 3.4 大小函数

- `int size() const`：返回向量中元素的个数
- `int capacity() const`：返回当前向量所能容纳的最大元素值
  - capacity会大于等于size，因为存在预分配
- `int max_size() const`：返回最大可允许的vector元素数量值

##### 3.5 其他函数

- 交换两个向量：`M.swap(N)`

  ​	原型：`void swap(vector& x)`

- 修改向量数据大小：`M.resize(n)`

- 设置值

  - `void assign(int n, const T& x)`：设置向量中前n个元素的值为x

  - `void assign(const_iterator first, const_iterator last)`：向量N中[first, last)的元素设置为当前向量M的元素

    ```C++
      std::vector<int> first;
      std::vector<int> second;
      std::vector<int> third;
    
      first.assign (7,100);             // 7 ints with a value of 100
    
      std::vector<int>::iterator it;
      it=first.begin()+1;
    
      second.assign (it,first.end()-1); // the 5 central values of first
    
      int myints[] = {1776,7,4};
      third.assign (myints,myints+3);   // assigning from array.
    
      std::cout << "Size of first: " << int (first.size()) << '\n';
      std::cout << "Size of second: " << int (second.size()) << '\n';
      std::cout << "Size of third: " << int (third.size()) << '\n';
    ////     output:     /////
    //// Size of first: 7
    //// Size of second: 5
    //// Size of third: 3
    ```

  ##### 3.6 向量中的元素排序

   需要添加头文件`#include<algorithm>`

  ​		`sort(M.begin(), M.end())`，默认为升序排列；若需降序则可以重写compare函数，如下：

  ```c++
  bool compare(int a,int b) 
  { 
      return a < b; //升序排列，如果改为return a>b，则为降序 
  } 
  int a[20]={2,4,1,23,5,76,0,43,24,65},i; 
  for(i=0;i<20;i++) 
      cout<< a[i]<< endl; 
  sort(a,a+20,compare);
  ```

##### 3.7 向量比较

- 若vector为基本类型，则可以直接使用`==`与`!=`，否则需要重载运算符