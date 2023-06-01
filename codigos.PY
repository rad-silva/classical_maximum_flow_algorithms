# BAP

print("Nome da instancia,Tempo grafo,Tempo grafo residual,Tempo WCP,Tempo empurra fluxo,vertices,arcos,fluxo,tempo(ns),caminhos")
arquivoDados.write("Nome da instancia,Tempo grafo,Tempo grafo residual,Tempo WCP,Tempo empurra fluxo,vertices,arcos,fluxo,tempo(ns),caminhos\n")

for i in instancesNames:
  nomeArquivoDeSolucao = testeName + "/" + i + ".log"
  arquivoDeSolucao = open(nomeArquivoDeSolucao, "r")
  solucao = arquivoDeSolucao.read()
  tgrafo = levalor("Tempo grafo: ", solucao)
  t_r_grafo = levalor("Tempo grafo residual: ", solucao)
  tWCP = levalor("Tempo WCP: ", solucao)
  t_empurra_fluxo = levalor("Tempo empurra fluxo: ", solucao)
  vertices = levalor("Vertices: ", solucao)
  arcos = levalor("Arcos: ", solucao)
  fluxo = levalor("Fluxo: ", solucao)
  tempo = levalor("Tempo(ns): ", solucao)
  caminhos = levalor("Caminhos: ", solucao)
  print(i + "," + tgrafo + "," + t_r_grafo + "," + tWCP + "," + t_empurra_fluxo + "," + vertices + "," + arcos + "," + fluxo + "," + tempo + "," + caminhos)
  arquivoDados.write(i + "," + tgrafo + "," + t_r_grafo + "," + tWCP + "," + t_empurra_fluxo + "," + vertices + "," + arcos + "," + fluxo + "," + tempo + "," + caminhos + "\n")
  arquivoDeSolucao.close()
arquivoDados.close()


# SAP

print("Nome da instancia,Tempo grafo,Tempo grafo residual,Tempo BFS,Tempo calcula capacidade,Tempo empurra fluxo,vertices,arcos,fluxo,tempo(ns),caminhos")
arquivoDados.write("Nome da instancia,Tempo grafo,Tempo grafo residual,Tempo calcula capacidade,Tempo BFS,Tempo empurra fluxo,vertices,arcos,fluxo,tempo(ns),caminhos\n")

for i in instancesNames:
  nomeArquivoDeSolucao = testeName + "/" + i + ".log"
  arquivoDeSolucao = open(nomeArquivoDeSolucao, "r")
  solucao = arquivoDeSolucao.read()
  tgrafo = levalor("Tempo grafo: ", solucao)
  t_r_grafo = levalor("Tempo grafo residual: ", solucao)
  tBFS = levalor("Tempo BFS: ", solucao)
  t_calcula_capacidade = levalor("Tempo calcula capacidade caminho: ", solucao)
  t_empurra_fluxo = levalor("Tempo empurra fluxo: ", solucao)
  vertices = levalor("Vertices: ", solucao)
  arcos = levalor("Arcos: ", solucao)
  fluxo = levalor("Fluxo: ", solucao)
  tempo = levalor("Tempo(ns): ", solucao)
  caminhos = levalor("Caminhos: ", solucao)
  print(i + "," + tgrafo + "," + t_r_grafo + "," + tBFS + "," + t_calcula_capacidade + "," + t_empurra_fluxo + "," + vertices + "," + arcos + "," + fluxo + "," + tempo + "," + caminhos)
  arquivoDados.write(i + "," + tgrafo + "," + t_r_grafo + "," + tBFS + "," + t_calcula_capacidade + "," + t_empurra_fluxo + "," + vertices + "," + arcos + "," + fluxo + "," + tempo + "," + caminhos + "\n")
  arquivoDeSolucao.close()
arquivoDados.close()


# CS

print("Nome da instancia,Tempo grafo,Tempo grafo residual,Tempo grafo residual(delta),Tempo BFS,Tempo calcula capacidade,Tempo empurra fluxo,vertices,arcos,fluxo,tempo(ns),caminhos")
arquivoDados.write("Nome da instancia,Tempo grafo,Tempo grafo residual,Tempo grafo residual(delta),Tempo calcula capacidade,Tempo BFS,Tempo empurra fluxo,vertices,arcos,fluxo,tempo(ns),caminhos\n")

