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
[]

#[AuxVariables]
#  [./S11]
#    order = CONSTANT
#    family = MONOMIAL
#  [../]
#  [./S22]
#    order = CONSTANT
#    family = MONOMIAL
#  [../]
#  [./S12]
#    order = CONSTANT
#    family = MONOMIAL
#  [../]
#  [./S13]
#    order = CONSTANT
#    family = MONOMIAL
#  [../]
#  [./von_mises]
#    #Dependent variable used to visualize the Von Mises stress
#    order = CONSTANT
#    family = MONOMIAL
#  [../]
#[]

#[AuxKernels]
#  [./von_mises_kernel]
#    #Calculates the von mises stress and assigns it to von_mises
#    type = RankTwoScalarAux
#    variable = von_mises
#    rank_two_tensor = stress
#    execute_on = timestep_end
#    scalar_type = VonMisesStress
#  [../]
#  [./S11]
#    type = RankTwoAux
#    variable = S11
#    rank_two_tensor = stress
#    index_j = 0
#    index_i = 0
#    block = 0
#  [../]
#  [./S22]
#    type = RankTwoAux
#    variable = S22
#    rank_two_tensor = stress
#    index_j = 1
#    index_i = 1
#    block = 0
#  [../]
#  [./S12]
#    type = RankTwoAux
#    variable = S12
#    rank_two_tensor = stress
#    index_j = 1
#    index_i = 0
#    block = 0
#  [../]
#  [./S13]
#    type = RankTwoAux
#    variable = S13
#    rank_two_tensor = stress
#    index_j = 2
#    index_i = 0
#    block = 0
#  [../]
#[]

[Kernels]
  [./TensorMechanics]
    displacements = 'disp_x disp_y'
  [../]
  [./AC_bulk]
    type = AllenCahn
    variable = e
    f_name = F
  [../]
  [./AC_int]
    type = ACInterface
    variable = e
  [../]
  [./e_dot]
    type = TimeDerivative
    variable = e
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
    function = e
    args = e
    f_name = var_dep
    enable_jit = true
    derivative_order = 2
  [../]
  [./eigenstrain]
    type = ComputeVariableEigenstrain
    block = 0
    eigen_base = '0.03 0 0 0 0 0'
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
  [./free_energy]
    type = DerivativeSumMaterial
    block = 0
    f_name = F
    sum_materials = 'C E'
    args = 'e'
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
  #We solve a steady state problem using Newton's iteration
  type = Transient
  solve_type = 'PJFNK'
  #scheme = bdf2
  #solve_type = 'PJFNK'
  nl_rel_tol = 1e-9
  nl_abs_tol = 1e-12
  l_max_its = 30
  l_tol = 1e-4
  nl_max_its = 10
  petsc_options_iname = '-pc_type -pc_hypre_type -ksp_gmres_restart'
  petsc_options_value = 'hypre boomeramg 31'

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
