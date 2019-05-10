# coding: utf-8

# Design:
# Python wrapper too weak

import numpy as np

# https://github.com/MRPT/mrpt/tree/master/python/samples
from pymrpt.maps import CSimplePointsMap
from pymrpt.gui import CDisplayWindow3D  # 3D only
import pymrpt
from pymrpt.slam import CICP


if __name__ == '__main__':
    m1 = CSimplePointsMap()
    help(CICP)

    m1.insertPointFast(10.0, 5 / 10.0, 0.0)
    m1.insertPointFast(10.0, -5 / 10.0, 0.0)

    # getAllPoints()
    size = m1.getSize()
    for i in range(size):
        print m1.getPointAllFieldsFast(i)

    # print help(pymrpt.gui)
    # print help(pymrpt.maps)

    # win = CDisplayWindowPlots("ICP results")
