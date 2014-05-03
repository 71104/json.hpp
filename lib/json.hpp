#ifndef __JSON_HPP__
#define __JSON_HPP__

#include <cctype>
#include <string>
#include <iostream>
#include <iterator>
#include <sstream>
#include <vector>
#include <array>
#include <map>
#include <regex>

namespace JSON {
	using namespace std;

	struct Error {};
	struct SyntaxError : public Error {};
	struct LoadError : public Error {};
	struct AccessError : public Error {};

	template<typename _Value, char const ..._szName>
	struct Field {
		static char const s_szName[];
		typedef _Value Type;
	};

	template<typename _Value, char const ..._szName>
	char const Field<_Value, _szName...>::s_szName[] = { _szName... };

	template<typename _Value, char const ..._szName>
	struct OptionalField {
		static char const s_szName[];
		typedef _Value Type;
	};

	template<typename _Value, char const ..._szName>
	char const OptionalField<_Value, _szName...>::s_szName[] = { _szName... };

	template<char const ..._sz>
	struct FieldName {
		static char const s_szName[];
	};

	template<char const ..._sz>
	char const FieldName<_sz...>::s_szName[] = { _sz... };

	template<typename _Name, typename ..._Fields>
	struct FieldType {};

	template<typename _Value, char const ..._szName, typename ..._OtherFields>
	struct FieldType<FieldName<_szName...>, Field<_Value, _szName...>, _OtherFields...> {
		typedef _Value Type;
	};

	template<typename _Value, char const ..._szName, typename ..._OtherFields>
	struct FieldType<FieldName<_szName...>, OptionalField<_Value, _szName...>, _OtherFields...> {
		typedef _Value Type;
	};

	template<char const ..._szFieldName, typename _FirstField, typename ..._OtherFields>
	struct FieldType<FieldName<_szFieldName...>, _FirstField, _OtherFields...> {
		typedef typename FieldType<FieldName<_szFieldName...>, _OtherFields...>::Type Type;
	};

	template<typename ..._Fields>
	struct Object {};

	template<typename _FieldName, typename ..._Fields>
	struct Getter {};

	template<typename _Value, char const ..._szName, typename ..._OtherFields>
	struct Getter<FieldName<_szName...>, Field<_Value, _szName...>, _OtherFields...> {
		typedef _Value Type;

		inline static Type &Get(Object<Field<_Value, _szName...>, _OtherFields...> &rObject) {
			return rObject.m_Value;
		}

		inline static Type const &Get(Object<Field<_Value, _szName...>, _OtherFields...> const &rObject) {
			return rObject.m_Value;
		}
	};

	template<typename _Value, char const ..._szName, typename ..._OtherFields>
	struct Getter<FieldName<_szName...>, OptionalField<_Value, _szName...>, _OtherFields...> {
		typedef _Value Type;

		inline static bool Has(Object<OptionalField<_Value, _szName...>, _OtherFields...> const &rObject) {
			return rObject.m_fPresent;
		}

		inline static Type &Get(Object<OptionalField<_Value, _szName...>, _OtherFields...> &rObject) {
			return rObject.m_Value;
		}

		inline static Type const &Get(Object<OptionalField<_Value, _szName...>, _OtherFields...> const &rObject) {
			return rObject.m_Value;
		}
	};

	template<char const ..._szName, typename _FirstField, typename ..._OtherFields>
	struct Getter<FieldName<_szName...>, _FirstField, _OtherFields...> {
		typedef typename FieldType<FieldName<_szName...>, _FirstField, _OtherFields...>::Type Type;

		inline static Type &Get(Object<_FirstField, _OtherFields...> &rObject) {
			return Getter<FieldName<_szName...>, _OtherFields...>::Get(rObject);
		}

		inline static Type const &Get(Object<_FirstField, _OtherFields...> const &rObject) {
			return Getter<FieldName<_szName...>, _FirstField, _OtherFields...>::Get(rObject);
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
			return Getter<FieldName<_szFieldName...>, Field<_Value, _szName...>, _OtherFields...>::Get(*this);
		}

