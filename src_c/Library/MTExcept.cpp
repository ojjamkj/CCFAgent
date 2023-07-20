#include	<string.h>
#include	<stdio.h>
#include	"MTExcept.h"

CMTException::CMTException() {
	m_lCode = -1;
	m_lpszMessage = new char[1];
	m_lpszMessage[0] = '\n';
}

CMTException::CMTException(long lCode, const char* lpszMessage) {
	m_lCode = lCode;
	m_lpszMessage = new char[strlen(lpszMessage) + 1];
	strcpy(m_lpszMessage, lpszMessage);
}

CMTException::~CMTException() {
	if (m_lpszMessage)
		delete[] m_lpszMessage;
}

long CMTException::GetECode(void) const {
	return m_lCode;
}

const char* CMTException::GetEMessage(void) const {
	return m_lpszMessage;
}

void CMTException::Delete(void) {
	delete this;
}
