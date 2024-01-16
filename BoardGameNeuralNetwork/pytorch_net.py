import torch
import torch.nn as nn
import numpy as np
import torch.nn.functional as F
from config import CONFIG
from torch.cuda.amp import autocast

class ResBlock(nn.Module):
  def __init__(self, num_filters=256):
    super().__init__()
    self.conv1 = nn.Conv2d(in_channels=num_filters, out_channels=num_filters, kernel_size=(3,3), stride=(1,1), padding=1)
    self.conv1_bn = nn.BatchNorm2d(num_filters, )
    self.conv1_act = nn.ReLU()
    self.conv2 = nn.Conv2d(in_channels=num_filters, out_channels=num_filters, kernel_size=(3,3), stride=(1,1), padding=1)
    self.conv2_bn = nn.BatchNorma2d(num_filters, )
    self.conv2_act = nn.ReLU()

  def forward(self, x):
    y = self.conv1(x)
    y = self.conv1_bn(y)
    y = self.conv1_act(y)
    y = self.conv2(y)
    y = self.conv2_bn(y)
    y = x + y
    return self.conv2_act(y)

class Net(nn.Module):

  def __init__(self, num_channels=256, num_res_blocks=7):
    super().__init__()
    