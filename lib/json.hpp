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
	struct Field {
		static char const s_szName;
		typedef _Value Type;
	};

	template<char const ..._sz>
	struct FieldName {};

	template<typename _Name, typename ..._Fields>
	struct GetFieldType {};

	template<typename _Value, char const ..._szName, typename ..._OtherFields>
	struct GetFieldType<FieldName<_szName...>, Field<_Value, _szName...>, _OtherFields...> {
		typedef _Value Type;
	};

	template<char const ..._szFieldName, typename _FirstField, typename ..._OtherFields>
	struct GetFieldType<FieldName<_szFieldName...>, _FirstField, _OtherFields...> {
		typedef typename GetFieldType<FieldName<_szFieldName...>, _OtherFields...>::Type Type;
	};

	template<typename _Value, char const ..._szName>
	char const Field<_Value, _szName...>::s_szName = { _szName... };

	template<typename ..._Fields>
	struct Object {};

	template<typename _Field, typename ..._Fields>
	struct Getter {};

	template<typename _Value, char const ..._szName, typename ..._OtherFields>
	struct Getter<Field<_Value, _szName...>, Field<_Value, _szName...>, _OtherFields...> {
		static _Value &Get(Object<Field<_Value, _szName...>, _OtherFields...> &rObject) {
			return rObject.m_Value;
		}

		static _Value const &Get(Object<Field<_Value, _szName...>, _OtherFields...> const &rObject) {
			return rObject.m_Value;
		}
	};

	template<typename _Value, char const ..._szName, typename _FirstField, typename ..._OtherFields>
	struct Getter<Field<_Value, _szName...>, _FirstField, _OtherFields...> {
		static _Value &Get(Object<_FirstField, _OtherFields...> &rObject) {
			return Getter<Field<_Value, _szName...>, _OtherFields...>::Get(rObject);
		}

		static _Value const &Get(Object<_FirstField, _OtherFields...> const &rObject) {
			return Getter<Field<_Value, _szName...>, _FirstField, _OtherFields...>::Get(rObject);
		}
	};

	template<>
	struct Object<> {
		Object() {}
		virtual ~Object() {}
	};

	template<typename _Value, char const ..._szName, typename ..._OtherFields>
	struct Object<Field<_Value, _szName...>, _OtherFields...> :
		public Object<_OtherFields...>
	{
		static char const s_szName[];
		_Value m_Value;

		Object() {}

		virtual ~Object() {}

		template<char const ..._szFieldName>
		typename GetFieldType<FieldName<_szFieldName...>, Field<_Value, _szName...>, _OtherFields...>::Type &Get() {
			return Getter<Field<_Value, _szFieldName...>, Field<_Value, _szName...>, _OtherFields...>::Get(*this);
		}

		template<char const ..._szFieldName>
		typename GetFieldType<FieldName<_szFieldName...>, Field<_Value, _szName...>, _OtherFields...>::Type const &Get() const {
			return Getter<Field<_Value, _szFieldName...>, Field<_Value, _szName...>, _OtherFields...>::Get(*this);
		}
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
			throw Error();
		}
	};

	template<>
	struct Loader<bool> {
		static bool Load(istream &ris) {
			// TODO
			throw Error();
		}
	};

	template<>
	struct Loader<int> {
		static int Load(istream &ris) {
			// TODO
			throw Error();
		}
	};

	template<>
	struct Loader<unsigned int> {
		static unsigned int Load(istream &ris) {
			// TODO
			throw Error();
		}
	};

	template<>
	struct Loader<double> {
		static double Load(istream &ris) {
			// TODO
			throw Error();
		}
	};

	template<>
	struct Loader<string> {
		static string Load(istream &ris) {
			// TODO
			throw Error();
		}
	};

	template<typename ..._Fields>
	struct Loader<Object<_Fields...>> {
		static Object<_Fields...> Load(istream &ris) {
			// TODO
			throw Error();
		}
	};

	template<typename _Element>
	struct Loader<vector<_Element>> {
		static vector<_Element> Load(istream &ris) {
			// TODO
			throw Error();
		}
	};

	template<typename _Type>
	_Type Load(istream &ris) {
		return Loader<_Type>::Load(ris);
	}
}

#define UNPACK(sz) (sz)[0], ((sz)[0] > 0) ? UNPACK((sz) + 1) : 0

#endif
