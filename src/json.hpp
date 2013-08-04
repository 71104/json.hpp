#ifndef __JSON_HPP__
#define __JSON_HPP__

#include <iostream>
#include <iterator>
#include <string>
#include <sstream>
#include <array>
#include <vector>
#include <algorithm>
#include <set>

namespace Json {
	using namespace std;

	struct ParseError {};

	template<typename _CharType = char, typename _Traits = char_traits<_CharType>>
	basic_string<_CharType, _Traits> const Escape(basic_string<_CharType, _Traits> const &rstr) {
		basic_string<_CharType, _Traits> strEscaped;
		for (auto it = rstr.begin(); it != rstr.end(); ++it) {
			if (*it != '\"') {
				strEscaped += *it;
			} else {
				strEscaped += '\"';
			}
		}
		return strEscaped;
	}

	template<typename _CharType = char, typename _Traits = char_traits<_CharType>>
	basic_string<_CharType, _Traits> const Unescape(basic_istream<_CharType, _Traits> &ris) {
		basic_string<_CharType, _Traits> str;
		while (ris.peek() != '\"') {
			_CharType ch = ris.get();
			if (ch == '\\') {
				ch = ris.get();
				switch (ch) {
				case 'n':
					ch = '\n';
					break;
				case 't':
					ch = '\t';
					break;
				// TODO process other special escape codes
				}
			}
			str += ch;
		}
		return str;
	}

	template<typename _CharType = char, typename _Traits = char_traits<_CharType>>
	basic_ostream<_CharType, _Traits> &Indent(basic_ostream<_CharType, _Traits> &ros, unsigned int c) {
		struct {
			_CharType operator () () const {
				return '\t';
			}
		} Indentator;
		generate_n(ostream_iterator<_CharType, _CharType, _Traits>(ros), c, Indentator);
		return ros;
	}

	template<typename _Type, typename _CharType = char, typename _Traits = char_traits<_CharType>>
	struct TypeDefinition {
		static _Type Read(basic_istream<_CharType, _Traits>&);
		static void Write(basic_ostream<_CharType, _Traits>&, _Type const&);
		static void Print(basic_ostream<_CharType, _Traits>&, _Type const&, unsigned int);
	};

	template<typename _CharType, typename _Traits>
	struct TypeDefinition<nullptr_t, _CharType, _Traits> {
	private:
		static string const s_strNull;

	public:
		static nullptr_t Read(basic_istream<_CharType, _Traits> &ris) {
			basic_string<_CharType, _Traits> str;
			ris >> str;
			if (str != basic_string<_CharType, _Traits>(s_strNull.begin(), s_strNull.end())) {
				ris.setstate(ios::failbit);
				throw ParseError();
			} else {
				return nullptr;
			}
		}

		static void Write(basic_ostream<_CharType, _Traits> &ros, nullptr_t const &rValue) {
			ros << basic_string<_CharType, _Traits>(s_strNull.begin(), s_strNull.end());
		}

		static void Print(basic_ostream<_CharType, _Traits> &ros, nullptr_t const &rValue, unsigned int cIndent) {
			ros << basic_string<_CharType, _Traits>(s_strNull.begin(), s_strNull.end());
		}
	};

	template<typename _CharType, typename _Traits>
	string const TypeDefinition<nullptr_t, _CharType, _Traits>::s_strNull = "null";

	template<typename _CharType, typename _Traits>
	struct TypeDefinition<bool, _CharType, _Traits> {
	private:
		static string const s_strTrue;
		static string const s_strFalse;

	public:
		static bool Read(basic_istream<_CharType, _Traits> &ris) {
			basic_string<_CharType, _Traits> str;
			ris >> str;
			if (str == basic_string<_CharType, _Traits>(s_strTrue.begin(), s_strTrue.end())) {
				return true;
			} else if (str == basic_string<_CharType, _Traits>(s_strFalse.begin(), s_strFalse.end())) {
				return false;
			} else {
				ris.setstate(ios::failbit);
				throw ParseError();
			}
		}

