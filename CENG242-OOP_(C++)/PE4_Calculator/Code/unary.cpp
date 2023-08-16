#include "nullary.h"
#include "unary.h"
#include "binary.h"
#include <math.h>

namespace sym 
{
	bool NegOp::is_neg() const {return true;}

	__expr_t* NegOp::eval(const var_map_t& vars) const 
	{
		__expr_t* ePtr = operand->eval(vars);
		const Const* ptr = dynamic_cast<const Const*>(ePtr);
		if (ptr != NULL)
		{
			Const* result = new Const(-ptr->get_value());
			delete ePtr;
			return result;
		}
		else
		{
			return new NegOp(ePtr);
		}
	}

	__expr_t* NegOp::diff(const std::string& v) const
	{
		return new NegOp(operand->diff(v));
	}

	std::ostream& NegOp::operator<< (std::ostream &out) const
	{
		out << "-";
		if (operand->is_nullary()) out << *operand;
		else out << "(" << *operand << ")";
		return out;
	}

	bool NegOp::operator==(const __expr_t& other_) const
	{
		const NegOp* ptr = dynamic_cast<const NegOp*>(&other_);
		if (ptr == NULL) return false;
		else return *operand == *(ptr->operand);
	}
}

namespace sym 
{
	bool ExpOp::is_exp() const {return true;}

	__expr_t* ExpOp::eval(const var_map_t& vars) const
	{
		__expr_t* ePtr = operand->eval(vars);
		const Const* ptr = dynamic_cast<const Const*>(ePtr);
		if (ptr != NULL)
		{
			Const* result = new Const(std::exp(ptr->get_value()));
			delete ePtr;
			return result;
		}
		else
		{
			return new ExpOp(ePtr);
		}
	}

	__expr_t* ExpOp::diff(const std::string& v) const 
	{
		__expr_t* diffed = new MulOp(operand->diff(v), new ExpOp(operand->eval()));
		__expr_t* simplified = diffed->eval();
		delete diffed;
		return simplified;
	}

	std::ostream& ExpOp::operator<< (std::ostream &out) const 
	{
		out << "e^";
		if (operand->is_nullary()) out << *operand;
		else out << "(" << *operand << ")";
		return out;
	}

	bool ExpOp::operator==(const __expr_t& other_) const 
	{
		const ExpOp* ptr = dynamic_cast<const ExpOp*>(&other_);
		if (ptr == NULL) return false;
		else return *operand == *(ptr->operand);
	}
}
