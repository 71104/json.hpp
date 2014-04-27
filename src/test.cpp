#include <iostream>
#include <json.hpp>

using namespace std;

int main() {
	JSON::Object<
		JSON::Field<int, UNPACK("integer")>,
		JSON::Field<double, UNPACK("floating point")>,
		JSON::Field<string, UNPACK("string")>,
		JSON::Field<JSON::Object<
			JSON::Field<bool, UNPACK("flag")>
		>, UNPACK("object")>,
		JSON::Field<vector<vector<JSON::Object<
			JSON::Field<double, 'x'>,
			JSON::Field<double, 'y'>
		>>>, UNPACK("matrix")>
	> o;

	o.Get<UNPACK("integer")>();
	o.Get<UNPACK("matrix")>();
	o.Get<UNPACK("object")>().Get<UNPACK("flag")>();

	JSON::Object<JSON::Field<bool, UNPACK("boolean")>>().Get<UNPACK("boolean")>();

	cout << JSON::Field<int, UNPACK("Hello, world!")>::s_szName << endl;

	return 0;
}
