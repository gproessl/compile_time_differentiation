#ifndef SYMBOLIC_DIFFERENTIATION_DIFFERENTIATION_HPP
#define SYMBOLIC_DIFFERENTIATION_DIFFERENTIATION_HPP

#include "ast.hpp"

namespace diff
{
    template<typename T, size_t x_i>
    struct derivative;

    template<typename T, size_t x_i>
    using derivative_t = typename derivative<T, x_i>::type;

    template<size_t i, size_t x_i>
    struct derivative<ast::var<i>, x_i>
    {
        using type = std::conditional_t<x_i == i, ast::constant<1>, ast::constant<0>>;
    };

    template<long c, size_t x_i>
    struct derivative<ast::constant<c>, x_i>
    {
        using type = ast::constant<0>;
    };

    template<size_t x_i>
    struct derivative<ast::e, x_i>
    {
        using type = ast::constant<0>;
    };

    template<typename lhs, typename rhs, size_t x_i>
    struct derivative<ast::plus<lhs, rhs>, x_i>
    {
        using type = ast::plus<derivative_t<lhs, x_i>, derivative_t<rhs, x_i>>;
    };

    template<typename lhs, typename rhs, size_t x_i>
    struct derivative<ast::minus<lhs, rhs>, x_i>
    {
        using type = ast::minus<derivative_t<lhs, x_i>, derivative_t<rhs, x_i>>;
    };

    template<typename lhs, typename rhs, size_t x_i>
    struct derivative<ast::times<lhs, rhs>, x_i>
    {
        using type = ast::plus<ast::times<derivative_t<lhs, x_i>, rhs>, ast::times<lhs, derivative_t<rhs, x_i>>>;
    };

    template<typename lhs, typename rhs, size_t x_i>
    struct derivative<ast::divide<lhs, rhs>, x_i>
    {
        using type = ast::divide<
                ast::minus<
                        ast::times<derivative_t<lhs, x_i>, rhs>,
                        ast::times<lhs, derivative_t<rhs, x_i>>>,
                ast::pow<rhs, ast::constant<2>>>;
    };

    template<size_t i, long c, size_t d>
    struct derivative<ast::pow<ast::var<i>, ast::constant<c>>, d>
    {
        using type = std::conditional_t<i == d,
                ast::times<ast::constant<c>, ast::pow<ast::var<i>, ast::constant<c - 1>>>,
                ast::constant<0>>;
    };

    template<typename T, size_t d>
    struct derivative<ast::pow<ast::e, T>, d>
    {
        using type = ast::times<derivative_t<T, d>, ast::pow<ast::e, T>>;
    };

    template<typename F, typename G, size_t d>
    struct derivative<ast::pow<F, G>, d>
    {
        using type = ast::times<
                ast::pow<F, G>,
                ast::plus<
                        ast::times<derivative_t<G, d>, ast::ln<F>>,
                        ast::times<G, ast::divide<derivative_t<F, d>, F>>>>;
    };

    template<typename T, size_t x_i>
    struct derivative<ast::sin<T>, x_i>
    {
        using type = ast::times<derivative_t<T, x_i>, ast::cos<T>>;
    };

    template<typename T, size_t x_i>
    struct derivative<ast::cos<T>, x_i>
    {
        using type = ast::times<derivative_t<T, x_i>, ast::times<ast::constant<-1>, ast::sin<T>>>;
    };


    template<typename T, size_t x_i>
    struct derivative<ast::ln<T>, x_i>
    {
        using type = ast::divide<derivative_t<T, x_i>, T>;
    };
}

#endif //SYMBOLIC_DIFFERENTIATION_DIFFERENTIATION_HPP
