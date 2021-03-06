#include "../util/template.hpp"

namespace math {

template <typename T, size_type M, size_type N>
struct Matrix : mdarray<T, M, N> {
    using mdarray<T, M, N>::mdarray;

    constexpr Matrix& operator+=(const Matrix<T, M, N> &rhs) {
        for (size_type i = 0; i < M; i++) for (size_type j = 0; j < N; j++) (*this)[i][j] += rhs[i][j];
    }

    constexpr Matrix operator+(const Matrix<T, M, N> &rhs) const {
        return Matrix(*this) += rhs;
    }
    
    template <size_type L>
    constexpr Matrix<T, M, L> operator*(const Matrix<T, N, L> &rhs) const {
        Matrix<T, M, L> ret;
        for (size_type i = 0; i < M; i++) for (size_type j = 0; j < L; j++) ret[i][j] = T();
        for (size_type i = 0; i < M; i++) {
            for (size_type j = 0; j < N; j++) {
                for (size_type k = 0; k < L; k++) {
                    ret[i][k] += (*this)[i][j] * rhs[j][k];
                }
            }
        }
        return ret;
    }

    constexpr Matrix& operator*=(const T &c) {
        for (size_type i = 0; i < M; i++) for (size_type j = 0; j < N; j++) (*this)[i][j] *= c;
        return *this;
    }

    constexpr Matrix operator*(const T &c) const {
        return Matrix(*this) *= c;
    }

    size_type row() const noexcept {
        return M;
    }

    size_type col() const noexcept {
        return N;
    }

    static Matrix add_id_ele() {
        Matrix ret;
        fill_seq(ret, [&](size_type, size_type) { return T(0); });
        return ret;
    }

    static Matrix mul_id_ele() {
        static_assert(M == N, "Matrix must be square.");
        Matrix ret;
        fill_seq(ret, [&](size_type i, size_type j) { 
            return i == j ? T(1) : T(0);  // TODO : is this correct ??
        });
        return ret;
    }
};

template <typename T, size_type M, size_type N>
std::ostream& operator<<(std::ostream &os, const Matrix<T, M, N> &mat) {
    for (size_type i = 0; i < M; i++) {
        os << (i == 0 ? "[ " : "  ");
        for (size_type j = 0; j < N; j++) os << mat[i][j] << ", ";
        os << (i + 1 == M ? " ]" : "\n");
    }
    return os;
}

}
