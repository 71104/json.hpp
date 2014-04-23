#ifndef __JSON_HPP__
#define __JSON_HPP__

#include <string>

namespace JSON {
	using namespace std;

	struct Error {};
	struct SyntaxError : public Error {};
	struct LoadError: public Error {};

	template<typename ..._Fields>
	struct Object {};

	template<typename _Field>
	struct Object<_Field> {
		// TODO
	};

	template<typename _FirstField, typename ..._OtherFields>
	struct Object<_FirstField, _OtherFields...> :
		public Object<_OtherFields...>
	{
		// TODO
	};

	template<typename _Type>
	struct Synthesize {};

	template<>
	struct Synthesize<nullptr_t> {
		typedef nullptr_t Type;
	};

	template<>
	struct Synthesize<bool> {
		typedef bool Type;
	};

	template<>
	struct Synthesize<int> {
		typedef int Type;
	};

	template<>
	struct Synthesize<unsigned int> {
		typedef unsigned int Type;
	};

	template<>
	struct Synthesize<double> {
		typedef double Type;
	};

	template<>
	struct Synthesize<string> {
		typedef string Type;
	};

	template<typename _Type>
	typename Synthesize<_Type>::Type Load(istream &ris);

	template<>
	nullptr_t Load<nullptr_t>(istream &ris);
}

#endif
