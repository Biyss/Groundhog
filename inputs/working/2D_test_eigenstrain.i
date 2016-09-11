[Mesh]
  type = GeneratedMesh
  dim = 2
  nx = 10
  ny = 10
[]

[GlobalParams]
  displacements = 'disp_x disp_y'
[]

[Variables]
  [./disp_x]
  [../]
  [./disp_y]
  [../]
[]

[Kernels]
  [./TensorMechanics]
  [../]
[]

[BCs]
  [./top_x]
    type = PresetBC
    variable = 'disp_x'
    boundary = 'top'
    value = 0
  [../]
  [./top_y]
    type = PresetBC
    variable = 'disp_y'
    boundary = 'top'
    value = 0
  [../]
  [./bottom_x]
    type = PresetBC
    variable = 'disp_x'
    boundary = 'bottom'
    value = 0
  [../]
  [./bottom_y]
    type = PresetBC
    variable = 'disp_y'
    boundary = 'bottom'
    value = 0
  [../]
[]

[Materials]
  [./elasticity_tensor]
    type = ComputeElasticityTensor
    C_ijkl = '64 25'
    fill_method = symmetric_isotropic
  [../]
  [./stress]
    type = ComputeLinearElasticStress
  [../]
  [./eigenstrain]
    type = ComputeEigenstrain
    eigen_base = '0 0 0 0 0 0.1'
  [../]
  [./strain]
    type = ComputeSmallStrain
  [../]
  [./elastic_free_energy]
    type = ElasticEnergyMaterial
    f_name = E_el
    args = ''
    outputs = exodus
    output_properties = E_el
    derivative_order = 2
  [../]
[]

[Preconditioning]
  [./SMP]
    type = SMP
    full = true
  [../]
[]

[Executioner]
  type = Steady
  solve_type = 'PJFNK'
  nl_rel_tol = 1e-9
  nl_abs_tol = 1e-12
  l_max_its = 30
  l_tol = 1e-4
  nl_max_its = 10

  petsc_options_iname = '-pc_type -pc_hypre_type -ksp_gmres_restart'
  petsc_options_value = 'hypre boomeramg 31'
[]

[Outputs]
  exodus = true
  output_on = 'timestep_end initial'
[]
