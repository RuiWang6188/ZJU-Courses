## Report

​																														Group 6

### 0. Backgrounds

To find the projection of a 3D point onto the image plane, we first need to transform the point from **world coordinate system** to the **camera coordinate system** using the **extrinsic parameters** (Rotation ![\mathbf{R}](https://www.learnopencv.com/wp-content/ql-cache/quicklatex.com-0cea5f663ef3c8e9dd00552811fc1680_l3.png) and Translation ![\mathbf{t}](https://www.learnopencv.com/wp-content/ql-cache/quicklatex.com-40e2ca75c8f1347fce0071f5fc0646ad_l3.png)).

Next, using the **intrinsic parameters** of the camera, we project the point onto the image plane.

The equations that relate 3D point ![(X_w, Y_w, Z_w)](https://www.learnopencv.com/wp-content/ql-cache/quicklatex.com-f7eb17644f2258f47077241c20ece659_l3.png) in world coordinates to its projection ![(u, v)](https://www.learnopencv.com/wp-content/ql-cache/quicklatex.com-c8088bef813b0c456d97df9313287889_l3.png) in the image coordinates are shown below

$$
\begin{align*} \begin{bmatrix}  u' \\ v' \\ z'  \end{bmatrix}  &=  \mathbf{P} \begin{bmatrix} X_w \\ Y_w \\ Z_w \\ 1 \end{bmatrix} \\ u &= \frac{u'}{w'} \\  v &= \frac{v'}{w'} \end{align*}](https://www.learnopencv.com/wp-content/ql-cache/quicklatex.com-ec53a37f80fbca3604c5fc89d803b837_l3.png)
$$
Where, ![\mathbf{P}](https://www.learnopencv.com/wp-content/ql-cache/quicklatex.com-6f00391a1a0ecabe52cae27086eae86a_l3.png) is a 3×4 Projection matrix consisting of two parts — the **intrinsic matrix** (![K](https://www.learnopencv.com/wp-content/ql-cache/quicklatex.com-b760ebc707e08dd6e1888ea8da4c2454_l3.png)) that contains the intrinsic parameters and the **extrinsic matrix** (![[\mathbf{R} \mid  \mathbf{t}]](https://www.learnopencv.com/wp-content/ql-cache/quicklatex.com-1d3f436321dd23b66c177216a042c228_l3.png) ) that is combination of 3×3 rotation matrix ![\mathbf{R}](https://www.learnopencv.com/wp-content/ql-cache/quicklatex.com-0cea5f663ef3c8e9dd00552811fc1680_l3.png) and a 3×1 translation ![\mathbf{t}](https://www.learnopencv.com/wp-content/ql-cache/quicklatex.com-40e2ca75c8f1347fce0071f5fc0646ad_l3.png) vector.

![\begin{align*}     \mathbf{P} &= \overbrace{\mathbf{K}}^\text{Intrinsic Matrix} \times \overbrace{[\mathbf{R} \mid  \mathbf{t}]}^\text{Extrinsic Matrix} \\[0.5em] \end{align*}](https://www.learnopencv.com/wp-content/ql-cache/quicklatex.com-14a1fda363f90358c4813106c61d4345_l3.png)

As mentioned in the previous post, the intrinsic matrix ![K](https://www.learnopencv.com/wp-content/ql-cache/quicklatex.com-b760ebc707e08dd6e1888ea8da4c2454_l3.png) is upper triangular

$$
![\begin{align*}     \mathbf{K} & = \begin{bmatrix}  f_x & \gamma & c_x \\ 0 & f_y & c_y \\ 0 & 0 & 1  \end{bmatrix} \end{align*}](https://www.learnopencv.com/wp-content/ql-cache/quicklatex.com-e889cfafac2d3a5d268d2dfb8063596a_l3.png)
$$
where

- ![f_x, f_y](https://www.learnopencv.com/wp-content/ql-cache/quicklatex.com-3333545c28a46de260db855da935df50_l3.png) are the x and y focal lengths ( yes, they are usually the same ).

- ![c_x, c_y](https://www.learnopencv.com/wp-content/ql-cache/quicklatex.com-a064660332306082badd534af0191df7_l3.png) are the x and y coordinates of the optical center in the image plane. Using the center of the image is usually a good enough approximation.

- ![\gamma](https://www.learnopencv.com/wp-content/ql-cache/quicklatex.com-2afeb87159a4918337409f4bf56d689a_l3.png) is the skew between the axes. It is usually 0.



### 1. Finish intrinsic and extrinsic calibration

​	In the experiment,  we first define real world coordinates with chessboard pattern. In the process of calibration we calculate the camera parameters by a set of know 3D points $(X_w, Y_w, Z_w)$ and their corresponding pixel location $(u,v)$ in the image.

​	For the 3D points we photograph a chessboard pattern with known dimensions at many different orientations. The world coordinate is attached to the chessboard and since all the corner points lie on a plane, we can arbitrarily choose $Z_w$ for every point to be 0. Since points are equally spaced in the chessboard, the $(X_w,Y_w)$ coordinates of each 3D point are easily defined by taking one point as reference $(0, 0)$ and defining remaining with respect to that reference point.

​	Then we captured 13 images of the chessboard from different viewpoints and find 2D coordinates of chessboard using `findChessboardCorners()` and `cornerSubPix()` functions.

![image-20201008105609767](.\Report.assets\image-20201008105609767.png)

<center style="font-size:14px;color:#000000;">Figure 1: Experiment photos of different viewpoint</center>	

​	After that, we press the 3D points in world coordinates and their 2D locations in all images to openCV's `calibrateCamera` method. And we can get the parameters like *camera matrix, distortion coefficients, rotation vector and translation vector*. The camera matrix and distortion coefficients are as follows.
$$
\text{camera matrix} = 
	\left[
	\begin{matrix}
		3708.495099506612 & 0 & 1212.71094321327\\
		0 & 3817.367871914058 & 834.8141758973154\\
		0 & 0 & 1
	\end{matrix}
	\right]\\ 
	\begin{align}
	\text{distortion coefficients} &= \begin{bmatrix}
    -0.3624889725299742\\ -1.388093717775788\\ 0.0153110849269273\\ -0.005414308184240487\\ 2.518806915486268
    \end{bmatrix}^T
	\end{align}
$$

<center style="font-size:14px;color:#000000;">Figure 2: Experiment data of camera matrix and distortion coefficients</center>	

![image-20201008111715981](.\Report.assets\image-20201008111715981.png)

<center style="font-size:14px;color:#000000;">Figure 2: Experiment data of some parameters</center>	

​	Thus we derived the intrinsic and extrinsic parameters of the camera.



### 2. Impact of different factors over reprojection error



### 3. Output estimated camera centers and chessboard into MeshLab

#### 3.1 Colmap 3D rebuild

​	In the previous process, we got the intrinsic calibration of the camera, so we tried to input the camera parameters and the photos into colmap to rebuild a 3D scene.

​	The whole process has gone through feature point extraction, feature point matching, sparse reconstruction and dense reconstruction.

<img src="C:\Users\Echo\AppData\Roaming\Typora\typora-user-images\image-20201011000637343.png" alt="image-20201011000637343" style="zoom:33%;" />![image-20201011001139880](C:\Users\Echo\AppData\Roaming\Typora\typora-user-images\image-20201011001139880.png)

<center style="font-size:14px;color:#000000;">Figure 3: Feature extraction</center>	

<img src="C:\Users\Echo\AppData\Roaming\Typora\typora-user-images\image-20201011001149920.png" alt="image-20201011001149920" style="zoom:33%;" />

<center style="font-size:14px;color:#000000;">Figure 4: Feature matching</center>	

<img src="C:\Users\Echo\AppData\Roaming\Typora\typora-user-images\image-20201011001317089.png" alt="image-20201011001317089" style="zoom:33%;" />

<center style="font-size:14px;color:#000000;">Figure 5: Dense reconstruction</center>	

​	From the results, it is not very good, colmap can not recognize the black and white pattern lattice. But we can see that it recognizes several icons in the iPad sidebar

<img src="C:\Users\Echo\AppData\Roaming\Typora\typora-user-images\image-20201011005621895.png" alt="image-20201011005621895" style="zoom:50%;" />

<center style="font-size:14px;color:#000000;">Figure 6: Result of sparse reconstruction</center>	

<img src="C:\Users\Echo\AppData\Roaming\Typora\typora-user-images\image-20201011005805299.png" alt="image-20201011005805299" style="zoom:50%;" />

<center style="font-size:14px;color:#000000;">Figure 7: Result of dense reconstruction</center>

#### 3.2 Creating a Ply File

​	Because the result of reconstructing the 3D scene directly from the photos is that the model can't recognize the checkerboard, we manually generated a ply file according to the coordinates obtained before, as shown in the figure below.

<img src="C:\Users\Echo\AppData\Roaming\Typora\typora-user-images\image-20201011021551914.png" alt="image-20201011021551914" style="zoom:50%;" />

<center style="font-size:14px;color:#000000;">Figure 8: 3D model of chessboard</center>

### 4. Project interesting 3D points



### 5. [Bonus] A real-time demo

