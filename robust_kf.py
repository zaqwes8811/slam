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

        import numpy as np
        from kalman import KalmanFilter
        from common import Q_discrete_white_noise

        from deps.Kalman_and_Bayesian_Filters_in_Python_master.kf_book import book_plots

        pass
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
