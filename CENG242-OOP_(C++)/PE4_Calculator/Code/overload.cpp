#include "nullary.h"
#include "unary.h"
#include "binary.h"

namespace sym {
	__expr_t& operator-(const __expr_t &op) 
	{
		__expr_t* instance;
		if (op.is_nullary())
		{
			instance = new NegOp(op.eval());
		}
		else
		{
			instance = new NegOp(&op);
		}
		//__expr_t* instance = new NegOp(&op);
		return *instance;
	}
	__expr_t& exp(const __expr_t &op) 
	{
		__expr_t* instance;
		if (op.is_nullary())
		{
			instance = new ExpOp(op.eval());
		}
		else
		{
			instance = new ExpOp(&op);
		}
		//__expr_t* instance = new ExpOp(&op);
		return *instance;
	}

	__expr_t& operator+(const __expr_t &lhs, const __expr_t &rhs) 
	{
		__expr_t* instance;
		if (lhs.is_nullary() && rhs.is_nullary())
		{
			instance = new AddOp(lhs.eval(), rhs.eval());
		}
		else if (lhs.is_nullary())
		{
			instance = new AddOp(lhs.eval(), &rhs);
		}
		else if (rhs.is_nullary())
		{
			instance = new AddOp(&lhs, rhs.eval());
		}
		else
		{
			instance = new AddOp(&lhs, &rhs);
		}
		//__expr_t* instance = new AddOp(&lhs, &rhs);
		return *instance;
	}
	__expr_t& operator+(double lhs, const __expr_t &rhs) 
	{
		__expr_t* instance;
		if (rhs.is_nullary())
		{
			instance = new AddOp(new Const(lhs), rhs.eval());
		}
		else
		{
			instance = new AddOp(new Const(lhs), &rhs);
		}
		//__expr_t* instance = new AddOp(new Const(lhs), &rhs);
		return *instance;
	}
	__expr_t& operator+(const __expr_t &lhs, double rhs) 
	{
		__expr_t* instance;
		if (lhs.is_nullary())
		{
			instance = new AddOp(lhs.eval(), new Const(rhs));
		}
		else
		{
			instance = new AddOp(&lhs, new Const(rhs));
		}
		//__expr_t* instance = new AddOp(&lhs, new Const(rhs));
		return *instance;
	}

	__expr_t& operator*(const __expr_t &lhs, const __expr_t &rhs) 
	{
		__expr_t* instance;
		if (lhs.is_nullary() && rhs.is_nullary())
		{
			instance = new MulOp(lhs.eval(), rhs.eval());
		}
		else if (lhs.is_nullary())
		{
			instance = new MulOp(lhs.eval(), &rhs);
		}
		else if (rhs.is_nullary())
		{
			instance = new MulOp(&lhs, rhs.eval());
		}
		else
		{
			instance = new MulOp(&lhs, &rhs);
		}
		//__expr_t* instance = new MulOp(&lhs, &rhs);
		return *instance;
	}
	__expr_t& operator*(double lhs, const __expr_t &rhs) 
	{
		__expr_t* instance;
		if (rhs.is_nullary())
		{
			instance = new MulOp(new Const(lhs), rhs.eval());
		}
		else
		{
			instance = new MulOp(new Const(lhs), &rhs);
		}
		//__expr_t* instance = new MulOp(new Const(lhs), &rhs);
		return *instance;
	}
	__expr_t& operator*(const __expr_t &lhs, double rhs) 
	{
		__expr_t* instance;
		if (lhs.is_nullary())
		{
			instance = new MulOp(lhs.eval(), new Const(rhs));
		}
		else
		{
			instance = new MulOp(&lhs, new Const(rhs));
		}
		//__expr_t* instance = new MulOp(&lhs, new Const(rhs));
		return *instance;
	}
}
