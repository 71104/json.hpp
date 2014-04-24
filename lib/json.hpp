#ifndef __JSON_HPP__
#define __JSON_HPP__

#include <cctype>
#include <string>
#include <vector>
#include <regex>

namespace JSON {
	using namespace std;

	struct Error {};
	struct SyntaxError : public Error {};
	struct LoadError : public Error {};
	struct AccessError : public Error {};

	template<typename _Value, char const ..._szName>
	struct Field {};

	template<typename ..._Fields>
	struct Object {};

	template<typename _Object, char const ..._szName>
	struct Getter {};

	template<typename _Value, char const ..._szName, typename ..._OtherFields>
	struct Getter<Object<Field<_Value, _szName...>, _OtherFields...>, _szName...> {
		static _Value &Get(Object<Field<_Value, _szName...>, _OtherFields...> &rObject) {
			return rObject.m_Value;
		}

		static _Value const &Get(Object<Field<_Value, _szName...>, _OtherFields...> const &rObject) {
			return rObject.m_Value;
		}
	};

	template<typename _Value, char const ..._szName>
	struct Object<Field<_Value, _szName...>> {
		static char const s_szName[];
		_Value m_Value;

		Object() {}

		Object(_Value const &a_rValue)
			:
		m_Value(a_rValue) {}

		Object(_Value &&a_rrValue)
			:
		m_Value(move(a_rrValue)) {}

		virtual ~Object() {}

		template<char const ..._szFieldName>
		_Value &Get() {
			return Getter<Object<Field<_Value, _szFieldName...>>, _szFieldName...>::Get(*this);
		}

		template<char const ..._szFieldName>
		_Value const &Get() const {
			return Getter<Object<Field<_Value, _szFieldName...>>, _szFieldName...>::Get(*this);
		}
	};

	template<typename _Value, char const ..._szName>
	char const Object<Field<_Value, _szName...>>::s_szName[] = { _szName... };

	template<typename _Value, char const ..._szName, typename ..._OtherFields>
	struct Object<Field<_Value, _szName...>, _OtherFields...> :
		public Object<_OtherFields...>
	{
		static char const s_szName[];
		_Value m_Value;

		Object() {}

		virtual ~Object() {}
	};

	template<typename _Value, char const ..._szName, typename ..._OtherFields>
	char const Object<Field<_Value, _szName...>, _OtherFields...>::s_szName[] = { _szName... };

	template<typename _Type>
	struct Loader {
		static _Type Load(istream &ris);
	};

	template<>
	struct Loader<nullptr_t> {
		static nullptr_t Load(istream &ris) {
			// TODO
		}
	};

	template<>
	struct Loader<bool> {
		static bool Load(istream &ris) {
			// TODO
		}
	};

	template<>
	struct Loader<int> {
		static int Load(istream &ris) {
			// TODO
		}
	};

	template<>
	struct Loader<unsigned int> {
		static unsigned int Load(istream &ris) {
			// TODO
		}
	};

	template<>
	struct Loader<double> {
		static double Load(istream &ris) {
			// TODO
		}
	};

	template<>
	struct Loader<string> {
		static string Load(istream &ris) {
			// TODO
		}
	};

	template<typename ..._Fields>
	struct Loader<Object<_Fields...>> {
		static Object<_Fields...> Load(istream &ris) {
			// TODO
		}
	};

	template<typename _Element>
	struct Loader<vector<_Element>> {
		static vector<_Element> Load(istream &ris) {
			// TODO
		}
	};

	template<typename _Type>
	_Type Load(istream &ris) {
		return Loader<_Type>::Load(ris);
	}
}

#endif
