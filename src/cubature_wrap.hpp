#include <cubature.h>

namespace cub{

	template <typename F>
		class integrand{
			private:
				F func;
				unsigned f_size;
				unsigned x_size;
				std::size_t limit;

				static int cubature_int(
						unsigned ndim,
						const double *x,
						void *params,
						unsigned fdim,
						double *fval)
				{
					integrand *i = reinterpret_cast<intgrand*>(params);
					std::vector<double> x_arr;
					for(std::size_t j = 0; j < x_size; ++j){
						x_arr.push_back(x[i]);
					}
					std::vector<double> f_arr = i->func(x_arr);
					for(std::size_t j = 0; j < f_size; ++j){}
					return i->func(x_arr);
				}
		}

} /* end namespace cub */
