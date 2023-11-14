# -*- coding: utf-8 -*-


class Functor:
	def __init__(self, func, *args):
		self.func = func
		self.args = args

	def __call__(self, *args):
		self.func(*(self.args + args))
		