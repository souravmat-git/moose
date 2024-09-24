[Mesh]
  [square]
    type = GeneratedMeshGenerator
    dim = 2
    nx = 16
    ny = 16
    xmin = 0
    xmax = 1
    ymin = 0
    ymax = 1
  []
[]

[Variables/u]
[]

[Functions]
  [rxn_func]
    type = ParsedFunction
    expression = 'exp(x * y) - 1'
  []
[]

[Materials]
  [ad_dc_prop]
    type = ADParsedMaterial
    expression = '1 + u'
    coupled_variables = 'u'
    property_name = dc_prop
  []
  [ad_rxn_prop]
    type = ADGenericFunctionMaterial
    prop_values = 'rxn_func'
    prop_names = rxn_prop
    outputs = exodus
  []
  #ADMatReaction includes a negative sign in residual evaluation, so we need to
  #reverse this with a negative reaction rate. However, we wanted the parameter
  #to remain positive in the optimization problem so we do the same thing here,
  #which is why there is one object to evaluate function and another to flip
  #it's sign for the kernel
  [ad_neg_rxn_prop]
    type = ADParsedMaterial
    expression = '-rxn_prop'
    material_property_names = 'rxn_prop'
    property_name = 'neg_rxn_prop'
  []
[]

[Kernels]
  [udot]
    type = ADTimeDerivative
    variable = u
  []
  [diff]
    type = ADMatDiffusion
    variable = u
    diffusivity = dc_prop
  []
  [reaction]
    type = ADMatReaction
    variable = u
    reaction_rate = neg_rxn_prop
  []
  [src]
    type = ADBodyForce
    variable = u
    value = 1
  []
[]

[BCs]
  [dirichlet]
    type = DirichletBC
    variable = u
    boundary = 'left bottom'
    value = 0
  []
[]

[Executioner]
  type = Transient
  solve_type = NEWTON
  petsc_options_iname = '-pc_type -pc_factor_mat_solver_package'
  petsc_options_value = 'lu superlu_dist'
  dt = 0.1
  end_time = 1
[]

[Reporters]
  [sample]
    type = TransientSyntheticDataCreator
    variable = u
    single_set_of_measurement_points = '0.625 0.625 0  0.5 0.5 0  0.25 0.25 0
    0.875 0.625 0  0.625 0.875 0  0.75 0.25 0  0.25 0.75 0  0.8125 0.9375 0
    0.75   0.75   0
    0.8125 0.75   0
    0.875  0.75   0
    0.9375 0.75   0
    1      0.75   0
    0.8125 0.8125 0
    0.875  0.8125 0
    0.9375 0.8125 0
    1      0.8125 0
    0.875  0.875  0
    0.9375 0.875  0
    1      0.875  0
    0.9375 0.9375 0
    1      0.9375 0
    1      1      0'

    measurement_times_for_all_points = '0.1 0.2 0.3 0.4 0.5 0.6 0.7 0.8 0.9 1.0'
    outputs = csv_final
  []
[]

[AuxVariables]
  [reaction_rate]
  []
[]

[AuxKernels]
  [reaction_rate_aux]
    type = FunctionAux
    variable = reaction_rate
    function = rxn_func
    execute_on = TIMESTEP_END
  []
[]

[Postprocessors]
  [u1]
    type = PointValue
    variable = u
    point = '0.25 0.25 0'
  []
  [u2]
    type = PointValue
    variable = u
    point = '0.75 0.75 0'
  []
  [u3]
    type = PointValue
    variable = u
    point = '1 1 0'
  []
[]

[Outputs]
  exodus = true
  csv = true
  [csv_final]
    type = CSV
    execute_on = 'FINAL'
    file_base = forward_exact_csv
  []
[]
