# parallel_linear_algebra_cpp

### matrix.h
* Now support:
* Matrix declaration:
```
matrix<T> mat;
```
* Matrix additon:
```
matrix<int> mat_int({{1,2}});
matrix<double> mat_double({{1.1,2.5}});
auto mat_auto = mat_int + mat_double; //mat_auto will be matrix<double>({{2.1,4.5}}).
```
* Matrix mulitplication:
```
matrix<int> mat_int({{1,2}});
matrix<double> mat_double({{1.1,2.5}});
auto mat_auto = mat_int + mat_double; //mat_auto will be matrix<double>.
```
