#include <iostream>
#include <json.hpp>

using namespace std;

int main() {
	JSON::Object<
		JSON::Field<int, 'a'>,
		JSON::Field<double, 'b'>,
		JSON::Field<string, 'c'>,
		JSON::Field<vector<vector<JSON::Object<
			JSON::Field<double, 'x'>,
			JSON::Field<double, 'y'>
		>>>, 'x'>
	>().Get<'a'>();

	JSON::Object<JSON::Field<bool, 'f'>>().Get<'f'>();

	return 0;
}
