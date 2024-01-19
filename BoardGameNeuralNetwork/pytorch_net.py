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

	# features contain:
	# 1. play state, whether card is put on board already
	# 2. movable state, whether card can be moved
	# 3. skill trigger state, whether skill can be triggered,
	# 4. card motion type
	# 5. skill trigger geometry type
	# 6. skill effect geometry type
	# 7. skill type
	# 8. hp
	# 9. defence
	# 10. agility
	# 11. tags
	# 12. state tags
	# 13. state tags count down
	# 14. camp
	def __init__(self, in_features_num = 14, num_channels=256, num_res_blocks=7):
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
		# output nodes should contain actions including
		# 1. playing card in specific grid = 32 possible actions
		# 2. move card to specific grid = 64 possible actions
		# 3. trigger card skill = 1 possible action
		# which means there are possible 97 actions per card
		self.policy_fc = nn.Linear(16*8*8, 970)

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
		self.policy_value_net = Net().to(self.device)
		self.optimizer = torch.optim.Adam(params=self.policy_value_net.parameters(), lr=1e-3, betas=(0.9, 0.999), eps=1e-8, weight_decay=self.l2_const)
		if model_file:
			self.policy_value_net.load_state_dict(torch.load(model_file))

	def policy_value(self, state_batch):
		self.policy_value_net.eval()
		state_batch = torch.tensor(state_batch).to(self.device)
		log_act_probs, value = self.policy_value_net(state_batch)
		log_act_probs, value = log_act_probs.cpu(), value.cpu()
		act_probs = np.exp(log_act_probs.detach().numpy())
		return act_probs, value.detach().numpy()

	def policy_value_fn(self, board):
		self.policy_value_net.eval()


    