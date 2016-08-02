#ifndef TEST_CORE_LOCATION_HPP
#define TEST_CORE_LOCATION_HPP


namespace test
{

#define TEST_LOCATION ::test::Location(__FILE__, __LINE__)

	class Location
	{
	private:
		const char*		_fileName;
		int				_line;

	public:
		Location(const char* fileName, int line)
			: _fileName(fileName), _line(line)
		{ }

		const char* GetFileName() const { return _fileName; }
		int GetLine() const { return _line; }

		std::string ToString() const { return std::string(_fileName) + ":" + std::to_string(_line); }
	};

}

#endif
