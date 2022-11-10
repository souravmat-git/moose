a = 1.1

[Mesh]
  [gen_mesh]
    type = GeneratedMeshGenerator
    dim = 1
    xmin = 2
    xmax = 3
    nx = 2
  []
[]

[Problem]
  coord_type = 'RZ'
[]

[Variables]
  [v]
    family = MONOMIAL
    order = CONSTANT
    fv = true
    initial_condition = 1
  []
[]

[FVKernels]
  # Flux kernel
  [advection]
    type = FVAdvection
    variable = v
    velocity = '${a} 0 0'
    advected_interp_method = 'average'
  []
  [body_v]
    type = FVBodyForce
    variable = v
    function = 'forcing'
  []
[]

[FVBCs]
  [left_u]
    type = FVFunctionDirichletBC
    boundary = 'left'
    function = 'exact'
    variable = v
  []
  [right_u]
    type = FVConstantScalarOutflowBC
    variable = v
    velocity = '${a} 0 0'
    boundary = 'right'
  []
[]

[Functions]
  [exact]
    type = ParsedFunction
    value = 'sin(x)'
  []
  [forcing]
    type = ParsedFunction
    value = '(x*a*cos(x) + a*sin(x))/x'
    vars = 'a'
    vals = '${a}'
  []
[]

[Executioner]
  type = Steady
  solve_type = 'NEWTON'
  petsc_options_iname = '-pc_type -sub_pc_factor_shift_type -sub_pc_type'
  petsc_options_value = 'asm      NONZERO                   lu'
[]

[Outputs]
  exodus = true
  csv = true
[]

[Postprocessors]
  [error]
    type = ElementL2Error
    variable = v
    function = exact
    outputs = 'console csv'
    execute_on = 'timestep_end'
  []
  [h]
    type = AverageElementSize
    outputs = 'console csv'
    execute_on = 'timestep_end'
  []
[]
