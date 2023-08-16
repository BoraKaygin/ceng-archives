#include "type.h"
#include "nullary.h"

namespace sym 
{
	double Const::get_value() const {return value_;}

	Const::operator double() const {return value_;}

	bool Const::is_con() const 
	{
		return true;
	}

	__expr_t* Const::eval(const var_map_t& var_map) const {return new Const(value_);}

	__expr_t* Const::diff(const std::string& v) const {return new Const(0);}

	std::ostream& Const::operator<< (std::ostream &out) const
	{
		out << value_;
		return out;
	}

	bool Const::operator==(const __expr_t& other) const
	{
		const Const* ptr = dynamic_cast<const Const*>(&other);
		if (ptr!=NULL && ptr->value_ == value_) return true;
		else return false;
	}
}

namespace sym 
{
	std::string Var::get_variable() const {return var_;}
	Var::operator std::string () const {return var_;}
	
	bool Var::is_var() const {return true;}

	__expr_t* Var::eval(const var_map_t& vars) const
	{
		if (vars.count(var_) == 0) return new Var(var_);
		else return new Const(vars.at(var_));
	}

	__expr_t* Var::diff(const std::string& v) const 
	{
		if (v==var_) return new Const(1);
		else return new Const(0);
	}
	
	std::ostream& Var::operator<< (std::ostream &out) const
	{
		out << var_;
		return out;
	}

	bool Var::operator==(const __expr_t& other) const 
	{
		const Var* ptr = dynamic_cast<const Var*>(&other);
		if (ptr!=NULL && ptr->var_==var_) return true;
		else return false;
	}
}