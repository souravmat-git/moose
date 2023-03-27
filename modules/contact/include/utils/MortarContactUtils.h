//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#pragma once

#include "MooseConfig.h"

#include "FEProblemBase.h"
#include "ADReal.h"
#include "MooseMesh.h"

#include "libmesh/dof_object.h"

#include "metaphysicl/parallel_dualnumber.h"
#include "metaphysicl/parallel_semidynamicsparsenumberarray.h"

#include "timpi/parallel_sync.h"
#include "timpi/communicator.h"

#include <utility>
#include <array>
#include <unordered_map>
#include <vector>

namespace Moose
{
namespace Mortar
{
namespace Contact
{

/**
 * This function is used to communicate velocities across processes
 * @param dof_to_weighted_gap Map from degree of freedom to weighted (weak) gap
 * @param this_id Our processor id from domain decomposition
 * @param mesh Mesh used to locate nodes or elements
 * @param nodal Whether the element has Lagrange interpolation
 * @param communicator Process communicator
 */
template <typename T>
inline void
communicateVelocities(std::unordered_map<const DofObject *, std::array<T, 2>> & dof_map,
                      const processor_id_type this_id,
                      const MooseMesh & mesh,
                      const bool nodal,
                      const Parallel::Communicator & communicator)
{
  // We may have weighted gap information that should go to other processes that own the dofs
  using Datum = std::pair<dof_id_type, std::array<T, 2>>;
  std::unordered_map<processor_id_type, std::vector<Datum>> push_data;

  for (auto & pr : dof_map)
  {
    const auto * const dof_object = pr.first;
    const auto proc_id = dof_object->processor_id();
    if (proc_id == this_id)
      continue;

    push_data[proc_id].push_back(std::make_pair(dof_object->id(), std::move(pr.second)));
  }

  const auto & lm_mesh = mesh.getMesh();

  auto action_functor =
      [nodal, this_id, &lm_mesh, &dof_map](const processor_id_type libmesh_dbg_var(pid),
                                           const std::vector<Datum> & sent_data)
  {
    mooseAssert(pid != this_id, "We do not send messages to ourself here");
    libmesh_ignore(this_id);

    for (auto & pr : sent_data)
    {
      const auto dof_id = pr.first;
      const auto * const dof_object =
          nodal ? static_cast<const DofObject *>(lm_mesh.node_ptr(dof_id))
                : static_cast<const DofObject *>(lm_mesh.elem_ptr(dof_id));
      mooseAssert(dof_object, "This should be non-null");
      dof_map[dof_object][0] += std::move(pr.second[0]);
      dof_map[dof_object][1] += std::move(pr.second[1]);
    }
  };

  TIMPI::push_parallel_vector_data(communicator, push_data, action_functor);
}

/**
 * This function is used to communicate gaps across processes
 * @param dof_to_weighted_gap Map from degree of freedom to weighted (weak) gap
 * @param this_id Our processor id from domain decomposition
 * @param mesh Mesh used to locate nodes or elements
 * @param nodal Whether the element has Lagrange interpolation
 * @param normalize_c Whether to normalize with size the c coefficient in contact constraint
 * @param communicator Process communicator
 */
void communicateGaps(
    std::unordered_map<const DofObject *, std::pair<ADReal, Real>> & dof_to_weighted_gap,
    const processor_id_type this_id,
    const MooseMesh & mesh,
    const bool nodal,
    const bool normalize_c,
    const Parallel::Communicator & communicator);
}
}
}