		template<char const ..._szFieldName>
		inline typename FieldType<FieldName<_szFieldName...>, Field<_Value, _szName...>, _OtherFields...>::Type const &Get() const {
			return Getter<FieldName<_szFieldName...>, Field<_Value, _szName...>, _OtherFields...>::Get(*this);
		}
	};

	template<typename _Value, char const ..._szName, typename ..._OtherFields>
	char const Object<Field<_Value, _szName...>, _OtherFields...>::s_szName[] = { _szName... };

	template<typename _Value, char const ..._szName, typename ..._OtherFields>
	struct Object<OptionalField<_Value, _szName...>, _OtherFields...> :
		public Object<_OtherFields...>
	{
		static char const s_szName[];
		_Value m_Value;
		bool m_fPresent;

		Object()
			:
		m_fPresent(false) {}

		virtual ~Object() {}

		template<char const ..._szFieldName>
		inline bool Has() {
			return Getter<FieldName<_szFieldName...>, OptionalField<_Value, _szName...>, _OtherFields...>::Has(*this);
		}

		template<char const ..._szFieldName>
		inline typename FieldType<FieldName<_szFieldName...>, OptionalField<_Value, _szName...>, _OtherFields...>::Type &Get() {
			return Getter<FieldName<_szFieldName...>, OptionalField<_Value, _szName...>, _OtherFields...>::Get(*this);
		}

		template<char const ..._szFieldName>
		inline typename FieldType<FieldName<_szFieldName...>, OptionalField<_Value, _szName...>, _OtherFields...>::Type const &Get() const {
			return Getter<FieldName<_szFieldName...>, OptionalField<_Value, _szName...>, _OtherFields...>::Get(*this);
		}
	};

	template<typename _Value, char const ..._szName, typename ..._OtherFields>
	char const Object<OptionalField<_Value, _szName...>, _OtherFields...>::s_szName[] = { _szName... };

	struct Any {
		enum Type {
			TYPE_NULL,
			TYPE_BOOL,
			TYPE_INT,
			TYPE_UINT,
			TYPE_DOUBLE,
			TYPE_STRING,
			TYPE_OBJECT,
			TYPE_VECTOR,
			TYPE_ARRAY,
			TYPE_MAP
		} m_Type;
	};

	template<typename _Type>
	struct Serializer {
		template<typename _Iterator>
		static _Type Load(_Iterator &ri, _Iterator j);

		static string Store(_Type const &r);
	};

	template<>
	struct Serializer<nullptr_t> {
		template<typename _Iterator>
		static nullptr_t Load(_Iterator &ri, _Iterator j) {
			if (regex_search(ri, j, regex("^\\s*null"))) {
				return nullptr;
			} else {
				throw SyntaxError();
			}
		}

		static string Store(nullptr_t const &r) {
			return "null";
		}
	};

	template<>
	struct Serializer<bool> {
		template<typename _Iterator>
		static bool Load(_Iterator &ri, _Iterator j) {
			// TODO
			throw Error();
		}

		static string Store(bool const &r) {
			if (r) {
				return "true";
			} else {
				return "false";
			}
		}
	};

	template<>
	struct Serializer<int> {
		template<typename _Iterator>
		static int Load(_Iterator &ri, _Iterator j) {
			// TODO
			throw Error();
		}

		static string Store(int const &r) {
			ostringstream oss;
			oss << r;
			return oss.str();
		}
	};

	template<>
	struct Serializer<unsigned int> {
		template<typename _Iterator>
		static unsigned int Load(_Iterator &ri, _Iterator j) {
			// TODO
			throw Error();
		}

		static string Store(unsigned int const &r) {
			ostringstream oss;
			oss << r;
			return oss.str();
		}
	};

	template<>
	struct Serializer<double> {
		template<typename _Iterator>
		static double Load(_Iterator &ri, _Iterator j) {
			// TODO
			throw Error();
		}

