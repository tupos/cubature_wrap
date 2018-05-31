## cub_wrap - A `C++` wrapper for [cubature][cubature] `C` library



Contents
--------
	
- [Intro](#intro)
- [Installation](#installation)
- [Contact](#contact)
- [License](#license)

Intro
-----

This is a `C++` wrapper for [cubature][cubature] `C` library. 

The main goal that I wanted to achieve is to obtain the syntax as
close as possible to `mathematica`, i.e.,

```c++
cub::make_cubature_int(F, limit, x_min, x_max,
eps_abs, eps_rel, norm)
```

where `F` is any callable object, with the following signature

```c++
std::array<double, F_SIZE>
(*)(const std::array<double, X_SIZE> &)
```

Luckily, the modern `C++` standard allows one to have such a clean
syntax. Therefore, the wrapper implements a single function, which is
located in the namespace `cub` and has the signature

```c++
std::array<std::array<double,2>, F_SIZE>
make_cubature_int(
				F func,
				std::size_t limit,
				const std::array<double,X_SIZE>& x_min,
				const std::array<double,X_SIZE>& x_max,
				double eps_abs,
				double eps_rel,
				error_norm norm = ERROR_INDIVIDUAL
				)
```

However, the length of the vector function `F_SIZE` and the dimension
of the integration `X_SIZE` are determined by the compiler
automatically through the templates.

This function returns a two dimensional array of length `F_SIZE`,
where the first array element is the result and the second element is
the error respectively.

Consequently, the actual usage is the following

```c++

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

```



Contact
-------

If you have questions about the program or need help, please write me
an email <olegskor@gmail.com>.

If you have bug reports or feature suggestions, please use the [issue
tracker][issueTracker].


The author's homepage is <https://github.com/tupos>.

License
-------

effz is free software, you can redistribute it and/or modify it under
the terms of [Apache License 2.0][apache].

Unless required by applicable law or agreed to in writing, Licensor
provides the Work (and each Contributor provides its Contributions) on
an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either
express or implied, including, without limitation, any warranties or
conditions of TITLE, NON-INFRINGEMENT, MERCHANTABILITY, or FITNESS FOR
A PARTICULAR PURPOSE.

© 2018 cub_wrapper authors

[apache]: https://www.apache.org/licenses/LICENSE-2.0
[cubature]: https://github.com/stevengj/cubature
[jphysb]: http://iopscience.iop.org/article/10.1088/1361-6455/aa92e6/meta
[wikiHydrogen]: https://en.wikipedia.org/wiki/Hydrogen_atom
[wikiBasisSet]: https://en.wikipedia.org/wiki/Orthonormal_basis
[wikiThomasFermi]: https://en.wikipedia.org/wiki/Thomas%E2%80%93Fermi_model
[issueTracker]: https://github.com/tupos/effz/issues
[gsl]: https://www.gnu.org/software/gsl/
[tbb]: https://www.threadingbuildingblocks.org/
[scipy]: https://www.scipy.org/about.html
[scipyInstall]: https://www.scipy.org/install.html
[sympy]: http://www.sympy.org/de/index.html
[matplotlib]: https://matplotlib.org/index.html
[homebrew]: https://brew.sh/
[effztar]: https://github.com/tupos/effz/releases
