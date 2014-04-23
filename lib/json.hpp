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

	template<typename _Value, char const ..._szName>
	struct Object<Field<_Value, _szName...>> {
		static char constexpr s_szName[] = { _szName... };
		_Value m_Value;

		Object(_Value const &a_rValue)
			:
		m_Value(a_rValue) {}

		Object(_Value &&a_rrValue)
			:
		m_Value(move(a_rrValue)) {}

		virtual _Value &operator [] (string const &rstrName) {
			if (rstrName != s_szName) {
				throw AccessError();
			} else {
				return m_Value;
			}
		}

		virtual _Value const &operator [] (string const &rstrName) const {
			if (rstrName != s_szName) {
				throw AccessError();
			} else {
				return m_Value;
			}
		}
	};

	template<typename _Value, char const ..._szName, typename ..._OtherFields>
	struct Object<Field<_Value, _szName...>, _OtherFields...> :
		public Object<_OtherFields...>
	{
		static char constexpr s_szName[] = { _szName... };
		_Value m_Value;

		virtual _Value &operator [] (string const &rstrName) {
			if (rstrName != s_szName) {
				return Object<_OtherFields...>::operator [] (rstrName);
			} else {
				return m_Value;
			}
		}

		virtual _Value const &operator [] (string const &rstrName) const {
			if (rstrName != s_szName) {
				return Object<_OtherFields...>::operator [] (rstrName);
			} else {
				return m_Value;
			}
		}
	};

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
