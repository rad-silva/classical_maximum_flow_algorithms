import os
from os.path import isfile, join
import sys

mainPath = "/home/ricardo/Downloads/teste"
instancesFolderPath = "/home/ricardo/Downloads/teste/instancias"
extensionFile = ".max"
testeName = "HLPRv2_2023_06_05"

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

print("Nome da instancia,vertices,arcos,fluxo,tempo(ns),Pushs,Relabels")
arquivoDados.write("Nome da instancia,Vertices,Arcos,Fluxo,Tempo(ns),Pushs,Relabels\n")

for i in instancesNames:
  nomeArquivoDeSolucao = testeName + "/" + i + ".log"
  arquivoDeSolucao = open(nomeArquivoDeSolucao, "r")
  solucao = arquivoDeSolucao.read()
  vertices = levalor("Vertices: ", solucao)
  arcos = levalor("Arcos: ", solucao)
  fluxo = levalor("Fluxo: ", solucao)
  tempo = levalor("Tempo(ns): ", solucao)
  pushs = levalor("Pushs: ", solucao)
  relabels = levalor("Relabels: ", solucao)
  print(i + "," + vertices + "," + arcos + "," + fluxo + "," + tempo + "," + pushs + "," + relabels)
  arquivoDados.write(i + "," + vertices + "," + arcos + "," + fluxo + "," + tempo +  "," + pushs + "," + relabels + "\n")
  arquivoDeSolucao.close()
arquivoDados.close()
