#ifndef MITANA_TREEMOD_BOOLEANMOD_H
#define MITANA_TREEMOD_BOOLEANMOD_H

#include "MitAna/TreeMod/interface/BaseMod.h"

namespace mithep {

  class BooleanMod : public BaseMod {
  public:
    class Expression : public TObject {
    public:
      enum Operator {
        kPASS,
        kNOT,
        kAND,
        kOR
      };

      Expression(BaseMod const*, Operator = kPASS);
      Expression(Expression const*, Operator);
      Expression(BaseMod const*, BaseMod const*, Operator);
      Expression(BaseMod const*, Expression const*, Operator);
      Expression(Expression const*, BaseMod const*, Operator);
      Expression(Expression const*, Expression const*, Operator);

      Expression(Expression const&);
      ~Expression();

      Expression& operator=(Expression const&);

      Bool_t Eval() const;

    private:
      Operator fOperator{kPASS};
      BaseMod const* fMod{0};
      Expression const* fExpr[2] = {0, 0};
      Bool_t fMustDelete[2] = {kFALSE, kFALSE};

      ClassDef(Expression, 0)
    };

    BooleanMod(char const* name = "BooleanMod", char const* title = "boolean operation") : BaseMod(name, title) {}
    ~BooleanMod() {}

    void SetExpression(Expression const* expr) { fExpression = expr; }

  private:
    void Process() override
    {
      if (!fExpression || !fExpression->Eval())
        SkipEvent();
    }

    Expression const* fExpression{0};

    ClassDef(BooleanMod, 0)
  };

}

#endif
