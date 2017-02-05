#include <boost/dll/shared_library.hpp>
#include <module_loader/exception.hpp>
#include <module_loader/object_decorator.hpp>
#include <module_loader/offer_decorator.hpp>
#include <module_loader/shared_library_offer_factory.hpp>
#include <module_loader/type_name.hpp>
#include <exception>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>
#include <typeinfo>
#include <utility>

namespace module_loader {

namespace {

template <typename Func>
decltype(auto) guard (Func && func, const boost::dll::shared_library & so) {
	try {
		return std::forward<Func>(func)();
	} catch (const std::exception & ex) {
		throw shared_library_offer_factory::error(
			ex,
			so
		);
	} catch (...) {
		throw shared_library_offer_factory::error(
			current_exception_type(),
			std::string{},
			so
		);
	}
}

template <typename Pointer>
class object_wrapper : public object_decorator<Pointer> {
private:
	using base = object_decorator<Pointer>;
	boost::dll::shared_library so_;
	std::string name_;
public:
	object_wrapper () = delete;
	object_wrapper (boost::dll::shared_library so, std::string name, Pointer inner)
		:	base(std::move(inner)),
			so_(std::move(so)),
			name_(std::move(name))
	{	}
	~object_wrapper () noexcept {
		//	The pointee needs to be gone by the time
		//	the shared library is destroyed
		base::reset();
	}
	virtual const std::string & name () const noexcept override {
		return name_;
	}
};

std::string get_name (const boost::dll::shared_library & so, const offer & o) {
	std::ostringstream ss;
	ss << o.name() << " (" << so.location().string() << ")";
	return ss.str();
}

template <typename Pointer>
class offer_wrapper : public offer_decorator<Pointer> {
private:
	using base = offer_decorator<Pointer>;
	boost::dll::shared_library so_;
	std::string name_;
public:
	using fulfill_type = typename base::fulfill_type;
	offer_wrapper () = delete;
	offer_wrapper (boost::dll::shared_library so, Pointer inner)
		:	base(std::move(inner)),
			so_(std::move(so)),
			name_(get_name(so_,base::offer()))
	{	}
	~offer_wrapper () noexcept {
		//	Make sure the pointee is cleaned up
		//	before the shared library goes out
		//	of scope otherwise there'll be bad
		//	business
		base::reset();
	}
	virtual const std::string & name () const noexcept override {
		return name_;
	}
	virtual std::unique_ptr<object> fulfill (const fulfill_type & objects) override {
		auto ptr = guard([&] () {	return base::fulfill(objects);	},so_);
		return std::make_unique<object_wrapper<std::unique_ptr<object>>>(
			so_,
			name_,
			std::move(ptr)
		);
	}
	virtual std::shared_ptr<object> fulfill_shared (const fulfill_type & objects) override {
		auto ptr = guard([&] () {	return base::fulfill_shared(objects);	},so_);
		return std::make_shared<object_wrapper<std::shared_ptr<object>>>(
			so_,
			name_,
			std::move(ptr)
		);
	}
};

}

bool shared_library_offer_factory::get_offers () {
	while (offers_.empty()) {
		curr_ = slf_.next();
		if (!curr_) return false;
		auto fn = curr_.get<void (shared_library_offer_factory &)>(name_);
		dispatch_begin_load();
		guard([&] () {	fn(*this);	},curr_);
		dispatch_end_load();
	}
	return true;
}

std::unique_ptr<offer> shared_library_offer_factory::next_impl () {
	if (!get_offers()) return std::unique_ptr<offer>{};
	auto retr = std::move(offers_.front());
	offers_.pop_front();
	return retr;
}

void shared_library_offer_factory::check_add () const {
	if (!next_active_) throw std::logic_error("Do not call shared_library_offer_factory::add except from within shared_library_offer_factory::next or ::next_shared");
}

void shared_library_offer_factory::dispatch_add (const offer & o) {
	if (!o_) return;
	shared_library_offer_factory_observer::add_event event(curr_,o);
	o_->on_add(std::move(event));
}

void shared_library_offer_factory::dispatch_begin_load () {
	if (!o_) return;
	shared_library_offer_factory_observer::begin_load_event event(curr_);
	o_->on_begin_load(std::move(event));
}

void shared_library_offer_factory::dispatch_end_load () {
	if (!o_) return;
	shared_library_offer_factory_observer::end_load_event event(curr_);
	o_->on_end_load(std::move(event));
}

static std::string get_what (const std::type_info & type, const std::string & what, const boost::dll::shared_library & so) {
	std::ostringstream ss;
	ss << type_name(type) << " thrown from " << so.location().string();
	if (!what.empty()) ss << ": " << what;
	return ss.str();
}

shared_library_offer_factory::error::error (const std::type_info & type, std::string what, boost::dll::shared_library so)
	:	std::runtime_error(get_what(type,what,so)),
		type_(&type),
		what_(std::move(what)),
		so_(std::move(so))
{	}

shared_library_offer_factory::error::error (const std::exception & ex, boost::dll::shared_library so)
	:	error(typeid(ex),ex.what(),so)
{	}


shared_library_offer_factory::shared_library_offer_factory (shared_library_factory & slf)
	:	slf_(slf),
		o_(nullptr),
		name_("load"),
		next_active_(false)
{	}

shared_library_offer_factory::shared_library_offer_factory (shared_library_factory & slf, shared_library_offer_factory_observer & o)
	:	shared_library_offer_factory(slf)
{
	o_ = &o;
}

std::unique_ptr<offer> shared_library_offer_factory::next () {
	std::unique_ptr<offer> retr;
	next_active_ = true;
	try {
		retr = next_impl();
	} catch (...) {
		next_active_ = false;
		throw;
	}
	next_active_ = false;
	return retr;
}

std::shared_ptr<offer> shared_library_offer_factory::next_shared () {
	auto ptr = next();
	if (!ptr) return std::shared_ptr<offer>{};
	return std::shared_ptr<offer>(ptr.release());
}

void shared_library_offer_factory::add (std::unique_ptr<offer> o) {
	check_add();
	dispatch_add(*o);
	offers_.push_back(std::make_unique<offer_wrapper<std::unique_ptr<offer>>>(curr_,std::move(o)));
}

void shared_library_offer_factory::add (std::shared_ptr<offer> o) {
	check_add();
	dispatch_add(*o);
	offers_.push_back(std::make_unique<offer_wrapper<std::shared_ptr<offer>>>(curr_,std::move(o)));
}

}
