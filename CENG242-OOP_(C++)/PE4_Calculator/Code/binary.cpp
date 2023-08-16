#include "binary.h"
#include "nullary.h"
#include <math.h>

namespace sym 
{
	bool AddOp::is_add() const {return true;}

	__expr_t* AddOp::eval(const var_map_t& vars) const
	{
		__expr_t *lhsEvalPtr,*rhsEvalPtr;
		lhsEvalPtr = lhs_->eval(vars);
		rhsEvalPtr = rhs_->eval(vars);
		const Const* lhsPtr = dynamic_cast<const Const*>(lhsEvalPtr);
		const Const* rhsPtr = dynamic_cast<const Const*>(rhsEvalPtr);
		if (lhsPtr != NULL && lhsPtr->get_value() == 0)
		{
			delete lhsEvalPtr;
			return rhsEvalPtr;
		}
		if (rhsPtr != NULL && rhsPtr->get_value() == 0)
		{
			delete rhsEvalPtr;
			return lhsEvalPtr;
		}
		if (lhsPtr != NULL && rhsPtr != NULL)
		{
			Const* result = new Const(lhsPtr->get_value() + rhsPtr->get_value());
			delete lhsEvalPtr;
			delete rhsEvalPtr;
			return result;
		}
		return new AddOp(lhsEvalPtr, rhsEvalPtr);
	}

	__expr_t* AddOp::diff(const std::string& v) const
	{
		__expr_t* diffed = new AddOp(lhs_->diff(v), rhs_->diff(v));
		__expr_t* simplified = diffed->eval();
		delete diffed;
		return simplified;
	}

	std::ostream& AddOp::operator<< (std::ostream &out) const
	{
		if (lhs_->is_nullary()) out << *lhs_;
		else out << "(" << *lhs_ << ")";
		out << " + ";
		if (rhs_->is_nullary()) out << *rhs_;
		else out << "(" << *rhs_ << ")";
		return out;
	}

	bool AddOp::operator==(const __expr_t& other_) const 
	{
		const AddOp* ptr = dynamic_cast<const AddOp*>(&other_);
		if (ptr == NULL) return false;
		else
		{
			if (*lhs_ == *(ptr->lhs_) && *rhs_ == *(ptr->rhs_)) return true;
			if (*lhs_ == *(ptr->rhs_) && *rhs_ == *(ptr->lhs_)) return true;
			else return false;
		}
	}
}

namespace sym 
{
	bool MulOp::is_mul() const {return true;}

	__expr_t* MulOp::eval(const var_map_t& vars) const 
	{
		__expr_t *lhsEvalPtr,*rhsEvalPtr;
		lhsEvalPtr = lhs_->eval(vars);
		rhsEvalPtr = rhs_->eval(vars);
		const Const* lhsPtr = dynamic_cast<const Const*>(lhsEvalPtr);
		const Const* rhsPtr = dynamic_cast<const Const*>(rhsEvalPtr);
		if (lhsPtr != NULL && lhsPtr->get_value() == 0 || rhsPtr != NULL && rhsPtr->get_value() == 0)
		{
			delete lhsEvalPtr;
			delete rhsEvalPtr;
			return new Const(0);
		}
		if (lhsPtr != NULL && lhsPtr->get_value() == 1)
		{
			delete lhsEvalPtr;
			return rhsEvalPtr;
		}
		if (rhsPtr != NULL && rhsPtr->get_value() == 1)
		{
			delete rhsEvalPtr;
			return lhsEvalPtr;
		}
		if (lhsPtr != NULL && rhsPtr != NULL)
		{
			Const* result = new Const(lhsPtr->get_value() * rhsPtr->get_value());
			delete lhsEvalPtr;
			delete rhsEvalPtr;
			return result;
		}
		return new MulOp(lhsEvalPtr, rhsEvalPtr);
	}

	__expr_t* MulOp::diff(const std::string& v) const 
	{
		__expr_t* diffed = new AddOp(new MulOp(lhs_->diff(v), rhs_->eval()), new MulOp(lhs_->eval(), rhs_->diff(v)));
		__expr_t* simplified = diffed->eval();
		delete diffed;
		return simplified;
	}

	std::ostream& MulOp::operator<< (std::ostream &out) const 
	{
		if (lhs_->is_nullary()) out << *lhs_;
		else out << "(" << *lhs_ << ")";
		out << " * ";
		if (rhs_->is_nullary()) out << *rhs_;
		else out << "(" << *rhs_ << ")";
		return out;
	}

	bool MulOp::operator==(const __expr_t& other_) const 
	{
		const MulOp* ptr = dynamic_cast<const MulOp*>(&other_);
		if (ptr == NULL) return false;
		else
		{
			if (*lhs_ == *(ptr->lhs_) && *rhs_ == *(ptr->rhs_)) return true;
			if (*lhs_ == *(ptr->rhs_) && *rhs_ == *(ptr->lhs_)) return true;
			else return false;
		}
	}
}
