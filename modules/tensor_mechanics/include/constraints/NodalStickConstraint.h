//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#pragma once

#include "NodalConstraint.h"

class NodalStickConstraint : public NodalConstraint
{
public:
  NodalStickConstraint(const InputParameters & parameters);

  virtual void meshChanged() override;

  virtual void computeResidual(NumericVector<Number> & residual) override;
  virtual void computeJacobian(SparseMatrix<Number> & jacobian) override;

protected:
  /**
   * Update the sets of nodes with constrained DOFs
   */
  void updateConstrainedNodes();

  virtual Real computeQpResidual(Moose::ConstraintType type) override;
  virtual Real computeQpJacobian(Moose::ConstraintJacobianType type) override;

  /// Holds the slave node set or side set
  BoundaryName _master_boundary_id;

  /// Holds the slave node set or side set
  BoundaryName _slave_boundary_id;

  /// Tangential stiffness of spring in all directions
  const Real & _penalty;

  /// master node id connected to each slave node in _connected_nodes
  std::vector<dof_id_type> _master_conn;
};
