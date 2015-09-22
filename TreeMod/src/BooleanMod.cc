#include "MitAna/TreeMod/interface/BooleanMod.h"

#include <stdexcept>

using namespace mithep;

ClassImp(mithep::BooleanMod)
ClassImp(mithep::BooleanMod::Expression)

BooleanMod::Expression::Expression(BaseMod const* mod, Operator optr/* = kPASS*/) :
  fOperator(optr),
  fMod(mod)
{
  if (fOperator != kPASS && fOperator != kNOT)
    throw std::runtime_error("Binary operator for a single operand");
}

BooleanMod::Expression::Expression(Expression const* expr1, Operator optr) :
  fOperator(optr),
  fExpr{expr1, 0}
{
  if (fOperator != kPASS && fOperator != kNOT)
    throw std::runtime_error("Binary operator for a single operand");
}

BooleanMod::Expression::Expression(BaseMod const* mod1, BaseMod const* mod2, Operator optr) :
  fOperator(optr)
{
  if (fOperator == kPASS || fOperator == kNOT)
    throw std::runtime_error("Unary operator for a two operands");

  fExpr[0] = new Expression(mod1);
  fExpr[1] = new Expression(mod2);
  fMustDelete[0] = fMustDelete[1] = kTRUE;
}

BooleanMod::Expression::Expression(BaseMod const* mod1, Expression const* expr2, Operator optr) :
  fOperator(optr),
  fExpr{0, expr2}
{
  if (fOperator == kPASS || fOperator == kNOT)
    throw std::runtime_error("Unary operator for a two operands");

  fExpr[0] = new Expression(mod1);
  fMustDelete[0] = kTRUE;
}

BooleanMod::Expression::Expression(Expression const* expr1, BaseMod const* mod2, Operator optr) :
  fOperator(optr),
  fExpr{expr1, 0}
{
  if (fOperator == kPASS || fOperator == kNOT)
    throw std::runtime_error("Unary operator for a two operands");

  fExpr[1] =new Expression(mod2);
  fMustDelete[1] = kTRUE;
}

BooleanMod::Expression::Expression(Expression const* expr1, Expression const* expr2, Operator optr) :
  fOperator(optr),
  fExpr{expr1, expr2}
{
  if (fOperator == kPASS || fOperator == kNOT)
    throw std::runtime_error("Unary operator for a two operands");
}

BooleanMod::Expression::Expression(Expression const& orig) :
  fOperator(orig.fOperator),
  fMod(orig.fMod),
  fExpr{orig.fExpr[0], orig.fExpr[1]}
{
}

BooleanMod::Expression::~Expression()
{
  for (unsigned iE = 0; iE != 2; ++iE) {
    if (fExpr[iE] && fMustDelete[iE])
      delete fExpr[iE];
  }
}

BooleanMod::Expression&
BooleanMod::Expression::operator=(Expression const& rhs)
{
  for (unsigned iE = 0; iE != 2; ++iE) {
    if (fExpr[iE] && fMustDelete[iE])
      delete fExpr[iE];
  }

  fOperator = rhs.fOperator;
  fMod = rhs.fMod;
  fExpr[0] = rhs.fExpr[0];
  fExpr[1] = rhs.fExpr[1];
  fMustDelete[0] = kFALSE;
  fMustDelete[1] = kFALSE;

  return *this;
}

Bool_t
BooleanMod::Expression::Eval() const
{
  if (fMod) {
    if (fOperator == kPASS)
      return fMod->IsActive();
    else if (fOperator == kNOT)
      return !fMod->IsActive();
    else
      return kFALSE;
  }

  if (fExpr[0] && !fExpr[1]) {
    if (fOperator == kPASS)
      return fExpr[0]->Eval();
    else if (fOperator == kNOT)
      return !fExpr[0]->Eval();
    else
      return kFALSE;
  }

  if (fExpr[0] && fExpr[1]) {
    if (fOperator == kAND)
      return fExpr[0]->Eval() && fExpr[1]->Eval();
    else if (fOperator == kOR)
      return fExpr[0]->Eval() || fExpr[1]->Eval();
    else
      return kFALSE;
  }

  return kFALSE;
}
