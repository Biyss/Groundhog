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
  [../]
  [./T]
    initial_condition = 600
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
  [./HeatCond]
    type = HeatConduction
    variable = T
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
    value = 0.1
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
    [./temp]
      auto_direction = 'y'
      variable = 'T'
    [../]
  [../]
  [./left_T]
    type = PresetBC
    variable = T
    boundary = left
    value = 600
  [../]
  [./right_flux]
    type = NeumannBC
    variable = T
    boundary = right
    value = 5e-10
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
    sum_materials = 'C E Fc Fn'
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

  [./thermal_conductivity]
    type = ParsedMaterial
    block = 0
    f_name = thermal_conductivity
    constant_names = 'length_scale k_v k_b'
    constant_expressions = '1e-9 0.42 15.0'
    function = 'sk_b:= length_scale*k_b;
    sk_v:= length_scale*k_v;
    sk_in:= (1-cv^2)*sk_b+cv^2*sk_v;
    if(cv<0.9,sk_in,sk_v)'
    outputs = exodus
    args = 'cv T'
  [../]

  [./probability]
  # This is a made up toy nucleation rate it should be replaced by
  # classical nucleation theory in a real simulation.
    type = ParsedMaterial
    block = 0
    f_name = P
    args = cv
    function = 0.1
    outputs = exodus
  [../]
  [./nucleation]
    # The nucleation material is configured to insert nuclei into the free energy
    # tht force the concentration to go to 0.95, and holds this enforcement for 500
    # time units.
  type = DiscreteNucleation
    block = 0
    f_name = Fn
    op_names  = cv
    op_values = 0.5
    penalty = 5
    penalty_mode = MIN
    map = map
    outputs = exodus
  [../]
[]


[UserObjects]
  [./inserter]
    # The inserter runs at the end of each time step to add nucleation events
    # that happend during the timestep (if it converged) to the list of nuclei
    type = DiscreteNucleationInserter
    hold_time = 0.02
    probability = P
  [../]
  [./map]
    # The map UO runs at the beginning of a timestep and generates a per-element/qp
    # map of nucleus locations. The map is only regenerated if the mesh changed or
    # the list of nuclei was modified.
    # The map converts the nucleation points into finite area objects with a given radius.
    type = DiscreteNucleationMap
    radius = 0.5
    int_width = 0.1
    periodic = cv
    inserter = inserter
  [../]
[]

[Postprocessors]
  [./right_T]
    type = SideAverageValue
    variable = T
    boundary = right
  [../]
  [./effect_th_cond]
    type = ThermalConductivity
    variable = T
    flux = 5e-10
    length_scale = 1e-09
    T_hot = 600
    dx = 20
    k0 = 15
    boundary = right
  [../]
  [./ElementInt_cv]
    type = ElementIntegralVariablePostprocessor
    variable = cv
  [../]
[]

#[VectorPostprocessors]
#  [./c]
#    type = LineValueSampler
#    start_point = '-10 0 0'
#    end_point = '10 0 0'
#    variable = cv
#    num_points = 40
#    sort_by = x
#    execute_on = timestep_end
#    outputs = csv
#  [../]
#[]

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
  num_steps = 100

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
