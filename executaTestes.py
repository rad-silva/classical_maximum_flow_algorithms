import os
from os.path import isfile, join

# Compilar programa principal antes de rodar
# Editar nome do teste e data

# ["SAP", "CS", "BAP", "HPRv1", "HPRv2", "HPRv4", "HPRv5", "PR"]
method = ["HPRv1", "HPRv2"]
date = "2023_06_05"


for mtd in method:
  mainScript = "./a $instance " + mtd
  testeName = mtd + "_" + date

  mainPath = "/home/ricardo/Downloads/teste"
  instancesFolderPath = "/home/ricardo/Downloads/teste/instancias"
  extensionFile = ".max"

  instancesNames = [f for f in os.listdir(instancesFolderPath) if isfile(join(instancesFolderPath, f))]
  instancesNames = [instance for instance in instancesNames if extensionFile in instance]

  if not os.path.exists(testeName):
    print("Criando folder " + testeName)
    os.makedirs(testeName)

  for i in instancesNames:
    comando = mainScript.replace("$instance", instancesFolderPath + "/" + i)
    comando = comando + " > " + "./" + testeName + "/" + i + ".log"
    print(comando)
    os.system(comando)

    