		static string Store(double const &r) {
			ostringstream oss;
			oss << r;
			return oss.str();
		}
	};

	template<>
	struct Serializer<string> {
		template<typename _Iterator>
		static string Load(_Iterator &ri, _Iterator j) {
			// TODO
			throw Error();
		}

		static string Store(string const &r) {
			return "\"" + r + "\""; // FIXME escape special characters
		}
	};

	template<typename ..._Fields>
	struct Serializer<Object<_Fields...>> {
		template<typename _Iterator>
		static Object<_Fields...> Load(_Iterator &ri, _Iterator j) {
			// TODO
			throw Error();
		}
	};

	template<typename _Element>
	struct Serializer<vector<_Element>> {
		template<typename _Iterator>
		static vector<_Element> Load(_Iterator &ri, _Iterator j) {
			// TODO
			throw Error();
		}

		static string Store(vector<_Element> const &r) {
			string str = "[";
			auto it = r.begin();
			if (it != r.end()) {
				str += Serializer<_Element>::Store(*it);
				while (++it != r.end()) {
					str += "," + Serializer<_Element>::Store(*it);
				}
			}
			return str += "]";
		}
	};

	template<typename _Element, unsigned int const _c>
	struct Serializer<array<_Element, _c>> {
		template<typename _Iterator>
		static array<_Element, _c> Load(_Iterator &ri, _Iterator j) {
			// TODO
			throw Error();
		}
	};

	template<typename _Element>
	struct Serializer<map<string, _Element>> {
		template<typename _Iterator>
		static map<string, _Element> Load(_Iterator &ri, _Iterator j) {
			// TODO
			throw Error();
		}

		static string Store(map<string, _Element> const &r) {
			string str = "{";
			auto it = r.begin();
			if (it != r.end()) {
				str += "\"" + it->first + "\":" + Serializer<_Element>::Store(it->second); // FIXME escape special characters
				while (++it != r.end()) {
					str += ",\"" + it->first + "\":" + Serializer<_Element>::Store(it->second); // FIXME escape special characters
				}
			}
			return str += "}";
		}
	};

	template<>
	struct Serializer<Any> {
		// TODO
	};

	template<typename _Type>
	inline _Type Load(string const &rstr) {
		return Serializer<_Type>::Load(rstr.begin(), rstr.end());
	}

	template<typename _Type>
	inline _Type Load(istream &ris) {
		string const str = string(istreambuf_iterator<char>(ris), istreambuf_iterator<char>());
		auto it = str.begin();
		return Serializer<_Type>::Load(it, str.end());
	}

	template<typename _Type>
	inline string Store(_Type const &r) {
		return Serializer<_Type>::Store(r);
	}

	template<typename _Type>
	inline ostream &Store(ostream &ros, _Type const &r) {
		return ros << Serializer<_Type>::Store(r);
	}
}

template<typename ..._Fields>
inline std::istream &operator >> (std::istream &ris, JSON::Object<_Fields...> &rObject) {
	rObject = JSON::Load<JSON::Object<_Fields...>>(ris);
	return ris;
}

template<typename ..._Fields>
inline std::ostream &operator << (std::ostream &ros, JSON::Object<_Fields...> const &rObject) {
	return JSON::Store<JSON::Object<_Fields...>>(ros, rObject);
}