		static void Write(basic_ostream<_CharType, _Traits> &ros, bool const &rf) {
			if (rf) {
				ros << basic_string<_CharType, _Traits>(s_strTrue.begin(), s_strTrue.end());
			} else {
				ros << basic_string<_CharType, _Traits>(s_strFalse.begin(), s_strFalse.end());
			}
		}

		static void Print(basic_ostream<_CharType, _Traits> &ros, bool const &rf, unsigned int cIndent) {
			if (rf) {
				ros << basic_string<_CharType, _Traits>(s_strTrue.begin(), s_strTrue.end());
			} else {
				ros << basic_string<_CharType, _Traits>(s_strFalse.begin(), s_strFalse.end());
			}
		}
	};

	template<typename _CharType, typename _Traits>
	string const TypeDefinition<bool, _CharType, _Traits>::s_strTrue = "true";

	template<typename _CharType, typename _Traits>
	string const TypeDefinition<bool, _CharType, _Traits>::s_strFalse = "false";

	template<typename _CharType, typename _Traits>
	struct TypeDefinition<int, _CharType, _Traits> {
		static int Read(basic_istream<_CharType, _Traits> &ris) {
			int n;
			ris >> n;
			return n;
		}

		static void Write(basic_ostream<_CharType, _Traits> &ros, int const &rn) {
			ros << rn;
		}

		static void Print(basic_ostream<_CharType, _Traits> &ros, int const &rn, unsigned int cIndent) {
			ros << rn;
		}
	};

	template<typename _CharType, typename _Traits>
	struct TypeDefinition<unsigned int, _CharType, _Traits> {
		static unsigned int Read(basic_istream<_CharType, _Traits> &ris) {
			unsigned int n;
			ris >> n;
			return n;
		}

		static void Write(basic_ostream<_CharType, _Traits> &ros, unsigned int const &rn) {
			ros << rn;
		}

		static void Print(basic_ostream<_CharType, _Traits> &ros, unsigned int const &rn, unsigned int cIndent) {
			ros << rn;
		}
	};

	template<typename _CharType, typename _Traits>
	struct TypeDefinition<double, _CharType, _Traits> {
		static double Read(basic_istream<_CharType, _Traits> &ris) {
			double f;
			ris >> f;
			return f;
		}

		static void Write(basic_ostream<_CharType, _Traits> &ros, double const &rf) {
			ros << rf;
		}

		static void Print(basic_ostream<_CharType, _Traits> &ros, double const &rf, unsigned int cIndent) {
			ros << rf;
		}
	};

	template<typename _CharType, typename _Traits>
	struct TypeDefinition<basic_string<_CharType, _Traits>, _CharType, _Traits> {
	private:
		static basic_string<_CharType, _Traits> const s_strQuotes;

	public:
		static basic_string<_CharType, _Traits> Read(basic_istream<_CharType, _Traits> &ris) {
			ris >> ws >> s_strQuotes;
			basic_string<_CharType, _Traits> str = Unescape<_CharType, _Traits>(ris);
			ris >> s_strQuotes;
			return str;
		}

		static void Write(basic_ostream<_CharType, _Traits> &ros, basic_string<_CharType, _Traits> const &rstr) {
			ros << s_strQuotes << Escape<_CharType, _Traits>(rstr) << s_strQuotes;
		}

		static void Print(basic_ostream<_CharType, _Traits> &ros, basic_string<_CharType, _Traits> const &rstr, unsigned int cIndent) {
			ros << s_strQuotes << Escape<_CharType, _Traits>(rstr) << s_strQuotes;
		}
	};

	template<typename _CharType, typename _Traits>
	basic_string<_CharType, _Traits> const TypeDefinition<basic_string<_CharType, _Traits>, _CharType, _Traits>::s_strQuotes = '\"';

