import pickle
import os
#import zip_array
from Board import Board
from Mcts import MCTSPlayer
from PytorchNet import PolicyValueNet


class AISelfPlayRoom(object):
	def __init__(self):
		self.board = Board()
		self.mctsPlayer = MCTSPlayer()

	def LoadModel(self):
		model_path = CONFIG['pytorch_model_path']
		self.policyValueNet = PolicyValueNet(model_file=model_path)
		self.mctsPlayer = MCTSPlayer(self.policyValueNet.PolicyValueEvaluation,
									c_puct=self.c_puct,
									n_playout=self.n_playout,
									is_selfplay=1)

	'''def GetEquiData(self, playData):
		extendData = []
		for state, mctsProb, winner in playData:
			extendData.append(zip_array.zip_state)
		return extendData
	'''

	def StartSelfPlay(self, nGames = 1):
		self.LoadModel()
		for i in range(nGames):
			winner, playData = self.board.AISelfPlay(self.mctsPlayer)
			playData = list(playData)[:]
			#playData = self.GetEquiData(playData)
			if os.path.exists():
				while True:
					try:
						with open(xxx, 'rb') as dataDict:
							dataFile = pickle.load(dataDict)
							self.dataBuffer = deque()
							self.dataBuffer.extend(dataFile['dataBuffer'])
							self.iters = dataFile["iters"]
							del dataFile
							self.iters += 1
							self.dataBuffer.extend(playData)
						break
					except:
						time.sleep(30)
			else:
				self.dataBuffer.extend(playData)
				self.iters += 1
			dataDict = {"dataBuffer": self.dataBuffer, "iters": self.iters}
			with open(CONFIG['train_data_buffer_path'], 'wb') as data_file:
				pickle.dump(dataDict, dataFile)
		return self.iters
	
	def run(self):
		try:
			while True:
				iters = self.StartSelfPlay()
		except KeyboardInterrupt:
			print("\n\rquit")


