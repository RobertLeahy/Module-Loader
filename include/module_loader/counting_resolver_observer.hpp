/**
 *	\file
 */

#pragma once

#include "resolver_observer.hpp"
#include <cstddef>

namespace module_loader {

/**
 *	A \ref resolver_observer which simply
 *	counts the number of times it receives
 *	each event.
 */
class counting_resolver_observer : public resolver_observer {
private:
	std::size_t resolve_;
	std::size_t create_;
	std::size_t destroy_;
public:
	/**
	 *	Creates a new counting_resolver_observer.
	 */
	counting_resolver_observer ();
	virtual void on_resolve (resolve_event) override;
	virtual void on_create (create_event) override;
	virtual void on_destroy (destroy_event) override;
	/**
	 *	Determines the number of times \ref on_resolve
	 *	has been invoked.
	 *
	 *	\return
	 *		The count.
	 */
	std::size_t resolve () const noexcept;
	/**
	 *	Determines the number of times \ref on_create
	 *	has been invoked.
	 *
	 *	\return
	 *		The count.
	 */
	std::size_t create () const noexcept;
	/**
	 *	Determines the number of times \ref on_destroy
	 *	has been invoked.
	 *
	 *	\return
	 *		The count.
	 */
	std::size_t destroy () const noexcept;
};

}
