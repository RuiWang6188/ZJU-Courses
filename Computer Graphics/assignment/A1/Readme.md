## Readme

​																									王睿 3180103650

- 文件及输入说明

  - axis_aligned.cpp     

    - 功能：绘制与坐标轴平行的椭圆

    - 输入参数：椭圆中心坐标$(x_0,y_0)$，长半轴$R_a$，短半轴$R_b$

    - 输入格式：`x0 y0 Ra Rb`

      - 例：![image-20200924172533493](D:\ZJU Courses\CS Course\Computer Graphics\A1\Readme.assets\image-20200924172533493.png)

           表示椭圆中心坐标为$(200,150)$，长半轴为$100$，短半轴为$50$

    - 输出：ellipse.ppm

  - non-axis_aligned.cpp

    - 功能：绘制任意的椭圆

    - 输入参数：椭圆中心坐标$(x_0,y_0)$，长半轴$R_a$，短半轴$R_b$，旋转角度$\theta$

      ![image-20200924173028357](D:\ZJU Courses\CS Course\Computer Graphics\A1\Readme.assets\image-20200924173028357.png)

    - 输入格式：

      ```ini
      x0 y0 Ra Rb
      theta
      ```

      注意：旋转角度$\theta$应该为弧度制（默认以PI结尾）

      - 例：![image-20200924172932173](D:\ZJU Courses\CS Course\Computer Graphics\A1\Readme.assets\image-20200924172932173.png)

           表示绘制一个中心坐标为$(200,150)$，长半轴为$100$，短半轴为$50$，旋转角度为$\frac{\pi}{4}$的椭圆

    - 输出：ellipse.ppm

- 注意事项

  - 默认背景为$400\times 300$像素

  - 默认背景的左上角为坐标原点，向右为$x$轴正方向，向下为$y$轴负方向，如图

    ![Lesson 02: Pixels and Coordinates | by Adrian | codeguppy | Medium](data:image/png;base64,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)

- 参考文献

  [A New Algorithm for Scan Conversion of a General Ellipse](http://www.crbond.com/papers/ell_alg.pdf)

