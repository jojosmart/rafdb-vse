#!/usr/bin/env python
#encoding=utf-8
#
# Copyright 2014 iminer. All Rights Reserved.
# Author:Zhouxiaoliang
# Date  : 2013-04-03 15:52:52
# File  : test.py
# Brief : 

import ctypes
pdll = ctypes.CDLL('./libtest.so')
print pdll.fact(4)
