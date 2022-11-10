//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#pragma once

#include <shared_mutex>

#include "ExecFlagEnum.h"

namespace moose
{
namespace internal
{

class ExecFlagRegistry;

/**
 * Get the global ExecFlagRegistry singleton.
 */
ExecFlagRegistry & getExecFlagRegistry();

/**
 * Registry for statically defining execute flags with consistent numbering.
 */
class ExecFlagRegistry
{
public:
  /**
   * Registers an execute flag.
   * @param name The name of the execute flag.
   * @param is_default Whether or not to define the flag as a default (available to all objects that
   * use the SetupInterface).
   */
  const ExecFlagType & registerFlag(const std::string & name, const bool is_default);

  /**
   * \returns The registered exec flags.
   */
  const ExecFlagEnum & getFlags() const { return _flags; };

  /**
   * \returns The registered default exec flags.
   */
  const ExecFlagEnum & getDefaultFlags() const { return _default_flags; };

private:
  ExecFlagRegistry();

  /// So it can be constructed
  friend ExecFlagRegistry & getExecFlagRegistry();

  /// Mutex for guarding access to _flags
  std::shared_mutex _flags_mutex;

  /// Mutex for guarding access to _default_flags;
  std::shared_mutex _default_flags_mutex;

  /// The registered flags
  ExecFlagEnum _flags;

  /// The default flags
  ExecFlagEnum _default_flags;
};

}
}

#define registerExecFlag(flag) moose::internal::getExecFlagRegistry().registerFlag(flag, false)
#define registerDefaultExecFlag(flag)                                                              \
  moose::internal::getExecFlagRegistry().registerFlag(flag, true)
