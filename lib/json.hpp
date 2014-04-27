#ifndef __JSON_HPP__
#define __JSON_HPP__

#include <cctype>
#include <string>
#include <iostream>
#include <iterator>
#include <sstream>
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

		// TODO getters
	};

	template<typename _Value, char const ..._szName, typename ..._OtherFields>
	char const Object<OptionalField<_Value, _szName...>, _OtherFields...>::s_szName[] = { _szName... };

	template<typename _Type>
	struct Serializer {
		template<typename _Iterator>
		static _Type Load(_Iterator i, _Iterator j);

		static string Store(_Type const &r);
	};

	template<>
	struct Serializer<nullptr_t> {
		template<typename _Iterator>
		static nullptr_t Load(_Iterator i, _Iterator j) {
			// TODO
			throw Error();
		}

		static string Store(nullptr_t const &r) {
			return "null";
		}
	};

	template<>
	struct Serializer<bool> {
		template<typename _Iterator>
		static bool Load(_Iterator i, _Iterator j) {
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
		static int Load(_Iterator i, _Iterator j) {
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
		static unsigned int Load(_Iterator i, _Iterator j) {
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
		static double Load(_Iterator i, _Iterator j) {
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
		static string Load(_Iterator i, _Iterator j) {
			// TODO
			throw Error();
		}

		static string Store(ostream &ros, string const &r) {
			return "\"" + r + "\""; // FIXME escape special characters
		}
	};

	template<typename ..._Fields>
	struct Serializer<Object<_Fields...>> {
		template<typename _Iterator>
		static Object<_Fields...> Load(_Iterator i, _Iterator j) {
			// TODO
			throw Error();
		}
	};

	template<typename _Element>
	struct Serializer<vector<_Element>> {
		template<typename _Iterator>
		static vector<_Element> Load(_Iterator i, _Iterator j) {
			// TODO
			throw Error();
		}
	};

	template<typename _Element, unsigned int _c>
	struct Serializer<array<_Element, _c>> {
		template<typename _Iterator>
		static array<_Element, _c> Load(_Iterator i, _Iterator j) {
			// TODO
			throw Error();
		}
	};

	template<typename _Type>
	inline _Type Load(string const &rstr) {
		return Serializer<_Type>::Load(rstr.begin(), rstr.end());
	}

	template<typename _Type>
	inline _Type Load(istream &ris) {
		return Serializer<_Type>::Load(istreambuf_iterator<char>(ris), istreambuf_iterator<char>());
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
inline std::ostream &operator << (std::ostream &ros, JSON::Object<_Fields...> &rObject) {
	return JSON::Store<JSON::Object<_Fields...>>(ros, rObject);
}

static constexpr char JSON_UNPACK(char const sz[], unsigned int i) {
	return (void)sz, (void)i, 0;
}

static constexpr char (*JSON_ALMOST_UNPACK())(char const[], unsigned int) {
	return JSON_UNPACK;
}

#define JSON_EMPTY(...)
#define JSON_DEFER(...) __VA_ARGS__ JSON_EMPTY()
#define JSON_EXPAND(...) __VA_ARGS__

#define JSON_EXPAND0 JSON_EXPAND
#define JSON_EXPAND1(...) JSON_EXPAND0(JSON_EXPAND0(__VA_ARGS__))
#define JSON_EXPAND2(...) JSON_EXPAND1(JSON_EXPAND1(__VA_ARGS__))
#define JSON_EXPAND3(...) JSON_EXPAND2(JSON_EXPAND2(__VA_ARGS__))
#define JSON_EXPAND4(...) JSON_EXPAND3(JSON_EXPAND3(__VA_ARGS__))
#define JSON_EXPAND5(...) JSON_EXPAND4(JSON_EXPAND4(__VA_ARGS__))
#define JSON_EXPAND6(...) JSON_EXPAND5(JSON_EXPAND5(__VA_ARGS__))
#define JSON_EXPAND_ALL JSON_EXPAND6

#define JSON_ALMOST_UNPACK() JSON_UNPACK
#define JSON_UNPACK(sz, i) ((i) < sizeof(sz)) ? (sz)[i] : 0, JSON_DEFER(JSON_ALMOST_UNPACK)()(sz, i + 1)

#define UNPACK(sz) JSON_EXPAND_ALL(JSON_UNPACK(sz, 0))

#endif
