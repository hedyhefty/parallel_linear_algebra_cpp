# parallel_linear_algebra_cpp

* Library for the use of fast linear algebra calculations by c++.
* Implemented with the use of std::thread to accelerate the speed of matrix calculations.

### matrix.h
* Now supporting:
* Matrix declaration:
```
matrix<T> mat;
```
* Matrix transpose:
```
matrix<T> mat({{1,2}});
mat.transpose(); //return matrix<T>({{1},{2}});
```
* Negative:
```
matrix<T> mat({{1,2}});
-mat; //return matrix<T>({{-1,-2}});
```
* Matrix additon:
```
matrix<int> mat_int({{1,2}});
matrix<double> mat_double({{1.1,2.5}});
auto mat_auto = mat_int + mat_double; //mat_auto will be matrix<double>({{2.1,4.5}}).
```
* Matrix subtraction:
```
matrix<int> mat_int({{1,2}});
matrix<double> mat_double({{1.1,2.5}});
auto mat_auto = mat_int - mat_double; //mat_auto will be matrix<double>({{-0.1,-0.5}}).
```
* Matrix mulitplication:
```
matrix<int> mat_int({{1,2}});
matrix<double> mat_double({{1.1,2.5}});
auto mat_auto = mat_int * mat_double.transpose(); //mat_auto will be matrix<double>({{5.6}}).
```
