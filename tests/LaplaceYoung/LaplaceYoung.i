[Mesh]
  type = GeneratedMesh
  dim = 2
  nx = 10
  ny = 10
  xmax = 10
  ymax = 10
[]

[Variables]
  [./p]
    block = 0
  [../]
[]

[Kernels]
  [./LY]
    type = LaplaceYoung
    variable = p
    block = 0
  [../]
[]

[BCs]
  [./BC]
    type = DirichletBC
    variable = p
    boundary = 'bottom left right top'
    value = 0.2
  [../]
[]

[Executioner]
  type = Steady
  solve_type = NEWTON
[]

[Outputs]
  exodus = true
[]

