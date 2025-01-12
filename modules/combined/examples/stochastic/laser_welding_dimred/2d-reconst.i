!include parameters.i

!include mesh.i

[Variables]
  [vel]
    family = LAGRANGE_VEC
  []
  [T]
  []
  [p]
  []
  [disp_x]
  []
  [disp_y]
  []
[]

[AuxVariables]
  [T_reconst]
  []
[]

!include physics_objects.i

[UserObjects]
  [inverse]
    type = InverseMapping
    mapping = pod
    variable_to_fill = "T_reconst"
    variable_to_reconstruct = "T"
    surrogate = mogp
    parameters = '${R} ${power}'
    execute_on = TIMESTEP_END
  []
[]

[Surrogates]
  [mogp]
    type = GaussianProcessSurrogate
    filename = "train_mogp_out_mogp.rd"
  []
[]

[VariableMappings]
  [pod]
    type = PODMapping
    filename = "train_pod_out_pod.rd"
  []
[]

[Executioner]
  type = Transient
  end_time = ${endtime}
  dtmin = 1e-10
  dtmax = 1e-5
  petsc_options_iname = '-pc_type -pc_factor_shift_type'
  petsc_options_value = 'lu       NONZERO'
  petsc_options = '-snes_converged_reason -ksp_converged_reason -options_left'
  solve_type = 'NEWTON'
  line_search = 'none'
  nl_max_its = 16
  l_max_its = 100
  [TimeStepper]
    type = IterationAdaptiveDT
    optimal_iterations = 10
    iteration_window = 4
    dt = ${timestep}
    linear_iteration_ratio = 1e6
    growth_factor = 1.25
  []
[]

[Postprocessors]
  [l2error]
    type = ElementL2Difference
    variable = T
    other_variable = T_reconst
  []
[]

[Debug]
  show_var_residual_norms = true
[]

[Postprocessors]
[]
