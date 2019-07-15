//
// Created by SHANGTONG WANG on 2019-07-12.
//

#ifndef LINEARALGEBRALIB_MATRIX_H
#define LINEARALGEBRALIB_MATRIX_H

#include <iostream>
#include <vector>
#include <thread>
#include <cmath>
#include <exception>

static unsigned int max_thread = std::thread::hardware_concurrency();

//class for m x n matrix operation.
template<typename T>
class matrix {
public:
    T type_identifier = 1;

    matrix(std::initializer_list<T> init) {
        //std::cout << "sg_init_list constructor called." << std::endl;
        m = 1;
        n = init.size();
        elements = std::vector<std::vector<T>>(m, std::vector<T>(n));

        int i = 0;
        for (auto r:init) {
            elements[m - 1][i] = r;
            ++i;
        }
    }

    matrix(std::initializer_list<std::initializer_list<T>> init) {
        std::cout << "init_list constructor called." << std::endl;
        m = init.size();
        n = init.begin()->size();
        elements = std::vector<std::vector<T>>(m, std::vector<T>(n));

        int i = 0;
        int j = 0;
        for (auto r:init) {
            for (auto c:r) {
                elements[i][j] = c;
                ++j;
            }
            ++i;
            j = 0;
        }

    }

    matrix(const std::vector<std::vector<T>> &mat) : elements(mat) {
        m = elements.size();
        if (m > 0) {
            n = elements[0].size();
        } else {
            n = 0;
        }
    }

    template<typename U>
    matrix(const matrix<U> &mat):m(mat.size()[0]), n(mat.size()[1]) {
        //std::cout << "type transfer constructor called." << std::endl;
        elements = std::vector<std::vector<T>>(m, std::vector<T>(n));
        for (size_t i = 0; i < m; ++i) {
            for (size_t j = 0; j < n; ++j) {
                elements[i][j] = mat[i][j];
            }
        }
    }

    matrix() : m(0), n(0) {
        std::cout << "default called." << std::endl;
    }

    template<typename U>
    matrix &operator=(const matrix<U> &mat) {
        //std::cout << "= called." << std::endl;
        m = mat.size()[0];
        n = mat.size()[1];
        elements = std::vector<std::vector<T>>(m, std::vector<T>(n, 0));

        for (size_t i = 0; i < m; ++i) {
            for (size_t j = 0; j < n; ++j) {
                elements[i][j] = mat[i][j];
            }
        }
        return *this;
    }

    matrix transpose() const {
        std::vector<std::vector<T>> mt(n, std::vector<T>(m, 0));
        //int dev_work = ceil(n / max_thread);

        std::vector<std::thread> t(max_thread);
        for (size_t i = 0; i < max_thread; ++i) {
            t[i] = std::thread(&matrix::paralleltrans, *this, ref(mt), i);
            //paralleltrans(mt, i);
        }

        for (size_t i = 0; i < max_thread; ++i) {
            t[i].join();
        }

        matrix mT(mt);
        return mT;
    }

    void paralleltrans(std::vector<std::vector<T>> &mt, size_t work_num) {
        int dev_work = ceil(double(n) / double(max_thread));
        size_t start_index = work_num * dev_work;
        size_t end_index = ((work_num + 1) * dev_work < n) ? (work_num + 1) * dev_work : n;

        for (; start_index < end_index; ++start_index) {
            for (size_t j = 0; j < m; ++j) {
                mt[start_index][j] = elements[j][start_index];
            }
        }
    }

    std::vector<T> &operator[](size_t r) {
        if (r >= 0 && r < m) {
            return elements[r];
        }
        throw std::out_of_range("bad access.");
    }

    const std::vector<T> &operator[](size_t r) const {
        if (r >= 0 && r < m) {
            return elements[r];
        }
        throw std::out_of_range("bad access.");
    }

    matrix operator-() {
        return *this * -1;
    }

    const std::vector<const size_t> size() const {
        const std::vector<const size_t> size_v{m, n};
        return size_v;
    }

    bool isSingleton() {
        if (m == 1 && n == 1) {
            return true;
        }
        return false;
    }

private:
    std::vector<std::vector<T>> elements;
    size_t m;
    size_t n;
};

template<typename T>
std::ostream &operator<<(std::ostream &ost, matrix<T> mat) {
    for (size_t i = 0; i < mat.size()[0]; i++) {
        for (size_t j = 0; j < mat.size()[1]; j++) {
            ost << mat[i][j] << " ";
        }
        ost << std::endl;
    }

    return ost;
}

template<typename Y, typename U>
void parallelScaMulti(matrix<Y> &m1, U v, size_t work_id) {
    unsigned int dev_work = ceil(double(m1.size()[0]) / double(max_thread));
    size_t start_index = work_id * dev_work;
    size_t end_index = ((work_id + 1) * dev_work < m1.size()[0]) ? (work_id + 1) * dev_work : m1.size()[0];

    for (; start_index < end_index; ++start_index) {
        for (size_t j = 0; j < m1.size()[1]; ++j) {
            m1[start_index][j] *= v;
        }
    }
}


