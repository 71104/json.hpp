#include <iostream>
#include <json.hpp>

using namespace std;
using namespace JSON;

int main() {
	Object<
		Field<int, UNPACK("integer")>,
		Field<double, UNPACK("floating point")>,
		Field<string, UNPACK("string")>,
		OptionalField<vector<bool>, UNPACK("optional field")>,
		Field<Object<
			Field<bool, UNPACK("flag")>
		>, UNPACK("object")>,
		Field<vector<vector<Object<
			Field<double, UNPACK("x")>,
			Field<double, UNPACK("y")>
		>>>, UNPACK("matrix")>
	> o;

	o.Get<UNPACK("integer")>();
	o.Get<UNPACK("matrix")>();
	o.Get<UNPACK("object")>().Get<UNPACK("flag")>();

	if (o.Has<UNPACK("optional field")>()) {
		o.Get<UNPACK("optional field")>();
	}

	Object<Field<bool, UNPACK("boolean")>>().Get<UNPACK("boolean")>();

	cout << Field<int, UNPACK("Hello, world!")>::s_szName << endl;

	Load<nullptr_t>(cin);

	return 0;
}
