#include <iostream>
#include <json.hpp>

using namespace std;
using namespace JSON;

int main() {
	cout << Field<int, UNPACK("Hello, world!")>::s_szName << endl;

	Load<nullptr_t>(cin);

	Object<
		Field<Object<
			Field<unsigned int, UNPACK("major")>,
			Field<unsigned int, UNPACK("minor")>,
			Field<unsigned int, UNPACK("build")>
		>, UNPACK("version")>,
		Field<string, UNPACK("name")>,
		Field<array<array<double, 3>, 3>, UNPACK("matrix")>,
		Field<map<string, Object<
			Field<bool, UNPACK("deleted")>,
			Field<Object<
				Field<unsigned int, UNPACK("i0")>,
				Field<unsigned int, UNPACK("j0")>,
				Field<unsigned int, UNPACK("iSpan")>,
				Field<unsigned int, UNPACK("jSpan")>
			>, UNPACK("layout")>,
			Field<Object<
				Field<double, UNPACK("x")>,
				Field<double, UNPACK("y")>
			>, UNPACK("offset")>,
			Field<vector<Object<
				Field<unsigned int, UNPACK("id")>,
				OptionalField<unsigned int, UNPACK("duration")>
			>>, UNPACK("frames")>,
			Field<bool, UNPACK("walkable")>,
			Field<bool, UNPACK("mutable")>,
			Field<map<string, Any>, UNPACK("properties")>
		>>, UNPACK("tiles")>
	> o;

	o.Get<UNPACK("name")>();
	o.Get<UNPACK("matrix")>();
	o.Get<UNPACK("version")>().Get<UNPACK("minor")>();

	if (o.Get<UNPACK("tiles")>()["0"].Get<UNPACK("frames")>()[0].Has<UNPACK("duration")>()) {
		o.Get<UNPACK("tiles")>()["0"].Get<UNPACK("frames")>()[0].Get<UNPACK("duration")>();
	}

	cin >> o;
	cout << o;

	return 0;
}
