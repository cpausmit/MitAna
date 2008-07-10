//--------------------------------------------------------------------------------------------------
// $Id: Array.h,v 1.9 2008/07/03 08:22:17 loizides Exp $
//
// Array
//
// Implementation of Collection interface using TClonesArray class. 
//
// Authors: C.Loizides
//--------------------------------------------------------------------------------------------------

#ifndef DATATREE_ARRAY_H
#define DATATREE_ARRAY_H

#include <TClonesArray.h>
#include "MitAna/DataTree/interface/Collection.h"

namespace mithep 
{
  template<class ArrayElement>
  class Array : public Collection<ArrayElement> 
  {
    public:
      Array(const char *name=0, Int_t size=0);
      ~Array() {}

      ArrayElement        *AddNew();
      ArrayElement        *Allocate();
      const TClonesArray  &Arr()                                const { return fArray; }
      ArrayElement        *At(UInt_t idx);
      const ArrayElement  *At(UInt_t idx)                       const;
      UInt_t               GetEntries()                         const { return fNumEntries; }
      const char*          GetName()                            const { return fArray.GetName(); }
      Bool_t               IsOwner()                            const { return kTRUE; }
      TIterator           *MakeIterator(Bool_t dir = kIterForward) const;
      Bool_t               MustClear()                          const { return this->TestBit(14); }
      void                 Reset()                                    { Clear(); }
      void                 Trim()                                     { fArray.Compress(); }
      void                 SetMustClearBit()                          { this->SetBit(14); }
      void                 SetName(const char* name)                  { fArray.SetName(name); }
      ArrayElement        *UncheckedAt(UInt_t idx);                 
      const ArrayElement  *UncheckedAt(UInt_t idx)              const;

      ArrayElement        *operator[](UInt_t idx);
      const ArrayElement  *operator[](UInt_t idx)               const;

    protected:
      void                 Clear();

      TClonesArray         fArray;        //array for storage
      UInt_t               fNumEntries;   //number of entries in the array

    private:
      Array(const Array &a);

    ClassDefT(Array,1) // Wrapper around TClonesArray class
  };
}

//--------------------------------------------------------------------------------------------------
template<class ArrayElement>
inline mithep::Array<ArrayElement>::Array(const char *name, Int_t size) : 
  fArray(ArrayElement::Class_Name(),size), 
  fNumEntries(0)
{
   // Default constructor.

  fArray.BypassStreamer(kTRUE);

  if (name) 
    fArray.SetName(name);

  ArrayElement test;
  if (test.GetClearBit())
    SetMustClearBit();
}

//--------------------------------------------------------------------------------------------------
template<class ArrayElement>
inline ArrayElement *mithep::Array<ArrayElement>::AddNew()
{
  // Add new entry and return pointer to it.

  return new(Allocate()) ArrayElement(); 
}

//--------------------------------------------------------------------------------------------------
template<class ArrayElement>
inline ArrayElement *mithep::Array<ArrayElement>::Allocate()
{
   // Allocate a slot in the array, *only* to be used in placement new operator.
   
   return static_cast<ArrayElement*>(fArray[fNumEntries++]);
}

//--------------------------------------------------------------------------------------------------
template<class ArrayElement>
inline ArrayElement* mithep::Array<ArrayElement>::At(UInt_t idx)
{
  // Return entry at given index.

  if (idx<fNumEntries)
    return static_cast<ArrayElement*>(fArray.UncheckedAt(idx));

  Fatal("At","Index too large: (%ud < %ud violated) for %s with type %s",
        idx, fNumEntries, GetName(), Array::ClassName()); 
  return 0;
}

//--------------------------------------------------------------------------------------------------
template<class ArrayElement>
inline const ArrayElement* mithep::Array<ArrayElement>::At(UInt_t idx) const
{
  // Return entry at given index.

  if (idx<fNumEntries)
    return static_cast<const ArrayElement*>(fArray.UncheckedAt(idx));

  Fatal("At","Index too large: (%ud < %ud violated) for %s with type %s",
        idx, fNumEntries, GetName(), Array::ClassName()); 
  return 0;
}

//--------------------------------------------------------------------------------------------------
template<class ArrayElement>
inline void mithep::Array<ArrayElement>::Clear()
{
   // Default implementation for clearing the array.

#if 1
  fArray.Delete();
#else
  if (this->MustClear())
    fArray.Clear("C"); //with opt=="C" will call clear for every element
  else 
    fArray.Clear();
#endif
  fNumEntries = 0;
}

//--------------------------------------------------------------------------------------------------
template<class ArrayElement>
inline TIterator *mithep::Array<ArrayElement>::MakeIterator(Bool_t dir) const 
{ 
  // Return ROOT collection iterator.

  return fArray.MakeIterator(dir); 
}

//--------------------------------------------------------------------------------------------------
template<class ArrayElement>
inline const ArrayElement* mithep::Array<ArrayElement>::operator[](UInt_t idx) const
{
  // Return entry at given index.

  return At(idx);
}

//--------------------------------------------------------------------------------------------------
template<class ArrayElement>
inline ArrayElement* mithep::Array<ArrayElement>::operator[](UInt_t idx)
{
  // Return entry at given index.

  return At(idx);
}

//--------------------------------------------------------------------------------------------------
template<class ArrayElement>
inline ArrayElement* mithep::Array<ArrayElement>::UncheckedAt(UInt_t idx)
{
  // Return entry at given index.

  return static_cast<ArrayElement*>(fArray.UncheckedAt(idx));
}

//--------------------------------------------------------------------------------------------------
template<class ArrayElement>
inline const ArrayElement* mithep::Array<ArrayElement>::UncheckedAt(UInt_t idx) const
{
  // Return entry at given index.

  return static_cast<const ArrayElement*>(fArray.UncheckedAt(idx));
}
#endif
