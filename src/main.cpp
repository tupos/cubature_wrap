#include <iostream>
#include <array>
#include <cmath>

#include <cubature_wrap.hpp>

std::array<double,1> f(const std::array<double,3> &x){
	double e = 0.;
	for(const auto &el: x){
		e += el*el;
	}
	return {exp(-e)};
}

int main(int argc, char *argv[])
{
	double lim = 5. * 1./sqrt(2.);
	std::array<double,3> x_min = {-lim,-lim,-lim};
	std::array<double,3> x_max = {lim,lim,lim};
	double eps_abs = 0.0001;
	double eps_rel = 0.0001;

	std::cout << "gauss 3d = "
	   << cub::make_cubature_int(
			   f,100000,x_min,x_max,eps_abs,eps_rel)[0][0] << "\n";
	std::cout << "exact result = " << sqrt(M_PI) * M_PI << "\n";
	
	return 0;
}
