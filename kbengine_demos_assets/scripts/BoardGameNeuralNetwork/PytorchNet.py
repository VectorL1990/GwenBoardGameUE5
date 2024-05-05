import torch
import GlobalConst
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
	def __init__(self, in_features_num = 14, num_channels=256, num_res_blocks=7):
		# in_features_num represents feature descriptions of the board, which is 
		super().__init__()
		self.conv_block = nn.Conv2d(in_channels=in_features_num, out_channels=num_channels, kernel_size=(3,3), stride=(1,1), padding=1)
		self.conv_block_bn = nn.BatchNorm2d(256)
		self.conv_block_act = nn.ReLU()

		# resnet for features extraction
		self.res_blocks = nn.ModuleList([ResBlock(num_filters=num_channels) for _ in range(num_res_blocks)])

		# policy head
		self.policy_conv = nn.Conv2d(in_channels=num_channels, out_channels=16, kernel_size=(1,1), stride=(1,1))
		self.policy_bn = nn.BatchNorm2d(16)
		self.policy_act = nn.ReLU()
		self.policy_fc = nn.Linear(16*8*8, GlobalConst.totalMoveNb)

		self.play_card_type_policy_conv = nn.Conv2d(in_channels=num_channels, out_channels=16, kernel_size=(1,1), stride=(1,1))
		self.play_card_type_policy_bn = nn.BatchNorm2d(16)
		self.play_card_type_policy_act = nn.ReLU()
		self.play_card_type_policy_fc = nn.Linear(16*8*8, GlobalConst.totalPlayCardTypeNb)

		# value head
		self.value_conv = nn.Conv2d(in_channels=num_channels, out_channels=8, kernel_size=(1,1), stride=(1,1))
		self.value_bn = nn.BatchNorm2d(8)
		self.value_act1 = nn.ReLU()
		self.value_fc1 = nn.Linear(8*8*8, 256)
		self.value_act2 = nn.ReLU()
		self.value_fc2 = nn.Linear(256, 1)

	def forward(self, x):
		x = self.conv_block(x)
		x = self.conv_block_bn(x)
		x = self.conv_block_act(x)
		for layer in self.res_blocks:
			x = layer(x)

		# policy head
		policy = self.policy_conv(x)
		policy = self.policy_bn(policy)
		policy = self.policy_act(policy)
		policy = torch.reshape(policy, [-1, 16*8*8])
		policy = self.policy_fc(policy)
		policy = F.log_softmax(policy)

		# value head
		value = self.value_conv(x)
		value = self.value_bn(value)
		value = self.value_act1(value)
		value = torch.reshape(value, [-1, 8*8*8])
		value = self.value_fc1(value)
		value = self.value_act1(value)
		value = self.value_fc2(value)
		value = F.tanh(value)

		return policy, value

class PolicyValueNet:
	def __init__(self, model_file=None, use_gpu=True, device='cuda'):
		self.use_gpu = use_gpu
		self.l2_const = 2e-3
		self.device = device
		self.policyValueNet = Net().to(self.device)
		self.optimizer = torch.optim.Adam(params=self.policyValueNet.parameters(), lr=1e-3, betas=(0.9, 0.999), eps=1e-8, weight_decay=self.l2_const)
		if model_file:
			self.policyValueNet.load_state_dict(torch.load(model_file))

	def PolicyValueBatchEvaluation(self, stateBatch):
		self.policyValueNet.eval()
		stateBatch = torch.tensor(stateBatch).to(self.device)
		logActProbs, value = self.policyValueNet(stateBatch)
		logActProbs, value = logActProbs.cpu(), value.cpu()
		actProbs = np.exp(logActProbs.detach().numpy())
		return actProbs, value.detach().numpy()

	def PolicyValueEvaluation(self, board):
		self.policyValueNet.eval()

		legalMoves = board.GetLegalMoves()
		currentState = np.ascontiguousarray(board.CurrentState().reshape(-1, 9, 10, 9)).astype('float16')
		currentState = torch.as_tensor(currentState).to(self.device)

		with autocast():
			logActProbs, value = self.policyValueNet(currentState)
		logActProbs, value = logActProbs.cpu(), value.cpu()
		actProbTuples = np.exp(logActProbs.detach().numpy().astype('float16').flatten())
		actProbTuples = zip(legalMoves, actProbTuples[legalMoves])
		return actProbTuples, value.detach().numpy()

	def SaveModel(self, modelFile):
		torch.save(self.policyValueNet.state_dict(), modelFile)

	def TrainStep(self, stateBatch, mctsProbBatch, winnerBatch, lr=0.002):
		self.policyValueNet.train()

		stateBatch = torch.tensor(stateBatch).to(self.device)
		mctsProbBatch = torch.tensor(mctsProbBatch).to(self.device)
		winnerBatch = torch.tensor(winnerBatch).to(self.device)

		self.optimizer.zero_grad()
		for params in self.optimizer.param_groups:
			params['lr'] = lr

		logActProbs, value = self.plicyValueNet(stateBatch)
		value = torch.reshape(value, shape=[-1])

		valueLoss = F.mse_loss(input=value, target=winnerBatch)
		policyLoss = -torch.mean(torch.sum(mctsProbBatch*logActProbs, dim=1))
		loss = valueLoss + policyLoss

		loss.backward()
		self.optimizer.step()
		
		with torch.no_grad():
			entropy = -torch.mean(torch.sum(torch.exp(logActProbs)*logActProbs, dim=1))

		return loss.detach().cpu().numpy(), entropy.detach().cpu().numpy()



    