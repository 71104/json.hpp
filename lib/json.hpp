#ifndef __JSON_HPP__
#define __JSON_HPP__

#include <cctype>
#include <iostream>
#include <string>
#include <vector>
#include <array>
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

	template<typename _Value, char const ..._szName>
	char const Field<_Value, _szName...>::s_szName = { _szName... };

	template<char const ..._sz>
	struct FieldName {};

	template<typename _Name, typename ..._Fields>
	struct FieldType {};

	template<typename _Value, char const ..._szName, typename ..._OtherFields>
	struct FieldType<FieldName<_szName...>, Field<_Value, _szName...>, _OtherFields...> {
		typedef _Value Type;
	};

	template<char const ..._szFieldName, typename _FirstField, typename ..._OtherFields>
	struct FieldType<FieldName<_szFieldName...>, _FirstField, _OtherFields...> {
		typedef typename FieldType<FieldName<_szFieldName...>, _OtherFields...>::Type Type;
	};

	template<typename ..._Fields>
	struct Object {};

	template<typename _Field, typename ..._Fields>
	struct Getter {};

	template<typename _Value, char const ..._szName, typename ..._OtherFields>
	struct Getter<Field<_Value, _szName...>, Field<_Value, _szName...>, _OtherFields...> {
		inline static _Value &Get(Object<Field<_Value, _szName...>, _OtherFields...> &rObject) {
			return rObject.m_Value;
		}

		inline static _Value const &Get(Object<Field<_Value, _szName...>, _OtherFields...> const &rObject) {
			return rObject.m_Value;
		}
	};

	template<typename _Value, char const ..._szName, typename _FirstField, typename ..._OtherFields>
	struct Getter<Field<_Value, _szName...>, _FirstField, _OtherFields...> {
		inline static _Value &Get(Object<_FirstField, _OtherFields...> &rObject) {
			return Getter<Field<_Value, _szName...>, _OtherFields...>::Get(rObject);
		}

		inline static _Value const &Get(Object<_FirstField, _OtherFields...> const &rObject) {
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
		inline typename FieldType<FieldName<_szFieldName...>, Field<_Value, _szName...>, _OtherFields...>::Type &Get() {
			return Getter<
				Field<typename FieldType<
					FieldName<_szFieldName...>,
					Field<_Value, _szName...>,
					_OtherFields...
				>::Type, _szFieldName...>,
				Field<_Value, _szName...>,
				_OtherFields...
			>::Get(*this);
		}

		template<char const ..._szFieldName>
		inline typename FieldType<FieldName<_szFieldName...>, Field<_Value, _szName...>, _OtherFields...>::Type const &Get() const {
			return Getter<
				Field<typename FieldType<
					FieldName<_szFieldName...>,
					Field<_Value, _szName...>,
					_OtherFields...
				>::Type, _szFieldName...>,
				Field<_Value, _szName...>,
				_OtherFields...
			>::Get(*this);
		}
	};

	template<typename _Value, char const ..._szName, typename ..._OtherFields>
	char const Object<Field<_Value, _szName...>, _OtherFields...>::s_szName[] = { _szName... };

	template<typename _Type>
	struct Serializer {
		static _Type Load(istream &ris);
		static ostream &Store(ostream &ros, _Type const &r);
	};

	template<>
	struct Serializer<nullptr_t> {
		static nullptr_t Load(istream &ris) {
			// TODO
			throw Error();
		}

		static ostream &Store(ostream &ros, nullptr_t const &r) {
			return ros << "null";
		}
	};

	template<>
	struct Serializer<bool> {
		static bool Load(istream &ris) {
			// TODO
			throw Error();
		}

		static ostream &Store(ostream &ros, bool const &r) {
			if (r) {
				return ros << "true";
			} else {
				return ros << "false";
			}
		}
	};

	template<>
	struct Serializer<int> {
		static int Load(istream &ris) {
			// TODO
			throw Error();
		}

		static ostream &Store(ostream &ros, int const &r) {
			return ros << r;
		}
	};

	template<>
	struct Serializer<unsigned int> {
		static unsigned int Load(istream &ris) {
			// TODO
			throw Error();
		}

		static ostream &Store(ostream &ros, unsigned int const &r) {
			return ros << r;
		}
	};

	template<>
	struct Serializer<double> {
		static double Load(istream &ris) {
			// TODO
			throw Error();
		}

		static ostream &Store(ostream &ros, double const &r) {
			return ros << r;
		}
	};

	template<>
	struct Serializer<string> {
		static string Load(istream &ris) {
			// TODO
			throw Error();
		}

		static ostream &Store(ostream &ros, string const &r) {
			return ros << '\"' << r << '\"'; // FIXME escape special characters
		}
	};

	template<typename ..._Fields>
	struct Serializer<Object<_Fields...>> {
		static Object<_Fields...> Load(istream &ris) {
			// TODO
			throw Error();
		}
	};

	template<typename _Element>
	struct Serializer<vector<_Element>> {
		static vector<_Element> Load(istream &ris) {
			// TODO
			throw Error();
		}
	};

	template<typename _Element, unsigned int _c>
	struct Serializer<array<_Element, _c>> {
		static array<_Element, _c> Load(istream &ris) {
			// TODO
			throw Error();
		}
	};

	template<typename _Type>
	inline _Type Load(istream &ris) {
		return Serializer<_Type>::Load(ris);
	}

	template<typename _Type>
	inline ostream &Store(ostream &ros, _Type const &r) {
		return Serializer<_Type>::Store(ros, r);
	}
}

template<typename ..._Fields>
inline std::istream &operator >> (std::istream &ris, JSON::Object<_Fields...> &rObject) {
	rObject = JSON::Load<JSON::Object<_Fields...>>(ris);
	return ris;
}

template<typename ..._Fields>
inline std::ostream &operator << (std::ostream &ros, JSON::Object<_Fields...> &rObject) {
	return JSON::Store<JSON::Object<_Fields...>>(ros, rObject);
}

static char UNPACK(char const sz[]) {
	return 0;
}

static char (*JSON_ALMOST_UNPACK())(char const[]) {
	return UNPACK;
}

#define JSON_EMPTY(...)
#define JSON_DEFER(...) __VA_ARGS__ JSON_EMPTY()
#define JSON_EXPAND(...) __VA_ARGS__

#define JSON_ALMOST_UNPACK() UNPACK
#define UNPACK(sz) (sz)[0], ((sz)[0] > 0) ? JSON_DEFER(JSON_ALMOST_UNPACK)()((sz) + 1) : 0

#endif
