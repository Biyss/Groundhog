[Mesh]
  type = GeneratedMesh
  dim = 2
  nx = 40
  ny = 40
  xmin = -10
  xmax = 10
  ymin = -10
  ymax = 10
  uniform_refine = 1
[]

[Variables]
  [./e]
  [../]
  [./e2]
  [../]
  #[./e3]
  #[../]
  [./disp_x]
  [../]
  [./disp_y]
  [../]
[]

[Kernels]
  [./TensorMechanics]
    displacements = 'disp_x disp_y'
  [../]
  [./AC_bulk]
    type = AllenCahn
    variable = e
    f_name = F
    args = 'e2'
  [../]
  [./AC_int]
    type = ACInterface
    variable = e
  [../]
  [./e_dot]
    type = TimeDerivative
    variable = e
  [../]
  [./AC_bulk2]
    type = AllenCahn
    variable = e2
    f_name = F
    args = 'e'
  [../]
  [./AC_int2]
    type = ACInterface
    variable = e2
  [../]
  [./e_dot2]
    type = TimeDerivative
    variable = e2
  [../]
  #[./AC_bulk3]
  #  type = AllenCahn
  #  variable = e3
  #  f_name = F
  #  args = 'e e2'
  #[../]
  #[./AC_int3]
  #  type = ACInterface
  #  variable = e3
  #[../]
  #[./e_dot3]
  #  type = TimeDerivative
  #  variable = e3
  #[../]
[]

[ICs]
  [./e]
    type = SmoothCircleIC
    variable = 'e'
    int_width = 1
    radius = 3
    invalue = 1.0
    outvalue = 0.0
    x1 = -4.0
    y1 = 0.0
  [../]
  [./e2]
    type = SmoothCircleIC
    variable = 'e2'
    int_width = 1
    radius = 3
    invalue = 1.0
    outvalue = 0.0
    x1 = 4.0
    y1 = 0.0
  [../]
  #[./e2]
  #  type = ConstantIC
  #  variable = 'e3'
  #  value = 0
  #[../]
[]

[BCs]
  [./BC_yl]
    type = PresetBC
    variable = disp_y
    boundary = 'left'
    value = 0.0
  [../]
  [./BC_yr]
    type = PresetBC
    variable = disp_y
    boundary = 'right'
    value = 0.0
  [../]
  [./BC_xl]
    type = PresetBC
    variable = disp_x
    boundary = 'left'
    value = 0.0
  [../]
  [./BC_xr]
    type = FunctionPresetBC
    variable = disp_x
    boundary = 'right'
    function = 0.0
  [../]
[]

[Materials]
  [./Const_mat]
    type = GenericConstantMaterial
    block = 0
    prop_names = 'L kappa_op'
    prop_values = '1.0 0.2'
  [../]
  [./elasticity_tensor]
    type = ComputeElasticityTensor
    block = 0
    C_ijkl = '64 25'
    fill_method = symmetric_isotropic
  [../]
  [./stress]
    type = ComputeLinearElasticStress
    block = 0
  [../]
  [./var_dependence]
    type = DerivativeParsedMaterial
    block = 0
    function = 'e'
    args = 'e'
    f_name = var_dep_1
    enable_jit = true
    derivative_order = 2
  [../]
  [./var_dependence2]
    type = DerivativeParsedMaterial
    block = 0
    function = 'e2'
    args = 'e2'
    f_name = var_dep_2
    enable_jit = true
    derivative_order = 2
  [../]
  [./var_dependence3]
    type = DerivativeParsedMaterial
    block = 0
    function = '-3'
    f_name = var_dep_3
    enable_jit = true
    derivative_order = 2
  [../]
  [./eigenstrain]
    type = ThreePhaseVariableEigenstrain
    block = 0
    eigen_base_1 = '0 0 0 0.05 -0.03 0' # burger's vector along X direction
    eigen_base_2 = '0 0 0 0.05 0.03 0' # burger's vector along Y direction
    eigen_base_3 = '0 0 0 0.06 0 0'
    prefactor_1 = var_dep_1
    prefactor_2 = var_dep_2
    prefactor_3 = var_dep_3
    args = 'e e2'
  [../]
  [./strain]
    type = ComputeSmallStrain
    block = 0
    displacements = 'disp_x disp_y'
  [../]
  [./elastic_free_energy]
    type = ElasticEnergyMaterial
    f_name = E
    block = 0
    args = 'e e2'
    outputs = exodus
    derivative_order = 2
  [../]
  [./crystalline_energy]
    type = DerivativeParsedMaterial
    block = 0
    function = 'A*(1-cos(pi*(e-e2))*cos(pi*e2)*cos(pi*e))'
    constant_names = 'A pi'
    constant_expressions = '1 3.1415926'
    args = 'e e2'
    f_name = C
    derivative_order = 2
    outputs = exodus
  [../]
  [./free_energy]
    type = DerivativeSumMaterial
    block = 0
    f_name = F
    sum_materials = 'C E'
    args = 'e e2'
    derivative_order = 2
    outputs = exodus
  [../]
[]

[Postprocessors]
  [./E]
    type = ElementIntegralMaterialProperty
    mat_prop = E
    execute_on = 'TIMESTEP_END INITIAL'
  [../]
  [./C]
    type = ElementIntegralMaterialProperty
    mat_prop = C
    execute_on = 'TIMESTEP_END INITIAL'
  [../]
[]

[Preconditioning]
  [./SMP]
    type = SMP
    full=true
  [../]
[]

[Executioner]
  type = Transient
  solve_type = 'PJFNK'
  nl_rel_tol = 1e-9
  nl_abs_tol = 1e-12
  l_max_its = 30
  l_tol = 1e-4
  nl_max_its = 10
  petsc_options_iname = '-pc_type -pc_hypre_type -ksp_gmres_restart'
  petsc_options_value = 'hypre boomeramg 31'
  num_steps = 50
  [./TimeStepper]
    type = IterationAdaptiveDT
    dt = 0.01
    optimal_iterations = 6
    iteration_window = 1
  [../]
[]

[Outputs]
  exodus = true
  print_perf_log = true
  print_linear_residuals = true
[]
