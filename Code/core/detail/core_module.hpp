#ifndef DETAIL_CORE_MODULE_INCLUDED_
#define DETAIL_CORE_MODULE_INCLUDED_

#include <core/core_fwd.hpp>
#include <vector>
#include <iostream>
#include <memory>


// Allows you quickly stub out interfaces that are not required by the modules.
#define CORE_INTERFACE_STUB(type, func) friend void func(type &stub) {}

#define CORE_INTERFACE_ON_REG_STUB(type) friend void on_register(type &stub, const std::size_t id) {}
#define CORE_INTERFACE_ON_START_STUB(type) friend void on_start(type &stub) {}
#define CORE_INTERFACE_ON_THINK_STUB(type) friend void on_think(type &stub) {}
#define CORE_INTERFACE_ON_DATA_CHANGE_STUB(type) friend void on_data_change(type &stub, const std::vector<DataNode> &node) {}


namespace caffeine {
namespace core {
namespace detail {


/*
	Interface for core modules.
	You shouldn't need to use this directly for anything.
*/
class CoreModule final
{
public:

	template<typename T>
	CoreModule(T t) : object_(new CoreModuleModel<T>(t)) { std::cout << "ctor" << std::endl; }

	CoreModule(const CoreModule& a) : object_(a.object_->copy_()) {std::cout << "copy" << std::endl; }

	CoreModule(CoreModule&&) {}/*noexcept = default;*/

	CoreModule& operator=(const CoreModule& a)
	{
		std::cout << "ass" << std::endl; 
		CoreModule tmp(a);
		*this = std::move(tmp);
		return *this;
	}

	CoreModule& operator=(CoreModule&&) {} /*noexcept = default;*/

	// Module Interface
	friend void new_data(CoreModule& t)
	{
		std::cout << "zoop" << std::endl;
		t.object_->new_data_();
	}

	friend void on_register(CoreModule &m, const std::size_t id) { m.object_->on_register_(id); }
	friend void on_start(CoreModule &m) { m.object_->on_start_(); }
	friend void on_think(CoreModule &m)	{ m.object_->on_think_(); }
	friend void on_data_change(CoreModule &m, const std::vector<DataNode> &nodes) { m.object_->on_data_change_(nodes); }

private:

	struct CoreModuleConcept
	{
		virtual ~CoreModuleConcept() = default;
		virtual CoreModuleConcept* copy_() const = 0;
		virtual void new_data_() = 0;

		virtual void on_register_(const std::size_t id) = 0;
		virtual void on_start_() = 0;
		virtual void on_think_() = 0;
		virtual void on_data_change_(const std::vector<DataNode> &nodes) = 0;
	};

	template<typename T>
	struct CoreModuleModel : CoreModuleConcept
	{
		CoreModuleModel(T t) : data_(std::move(t)) {}
		CoreModuleConcept* copy_() const { return new CoreModuleModel(*this); }

		void new_data_() { new_data(data_); }

		void on_register_(const std::size_t id) { on_register(data_, id); }
		void on_start_() { on_start(data_); }
		void on_think_() { on_think(data_); }
		void on_data_change_(const std::vector<DataNode> &nodes) { on_data_change(data_, nodes); }

		T data_;
		
	};

	std::unique_ptr<CoreModuleConcept> object_;

}; // class


} // detail
} // namespace
} // namespace



#endif // include guard