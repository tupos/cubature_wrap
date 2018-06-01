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
#include <stdexcept>

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

				std::array<std::array<double,2>,F_SIZE> integrate_h(
						const std::array<double,X_SIZE>& x_min,
						const std::array<double,X_SIZE>& x_max,
						double eps_abs,
						double eps_rel,
						error_norm norm = ERROR_INDIVIDUAL
						)
				{
					return integrate(hcubature,x_min,x_max,
							eps_abs,eps_rel,norm);
				}

				std::array<std::array<double,2>,F_SIZE> integrate_p(
						const std::array<double,X_SIZE>& x_min,
						const std::array<double,X_SIZE>& x_max,
						double eps_abs,
						double eps_rel,
						error_norm norm = ERROR_INDIVIDUAL
						)
				{
					return integrate(pcubature,x_min,x_max,
							eps_abs,eps_rel,norm);
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

				typedef int (*cub_f_pointer)(unsigned, integrand, void*,
						unsigned, const double*, const double*,
						std::size_t, double, double, error_norm,
						double*, double*);

				std::array<std::array<double,2>,F_SIZE> integrate(
						cub_f_pointer cub_routine,
						const std::array<double,X_SIZE>& x_min,
						const std::array<double,X_SIZE>& x_max,
						double eps_abs,
						double eps_rel,
						error_norm norm)
				{
					integrand i = &cubature_int_wrapper;
					double *res = (double*)malloc(F_SIZE * sizeof(double));
					double *err = (double*)malloc(F_SIZE * sizeof(double));
					if((*cub_routine)(F_SIZE, i, this, X_SIZE, x_min.data(),
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
		auto make_cubature_h_int(
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

			return i.integrate_h(x_min, x_max, eps_abs, eps_rel, norm);
		}

	template <typename F,
			 std::size_t F_SIZE =
				 std::tuple_size<typename function_traits<
				 decltype(std::declval<F>())>::return_type>::value,
			 std::size_t X_SIZE =
				 std::tuple_size<typename function_traits<
				 decltype(std::declval<F>())>::template argument<0>
				 >::value
				 >
		auto make_cubature_p_int(
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

			return i.integrate_p(x_min, x_max, eps_abs, eps_rel, norm);
		}

} /* end namespace cub */

#endif /* CUBATURE_WRAP_HPP */
