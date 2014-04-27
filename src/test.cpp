#include <iostream>
#include <json.hpp>

using namespace std;

int main() {
	JSON::Object<
		JSON::Field<int, 'a'>,
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

	o.Get<'a'>();
	o.Get<'x'>();
	o.Get<'o'>().Get<'f'>();

	JSON::Object<JSON::Field<bool, 'f'>>().Get<'f'>();

	JSON::FieldName<JSON_EXPAND(JSON_EXPAND(UNPACK("")))>();

	return 0;
}
