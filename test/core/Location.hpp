#ifndef TEST_CORE_LOCATION_HPP
#define TEST_CORE_LOCATION_HPP


namespace test
{

#define TEST_LOCATION ::test::Location(__FILE__, __LINE__, __func__)

	class Location
	{
	private:
		const char*		_fileName;
		int				_line;
		const char*		_function;

	public:
		Location(const char* fileName, int line, const char* function)
			: _fileName(fileName), _line(line), _function(function)
		{ }

		const char* GetFileName() const { return _fileName; }
		int GetLine() const { return _line; }
		const char* GetFunction() const { return _function; }
	};

}

#endif
