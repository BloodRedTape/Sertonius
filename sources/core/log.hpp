#pragma once

#include <core/string_writer.hpp>
#include <core/list.hpp>
#include <core/string.hpp>

class Log {
	static constexpr size_t HistorySize = 20;
private:
	List<String> m_History;
public:
	Log();

	void DrawImGuiText();

	void Write(String &&line);
};

class LogWriter: public StringWriter{
private:
	Log& m_Log;
	String m_LineCache;
public:
	LogWriter(Log& log);

	void Write(const char* string, size_t size)override;
};

struct StringWriterCombiner: StringWriter{
	StringWriter* First;
	StringWriter* Second;

	StringWriterCombiner(StringWriter* first, StringWriter* second):
		First(first),
		Second(second)
	{}
	
	void Write(const char* string, size_t size)override {
		First->Write(string, size);
		Second->Write(string, size);
	}
};

struct LogInitializer {
	StringWriterCombiner* Out;
	StringWriterCombiner* Err;
	LogInitializer(LogWriter &log);

	~LogInitializer();
};