for i in instancesNames:
  nomeArquivoDeSolucao = testeName + "/" + i + ".log"
  arquivoDeSolucao = open(nomeArquivoDeSolucao, "r")
  solucao = arquivoDeSolucao.read()
  tgrafo = levalor("Tempo grafo: ", solucao)
  t_r_grafo = levalor("Tempo grafo residual: ", solucao)
  t_r_grafo_delta = levalor("Tempo grafo residual(delta): ", solucao)
  tBFS = levalor("Tempo BFS: ", solucao)
  t_calcula_capacidade = levalor("Tempo calcula capacidade caminho: ", solucao)
  t_empurra_fluxo = levalor("Tempo empurra fluxo: ", solucao)
  vertices = levalor("Vertices: ", solucao)
  arcos = levalor("Arcos: ", solucao)
  fluxo = levalor("Fluxo: ", solucao)
  tempo = levalor("Tempo(ns): ", solucao)
  caminhos = levalor("Caminhos: ", solucao)
  print(i + "," + tgrafo + "," + t_r_grafo + "," + t_r_grafo_delta + "," + tBFS + "," + t_calcula_capacidade + "," + t_empurra_fluxo + "," + vertices + "," + arcos + "," + fluxo + "," + tempo + "," + caminhos)
  arquivoDados.write(i + "," + tgrafo + "," + t_r_grafo + "," + t_r_grafo_delta + "," + tBFS + "," + t_calcula_capacidade + "," + t_empurra_fluxo + "," + vertices + "," + arcos + "," + fluxo + "," + tempo + "," + caminhos + "\n")
  arquivoDeSolucao.close()
arquivoDados.close()


# PR

print("Nome da instancia,Tempo grafo,Tempo grafo residual,Tempo push,Tempo relabel,Tempo heap,vertices,arcos,fluxo,tempo(ns)")
arquivoDados.write("Nome da instancia,Tempo grafo,Tempo grafo residual,Tempo push,Tempo relabel,Tempo heap,vertices,arcos,fluxo,tempo(ns)\n")

for i in instancesNames:
  nomeArquivoDeSolucao = testeName + "/" + i + ".log"
  arquivoDeSolucao = open(nomeArquivoDeSolucao, "r")
  solucao = arquivoDeSolucao.read()
  tgrafo = levalor("Tempo grafo: ", solucao)
  t_r_grafo = levalor("Tempo grafo residual: ", solucao)
  tpush = levalor("Tempo push: ", solucao)
  trelabel = levalor("Tempo relabel: ", solucao)
  theap = levalor("Tempo heap: ", solucao)
  vertices = levalor("Vertices: ", solucao)
  arcos = levalor("Arcos: ", solucao)
  fluxo = levalor("Fluxo: ", solucao)
  tempo = levalor("Tempo(ns): ", solucao)
  print(i + "," + tgrafo + "," + t_r_grafo + "," + tpush + "," + trelabel + "," + theap + "," + vertices + "," + arcos + "," + fluxo + "," + tempo )
  arquivoDados.write(i + "," + tgrafo + "," + t_r_grafo + "," + tpush + "," + trelabel + "," + theap + "," + vertices + "," + arcos + "," + fluxo + "," + tempo + "\n")
  arquivoDeSolucao.close()
arquivoDados.close()


# HLPRv1

print("Nome da instancia,Tempo grafo,Tempo grafo residual,Tempo push,Tempo relabel,vertices,arcos,fluxo,tempo(ns)")
arquivoDados.write("Nome da instancia,Tempo grafo,Tempo grafo residual,Tempo push,Tempo relabel,vertices,Arcos,Fluxo,Tempo(ns)\n")

for i in instancesNames:
  nomeArquivoDeSolucao = testeName + "/" + i + ".log"
  arquivoDeSolucao = open(nomeArquivoDeSolucao, "r")
  solucao = arquivoDeSolucao.read()
  tgrafo = levalor("Tempo grafo: ", solucao)
  t_r_grafo = levalor("Tempo grafo residual: ", solucao)
  tpush = levalor("Tempo push: ", solucao)
  trelabel = levalor("Tempo relabel: ", solucao)
  vertices = levalor("Vertices: ", solucao)
  arcos = levalor("Arcos: ", solucao)
  fluxo = levalor("Fluxo: ", solucao)
  tempo = levalor("Tempo(ns): ", solucao)
  print(i + "," + tgrafo + "," + t_r_grafo + "," + tpush + "," + trelabel + "," + vertices + "," + arcos + "," + fluxo + "," + tempo )
  arquivoDados.write(i + "," + tgrafo + "," + t_r_grafo + "," + tpush + "," + trelabel + "," + vertices + "," + arcos + "," + fluxo + "," + tempo + "\n")
  arquivoDeSolucao.close()
arquivoDados.close()


# HLPRv2


