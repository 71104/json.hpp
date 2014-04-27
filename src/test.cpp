#include <iostream>
#include <json.hpp>

using namespace std;

int main() {
	JSON::Object<
		JSON::Field<int, UNPACK("hello")>,
		JSON::Field<double, 'b'>,
		JSON::Field<string, 'c'>,
		JSON::Field<JSON::Object<
			JSON::Field<bool, 'f'>
		>, 'o'>,
		JSON::Field<vector<vector<JSON::Object<
			JSON::Field<double, 'x'>,
			JSON::Field<double, 'y'>
		>>>, 'x'>
	> o;

	o.Get<UNPACK("hello")>();
	o.Get<'x'>();
	o.Get<'o'>().Get<'f'>();

	JSON::Object<JSON::Field<bool, 'f'>>().Get<'f'>();

	cout << JSON::Field<bool, UNPACK("hello")>::s_szName << endl;

	return 0;
}