	template<typename _CharType, typename _Traits, _CharType const _szName[], typename _Type, bool _fOptional = false>
	struct BasicField {
	private:
		static basic_string<_CharType, _Traits> const s_strQuotes;
		static basic_string<_CharType, _Traits> const s_strColon;

	public:
		static basic_string<_CharType, _Traits> const s_strName;
		typedef _Type Type;

		_Type m_Value;

		BasicField() {}

		explicit BasicField(_Type const &a_rValue)
			:
		m_Value(a_rValue) {}

		operator basic_string<_CharType, _Traits> const () const {
			basic_ostringstream<_CharType, _Traits> oss;
			TypeDefinition<_Type, _CharType, _Traits>::Write(
				oss
					<< s_strQuotes
					<< Escape<_CharType, _Traits>(_szName)
					<< s_strQuotes
					<< s_strColon,
				m_Value
				);
			return oss.str();
		}
	};

	template<typename _CharType, typename _Traits, _CharType const _szName[], typename _Type, bool _fOptional>
	basic_string<_CharType, _Traits> const BasicField<_CharType, _Traits, _szName, _Type, _fOptional>::s_strQuotes = '\"';

	template<typename _CharType, typename _Traits, _CharType const _szName[], typename _Type, bool _fOptional>
	basic_string<_CharType, _Traits> const BasicField<_CharType, _Traits, _szName, _Type, _fOptional>::s_strColon = ':';

	template<typename _CharType, typename _Traits, _CharType const _szName[], typename _Type, bool _fOptional>
	basic_string<_CharType, _Traits> const BasicField<_CharType, _Traits, _szName, _Type, _fOptional>::s_strName = _szName;

	template<char const _szName[], typename _Type, bool _fOptional = false>
	struct Field :
		public BasicField<char, char_traits<char>, _szName, _Type, _fOptional> {};

	template<typename _CharType, typename _Traits, _CharType const _szName[], typename _Type, bool _fOptional, typename... _OtherFields>
	struct Accessor;

	template<typename _CharType, typename _Traits, _CharType const _szName[], typename _Type, bool _fOptional, typename... _OtherFields>
	struct ConstAccessor;

	template<typename _CharType, typename _Traits, typename... _Fields>
	struct BasicObject {
		bool LoadField(basic_istream<_CharType, _Traits>&, basic_string<_CharType, _Traits> const&);
		set<basic_string<_CharType, _Traits>> GetFields() const;
		basic_string<_CharType, _Traits> const ChainFieldStrings() const;
		void PrintFieldStrings(basic_ostream<_CharType, _Traits>&, unsigned int) const;
		operator basic_string<_CharType, _Traits> const () const;
		void Print(basic_ostream<_CharType, _Traits>&, unsigned int) const;
	};

	template<typename _CharType, typename _Traits>
	struct BasicObject<_CharType, _Traits> {
	private:
		static string const s_strEmpty;

	public:
		bool LoadField(basic_istream<_CharType, _Traits> &ris, basic_string<_CharType, _Traits> const &rstrName) {
			return false;
		}

		set<basic_string<_CharType, _Traits>> GetFields() const {
			return set<string>();
		}

		basic_string<_CharType, _Traits> const ChainFieldStrings() const {
			return basic_string<_CharType, _Traits>();
		}

		void PrintFieldStrings(basic_ostream<_CharType, _Traits> &ros, unsigned int cIndent) const {}

		operator basic_string<_CharType, _Traits> const () const {
			return basic_string<_CharType, _Traits>(s_strEmpty.begin(), s_strEmpty.end());
		}

		void Print(basic_ostream<_CharType, _Traits> &ros, unsigned int cIndent) const {
			return basic_string<_CharType, _Traits>(s_strEmpty.begin(), s_strEmpty.end());
		}
	};

