import random
import zipArray
import numpy as np
from Mcts import MCTSPlayer

# Trainer is an independent module
# it only ultilizes data collected from SelfPlayCollector
class Trainer:
	def __init__(self):
		sdf

	def PolicyUpdate(self):
		miniBatch = random.sample(self.dataBuffer, self.batchSize)
		miniBatch = [zipArray.RecoverStateMctsProb(data) for data in miniBatch]

		stateBatch = [data[0] for data in miniBatch]
		stateBatch = np.array(stateBatch).astype('float32')

		mctsProbsBatch = [data[1] for data in miniBatch]
		mctsProbsBatch = np.array(mctsProbsBatch).astype('float32')

		winnerBatch = [data[2] for data in miniBatch]
		winnerBatch = np.array(winnerBatch).astype('float32')

		oldProbs, oldValue = self.policyValueNet.PolicyValueBatchEvaluation(stateBatch)

		for i in range(self.epochs):
			loss, entropy = self.policyValueNet.trainStep()

			newProbs, newValue = self.policyValueNet.PolicyValueBatchEvalution(stateBatch)

	def run(self):
		try:
			for i in range(self.gameBatchNum):
				while True:
					try:
						with open(,"rb") as dataDict:
							dataFile = pickle.load(dataDict)
							self.dataBuffer = dataFile["dataBuffer"]
							self.iters = dataFile["iters"]
							del dataFile
						break
					except:
						time.sleep(30)
				
				loss, entropy = self.PolicyUpdate()
				self.policyValueNet.saveModel()

				if (i + 1)%self.checkFreq == 0:
					self.policyValueNet.SaveMode()

                