template<typename Y, typename U>
auto operator*(matrix<Y> m1, U v) -> matrix<decltype(m1.type_identifier * v)> {

    matrix<decltype(m1[0][0] * v)> mmat = m1;
    std::vector<std::thread> t(max_thread);

    for (size_t work_id = 0; work_id < max_thread; ++work_id) {
        t[work_id] = std::thread(parallelScaMulti<decltype(m1.type_identifier * v), U>, std::ref(mmat), v, work_id);
    }

    for (size_t work_id = 0; work_id < max_thread; ++work_id) {
        if (t[work_id].joinable()) {
            t[work_id].join();
        }
    }

    return mmat;
}

template<typename Y, typename U>
auto operator*(U v, matrix<Y> m1) -> matrix<decltype(m1.type_identifier * v)> {
    return m1 * v;
}

template<typename Y, typename U, typename D>
void parallelMulti(matrix<Y> &m1, matrix<U> &m2, matrix<D> &r, size_t work_id) {
    int m = m1.size()[0];
    int n = m1.size()[1];
    int k = m2.size()[1];
    int dev_work = ceil(double(m) / double(max_thread));
    size_t start_index = work_id * dev_work;
    size_t end_index = ((work_id + 1) * dev_work < m) ? (work_id + 1) * dev_work : m;

    for (; start_index < end_index; ++start_index) {
        for (size_t j = 0; j < k; ++j) {

            for (size_t kk = 0; kk < n; ++kk) {
                r[start_index][j] += m1[start_index][kk] * m2[kk][j];
            }

        }
    }
}

template<typename Y, typename U>
auto operator*(matrix<Y> m1, matrix<U> m2) -> matrix<decltype(m1.type_identifier * m2.type_identifier)> {
    if (m1.isSingleton()) {
        Y hold = m1[0][0];
        return m2 * hold;
    }

    if (m2.isSingleton()) {
        U hold = m2[0][0];
        return m1 * hold;
    }

    if (m1.size()[1] != m2.size()[0]) {
        throw std::runtime_error("matrix size mismatch.");
    }

    int m = m1.size()[0];
    int k = m2.size()[1];
    matrix<decltype(m1.type_identifier * m2.type_identifier)> ret(
            std::vector<std::vector<decltype(m1.type_identifier * m2.type_identifier)>>(m, std::vector<decltype(
            m1.type_identifier * m2.type_identifier)>(k, 0)));

    std::vector<std::thread> t(max_thread);

    for (size_t work_id = 0; work_id < max_thread; ++work_id) {
        t[work_id] = std::thread(parallelMulti<Y, U, decltype(m1.type_identifier * m2.type_identifier)>, std::ref(m1),
                                 std::ref(m2), std::ref(ret), work_id);
    }

    for (size_t work_id = 0; work_id < max_thread; ++work_id) {
        if (t[work_id].joinable()) {
            t[work_id].join();
        }
    }

    return ret;
}

template<typename Y, typename U>
void parallelPlus(matrix<Y> &m1, matrix<U> &m2, size_t work_id) {
    unsigned int dev_work = ceil(double(m1.size()[0]) / double(max_thread));
    size_t start_index = work_id * dev_work;
    size_t end_index = ((work_id + 1) * dev_work < m1.size()[0]) ? (work_id + 1) * dev_work : m1.size()[0];

    for (; start_index < end_index; ++start_index) {
        for (size_t j = 0; j < m1.size()[1]; ++j) {
            m1[start_index][j] += m2[start_index][j];
        }
    }
}

template<typename Y, typename U>
auto operator+(matrix<Y> m1, matrix<U> m2) -> matrix<decltype(m1.type_identifier + m2.type_identifier)> {
    if (m1.size() != m2.size()) {
        throw std::runtime_error("matrix size mismatch.");
    }

    std::vector<std::thread> t(max_thread);

    matrix<decltype(m1.type_identifier + m2.type_identifier)> ret = m1;

    for (size_t work_id = 0; work_id < max_thread; ++work_id) {
        t[work_id] = std::thread(parallelPlus<decltype(ret.type_identifier), U>, std::ref(ret), std::ref(m2), work_id);
    }

    for (size_t work_id = 0; work_id < max_thread; ++work_id) {
        if (t[work_id].joinable()) {
            t[work_id].join();
        }
    }

    return ret;
}

template<typename Y, typename U>
auto operator-(matrix<Y> m1, matrix<U> m2) -> matrix<decltype(m1.type_identifier - m2.type_identifier)> {
    return m1 + (-m2);
}


#endif //LINEARALGEBRALIB_MATRIX_H