	template<typename _CharType, typename _Traits>
	string const BasicObject<_CharType, _Traits>::s_strEmpty = "{}";

	template<typename _CharType, typename _Traits, _CharType const _szName[], typename _Type, bool _fOptional>
	struct BasicObject<_CharType, _Traits, BasicField<_CharType, _Traits, _szName, _Type, _fOptional>> {
		BasicField<_CharType, _Traits, _szName, _Type, _fOptional> m_Field;

		bool LoadField(basic_istream<_CharType, _Traits> &ris, basic_string<_CharType, _Traits> const &rstrName) {
			if (rstrName != _szName) {
				return false;
			} else {
				m_Field.m_Value = TypeDefinition<_Type, _CharType, _Traits>::Read(ris);
				return true;
			}
		}

		set<basic_string<_CharType, _Traits>> GetFields() const {
			set<basic_string<_CharType, _Traits>> Set;
			Set.insert(_szName);
			return Set;
		}

		basic_string<_CharType, _Traits> const ChainFieldStrings() const {
			return (basic_string<_CharType, _Traits> const)m_Field;
		}

		void PrintFieldStrings(basic_ostream<_CharType, _Traits> &ros, unsigned int cIndent) const {
			TypeDefinition<_Type, _CharType, _Traits>::Print(ros << "\"" << Escape<_CharType, _Traits>(_szName) << "\": ", m_Field.m_Value, cIndent);
		}

		operator basic_string<_CharType, _Traits> const () const {
			basic_string<_CharType, _Traits> str = '{';
			str += (basic_string<_CharType, _Traits> const)m_Field;
			str += '}';
			return str;
		}

		void Print(basic_ostream<_CharType, _Traits> &ros, unsigned int cIndent) const {
			PrintFieldStrings(ros << "{\n", cIndent + 1);
			Indent<_CharType, _Traits>(ros << "\n", cIndent) << "}";
		}

		template<_CharType const _szFieldName[]>
		_Type &Get() {
			return Accessor<_CharType, _Traits, _szFieldName, _Type, _fOptional>(*this).Get();
		}

		template<_CharType const _szFieldName[]>
		_Type const &Get() const {
			return ConstAccessor<_CharType, _Traits, _szFieldName, _Type, _fOptional>(*this).Get();
		}

		template<_CharType const _szFieldName[]>
		void Put(_Type const &rValue) {
			Accessor<_CharType, _Traits, _szFieldName, _Type, _fOptional>(*this).Put(rValue);
		}
	};

