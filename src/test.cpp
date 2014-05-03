#include <iostream>
#include <json.hpp>

using namespace std;

int main() {
	JSON::Object<
		JSON::Field<int, UNPACK("integer")>,
		JSON::Field<double, UNPACK("floating point")>,
		JSON::Field<string, UNPACK("string")>,
		JSON::OptionalField<vector<bool>, UNPACK("optional field")>,
		JSON::Field<JSON::Object<
			JSON::Field<bool, UNPACK("flag")>
		>, UNPACK("object")>,
		JSON::Field<vector<vector<JSON::Object<
			JSON::Field<double, UNPACK("x")>,
			JSON::Field<double, UNPACK("y")>
		>>>, UNPACK("matrix")>
	> o;

	o.Get<UNPACK("integer")>();
	o.Get<UNPACK("matrix")>();
	o.Get<UNPACK("object")>().Get<UNPACK("flag")>();

	if (o.Has<UNPACK("optional field")>()) {
		o.Get<UNPACK("optional field")>();
	}

	JSON::Object<JSON::Field<bool, UNPACK("boolean")>>().Get<UNPACK("boolean")>();

	cout << JSON::Field<int, UNPACK("Hello, world!")>::s_szName << endl;

	JSON::Load<nullptr_t>(cin);

	return 0;
}
