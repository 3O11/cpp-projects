#include <iostream>
#include <fstream>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <memory>
#include <string>
#include <charconv>
#include <sstream>
#include <optional>

// I would've preferred to split each section to its own file(s), but the file
// limit in ReCodEx wouldn't allow that.

//////////////////////////////////////////////////////////////////////////////
// General Utilities
//////////////////////////////////////////////////////////////////////////////

std::optional<int32_t> strToInt(std::string_view num)
{
	size_t parsed = 0;
	auto [ptr, ec] { std::from_chars(num.data(), num.data() + num.size(), parsed) };

	if (ec != std::errc() || ptr != num.data() + num.size()) return std::nullopt;

	return parsed;
}

std::vector<std::string> split(const std::string& str, char delim)
{
	std::vector<std::string> split;
	std::stringstream ss(str); // Can't use std::string_view :(
	std::string word;
	while(std::getline(ss, word, delim)) split.push_back(word);
	return split;
}

//////////////////////////////////////////////////////////////////////////////
// Argument Parsing
//////////////////////////////////////////////////////////////////////////////

struct ColIdentifier
{
	char Type;
	size_t Number;
};

class ArgParser
{
public:
	ArgParser(const std::vector<std::string>& args, const std::vector<char>& options)
		: m_args(args), m_opts(options)
	{}

	bool Parse()
	{
		for (auto& opt : m_opts)
		{
			for (size_t i = 1; i < m_args.size(); ++i)
			{
				if (m_args[i].substr(0, 2) == std::string('-', opt))
				{
					if (m_args[i].length() > 2)
					{
						m_optValues[opt] = m_args[i].substr(2, m_args[i].length() - 2);
						m_args.erase(m_args.begin() + i);
						--i; // This is to compensate for the increment at the end
					}
					else
					{
						if (m_args.size() <= i + 1 || m_args[i + 1][0] == '-' || std::isupper(m_args[i + 1][0]))
						{
							return false;
						}

						m_optValues[opt] = m_args[i + 1];
						m_args.erase(m_args.begin() + i, m_args.begin() + i + 2);
						--i; // This is to compensate for the increment at the end
					}
				}
			}
		}

		for (auto& arg : m_args)
		{
			if (!std::isupper(arg[0])) return false;

			auto colnum = strToInt(arg.substr(1, arg.length() - 1));

			if (!colnum) return false;

			m_parsedCols.push_back({arg[0], (size_t)*colnum});
		}

		std::reverse(m_parsedCols.begin(), m_parsedCols.end());

		return true;
	}

	bool HasOptionValue(char option)
	{
		return m_optValues.find(option) != m_optValues.end(); 
	}

	std::string GetOptionValue(char option)
	{
		return m_optValues[option];
	}

	std::vector<ColIdentifier> GetRequired()
	{
		return m_parsedCols;
	}
private:
	std::vector<std::string> m_args;
	std::vector<char> m_opts;
	std::vector<ColIdentifier> m_parsedCols;
	std::unordered_map<char, std::string> m_optValues;
};

//////////////////////////////////////////////////////////////////////////////
// Polymorphic Sort
//////////////////////////////////////////////////////////////////////////////

class IColumn
{
public:
  virtual bool operator< (const IColumn& col) const = 0;
  virtual ~IColumn() {}
private:
};

template <typename T>
class Column : public IColumn
{
public:
	Column(const T& item)
		: m_item(item)
	{}

	bool operator< (const IColumn& col) const override
	{
		// I'm assuming that when this call happens,
		// the underlying types are matching in order to avoid
		// additional checks.
		// THIS REQUIREMENT MUST BE ENSURED EXTERNALLY.
		return m_item < static_cast<const Column<T>&>(col).m_item;
	}

private:
	T m_item;
};

class Line
{
public:
	Line(const std::string& line)
		: m_line(line), m_sortCols()
	{}

	operator const std::string & () const
	{
		return m_line;
	}

	bool operator< (const Line& other) const
	{
		for (size_t i = 0; i < m_sortCols.size(); ++i) if ((*this)[i] < other[i]) return true;
		return false;
	}

	void addCol(std::unique_ptr<IColumn>&& col)
	{
		m_sortCols.emplace_back(std::move(col));
	}

private:
	const IColumn& operator[] (size_t index) const
	{
		return *m_sortCols[index];
	}

	std::string m_line;
	std::vector<std::unique_ptr<IColumn>> m_sortCols;
};

class PolySort
{
public:
	PolySort(const std::vector<ColIdentifier>& sortCols, std::istream& input = std::cin, std::ostream& output = std::cout)
		: m_input(input), m_output(output), m_sortCols(sortCols), m_message(nullptr)
	{}

	bool Sort()
	{
		std::string line;
		size_t lineNum = 0;
		while (std::getline(m_input, line))
		{
			m_lines.push_back({line});
			++lineNum;
		}

		lineNum = 0;
		for (auto& line : m_lines)
		{
			auto splitLine = split(line, m_tokenSeparator);
			for (auto&[type, number] : m_sortCols)
			{
				switch (type)
				{
				case 'S':
					line.addCol(std::make_unique<Column<std::string>>(splitLine[number - 1]));
					break;
				case 'N':
					{
						auto val = strToInt(splitLine[number - 1]);
						if (!val)
						{
							m_message = "error: radka " + std::to_string(lineNum) + ", sloupec " + std::to_string(number) + " - nepripustny format";
							return false;
						}

						line.addCol(std::make_unique<Column<int32_t>>(*val));
					}
					break;
				default:
					m_message = "error: radka " + std::to_string(lineNum) + ", sloupec " + std::to_string(number) + " - nepodporovany typ hodnoty";
					return false;
					break;
				}
			}
			++lineNum;
		}

		std::sort(m_lines.begin(), m_lines.end());

		for (const std::string& line : m_lines) m_output << line << '\n';

		return true;
	}

	void SetSeparator(char delim)
	{
		m_tokenSeparator = delim;
	}

	const std::string& GetMessage() const
	{
		return m_message;
	}

private:
	char m_tokenSeparator = ' ';
	std::vector<Line> m_lines;
	std::istream& m_input;
	std::ostream& m_output;
	std::vector<ColIdentifier> m_sortCols;

	std::string m_message;
};

//////////////////////////////////////////////////////////////////////////////
// Entry Point
//////////////////////////////////////////////////////////////////////////////

int main(int argc, char ** argv)
{
	if (argc <= 1) return 0;

	std::vector<std::string> args(argv + 1, argv + argc);
	ArgParser parser(args, {'i', 'o', 's'});
	
	if(!parser.Parse())
	{
		std::cerr << "error: chybne argumenty\n";
		return 0;
	}

	std::unique_ptr<std::istream> input  = nullptr;
	std::unique_ptr<std::ostream> output = nullptr;
	if (parser.HasOptionValue('i')) input  = std::make_unique<std::ifstream>(parser.GetOptionValue('i'));
	if (parser.HasOptionValue('o')) output = std::make_unique<std::ofstream>(parser.GetOptionValue('o'));

	PolySort p(parser.GetRequired(), input ? *input : std::cin, output ? *output : std::cout);
	if (parser.HasOptionValue('s')) p.SetSeparator(parser.GetOptionValue('s')[0]);
	p.Sort();
}