	template<typename _CharType, typename _Traits, _CharType const _szName[], typename _Type, bool _fOptional, typename... _OtherFields>
	struct BasicObject<_CharType, _Traits, BasicField<_CharType, _Traits, _szName, _Type, _fOptional>, _OtherFields...> :
		public BasicObject<_CharType, _Traits, _OtherFields...>
	{
	private:
		static basic_string<_CharType, _Traits> const s_strQuotes;
		static basic_string<_CharType, _Traits> const s_strColon;

	public:
		BasicField<_CharType, _Traits, _szName, _Type, _fOptional> m_Field;

		bool LoadField(basic_istream<_CharType, _Traits> &ris, basic_string<_CharType, _Traits> const &rstrName) {
			if (rstrName != _szName) {
				return BasicObject<_CharType, _Traits, _OtherFields...>::LoadField(ris, rstrName);
			} else {
				m_Field.m_Value = TypeDefinition<_Type, _CharType, _Traits>::Read(ris);
				return true;
			}
		}

		set<basic_string<_CharType, _Traits>> GetFields() const {
			set<basic_string<_CharType, _Traits>> Set = BasicObject<_CharType, _Traits, _OtherFields...>::GetFields();
			Set.insert(_szName);
			return Set;
		}

		basic_string<_CharType, _Traits> const ChainFieldStrings() const {
			return (basic_string<_CharType, _Traits> const)m_Field +
				basic_string<_CharType, _Traits>(',') +
				BasicObject<_CharType, _Traits, _OtherFields...>::ChainFieldStrings();
		}

		void PrintFieldStrings(basic_ostream<_CharType, _Traits> &ros, unsigned int cIndent) const {
			TypeDefinition<_Type, _CharType, _Traits>::Print(ros << s_strQuotes << _szName << s_strQuotes << s_strColon, m_Field.m_Value, cIndent);
			BasicObject<_CharType, _Traits, _OtherFields...>::Print(Indent<_CharType, _Traits>(ros << ",\n", cIndent), cIndent);
		}

		operator basic_string<_CharType, _Traits> const () const {
			return basic_string<_CharType, _Traits>('{') +
				ChainFieldStrings() +
				basic_string<_CharType, _Traits>('}');
		}

		void Print(basic_ostream<_CharType, _Traits> &ros, unsigned int cIndent) const {
			PrintFieldStrings(ros << "{\n", cIndent + 1);
			Indent<_CharType, _Traits>(ros << "\n", cIndent) << "}";
		}

		template<_CharType const _szFieldName[]>
		_Type &Get() {
			return Accessor<_CharType, _Traits, _szFieldName, _Type, _fOptional, _OtherFields...>(*this).Get();
		}

		template<_CharType const _szFieldName[]>
		_Type const &Get() const {
			return ConstAccessor<_CharType, _Traits, _szFieldName, _Type, _fOptional, _OtherFields...>(*this).Get();
		}

		template<_CharType const _szFieldName[]>
		void Put(_Type const &rValue) {
			Accessor<_CharType, _Traits, _szFieldName, _Type, _fOptional, _OtherFields...>(*this).Put(rValue);
		}
	};

	template<typename _CharType, typename _Traits, _CharType const _szName[], typename _Type, bool _fOptional, typename... _OtherFields>
	basic_string<_CharType, _Traits> const BasicObject<_CharType, _Traits, BasicField<_CharType, _Traits, _szName, _Type, _fOptional>, _OtherFields...>::s_strQuotes = '\"';

	template<typename _CharType, typename _Traits, _CharType const _szName[], typename _Type, bool _fOptional, typename... _OtherFields>
	basic_string<_CharType, _Traits> const BasicObject<_CharType, _Traits, BasicField<_CharType, _Traits, _szName, _Type, _fOptional>, _OtherFields...>::s_strColon = ':';

	template<typename _CharType, typename _Traits, _CharType const _szName[], typename _Type, bool _fOptional, typename... _OtherFields>
	struct Accessor {
		BasicObject<_CharType, _Traits, BasicField<_CharType, _Traits, _szName, _Type, _fOptional>, _OtherFields...> &m_rObject;

		explicit Accessor(BasicObject<_CharType, _Traits, BasicField<_CharType, _Traits, _szName, _Type, _fOptional>, _OtherFields...> &a_rObject)
			:
		m_rObject(a_rObject) {}

		_Type &Get() {
			return m_rObject.m_Field.m_Value;
		}

		void Put(_Type const &rValue) {
			m_rObject.m_Field.m_Value = rValue;
		}
	};

	template<typename _CharType, typename _Traits, _CharType const _szName[], typename _Type, bool _fOptional, typename... _OtherFields>
	struct ConstAccessor {
		BasicObject<_CharType, _Traits, BasicField<_CharType, _Traits, _szName, _Type, _fOptional>, _OtherFields...> const &m_rObject;

		explicit ConstAccessor(BasicObject<_CharType, _Traits, BasicField<_CharType, _Traits, _szName, _Type, _fOptional>, _OtherFields...> const &a_rObject)
			:
		m_rObject(a_rObject) {}

		_Type const &Get() const {
			return m_rObject.m_Field.m_Value;
		}
	};

	template<typename... _Fields>
	struct Object :
		public BasicObject<char, char_traits<char>, _Fields...> {};

