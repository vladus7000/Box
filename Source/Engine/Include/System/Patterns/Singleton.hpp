#pragma once

namespace box
{
#define SINGLETON(type) \
protected:\
	type() {}\
	~type() {}\
private:\
	type(const type& rhs) = delete;\
	type(type&& rhs) = delete;\
	type& operator=(type&& rhs) = delete;\
	type& operator=(type& rhs) = delete;\
public:\
	static type& Instance();\
private:

#define SINGLETON_ACCESSOR(type) type& type::Instance() { static type t; return t; }
}