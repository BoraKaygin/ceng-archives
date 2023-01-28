from math import exp
from unittest import result

def Sigmoid(Dataset):
    Datacount = len(Dataset)
    for i in range(Datacount):
        if Dataset[i] <= -700:
            Dataset[i] = 0
        elif -700 < Dataset[i] and Dataset[i] < 700:
            Dataset[i] = 1/(1+exp(-Dataset[i]))
        elif 700 <= Dataset[i]:
            Dataset[i] = 1
        else:
            print("Sigmoid value error")
    return Dataset

def ReLU(Dataset):
    Datacount = len(Dataset)
    for i in range(Datacount):
        Dataset[i] = max(0,Dataset[i])
    return Dataset

def LinearLayer(Dataset,Weights):
    WeightCount=len(Weights)
    DataCount=len(Dataset)
    Result = [0] * WeightCount
    for i in range(WeightCount):
        for n in range(DataCount):
            Result[i] = Result[i] + Dataset[n] * Weights[i][n]
    return Result

def forward_pass(Network,Dataset):
    LayerCount = len(Network)
    NextLayer = [0]
    NextLayer[:] = Dataset
    for i in range(LayerCount):
        if len(Network[i]) == 2:
            NextLayer[:] = LinearLayer(NextLayer,Network[i][1])
        elif Network[i][0:3] == "rel":
            NextLayer[:] = ReLU(NextLayer)
        elif Network[i][0:3] == "sig":
            NextLayer[:] = Sigmoid(NextLayer)
        else:
            print("Layer detection error.")
    return NextLayer