	template<typename _CharType, typename _Traits, typename... _Fields>
	struct TypeDefinition<BasicObject<_CharType, _Traits, _Fields...>, _CharType, _Traits> {
		static BasicObject<_CharType, _Traits, _Fields...> Read(basic_istream<_CharType, _Traits> &ris) {
			ris >> ws >> "{";
			BasicObject<_CharType, _Traits, _Fields...> Object;
			set<basic_string<_CharType, _Traits>> FieldSet = Object.GetFields();
			if ((ris >> ws).peek() != '}') {
				basic_string<_CharType, _Traits> strName = Unescape<_CharType, _Traits>(ris >> ws >> "\"");
				ris >> "\"" >> ws >> ":";
				if (Object.LoadField(ris, strName)) {
					FieldSet.erase(strName);
				}
				while ((ris >> ws).peek() != '}') {
					strName = Unescape<_CharType, _Traits>(ris >> "," >> ws >> "\"");
					ris >> "\"" >> ws >> ":";
					if (Object.LoadField(ris, strName)) {
						FieldSet.erase(strName);
					}
				}
			}
			ris.get();
			if (!FieldSet.empty()) {
				throw ParseError();
			}
			return Object;
		}

		static void Write(basic_ostream<_CharType, _Traits> &ros, BasicObject<_CharType, _Traits, _Fields...> const &rObject) {
			ros << (basic_string<_CharType, _Traits> const)rObject;
		}

		static void Print(basic_ostream<_CharType, _Traits> &ros, BasicObject<_CharType, _Traits, _Fields...> const &rObject, unsigned int cIndent) {
			rObject.Print(ros, cIndent);
		}
	};

	template<typename _CharType, typename _Traits, typename _Type, unsigned int _c>
	struct TypeDefinition<array<_Type, _c>, _CharType, _Traits> {
		static array<_Type, _c> Read(basic_istream<_CharType, _Traits> &ris) {
			ris >> ws >> "[";
			array<_Type, _c> Array;
			if (_c) {
				Array[0] = TypeDefinition<_Type, _CharType, _Traits>::Read(ris);
				for (unsigned int i = 1; i < _c; i++) {
					ris >> ws >> ",";
					Array[i] = TypeDefinition<_Type, _CharType, _Traits>::Read(ris);
				}
			}
			ris >> ws >> "]";
			return Array;
		}

		static void Write(basic_ostream<_CharType, _Traits> &ros, array<_Type, _c> const &ra) {
			ros << "[";
			if (_c) {
				TypeDefinition<_Type, _CharType, _Traits>::Write(ros, ra[0]);
				for (unsigned int i = 1; i < _c; i++) {
					TypeDefinition<_Type, _CharType, _Traits>::Write(ros << ",", ra[i]);
				}
			}
			ros << "]";
		}

		static void Print(basic_ostream<_CharType, _Traits> &ros, array<_Type, _c> const &ra, unsigned int cIndent) {
			ros << "[";
			if (_c) {
				Indent<_CharType, _Traits>(ros << "\n", cIndent + 1);
				TypeDefinition<_Type, _CharType, _Traits>::Print(ros, ra[0], cIndent + 1);
				for (unsigned int i = 1; i < _c; i++) {
					Indent<_CharType, _Traits>(ros << ",\n", cIndent + 1);
					TypeDefinition<_Type, _CharType, _Traits>::Print(ros, ra[i], cIndent + 1);
				}
				Indent<_CharType, _Traits>(ros << "\n", cIndent);
			}
			ros << "]";
		}
	};

	template<typename _Type, typename _CharType, typename _Traits>
	struct TypeDefinition<vector<_Type>, _CharType, _Traits> {
		static vector<_Type> Read(basic_istream<_CharType, _Traits> &ris) {
			ris >> ws >> "[" >> ws;
			vector<_Type> Vector;
			if (ris.peek() != ']') {
				Vector.push_back(TypeDefinition<_Type, _CharType, _Traits>::Read(ris));
				while ((ris >> ws).peek() != ']') {
					Vector.push_back(TypeDefinition<_Type, _CharType, _Traits>::Read(ris >> ","));
				}
			}
			ris.get();
			return Vector;
		}

