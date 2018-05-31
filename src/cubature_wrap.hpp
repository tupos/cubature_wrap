/*
	cubature_wrap a C++ wrapper for cubature C library
    Copyright (C) 2018 Oleg Skoromnik

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>
*/

#ifndef CUBATURE_WRAP_HPP
#define CUBATURE_WRAP_HPP

#include <cubature.h>
#include <tuple>
#include <array>
#include <utility>
#include <cstdlib>

namespace cub{

	template<class F>
		struct function_traits;

	// function pointer
	template<class R, class... Args>
		struct function_traits<R(*)(Args...)>
		: public function_traits<R(Args...)> {};

	// std::declval<F>()
	template<class R, class... Args>
		struct function_traits<R(*&&)(Args...)>
		: public function_traits<R(Args...)> {};

	// decltype()
	template<class R, class... Args>
		struct function_traits<R(Args...)>
		{
			using return_type = R;

			static constexpr std::size_t arity = sizeof...(Args);

			template <std::size_t N>
				struct argument
				{
					static_assert(N < arity,
							"error: invalid parameter index.");
					using type =
						typename std::tuple_element<N,
								 std::tuple<Args...>>::type;
				};
		};

	template <typename F,
			 std::size_t F_SIZE =
				 std::tuple_size<typename function_traits<
				 decltype(std::declval<F>())>::return_type>::value,
			 std::size_t X_SIZE =
				 std::tuple_size<typename function_traits<
				 decltype(std::declval<F>())>::template argument<0>
				 >::value
				 >
		class integrand_cxx{
			public:
				integrand_cxx(F func, std::size_t limit)
					:func(func),
					limit(limit) {}

				std::array<std::array<double,2>,F_SIZE> integrate(
						const std::array<double,X_SIZE>& x_min,
						const std::array<double,X_SIZE>& x_max,
						double eps_abs,
						double eps_rel,
						error_norm norm = ERROR_INDIVIDUAL
						)
				{
					integrand i = &cubature_int_wrapper;
					double *res = (double*)malloc(F_SIZE * sizeof(double));
					double *err = (double*)malloc(F_SIZE * sizeof(double));
					if(hcubature(F_SIZE, i, this, X_SIZE, x_min.data(),
							x_max.data(), limit, eps_abs, eps_rel,
							norm, res, err)){
						throw(std::runtime_error(
									"Error during integration"));
					}
					std::array<std::array<double,2>,F_SIZE> res_err;
					for(std::size_t j = 0; j < F_SIZE; ++j){
						res_err[j][0] = res[j];
						res_err[j][1] = err[j];
					}
					free(res);
					free(err);

					return res_err;
				}

			private:
				F func;
				const std::size_t limit;

				static int cubature_int_wrapper(
						unsigned ndim,
						const double *x,
						void *params,
						unsigned fdim,
						double *fval)
				{
					integrand_cxx *i = reinterpret_cast<integrand_cxx*>(
							params);
					std::array<double, X_SIZE> x_arr;
					for(std::size_t j = 0; j < X_SIZE; ++j){
						x_arr.at(j) = x[j];
					}
					std::array<double,F_SIZE> f_arr = i->func(x_arr);
					for(std::size_t j = 0; j < F_SIZE; ++j){
						fval[j] = f_arr.at(j);
					}
					return 0;
				}

		};

	template <typename F,
			 std::size_t F_SIZE =
				 std::tuple_size<typename function_traits<
				 decltype(std::declval<F>())>::return_type>::value,
			 std::size_t X_SIZE =
				 std::tuple_size<typename function_traits<
				 decltype(std::declval<F>())>::template argument<0>
				 >::value
				 >
		auto make_cubature_int(
				F func,
				std::size_t limit,
				const std::array<double,X_SIZE>& x_min,
				const std::array<double,X_SIZE>& x_max,
				double eps_abs,
				double eps_rel,
				error_norm norm = ERROR_INDIVIDUAL
				)
		{
			integrand_cxx<F,F_SIZE,X_SIZE> i(func,limit);

			return i.integrate(x_min, x_max, eps_abs, eps_rel, norm);
		}

} /* end namespace cub */

#endif /* CUBATURE_WRAP_HPP */
