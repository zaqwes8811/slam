# coding: utf-8

import sys

# for inner import
sys.path.insert(0, 'deps/robust_kalman')
sys.path.insert(0, 'deps/filterpy')
sys.path.insert(0, 'deps/Kalman_and_Bayesian_Filters_in_Python_master')

if __name__ == '__main__':
    key = 0
    # key = 1

    if key == 0:
        """
            "A Kalman Filter for Robust Outlier Detection" Jo-Anne Ting 1 , Evangelos Theodorou 1 , 
            and Stefan Schaal 1,2 of Southern California, Los Angeles, CA, 90089
            2 ATR Computational Neuroscience Laboratories, Kyoto, Japan
            
            Q: есть ли итеративный поиск чего-то, и есть ли реалтаймность 
            
            FIXME: может правильнее поставить как надо
            
            ipywidgets
        """
        from deps.filterpy.filterpy import kalman, common
        from scipy.linalg import block_diag
        import numpy as np
        from kalman import KalmanFilter
        from common import Q_discrete_white_noise

        from deps.Kalman_and_Bayesian_Filters_in_Python_master.kf_book import book_plots

        from numpy.random import randn
        import matplotlib.pyplot as plt
        import numpy as np

        # Gating
        from deps.filterpy.filterpy.stats import mahalanobis


        class PosSensor(object):
            def __init__(self, pos=(0, 0), vel=(0, 0), noise_std=1.):
                self.vel = vel

                self.noise_std = noise_std
                self.pos = [pos[0], pos[1]]

            def read(self):
                self.pos[0] += self.vel[0]

                self.pos[1] += self.vel[1]
                return [self.pos[0] + randn() * self.noise_std,
                        self.pos[1] + randn() * self.noise_std]


        pos, vel = (4, 3), (2, 1)
        sensor = PosSensor(pos, vel, noise_std=1)
        ps = np.array([sensor.read() for _ in range(50)])

        # book_plots.plot_measurements(ps[:, 0], ps[:, 1]);
        # plt.show()

        z = ps
        dt = 1.
        F = np.array([[1, dt, 0, 0],
                      [0, 1, 0, 0],
                      [0, 0, 1, dt],
                      [0, 0, 0, 1]])

        q = Q_discrete_white_noise(dim=2, dt=dt, var=0.001)
        Q = block_diag(q, q)

        H = np.array([[1 / 0.3048, 0, 0, 0],
                      [0, 0, 1 / 0.3048, 0]])

        R = np.array([[5., 0],
                      [0, 5]])
        x = np.array([[0, 0, 0, 0]]).T
        P = np.eye(4) * 500.


    elif key == 1:
        import subprocess

        # subprocess.check_call("latex")

        """
        numpy, scipy matplotlib
        
        500 Mb
        sudo apt-get install texlive dvipng texlive-latex-extra texlive-fonts-recommended
        
        https://github.com/empymod/empymod/issues/4
        """

        # https://github.com/milsto/robust-kalman
        # FIXME: что не нравится - итеративный поиск
        # вроде бы реалтайм
        from deps.robust_kalman.robust_kalman import RobustKalman

        from deps.robust_kalman.examples import example_advanced

        pass
