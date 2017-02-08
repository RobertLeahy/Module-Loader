#include <boost/filesystem.hpp>
#include <module_loader/directory_entry_filter.hpp>
#include <module_loader/directory_scanning_shared_library_factory.hpp>
#include <module_loader/shared_library_directory_entry_filter.hpp>
#include <algorithm>
#include <cctype>
#include <utility>

namespace module_loader {

bool directory_scanning_shared_library_factory::next_path () {
	auto end = paths_.cend();
	if (path_) {
		if (*path_ == end) return false;
		dispatch_end_directory();
		++*path_;
	} else {
		path_ = paths_.cbegin();
	}
	if (*path_ == end) return false;
	dispatch_begin_directory();
	return true;
}

bool directory_scanning_shared_library_factory::next_entry () {
	boost::filesystem::directory_iterator end;
	if (dir_ != end) ++dir_;
	while (dir_ == end) {
		if (!next_path()) return false;
		dir_ = boost::filesystem::directory_iterator(**path_);
	}
	return true;
}

bool directory_scanning_shared_library_factory::next_library () {
	shared_library_directory_entry_filter fallback;
	directory_entry_filter & filter = filter_ ? *filter_ : fallback;
	for (;;) {
		if (!next_entry()) return false;
		if (filter.check(*dir_)) return true;
	}
}

void directory_scanning_shared_library_factory::dispatch_begin_directory () const {
	if (!o_) return;
	directory_scanning_shared_library_factory_observer::begin_directory_event e(**path_);
	o_->on_begin_directory(std::move(e));
}

void directory_scanning_shared_library_factory::dispatch_end_directory () const {
	if (!o_) return;
	directory_scanning_shared_library_factory_observer::end_directory_event e(**path_);
	o_->on_end_directory(std::move(e));
}

void directory_scanning_shared_library_factory::dispatch_load (const boost::dll::shared_library & so) const {
	if (!o_) return;
	directory_scanning_shared_library_factory_observer::load_event e(so);
	o_->on_load(std::move(e));
}

directory_scanning_shared_library_factory::directory_scanning_shared_library_factory ()
	:	filter_(nullptr),
		o_(nullptr)
{	}

directory_scanning_shared_library_factory::directory_scanning_shared_library_factory (directory_entry_filter & filter)
	:	directory_scanning_shared_library_factory()
{
	filter_ = &filter;
}

directory_scanning_shared_library_factory::directory_scanning_shared_library_factory (directory_scanning_shared_library_factory_observer & o)
	:	directory_scanning_shared_library_factory()
{
	o_ = &o;
}

directory_scanning_shared_library_factory::directory_scanning_shared_library_factory (directory_entry_filter & filter, directory_scanning_shared_library_factory_observer & o)
	:	directory_scanning_shared_library_factory(filter)
{
	o_ = &o;
}

boost::dll::shared_library directory_scanning_shared_library_factory::next () {
	if (!next_library()) return boost::dll::shared_library{};
	boost::dll::shared_library retr(dir_->path());
	dispatch_load(retr);
	return retr;
}

bool directory_scanning_shared_library_factory::add (boost::filesystem::path path) {
	path = boost::filesystem::canonical(path);
	auto pair = paths_.insert(std::move(path));
	return pair.second;
}

}
