neml2_input = elasticity
N = 2

[GlobalParams]
  displacements = 'disp_x disp_y disp_z'
[]

[Mesh]
  [gmg]
    type = GeneratedMeshGenerator
    dim = 3
    nx = ${N}
    ny = ${N}
    nz = ${N}
  []
[]

[Physics]
  [SolidMechanics]
    [QuasiStatic]
      [all]
        strain = SMALL
        new_system = true
        add_variables = true
        formulation = TOTAL
        volumetric_locking_correction = true
      []
    []
  []
[]

[NEML2]
  input = 'models/${neml2_input}.i'
  [all]
    model = 'model'
    verbose = true
    device = 'cpu'

    moose_input_types = 'MATERIAL'
    moose_inputs = 'neml2_strain'
    neml2_inputs = 'forces/E'

    moose_parameter_types = 'MATERIAL POSTPROCESSOR'
    moose_parameters = 'youngs_modulus poisson_ratio'
    neml2_parameters = 'E nu'

    moose_output_types = 'MATERIAL'
    moose_outputs = 'neml2_stress'
    neml2_outputs = 'state/S'

    moose_derivative_types = 'MATERIAL'
    moose_derivatives = 'neml2_jacobian'
    neml2_derivatives = 'state/S forces/E'
  []
[]

[Functions]
  [E_function]
    type = ParsedFunction
    expression = '100+x*y+1000*t'
  []
[]

[Postprocessors]
  [poisson_ratio]
    type = ConstantPostprocessor
    value = 0.31
  []
[]

[Materials]
  [youngs_modulus]
    type = GenericFunctionMaterial
    prop_names = 'youngs_modulus'
    prop_values = 'E_function'
  []
  [convert_strain]
    type = RankTwoTensorToSymmetricRankTwoTensor
    from = 'mechanical_strain'
    to = 'neml2_strain'
  []
  [stress]
    type = ComputeLagrangianObjectiveCustomSymmetricStress
    custom_small_stress = 'neml2_stress'
    custom_small_jacobian = 'neml2_jacobian'
  []
[]

[BCs]
  [xfix]
    type = DirichletBC
    variable = disp_x
    boundary = left
    value = 0
  []
  [yfix]
    type = DirichletBC
    variable = disp_y
    boundary = bottom
    value = 0
  []
  [zfix]
    type = DirichletBC
    variable = disp_z
    boundary = back
    value = 0
  []
  [xdisp]
    type = FunctionDirichletBC
    variable = disp_x
    boundary = right
    function = t
    preset = false
  []
[]

[Executioner]
  type = Transient
  solve_type = NEWTON
  petsc_options_iname = '-pc_type'
  petsc_options_value = 'lu'
  automatic_scaling = true
  dt = 1e-3
  dtmin = 1e-3
  num_steps = 5
  residual_and_jacobian_together = true
[]

[Outputs]
  exodus = true
[]
