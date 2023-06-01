import os
from os.path import isfile, join
import sys

mainPath = "/home/ricardo/Downloads/teste"
instancesFolderPath = "/home/ricardo/Downloads/teste/instancias"
extensionFile = ".max"
testeName = "HLPRv2_2023_05_20"

instancesNames = [f for f in os.listdir(instancesFolderPath) if isfile(join(instancesFolderPath, f))]
instancesNames = [instance for instance in instancesNames if extensionFile in instance]

def levalor(texto, arquivo):
  pin = arquivo.find(texto)
  if pin == -1:
    return ""
  pfin = arquivo.find('\n',pin)
  tamanho = len(texto)
  return  arquivo[pin+tamanho:pfin]

method = testeName.split('_')[0]
nomeArquivoDados = testeName + "/" + method + ".csv"
arquivoDados = open(nomeArquivoDados, "w")

print("Nome da instancia,Tempo grafo,Tempo grafo residual,Tempo Sort,Tempo push,Tempo relabel,Tempo discharge,vertices,arcos,fluxo,tempo(ns)")
arquivoDados.write("Nome da instancia,Tempo grafo,Tempo grafo residual,Tempo Sort,Tempo push,Tempo relabel,Tempo discharge,Vertices,Arcos,Fluxo,Tempo(ns)\n")

for i in instancesNames:
  nomeArquivoDeSolucao = testeName + "/" + i + ".log"
  arquivoDeSolucao = open(nomeArquivoDeSolucao, "r")
  solucao = arquivoDeSolucao.read()
  tgrafo = levalor("Tempo grafo: ", solucao)
  t_r_grafo = levalor("Tempo grafo residual: ", solucao)
  tsort = levalor("Tempo sort: ", solucao)
  tpush = levalor("Tempo push: ", solucao)
  trelabel = levalor("Tempo relabel: ", solucao)
  tdischarge = levalor("Tempo discharge: ", solucao)
  vertices = levalor("Vertices: ", solucao)
  arcos = levalor("Arcos: ", solucao)
  fluxo = levalor("Fluxo: ", solucao)
  tempo = levalor("Tempo(ns): ", solucao)
  print(i + "," + tgrafo + "," + t_r_grafo + "," + tsort + "," + tpush + "," + trelabel +"," + tdischarge + "," + vertices + "," + arcos + "," + fluxo + "," + tempo)
  arquivoDados.write(i + "," + tgrafo + "," + t_r_grafo + "," + tsort + "," + tpush + "," + trelabel +"," + tdischarge + "," + vertices + "," + arcos + "," + fluxo + "," + tempo + "\n")
  arquivoDeSolucao.close()
arquivoDados.close()
