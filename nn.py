#!/usr/bin/python

import string
import random
import numpy as np

class Layer(object):
    def __init__(self,inputSize,outputSize,m):
        self.inputSize = inputSize
        self.outputSize = outputSize
        self.m = m
        self.weight = np.random.randn(outputSize,inputSize)*0.01
        self.bias = np.zeros((outputSize,1))

    def forward(self):
        self.z = np.dot(self.weight,self.input) + self.bias
        self.output = self.z

    def backward(self):
        self.dz = self.doutput
        self.dw = np.dot(self.dz,self.input.transpose())/self.m
        self.db = np.sum(self.dz,axis=1,keepdims=True)/self.m
        self.dinput =  np.dot(self.weight.transpose(),self.dz)

    def update(self,alpha):
        self.weight -= self.dw * alpha
        self.bias -= self.db * alpha

class ReLULayer(Layer):
    def __init__(self,inputSize,outputSize,m):
        super(ReLULayer,self).__init__(inputSize,outputSize,m)

    def forward(self):
        self.z = np.dot(self.weight,self.input) + self.bias
        self.output = self.z * (self.z > 0)

    def backward(self):
        self.dz = self.doutput * (self.z > 0);
        self.dw = np.dot(self.dz,self.input.transpose())/self.m
        self.db = np.sum(self.dz,axis=1,keepdims=True)/self.m
        self.dinput =  np.dot(self.weight.transpose(),self.dz)

class NeuralNetwork:
    def __init__(self,n0,n1,n2,m):
        self.layer1 = ReLULayer(n0,n1,m)
        self.layer2 = ReLULayer(n1,n2,m)
        self.layer3 = Layer(n2,1,m);
        self.alpha = 0.0001

    def forward(self,x):
        self.layer1.input = x
        self.layer1.forward()
        self.layer2.input = self.layer1.output
        self.layer2.forward()
        self.layer3.input = self.layer2.output
        self.layer3.forward()

    def backward(self,y):
        yhat = self.layer3.output
        self.layer3.doutput = yhat - y
        self.layer3.backward()
        self.layer2.doutput = self.layer3.dinput
        self.layer2.backward()
        self.layer1.doutput = self.layer2.dinput
        self.layer1.backward()

    def update(self):
        self.layer1.update(self.alpha)
        self.layer2.update(self.alpha)
        self.layer3.update(self.alpha)

    def train(self,x,y):
        self.forward(x)
        self.backward(y)
        self.update()

    def lastResult(self):
        return self.layer3.output

if __name__ == "__main__":
    testsize = 500

    with open('data') as f:
        lines = f.readlines()

    data = {}
    def readData(line):
        x,y = string.strip(line).split(' ')
        if x not in data:
            data[x] = y
        elif y > data[x]:
            data[x] = y

    dataset = []
    trainset = []
    testset = []

    map(readData,lines)
    for key in data:
        val = data[key]
        dataset.append([key,val])

    random.shuffle(dataset)
    trainsize = len(dataset) - testsize
    trainset = dataset[:trainsize]
    testset = dataset[trainsize:]

    x = np.zeros((81,trainsize))
    y = np.zeros((1,trainsize))
    for i,pair in enumerate(trainset):
        for j in range(len(pair[0])):
            x[j][i] = int(pair[0][j])
        y[0][i] = int(pair[1])

    nn = NeuralNetwork(81,100,100,trainsize)
    nn.layer1.weight = np.loadtxt("layer1.w",ndmin=2)
    nn.layer1.bias = np.loadtxt("layer1.b",ndmin=2)
    nn.layer2.weight = np.loadtxt("layer2.w",ndmin=2)
    nn.layer2.bias = np.loadtxt("layer2.b",ndmin=2)
    nn.layer3.weight = np.loadtxt("layer3.w",ndmin=2)
    nn.layer3.bias = np.loadtxt("layer3.b",ndmin=2)
    for i in range(1000):
        nn.train(x,y)
        print np.linalg.norm(nn.lastResult()-y)
    np.savetxt("layer1.w",nn.layer1.weight)
    np.savetxt("layer1.b",nn.layer1.bias)
    np.savetxt("layer2.w",nn.layer2.weight)
    np.savetxt("layer2.b",nn.layer2.bias)
    np.savetxt("layer3.w",nn.layer3.weight)
    np.savetxt("layer3.b",nn.layer3.bias)
