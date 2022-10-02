#include "core/log.hpp"
#include <imgui/widgets.hpp>

Log::Log(){
	m_History.Reserve(HistorySize);
	m_History.Add({});
}

void Log::DrawImGuiText(){
	for (const String& str : m_History)
		ImGui::Text("%s", str.Data());
}

void Log::Write(String &&line){
	if (m_History.Size() == HistorySize) {
		for (size_t i = 0; i < m_History.Size() - 1; i++)
			m_History[i] = Move(m_History[i + 1]);
		m_History.RemoveLast();
	}
	m_History.Add(Move(line));
}

LogWriter::LogWriter(Log& log):
	m_Log(log)
{}

void LogWriter::Write(const char* string, size_t size){
	for (size_t i = 0; i < size; i++) {
		if (string[i] == '\n') {
			m_Log.Write(Move(m_LineCache));
		} else {
			m_LineCache.Append({ &string[i], 1 });
		}
	}
}

LogInitializer::LogInitializer(LogWriter &log):
	Err(new StringWriterCombiner(StraitXError, &log)),
	Out(new StringWriterCombiner(StraitXOut, &log))
{
	StraitXError = Err;
	StraitXOut = Out;
}

LogInitializer::~LogInitializer(){
	StraitXOut = Out->First;
	StraitXError = Err->First;
	delete Err;
	delete Out;
}
