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
  [./disp_x]
  [../]
  [./disp_y]
  [../]
  [./cv]
  [../]
  [./wv]
    #scaling = 1e-5
  [../]
[]

[Kernels]
  [./TensorMechanics]
    displacements = 'disp_x disp_y'
  [../]
  [./AC_bulk]
    type = AllenCahn
    variable = e
    f_name = F2
  [../]
  [./AC_int]
    type = ACInterface
    variable = e
  [../]
  [./e_dot]
    type = TimeDerivative
    variable = e
  [../]
  [./c_dot]
    type = CoupledTimeDerivative
    variable = wv
    v = cv
  [../]
  [./c_res]
    type = SplitCHParsed
    variable = cv
    f_name = F
    kappa_name = kappa_c
    w = wv
    args = 'e'
  [../]
  [./w_res]
    type = SplitCHWRes
    variable = wv
    mob_name = Mv
    args = 'e'
  [../]
  [./e_dot2]
    type = CoefCoupledTimeDerivative
    variable = wv
    v = e
    coef = 1
  [../]
[]


[ICs]
  [./e]
    type = SmoothCircleIC
    variable = 'e'
    int_width = 1.5
    radius = 3
    invalue = 1.0
    outvalue = 0.0
    x1 = 0.0
    y1 = 0.0
  [../]
  [./cv_ic]
    variable = cv
    value = 0.21
    type = ConstantIC
  [../]
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
  [./Periodic]
    [./all]
      auto_direction = 'x y'
      variable = 'cv wv e'
    [../]
  [../]
[]

[Materials]
  [./Const_mat]
    type = GenericConstantMaterial
    block = 0
    prop_names = 'kappa_c Mv L kappa_op'
    prop_values = '0.2 1.0 1.0 0.2'
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
    function = -3+e
    args = e
    f_name = var_dep
    enable_jit = true
    derivative_order = 2
  [../]
  [./eigenstrain]
    type = ComputeVariableEigenstrain
    block = 0
    eigen_base = '0 0 0 0 0.03 0'
    prefactor = var_dep
    args = 'e'
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
    args = 'e'
    outputs = exodus
    derivative_order = 2
  [../]
  [./crystalline_energy]
    type = DerivativeParsedMaterial
    block = 0
    function = 'A*sin(pi*e)*sin(pi*e)'
    constant_names = 'A pi'
    constant_expressions = '0.5 3.14159'
    args = 'e'
    f_name = C
    derivative_order = 2
    outputs = exodus
  [../]
  [./vacancy_energy]
    type = DerivativeParsedMaterial
    block = 0
    #function = '(3*e^2-2*e^3)*(Ef*cv+kbT*(cv*log(cv)+(1-cv)*log(1-cv)))+(1-3*e^2+2*e^3)*A*(cv-cv0)^2+B*(1-e^2)*e^2'
    function = 'Ef*cv+kbT*(cv*log(cv)+(1-cv)*log(1-cv))'
    constant_names = 'Ef   kbT A cv0 B'
    constant_expressions = '1  0.05  1.0 2.094e-23 1'
    args = 'cv e'
    f_name = Fc
    derivative_order = 2
    outputs = exodus
  [../]
  [./free_energy]
    type = DerivativeSumMaterial
    block = 0
    f_name = F
    sum_materials = 'C E Fc'
    args = 'e cv'
    derivative_order = 2
    outputs = exodus
  [../]
  [./free_energy2]
    type = DerivativeSumMaterial
    block = 0
    f_name = F2
    sum_materials = 'C E'
    args = 'e'
    derivative_order = 2
    outputs = exodus
  [../]
[]

#[Postprocessors]
#  [./E]
#    type = ElementIntegralMaterialProperty
#    mat_prop = E
#    execute_on = 'TIMESTEP_END INITIAL'
#  [../]
#  [./C]
#    type = ElementIntegralMaterialProperty
#    mat_prop = C
#    execute_on = 'TIMESTEP_END INITIAL'
#  [../]
#[]

[VectorPostprocessors]
  [./eta]
    type = LineValueSampler
    start_point = '-10 0 0'
    end_point = '10 0 0'
    variable = e
    num_points = 40
    sort_by = x
    execute_on = timestep_end
    outputs = csv
  [../]
  [./c]
    type = LineValueSampler
    start_point = '-10 0 0'
    end_point = '10 0 0'
    variable = cv
    num_points = 40
    sort_by = x
    execute_on = timestep_end
    outputs = csv
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
  dt=0.02
  num_steps = 500

  #[./TimeStepper]
  #  type = IterationAdaptiveDT
  #  dt = 0.002
  #  optimal_iterations = 6
  #  iteration_window = 1
  #[../]
[]

[Outputs]
  exodus = true
  print_perf_log = true
  print_linear_residuals = true
  csv = true # turns on postprocessor and scalar variable output to *.csv file
#[./console]
#  type = Console
#  execute_postprocessors_on = false # disables printing to screen
#[../]
[]
