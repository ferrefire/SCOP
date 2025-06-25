#pragma once

#include <iostream>
#include <typeinfo>

#if defined(__clang__) || defined(__GNUC__)
#	define PRETTY __PRETTY_FUNCTION__
#elif defined(_MSC_VER)
#	define PRETTY __FUNCSIG__
#else
#	error Unsupported compiler
#endif

template <auto T>
constexpr std::string_view StringName()
{
	constexpr std::string_view stringLiteral = PRETTY;
	constexpr std::string_view key = "T = ";
	const std::size_t start = stringLiteral.find(key) + key.size();
	const std::size_t end = stringLiteral.find_first_of("];>", start);
	
	return (stringLiteral.substr(start, end - start));
}

template <typename T, T... elements>
struct EnumMap
{
	static constexpr std::string_view names[sizeof...(elements)] = {StringName<elements>()...};
};

template <typename T, std::size_t... elements>
constexpr auto CreateMap(int val, std::index_sequence<elements...> sequence)
{
	return (EnumMap<T, static_cast<T>(elements)...>{});
}

//template <typename T, int max>
template <typename T>
constexpr std::string_view EnumName(T val)
{
	const auto map = CreateMap<T>(static_cast<int>(val), std::make_index_sequence<128>{});
	return (map.names[static_cast<int>(val)]);
}

#define VAR_VAL(v) #v << ": " << v
#define ENUM_VAL(e) #e << ": " << EnumName(e)