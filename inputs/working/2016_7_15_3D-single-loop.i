[Mesh]
  type = GeneratedMesh
  dim = 3
  nx = 80
  ny = 80
  nz = 6
  xmin = -10
  xmax = 10
  ymin = -10
  ymax = 10
  zmax = 4
  zmin = -4
[]

[AuxVariables]
  [./f_density]   # Local energy density (eV/mol)
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./f_grad]   # Local energy density (eV/mol)
    order = CONSTANT
    family = MONOMIAL
  [../]
[]

[AuxKernels]
  # Calculates the energy density by combining the local and gradient energies
  [./f_density]   # (eV/mol/nm^2)
    type = DislocationTotalFreeEnergy
    variable = f_density
    f_name = 'F'
    kappa_names = 'kappa_op'
    interfacial_vars = e
    normal = '0 0 1'
  [../]
  [./f_grad]   # (eV/mol/nm^2)
    type = DislocationGradientEnergy
    variable = f_grad
    kappa_names = 'kappa_op'
    interfacial_vars = e
    normal = '0 0 1'
  [../]
[]

[Variables]
  [./e]
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
[]

[ICs]
  [./e]
    type = FunctionIC
    variable = 'e'
    function = 'if(z=0,if(x^2+y^2<=12.96,if(x^2+y^2>=9,(6-sqrt(x^2+y^2)/0.6),1),0),0)' # r=3 int=0.6
  [../]
[]

[BCs]
  [./BC_front_x]
    type = PresetBC
    variable = 'disp_x'
    boundary = 'front'
    value = 2.0
  [../]
  [./BC_front_y]
    type = PresetBC
    variable = 'disp_z'
    boundary = 'front'
    value = 0.0
  [../]
  [./BC_back]
    type = PresetBC
    variable = 'disp_x disp_y disp_z'
    boundary = 'back'
    value = 0.0
  [../]
  [./BC_left]
    type = PresetBC
    variable = 'disp_z'
    boundary = 'left'
    value = 0.0
  [../]
  [./BC_right]
    type = PresetBC
    variable = 'disp_z'
    boundary = 'right'
    value = 0.0
  [../]
  [./BC_top_yz]
    type = PresetBC
    variable = 'disp_y'
    boundary = 'top'
    value = 0.0
  [../]
  [./BC_bottom]
    type = PresetBC
    variable = 'disp_y'
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
    function = e^2
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
    displacements = 'disp_x disp_y disp_z'
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
  [./total_energy]          # Total free energy at each timestep
  type = ElementIntegralVariablePostprocessor
  variable = f_density
  execute_on = 'TIMESTEP_END INITIAL'
  [../]
  [./grad_energy]          # Total free energy at each timestep
  type = ElementIntegralVariablePostprocessor
  variable = f_grad
  execute_on = 'TIMESTEP_END INITIAL'
  [../]
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
  num_steps = 10
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
  #gnuplot = true
  print_linear_residuals = true
  [./console]
    type = Console
    perf_log = true
    output_on = 'timestep_end failed nonlinear linear'
  [../]
[]