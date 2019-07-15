# parallel_linear_algebra_cpp

* Library for the use of fast linear algebra calculations by c++.
* Implemented with the use of std::thread to accelerate the speed of matrix calculations.
* Using initialize list to support easy declaration of matrix & vector.

### matrix.h
* Now supporting:
* Matrix declaration:
```
matrix<T> def_mat;
matrix<T> vec{1,2,3}; //an row vector (1,2,3).
matrix<T> mat{{1,2},{3,4}}; //a 2 by 2 matrix (1,2; 3,4).
```
* Matrix transpose:
```
matrix<T> mat{1,2};
mat.transpose(); //return matrix<T>{{1},{2}};
```
* Negative:
```
matrix<T> mat{1,2};
-mat; //return matrix<T>{-1,-2};
```
* Matrix additon:
```
matrix<int> mat_int{1,2};
matrix<double> mat_double{1.1,2.5};
mat_int + mat_double; //return matrix<double>{2.1,4.5}.
```
* Matrix subtraction:
```
matrix<int> mat_int{1,2};
matrix<double> mat_double{1.1,2.5};
mat_int - mat_double; //return matrix<double>{-0.1,-0.5}.
```
* Matrix mulitplication:
```
matrix<int> mat_int{1,2};
matrix<double> mat_double{1.1,2.5};
mat_int * mat_double.transpose(); //return matrix<double>{5.6}.
```
