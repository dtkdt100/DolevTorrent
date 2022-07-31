#include "Handle.h"
#include "Exception.h"

Handle::Handle(Handle&& other) : m_handle(std::exchange(other.m_handle, INVALID_HANDLE_VALUE)) {

}

Handle::Handle(HANDLE handle) : m_handle(handle) {
	if (!isValid()) {
		throw Exception(INAVILD_HANDLE_EXCEPTION);
	}
}

void Handle::close() {
	if (isValid()) {
		CloseHandle(m_handle);
		m_handle = INVALID_HANDLE_VALUE;
	}
}

bool Handle::isValid() const {
	return m_handle != nullptr && m_handle != INVALID_HANDLE_VALUE;
}

HANDLE Handle::getRawHandle() {
	return m_handle;
}

Handle::~Handle() {
	try {
		close();
	}
	catch (...) {}
}