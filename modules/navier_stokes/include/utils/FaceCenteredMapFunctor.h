//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#pragma once

#include "MooseFunctor.h"
#include "CellCenteredMapFunctor.h"
#include "GreenGaussGradient.h"
#include "MathFVUtils.h"
#include "libmesh/utility.h"
#include "libmesh/type_tensor.h"
#include "libmesh/compare_types.h"
#include "libmesh/threads.h"

/**
 * A functor whose evaluation relies on querying a map where the keys are face info ids
 * and the values correspond to the face values. The primary purpose of this functor is to
 * store face based fields (face flux, face velocity, normal gradient) often encountered
 * in fluid dynamics problems using the finite volume method
 */
template <typename T, typename Map>
class FaceCenteredMapFunctor : public Moose::FunctorBase<T>, public Map
{
public:
  using typename Moose::FunctorBase<T>::ValueType;
  using typename Moose::FunctorBase<T>::GradientType;
  using typename Moose::FunctorBase<T>::DotType;
  using ElemArg = Moose::ElemArg;
  using ElemFromFaceArg = Moose::ElemFromFaceArg;
  using FaceArg = Moose::FaceArg;
  using SingleSidedFaceArg = Moose::SingleSidedFaceArg;
  using ElemQpArg = Moose::ElemQpArg;
  using ElemSideQpArg = Moose::ElemSideQpArg;
  using ElemPointArg = Moose::ElemPointArg;

  FaceCenteredMapFunctor(const MooseMesh & mesh, const std::string & name)
    : Moose::FunctorBase<T>(name), _mesh(mesh)
  {
  }

  FaceCenteredMapFunctor(const MooseMesh & mesh,
                         const std::set<SubdomainID> & sub_ids,
                         const std::string & name)
    : Moose::FunctorBase<T>(name),
      _mesh(mesh),
      _sub_ids(sub_ids == mesh.meshSubdomains() ? std::set<SubdomainID>() : sub_ids)
  {
  }

private:
  /// The mesh that this functor lives on
  const MooseMesh & _mesh;

  /// The subdomain IDs that this functor lives on. If empty, then we consider the functor to live
  /// on all subdomains
  const std::set<SubdomainID> _sub_ids;

  ValueType evaluate(const ElemArg & elem_arg, unsigned int state) const override final;

  ValueType evaluate(const FaceArg & face, unsigned int state) const override final;

  ValueType evaluate(const FaceInfo * const fi) const;

  ValueType evaluate(const ElemFromFaceArg &, unsigned int) const override
  {
    mooseError("not implemented");
  }

  ValueType evaluate(const ElemPointArg &, const unsigned int) const override
  {
    mooseError("not implemented");
  }

  ValueType evaluate(const SingleSidedFaceArg & ssf, unsigned int) const override
  {
    return (*this)(Moose::FV::makeCDFace(*ssf.fi));
  }

  ValueType evaluate(const ElemQpArg &, unsigned int) const override
  {
    mooseError("not implemented");
  }

  ValueType evaluate(const ElemSideQpArg &, unsigned int) const override
  {
    mooseError("not implemented");
  }
};
