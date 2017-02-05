/**
 *	\file
 */

#pragma once

#include "in_place_object.hpp"
#include "object.hpp"
#include "reference_object.hpp"
#include "variadic_offer.hpp"
#include "void_object.hpp"
#include <memory>
#include <string>
#include <type_traits>
#include <utility>

namespace module_loader {

namespace detail {

template <typename F, typename... Ts>
using function_offer_result_t = std::result_of_t<F (std::add_lvalue_reference_t<Ts>...)>;
template <typename F, typename... Ts>
constexpr bool is_function_offer_reference_v = std::is_lvalue_reference<function_offer_result_t<F,Ts...>>::value;
template <typename F, typename... Ts>
using function_offer_t = std::decay_t<function_offer_result_t<F,Ts...>>;

}

/**
 *	An \ref offer which offers the result of
 *	invoking a functor.
 *
 *	If the wrapped functor returns by lvalue
 *	reference then the offered object is of
 *	type \ref reference_object, otherwise it
 *	is of type \ref in_place_object.
 *
 *	\tparam F
 *		The type of functor to store and invoke.
 *	\tparam Ts
 *		The types which shall be requested and
 *		passed through to the functor of type
 *		\em F.  For each type parameter in the
 *		parameter pack one \ref request shall
 *		be generated for exactly 1 object of
 *		that type.
 */
template <typename F, typename... Ts>
class function_offer : public variadic_offer<detail::function_offer_t<F,Ts...>,Ts...> {
private:
	using base = variadic_offer<detail::function_offer_t<F,Ts...>,Ts...>;
public:
	using value_type = typename base::value_type;
	using fulfill_type = typename base::fulfill_type;
private:
	using object_type = std::conditional_t<
		detail::is_function_offer_reference_v<F,Ts...>,
		reference_object<value_type>,
		std::conditional_t<
			std::is_same<value_type,void>::value,
			void_object,
			in_place_object<value_type>
		>
	>;
	using tag_t = typename std::is_same<value_type,void>::type;
	static constexpr tag_t tag{};
	F func_;
	template <std::size_t... Is>
	decltype(auto) invoke (const fulfill_type & objects, std::index_sequence<Is...>) {
		base::check_requests(objects);
		return func_(*static_cast<Ts *>(*objects[Is].first)...);
	}
	decltype(auto) invoke (const fulfill_type & objects) {
		return invoke(objects,base::index_sequence);
	}
	std::unique_ptr<object> fulfill (const fulfill_type & objects, std::true_type) {
		invoke(objects);
		return std::make_unique<object_type>();
	}
	std::unique_ptr<object> fulfill (const fulfill_type & objects, std::false_type) {
		return std::make_unique<object_type>(*this,invoke(objects));
	}
	std::shared_ptr<object> fulfill_shared (const fulfill_type & objects, std::true_type) {
		invoke(objects);
		return std::make_shared<object_type>();
	}
	std::shared_ptr<object> fulfill_shared (const fulfill_type & objects, std::false_type) {
		return std::make_shared<object_type>(*this,invoke(objects));
	}
public:
	function_offer () = default;
	/**
	 *	Creates a function_offer with a default
	 *	name which wraps a certain functor.
	 *
	 *	\param [in] func
	 *		The functor to wrap.
	 */
	explicit function_offer (F func) : func_(std::forward<F>(func)) {	}
	/**
	 *	Creates a function_offer with a custom
	 *	name which wraps a certain functor.
	 *
	 *	\param [in] func
	 *		The functor to wrap.
	 *	\param [in] name
	 *		The name.
	 */
	function_offer (F func, std::string name) : base(std::move(name)), func_(std::forward<F>(func)) {	}
	virtual std::unique_ptr<object> fulfill (const fulfill_type & objects) override {
		return fulfill(objects,tag);
	}
	virtual std::shared_ptr<object> fulfill_shared (const fulfill_type & objects) override {
		return fulfill_shared(objects,tag);
	}
};

}
