#pragma once

#include <concepts>
#include <cstdio>
#include <memory>
#include <string_view>
#include <type_traits>

namespace NNDSL::Log {
	template <class T>
	concept ClassName = requires(T t) {
		{
			T::class_name()
		} -> std::same_as<std::string_view>;
	};

	struct ILogger {
		virtual void log(const std::string&) = 0;
	};

	template <ClassName T> struct DefaultLogger : public ILogger {
		void log(const std::string& to_log) override { std::printf("[%s]: %s\n", name, to_log.data()); }

	private:
		static inline auto name = T::class_name().data();
	};

	using Logger = std::unique_ptr<ILogger>;
	template <ClassName T> static Logger get() { return std::make_unique<DefaultLogger<T>>(); }

} // namespace NNDSL::Log
