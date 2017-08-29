#!/usr/bin/python

import string
import random
import time
import numpy as np

cton = {'.':0, 'r':1, 'g':2, 'b':3,
        'y':4, 'o':5, 'p':6, 'd':7,
        }

ctol = {'.':'0000000',
        'r':'0000001',
        'g':'0000010',
        'b':'0000100',
        'y':'0001000',
        'o':'0010000',
        'p':'0100000',
        'd':'1000000',
    }

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
    def __init__(self,n0,n1,n2,n3,m):
        self.layer1 = ReLULayer(n0,n1,m)
        self.layer2 = ReLULayer(n1,n2,m)
        self.layer3 = ReLULayer(n2,n3,m)
        self.layer4 = Layer(n3,1,m);
        self.alpha = 0.00003

    def forward(self,x):
        self.layer1.input = x
        self.layer1.forward()
        self.layer2.input = self.layer1.output
        self.layer2.forward()
        self.layer3.input = self.layer2.output
        self.layer3.forward()
        self.layer4.input = self.layer3.output
        self.layer4.forward()

    def backward(self,y):
        yhat = self.layer4.output
        self.layer4.doutput = yhat - y
        self.layer4.backward()
        self.layer3.doutput = self.layer4.dinput
        self.layer3.backward()
        self.layer2.doutput = self.layer3.dinput
        self.layer2.backward()
        self.layer1.doutput = self.layer2.dinput
        self.layer1.backward()

    def update(self):
        self.layer1.update(self.alpha)
        self.layer2.update(self.alpha)
        self.layer3.update(self.alpha)
        self.layer4.update(self.alpha)

    def train(self,x,y):
        self.forward(x)
        self.backward(y)
        self.update()

    def test(self,x,y):
        self.forward(x)
        return np.linalg.norm(self.lastResult()-y)/np.sqrt(x.shape[1])

    def compute(self,x):
        self.forward(x)
        return self.lastResult()

    def lastResult(self):
        return self.layer4.output

def compute(nn,bd):
    x = np.zeros((567,1))
    for i in range(9):
        line = replace(bd[i])
        for j in range(len(line)):
            x[i*len(line)+j][0] = int(line[j])
    return nn.compute(x)

def replace(line):
    ret = ''
    for i in range(len(line)):
        ret += ctol[line[i]]
    return ret

def shuffleData():
    with open('data') as f:
        lines = f.readlines()
    data = {}
    def readData(line):
        x,y = string.strip(line).split(' ')
        if x not in data:
            data[x] = y
        elif y > data[x]:
            data[x] = y
    map(readData,lines)
    dataset = []
    for key in data:
        val = data[key]
        dataset.append([key,val])
    random.shuffle(dataset)
    with open('shuffled_data','w') as f:
        for d in dataset:
            f.write("%s %s\n"%(d[0],d[1]))

def train(nn,trainset,N):
    trainsize = len(trainset)
    x = np.zeros((567,trainsize))
    y = np.zeros((1,trainsize))
    for i,pair in enumerate(trainset):
        for j in range(len(pair[0])):
            x[j][i] = int(pair[0][j])
        y[0][i] = float(pair[1])

    start = time.clock()
    lastnorm = None
    for i in range(N):
        nn.train(x,y)
        thisnorm = np.linalg.norm(nn.lastResult()-y)/np.sqrt(x.shape[1])
        if lastnorm == None:
            diffnorm = 0
        else:
            diffnorm = thisnorm - lastnorm
            if diffnorm > 0:
                nn.alpha *= 0.99
        lastnorm = thisnorm
        print "%d: err=%.10f, delta=%.10f, t=%.5fs, alpha=%.10f"%(i,thisnorm,diffnorm,time.clock()-start,nn.alpha)

    np.save("layer1.w.npy",nn.layer1.weight)
    np.save("layer1.b.npy",nn.layer1.bias)
    np.save("layer2.w.npy",nn.layer2.weight)
    np.save("layer2.b.npy",nn.layer2.bias)
    np.save("layer3.w.npy",nn.layer3.weight)
    np.save("layer3.b.npy",nn.layer3.bias)
    np.save("layer4.w.npy",nn.layer4.weight)
    np.save("layer4.b.npy",nn.layer4.bias)

def testnn(nn,testset):
    testsize = len(testset)

    x = np.zeros((567,testsize))
    y = np.zeros((1,testsize))
    for i,pair in enumerate(testset):
        for j in range(len(pair[0])):
            x[j][i] = int(pair[0][j])
        y[0][i] = float(pair[1])
    print nn.test(x,y)

if __name__ == "__main__":

    # shuffleData()
    # exit()

    trainset = []
    with open('train_data') as f:
        lines = f.readlines()
    for line in lines:
        x,y = string.strip(line).split(' ')
        trainset.append([x,y])

    testset = []
    with open('test_data') as f:
        lines = f.readlines()
    for line in lines:
        x,y = string.strip(line).split(' ')
        testset.append([x,y])

    trainsize = len(trainset)
    nn = NeuralNetwork(567,100,100,100,trainsize)
    nn.layer1.weight = np.load("layer1.w.npy")
    nn.layer1.bias = np.load("layer1.b.npy")
    nn.layer2.weight = np.load("layer2.w.npy")
    nn.layer2.bias = np.load("layer2.b.npy")
    nn.layer3.weight = np.load("layer3.w.npy")
    nn.layer3.bias = np.load("layer3.b.npy")
    nn.layer4.weight = np.load("layer4.w.npy")
    nn.layer4.bias = np.load("layer4.b.npy")
    nn.alpha = 0.00118

    # train(nn,trainset,1000)
    testnn(nn,testset)
