#include <iostream>
#include <json.hpp>

using namespace std;

int main() {
	cout << JSON::Object<
		JSON::Field<bool, 'f'>,
		JSON::Field<double, 'x'>
	>().Get<'x'>() << endl;
	return 0;
}
