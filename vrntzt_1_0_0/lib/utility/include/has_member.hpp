// ch 2021
// c++20
// v0.1.0
// u_lib
// header for compile-time check of member-existence

// usage like:
// class Bar { void foo(bool, float) {} };
// Has_Member_Function(Bar, &Bar::foo, void, bool, float);

// TODO: re-arrange file (e.g. concepts in anonymous namespace
// TODO: enable usage with static_assert
// TODO: add support for static functions & data members

#ifndef U_LIB_HAS_MEMBER_HPP
#define U_LIB_HAS_MEMBER_HPP

#include <iostream>

namespace u_lib
{
	namespace
	{
		// concept which fails if the passed method is not callable by arguments of
		// Arg_Types or doesn't return a value of Return_Type
		template <typename Classname, auto f_ptr,
			typename Return_Type, typename... Arg_Types>
		concept Has_Signature = requires (Classname obj, Arg_Types... args)
		{
			// unevaluated context
			{ (obj.*f_ptr)(args...) } -> std::same_as<Return_Type>;
		};

		// unspecialized version of _has_member_function; inherits from false_type
		template <typename T, typename Classname, auto f_ptr, typename Return_Type,
			typename... Arg_Types>
		struct _has_member_function : std::false_type
		{
			_has_member_function() = delete;
		};

		// specialized version of _has_member_function; is chosen if the passed
		// function satisfies the _has_signature constraints; inherits from true_type
		template <typename Classname, auto f_ptr, typename Return_Type,
			typename... Arg_Types>
		requires Has_Signature<Classname, f_ptr, Return_Type, Arg_Types...>
		struct _has_member_function<void, Classname, f_ptr, Return_Type, Arg_Types...>
			: std::true_type
		{
			_has_member_function() = delete;
		};

		/* ======================================================================== */

		// concept which fails if no constructor for Arg_Types arguments is available
		template <typename Classname, typename... Arg_Types>
		concept Constructible_With = requires (Arg_Types... args)
		{
			// unevaluated context
			{ Classname(args...) };
		};

		// unspecialized version of _constructible_with; inherits from false_type
		template <typename T, typename Classname, typename... Arg_Types>
		struct _constructible_with : std::false_type
		{
			_constructible_with() = delete;
		};

		// specialized version of _constructible_with; inherits from true_type;
		// needs to satisfy Constructible_With concept
		template <typename Classname, typename... Arg_Types>
		requires Constructible_With<Classname, Arg_Types...>
			struct _constructible_with<void, Classname, Arg_Types...>
			: std::true_type
		{
			_constructible_with() = delete;
		};
	}

	// simplifies call of _has_member_function by removing need to pass 'void' as
	// first template argument
	template <typename Classname, auto f_ptr, typename Return_Type,
		typename... Arg_Types>
		using has_member_function = _has_member_function<void, Classname, f_ptr,
		Return_Type, Arg_Types...>;

	// simplifies call of _constructible_with by removing need to pass 'void' as
	// first template argument
	template <typename Classname, typename... Arg_Types>
	using constructible_with = _constructible_with<void, Classname, Arg_Types...>;
}

#endif // !U_LIB_HAS_MEMBER_HPP