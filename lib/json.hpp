#ifndef __JSON_HPP__
#define __JSON_HPP__

#include <cctype>
#include <string>
#include <vector>

namespace JSON {
	using namespace std;

	struct Error {};
	struct SyntaxError : public Error {};
	struct LoadError : public Error {};
	struct AccessError : public Error {};

	istream &SkipSeparators(istream &ris) {
		while (isspace(ris.peek())) {
			ris.get();
		}
		return ris;
	}

	istream &Skip(istream &ris, string const &rstr) {
		for (auto it = rstr.begin(); it != rstr.end(); ++it) {
			if (ris.peek() != (int)(unsigned char)*it) {
				throw SyntaxError();
			} else {
				ris.get();
			}
		}
		return ris;
	}

	string ReadWord(istream &ris) {
		int ch = ris.get();
		if (ch < 0 || !isalpha(ch)) {
			throw SyntaxError();
		} else {
			string str = { (char)ch, 0 };
			while (ch = ris.peek(), ch > 0 && isalpha(ch)) {
				ris.get();
				str += ch;
			}
			return str;
		}
	}

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
			Skip(SkipSeparators(ris), "null");
			return nullptr;
		}
	};

	template<>
	struct Loader<bool> {
		static bool Load(istream &ris) {
			string const str = ReadWord(SkipSeparators(ris));
			if (str == "true") {
				return true;
			} else if (str == "false") {
				return false;
			} else {
				throw SyntaxError();
			}
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
			Skip(SkipSeparators(ris), "\"");
			string str;
			int ch;
			while ((ch = ris.get()) != '\"') {
				if (ch < 0) {
					throw SyntaxError();
				} else if (ch == '\\') {
					if ((ch = ris.get()) < 0) {
						throw SyntaxError();
					} else {
						switch (ch) {
						case 'n':
							str += '\n';
							break;
						case 'r':
							str += '\r';
							break;
						case 't':
							str += '\t';
							break;
						default:
							str += ch;
							break;
						}
					}
				} else {
					str += ch;
				}
			}
			return str;
		}
	};

	template<typename ..._Fields>
	struct Loader<Object<_Fields...>> {
		static Object<_Fields...> Load(istream &ris) {
			// TODO
			throw false;
		}
	};

	template<typename _Element>
	struct Loader<vector<_Element>> {
		static vector<_Element> Load(istream &ris) {
			SkipSeparators(Skip(SkipSeparators(ris), "["));
			if (ris.peek() != ']') {
				vector<_Element> v = { Loader<_Element>::Load(ris) };
				while (SkipSeparators(ris).peek() != ']') {
					v.push_back(Loader<_Element>::Load(ris));
				}
				ris.get();
				return v;
			} else {
				ris.get();
				return vector<_Element>();
			}
		}
	};

	template<typename _Type>
	_Type Load(istream &ris) {
		return Loader<_Type>::Load(ris);
	}
}

#endif