		static void Write(basic_ostream<_CharType, _Traits> &ros, vector<_Type> const &ra) {
			ros << "[";
			if (ra.size()) {
				TypeDefinition<_Type, _CharType, _Traits>::Write(ros, ra[0]);
				for (unsigned int i = 1; i < ra.size(); i++) {
					TypeDefinition<_Type, _CharType, _Traits>::Write(ros << ",", ra[i]);
				}
			}
			ros << "]";
		}

		static void Print(basic_ostream<_CharType, _Traits> &ros, vector<_Type> const &ra, unsigned int cIndent) {
			ros << "[";
			if (ra.size()) {
				Indent<_CharType, _Traits>(ros << "\n", cIndent + 1);
				TypeDefinition<_Type, _CharType, _Traits>::Print(ros, ra[0], cIndent + 1);
				for (unsigned int i = 1; i < ra.size(); i++) {
					Indent<_CharType, _Traits>(ros << ",\n", cIndent + 1);
					TypeDefinition<_Type, _CharType, _Traits>::Print(ros, ra[i], cIndent + 1);
				}
				Indent<_CharType, _Traits>(ros << "\n", cIndent);
			}
			ros << "]";
		}
	};

	template<typename _Type, typename _CharType = char, typename _Traits = char_traits<_CharType>>
	_Type Read(basic_istream<_CharType, _Traits> &ris) {
		ris.exceptions(ios::failbit | ios::badbit | ios::eofbit);
		return TypeDefinition<_Type, _CharType, _Traits>::Read(ris);
	}

	template<typename _Type, typename _CharType = char, typename _Traits = char_traits<_CharType>>
	_Type Read(basic_istream<_CharType, _Traits> &&rris) {
		return Read<_Type, _CharType, _Traits>(rris);
	}

	template<typename _Type, typename _CharType = char, typename _Traits = char_traits<_CharType>>
	void Write(basic_ostream<_CharType, _Traits> &ros, _Type const &rValue) {
		ros.exceptions(ios::failbit | ios::badbit | ios::eofbit);
		return TypeDefinition<_Type, _CharType, _Traits>::Write(ros, rValue);
	}

	template<typename _Type, typename _CharType = char, typename _Traits = char_traits<_CharType>>
	void Write(basic_ostream<_CharType, _Traits> &&rros, _Type const &rValue) {
		Write<_Type, _CharType, _Traits>(rros, rValue);
	}

	template<typename _Type, typename _CharType = char, typename _Traits = char_traits<_CharType>>
	void Print(basic_ostream<_CharType, _Traits> &ros, _Type const &rValue) {
		ros.exceptions(ios::failbit | ios::badbit | ios::eofbit);
		return TypeDefinition<_Type, _CharType, _Traits>::Print(ros, 0, rValue);
	}

	template<typename _Type, typename _CharType = char, typename _Traits = char_traits<_CharType>>
	void Print(basic_ostream<_CharType, _Traits> &&rros, _Type const &rValue) {
		Print<_Type, _CharType, _Traits>(rros, rValue);
	}
}

template<typename _CharType, typename _Traits, typename... _Fields>
std::basic_ostream<_CharType, _Traits> &operator << (std::basic_ostream<_CharType, _Traits> &ros, Json::BasicObject<_CharType, _Traits, _Fields...> const &rObject) {
	return ros << (std::basic_string<_CharType, _Traits> const)rObject;
}

template<typename... _Fields>
std::ostream &operator << (std::ostream &ros, Json::Object<_Fields...> const &rObject) {
	return ros << (std::string const)rObject;
}

#endif
