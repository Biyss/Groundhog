[Mesh]
  type = GeneratedMesh
  dim = 3
  nx = 80
  ny = 80
  nz = 32
  xmin = -10
  xmax = 10
  ymin = -10
  ymax = 10
  zmax = 4
  zmin = -4
[]

[Variables]
  [./e]
  [../]
  [./e2]
  [../]
  [./disp_x]
  [../]
  [./disp_y]
  [../]
  [./disp_z]
  [../]
[]

[Kernels]
  [./TensorMechanics]
    displacements = 'disp_x disp_y disp_z'
  [../]
  [./AC_bulk]
    type = AllenCahn
    variable = e
    f_name = F
    args = 'e2'
  [../]
  [./AC_int]
    type = SimpleACGradNormal
    variable = e
    normal = '0 0 1'
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
    type = SimpleACGradNormal
    variable = e2
    normal = '0 0 1'
  [../]
  [./e_dot2]
    type = TimeDerivative
    variable = e2
  [../]
[]

[ICs]
  [./e]
    type = PlateletBaseIC
    variable = 'e'
    normal = '0 0 1'
    radius = 3
    x1 = 3.5
    y1 = 0
    z1 = 0
    thickness = 0.5
    int_width_r = 1
    int_width_th = 0.25
  [../]
  [./e2]
    type = PlateletBaseIC
    variable = 'e2'
    normal = '0 0 1'
    radius = 3
    x1 = -3.5
    y1 = 0
    z1 = 0
    thickness = 0.5
    int_width_r = 1
    int_width_th = 0.25
  [../]
[]

[BCs]
  [./BC_front_y]
    type = PresetBC
    variable = 'disp_y'
    boundary = 'front'
    value = 1.2
  [../]
  [./BC_front_z]
    type = PresetBC
    variable = 'disp_z'
    boundary = 'front'
    value = 0.0
  [../]
  [./BC_back]
    type = PresetBC
    variable = 'disp_y'
    boundary = 'back'
    value = 0.0
  [../]
  [./BC_left]
    type = PresetBC
    variable = 'disp_x'
    boundary = 'left'
    value = 0.0
  [../]
  [./BC_right]
    type = PresetBC
    variable = 'disp_x'
    boundary = 'right'
    value = 0.0
  [../]
  [./BC_top_yz]
    type = PresetBC
    variable = 'disp_z'
    boundary = 'top'
    value = 0.0
  [../]
  [./BC_bottom]
    type = PresetBC
    variable = 'disp_z'
    boundary = 'bottom'
    value = 0.0
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
    function = '0'
    f_name = var_dep_3
    enable_jit = true
    derivative_order = 2
  [../]
  [./eigenstrain]
    type = ThreePhaseVariableEigenstrain
    block = 0
    eigen_base_1 = '0 0 0 0.08 -0.05 0' # burger's vector along X direction
    eigen_base_2 = '0 0 0 0.08 0.05 0' # burger's vector along Y direction
    eigen_base_3 = '0 0 0 0.00 0 0'
    prefactor_1 = var_dep_1
    prefactor_2 = var_dep_2
    prefactor_3 = var_dep_3
    args = 'e e2'
  [../]
  [./strain]
    type = ComputeSmallStrain
    block = 0
    displacements = 'disp_x disp_y disp_z'
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
    constant_expressions = '0.9 3.14159'
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
  num_steps = 25
  [./TimeStepper]
    type = IterationAdaptiveDT
    dt = 0.01
    optimal_iterations = 6
    iteration_window = 1
  [../]
[]

[Outputs]
  exodus = true
  output_on = 'timestep_end initial'
  csv = true
[]
