# -*- coding: utf-8 -*-
import cv2
import numpy as np

print('HDR Processing...')

# Loading exposure images into a list
img_fn = ["IMX378_3968x3008_HDR_L.ppm", "IMX378_3968x3008_HDR_S.ppm"]
img_list = [cv2.imread(fn) for fn in img_fn]

# Exposure fusion using Mertens
merge_mertens = cv2.createMergeMertens()
res_mertens = merge_mertens.process(img_list)

# Convert datatype to 8-bit and save
res_mertens_8bit = np.clip(res_mertens*255, 0, 255).astype('uint8')
cv2.imwrite("IMX378_3968x3008_HDR_fusion.ppm", res_mertens_8bit)
