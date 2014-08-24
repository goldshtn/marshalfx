Extensions for the Visual Studio C++/CLI marshaling framework
=============================================================

This project provides an alternative entry point for marshaling C++/CLI types
to C++ types and the other way around. It specifically adds support for marshaling
STL and CLR collections, such as ```std::vector``` and ```System::Collections::Generic::Dictionary```.
Nested collections are also supported (e.g. ```std::map<std::string, std::vector<int>>```).

This is a header-only library. You need the *marshal_fx.h* and *marshal_fx_details.h* header
files to use the library. The rest of the code are tests and cruft required by the
Visual Studio project system.

Usage examples (also see the unit tests for more examples):

```c++
#include "marshal_fx.h"

auto num = marshal_fx::marshal_as<int>(42); // useless but supported
auto str = marshal_fx::marshal_as<std::string>(gcnew System::String("string"));

namespace SCG = System::Collections::Generic;

auto list = marshal_fx::marshal_as<SCG::List<int>^>(std::vector<int>{ 1, 2, 3 });
auto vect = marshal_fx::marshal_as<std::vector<int>>(list);

auto dictionary = marshal_fx::marshal_as<SCG::Dictionary<System::String^, int>^>(
	std::map<std::string, int>{ { "first", 1 }, { "second", 2 } }
);
```

To extend the library such that it works with your own custom types, DO NOT specialize
the ```marshal_fx::marshal_as``` method. Instead, specialize the ```msclr::interop::marshal_as```
method, as per Microsoft instructions. Then, ```marshal_fx::marshal_as``` will pick up your
customization and use it.