#define JSON_CHAR(sz, i) ((i) < sizeof(sz)) ? (sz)[i] : 0
#define JSON_UNPACK(sz) \
	JSON_CHAR((sz), 0), \
	JSON_CHAR((sz), 1), \
	JSON_CHAR((sz), 2), \
	JSON_CHAR((sz), 3), \
	JSON_CHAR((sz), 4), \
	JSON_CHAR((sz), 5), \
	JSON_CHAR((sz), 6), \
	JSON_CHAR((sz), 7), \
	JSON_CHAR((sz), 8), \
	JSON_CHAR((sz), 9), \
	JSON_CHAR((sz), 10), \
	JSON_CHAR((sz), 11), \
	JSON_CHAR((sz), 12), \
	JSON_CHAR((sz), 13), \
	JSON_CHAR((sz), 14), \
	JSON_CHAR((sz), 15), \
	JSON_CHAR((sz), 16), \
	JSON_CHAR((sz), 17), \
	JSON_CHAR((sz), 18), \
	JSON_CHAR((sz), 19), \
	JSON_CHAR((sz), 20), \
	JSON_CHAR((sz), 21), \
	JSON_CHAR((sz), 22), \
	JSON_CHAR((sz), 23), \
	JSON_CHAR((sz), 24), \
	JSON_CHAR((sz), 25), \
	JSON_CHAR((sz), 26), \
	JSON_CHAR((sz), 27), \
	JSON_CHAR((sz), 28), \
	JSON_CHAR((sz), 29), \
	JSON_CHAR((sz), 30), \
	JSON_CHAR((sz), 31), \
	JSON_CHAR((sz), 32), \
	JSON_CHAR((sz), 33), \
	JSON_CHAR((sz), 34), \
	JSON_CHAR((sz), 35), \
	JSON_CHAR((sz), 36), \
	JSON_CHAR((sz), 37), \
	JSON_CHAR((sz), 38), \
	JSON_CHAR((sz), 39), \
	JSON_CHAR((sz), 40), \
	JSON_CHAR((sz), 41), \
	JSON_CHAR((sz), 42), \
	JSON_CHAR((sz), 43), \
	JSON_CHAR((sz), 44), \
	JSON_CHAR((sz), 45), \
	JSON_CHAR((sz), 46), \
	JSON_CHAR((sz), 47), \
	JSON_CHAR((sz), 48), \
	JSON_CHAR((sz), 49), \
	JSON_CHAR((sz), 50), \
	JSON_CHAR((sz), 51), \
	JSON_CHAR((sz), 52), \
	JSON_CHAR((sz), 53), \
	JSON_CHAR((sz), 54), \
	JSON_CHAR((sz), 55), \
	JSON_CHAR((sz), 56), \
	JSON_CHAR((sz), 57), \
	JSON_CHAR((sz), 58), \
	JSON_CHAR((sz), 59), \
	JSON_CHAR((sz), 60), \
	JSON_CHAR((sz), 61), \
	JSON_CHAR((sz), 62), \
	JSON_CHAR((sz), 63), \
	JSON_CHAR((sz), 64), \
	JSON_CHAR((sz), 65), \
	JSON_CHAR((sz), 66), \
	JSON_CHAR((sz), 67), \
	JSON_CHAR((sz), 68), \
	JSON_CHAR((sz), 69), \
	JSON_CHAR((sz), 70), \
	JSON_CHAR((sz), 71), \
	JSON_CHAR((sz), 72), \
	JSON_CHAR((sz), 73), \
	JSON_CHAR((sz), 74), \
	JSON_CHAR((sz), 75), \
	JSON_CHAR((sz), 76), \
	JSON_CHAR((sz), 77), \
	JSON_CHAR((sz), 78), \
	JSON_CHAR((sz), 79), \
	JSON_CHAR((sz), 80), \
	JSON_CHAR((sz), 81), \
	JSON_CHAR((sz), 82), \
	JSON_CHAR((sz), 83), \
	JSON_CHAR((sz), 84), \
	JSON_CHAR((sz), 85), \
	JSON_CHAR((sz), 86), \
	JSON_CHAR((sz), 87), \
	JSON_CHAR((sz), 88), \
	JSON_CHAR((sz), 89), \
	JSON_CHAR((sz), 90), \
	JSON_CHAR((sz), 91), \
	JSON_CHAR((sz), 92), \
	JSON_CHAR((sz), 93), \
	JSON_CHAR((sz), 94), \
	JSON_CHAR((sz), 95), \
	JSON_CHAR((sz), 96), \
	JSON_CHAR((sz), 97), \
	JSON_CHAR((sz), 98), \
	JSON_CHAR((sz), 99)

#define UNPACK JSON_UNPACK

#endif
